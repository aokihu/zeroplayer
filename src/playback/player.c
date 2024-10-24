/**
 * @file player.c
 * @brief 播放器实现
 */

#include "player.h"

/* ------- 播放器资源*/

/**
 * @brief 创建播放器上下文
 * @return 播放器上下文
 */
PlayerContext *player_new(void)
{
  PlayerContext *player_context = g_new0(PlayerContext, 1);

  // 初始化 PlayerMetadata
  player_context->current_track_metadata.metadata = NULL;
  player_context->current_track_metadata.qq_song_id = NULL;
  player_context->current_track_metadata.duration = NULL;
  player_context->current_track_metadata.title = NULL;
  player_context->current_track_metadata.artist = NULL;
  player_context->current_track_metadata.album = NULL;

  // 设置播放器上下文的URI
  player_context->uri = NULL;
  player_context->next_uri = NULL;

  // 创建播放器管道
  GstElement *pipeline = gst_element_factory_make("playbin3", PLAYBIN_NAME);
  player_context->pipeline = pipeline;

  return player_context;
}

/**
 * @brief 释放播放器上下文
 * @param player_context 播放器上下文
 */
void player_free(
    PlayerContext *player_context)
{
  gst_element_set_state(player_context->pipeline, GST_STATE_NULL);
  gst_object_unref(player_context->pipeline);
  g_free(player_context->uri);
  g_free(player_context->next_uri);
  
  // 释放 PlayerMetadata 中的内存
  g_free(player_context->current_track_metadata.metadata);
  g_free(player_context->current_track_metadata.qq_song_id);
  g_free(player_context->current_track_metadata.duration);
  g_free(player_context->current_track_metadata.title);
  g_free(player_context->current_track_metadata.artist);
  g_free(player_context->current_track_metadata.album);
  
  g_free(player_context);
}

/* ------- 播放器方法 ------- */

/**
 * @brief 设置播放器URI
 * @param player_context 播放器上下文
 * @param uri URI
 */
void player_set_uri(
    PlayerContext *player_context,
    const gchar *uri)
{
  g_free(player_context->uri);
  player_context->uri = g_strdup(uri);

  // 重置播放器管道
  gst_element_set_state(player_context->pipeline, GST_STATE_NULL);

  // 设置播放器管道URI
  g_object_set(player_context->pipeline, "uri", uri, NULL);
}

/**
 * @brief 获取播放器URI
 * @param player_context 播放器上下文
 * @return URI
 */
gchar *player_get_uri(PlayerContext *player_context)
{
  return player_context->uri;
}

/**
 * @brief 设置播放器下一个URI
 * @param player_context 播放器上下文
 * @param uri URI
 */
void player_set_next_uri(
    PlayerContext *player_context,
    const gchar *uri)
{
  g_free(player_context->next_uri);
  player_context->next_uri = g_strdup(uri);
}

/* ------- 播放器状态 ------- */

/**
 * @brief 播放
 * @param player_context 播放器上下文
 */
void player_play(
    PlayerContext *player_context)
{
  gst_element_set_state(player_context->pipeline, GST_STATE_PLAYING);
}

/**
 * @brief 暂停
 * @param player_context 播放器上下文
 */
void player_pause(
    PlayerContext *player_context)
{
  gst_element_set_state(player_context->pipeline, GST_STATE_PAUSED);
}

/**
 * @brief 停止
 * @param player_context 播放器上下文
 */
void player_stop(
    PlayerContext *player_context)
{
  gst_element_set_state(player_context->pipeline, GST_STATE_NULL);
}

/**
 * @brief 获取播放器传输状态
 * @param player_context 播放器上下文
 * @return 传输状态
 */
gchar *player_get_transport_state(PlayerContext *player_context)
{
  gchar *transport_state;
  GstState state;
  GstStateChangeReturn ret;

  // 获取管道当前状态
  ret = gst_element_get_state(player_context->pipeline, &state, NULL, GST_CLOCK_TIME_NONE);

  // 根据状态返回对应的字符串
  switch (state)
  {
  case GST_STATE_VOID_PENDING:
    transport_state = "TRANSITIONING";
    break;
  case GST_STATE_NULL:
    transport_state = "STOPPED";
    break;
  case GST_STATE_READY:
    transport_state = "STOPPED";
    break;
  case GST_STATE_PAUSED:
    transport_state = "PAUSED_PLAYBACK";
    break;
  case GST_STATE_PLAYING:
    transport_state = "PLAYING";
    break;
  default:
    transport_state = "UNKNOWN";
    break;
  }

  return transport_state;
}

/* ------- 播放器音量 ------- */

/**
 * @brief 设置播放器音量
 * @param player_context 播放器上下文
 * @param volume 音量
 */
void player_set_volume(
    PlayerContext *player_context,
    gdouble volume)
{
  g_object_set(player_context->pipeline, "volume", volume, NULL);
}

/**
 * @brief 获取播放器音量
 * @param player_context 播放器上下文
 * @return 音量
 */
gdouble player_get_volume(PlayerContext *player_context)
{
  gdouble volume;
  g_object_get(player_context->pipeline, "volume", &volume, NULL);
  return volume;
}

gboolean player_get_mute(PlayerContext *player_context)
{
  gboolean mute;
  g_object_get(player_context->pipeline, "mute", &mute, NULL);
  return mute;
}

/**
 * @brief 设置播放器静音
 * @param player_context 播放器上下文
 * @param mute 静音
 */
void player_set_mute(
    PlayerContext *player_context,
    gboolean mute)
{
  g_object_set(player_context->pipeline, "mute", mute, NULL);
}

/* ------- 播放器位置 ------- */

/**
 * @brief 设置播放器位置
 * @param player_context 播放器上下文
 * @param position 位置
 */
void player_set_position(
    PlayerContext *player_context,
    gint64 position)
{
  gboolean result = gst_element_seek(player_context->pipeline,
                                     1.0,
                                     GST_FORMAT_TIME,
                                     GST_SEEK_FLAG_FLUSH,
                                     GST_SEEK_TYPE_SET,
                                     position,
                                     GST_SEEK_TYPE_NONE,
                                     GST_CLOCK_TIME_NONE);
  if (!result)
  {
    g_warning("Failed to seek to position: %" G_GINT64_FORMAT, position);
  }
}

/**
 * @brief 获取播放器位置
 * @param player_context 播放器上下文
 * @return 位置
 */
gint64 player_get_position(PlayerContext *player_context)
{
  gint64 pos;
  gst_element_query_position(player_context->pipeline, GST_FORMAT_TIME, &pos);
  if (pos == -1)
  {
    g_warning("Failed to get position");
  }
  
  return pos;
}

/**
 * @brief 获取播放器位置字符串
 * @param player_context 播放器上下文
 * @return 位置字符串 "HH:MM:SS"
 */
gchar *player_get_position_string(PlayerContext *player_context)
{
  gint64 position_ns = player_get_position(player_context); // 您的 gint64 变量，单位为纳秒
  gint64 total_seconds = position_ns / GST_SECOND;

  gint hours = total_seconds / 3600;
  gint minutes = (total_seconds % 3600) / 60;
  gint seconds = total_seconds % 60;

  gchar *position_str = g_strdup_printf("%02d:%02d:%02d", hours, minutes, seconds);
  return position_str;
}

/**
 * @brief 获取播放器时长
 * @param player_context 播放器上下文
 * @return 时长
 */
gint64 player_get_duration(PlayerContext *player_context)
{
  gint64 duration;
  gst_element_query_duration(player_context->pipeline, GST_FORMAT_TIME, &duration);
  return duration;
}

/**
 * @brief 获取播放器时长字符串
 * @param player_context 播放器上下文
 * @return 时长字符串
 */
gchar *player_get_duration_string(PlayerContext *player_context)
{
  return player_util_get_timestamp_string(player_get_duration(player_context));
}

/* ------- 播放器播放进度 ------- */

/**
 * @brief 设置播放器播放进度
 * @param player_context 播放器上下文
 * @param position 进度
 */
void player_seek(
    PlayerContext *player_context,
    gint64 position)
{
  g_object_set(player_context->pipeline, "position", position, NULL);
}


/* ------- 播放器元数据 ------- */

/**
 * @brief 设置当前播放的QQ音乐歌曲ID
 * @param player_context 播放器上下文
 * @param song_id QQ音乐歌曲ID
 */
void player_set_qq_song_id(
    PlayerContext *player_context,
    const gchar *song_id)
{
  g_free(player_context->current_track_metadata.qq_song_id);
  player_context->current_track_metadata.qq_song_id = g_strdup(song_id);
}

/**
 * @brief 设置当前曲目的时长
 * @param player_context 播放器上下文
 * @param duration 时长
 */
void player_set_current_track_duration(
    PlayerContext *player_context,
    const gchar *duration)
{
  g_free(player_context->current_track_metadata.duration);
  player_context->current_track_metadata.duration = g_strdup(duration);
}

/**
 * @brief 设置当前曲目的标题
 * @param player_context 播放器上下文
 * @param title 标题
 */
void player_set_current_track_title(
    PlayerContext *player_context,
    const gchar *title)
{
  g_free(player_context->current_track_metadata.title);
  player_context->current_track_metadata.title = g_strdup(title);
}

/**
 * @brief 设置当前曲目的艺术家
 * @param player_context 播放器上下文
 * @param artist 艺术家
 */
void player_set_current_track_artist(
    PlayerContext *player_context,
    const gchar *artist)
{
  g_free(player_context->current_track_metadata.artist);
  player_context->current_track_metadata.artist = g_strdup(artist);
}

/**
 * @brief 设置当前曲目的专辑名
 * @param player_context 播放器上下文
 * @param album 专辑名
 */
void player_set_current_track_album(
    PlayerContext *player_context,
    const gchar *album)
{
  g_free(player_context->current_track_metadata.album);
  player_context->current_track_metadata.album = g_strdup(album);
}

/**
 * @brief 获取QQ音乐歌曲ID
 * @param player_context 播放器上下文
 * @return QQ音乐歌曲ID
 */
gchar *player_get_qq_song_id(PlayerContext *player_context)
{
  return player_context->current_track_metadata.qq_song_id;
}

/**
 * @brief 获取当前曲目时长
 * @param player_context 播放器上下文
 * @return 当前曲目时长
 */
gchar *player_get_current_track_duration(PlayerContext *player_context)
{
  return player_context->current_track_metadata.duration;
}

/**
 * @brief 设置原始元数据
 * @param player_context 播放器上下文
 * @param metadata 元数据
 */
void player_set_metadata(
    PlayerContext *player_context,
    const gchar *metadata)
{
    g_free(player_context->current_track_metadata.metadata);
    player_context->current_track_metadata.metadata = g_strdup(metadata);
}

/**
 * @brief 获取原始元数据
 * @param player_context 播放器上下文
 * @return 元数据
 */
gchar *player_get_metadata(PlayerContext *player_context)
{
    return player_context->current_track_metadata.metadata;
}

/* ------- 下一个曲目 ------- */

/**
 * @brief 设置下一个曲目元数据
 * @param player_context 播放器上下文
 * @param metadata 元数据
 */
void player_set_next_metadata(
    PlayerContext *player_context,
    const gchar *metadata)
{
  g_free(player_context->next_track_metadata.metadata);
  player_context->next_track_metadata.metadata = g_strdup(metadata);
}

/**
 * @brief 获取下一个曲目元数据
 * @param player_context 播放器上下文
 * @return 元数据
 */
gchar *player_get_next_metadata(PlayerContext *player_context)
{
  return player_context->next_track_metadata.metadata;
}

