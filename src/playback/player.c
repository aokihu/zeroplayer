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

  g_print("Position: %" G_GINT64_FORMAT "\n", pos);
  return pos;
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
