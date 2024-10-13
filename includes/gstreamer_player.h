#ifndef GSTREAMER_PLAYER_H
#define GSTREAMER_PLAYER_H

#include <glib.h>
#include <gst/gst.h>

// GStreamer播放器结构体
typedef struct _GstreamerPlayer
{
} GstreamerPlayer;

/**
 * @brief 创建新的GStreamer播放器实例
 *
 * @return GstreamerPlayer* 播放器实例指针，如果创建失败则返回NULL
 */
GstreamerPlayer *gstreamer_player_new(void);

/**
 * @brief 释放GStreamer播放器实例
 *
 * @param player 播放器实例指针
 */
void gstreamer_player_free(GstreamerPlayer *player);

/**
 * @brief 设置要播放的URI
 *
 * @param player 播放器实例指针
 * @param uri 要播放的媒体URI
 * @return gboolean 设置是否成功
 */
gboolean gstreamer_player_set_uri(GstreamerPlayer *player, const gchar *uri);

/**
 * @brief 开始播放
 *
 * @param player 播放器实例指针
 * @return gboolean 播放是否成功启动
 */
gboolean gstreamer_player_play(GstreamerPlayer *player);

/**
 * @brief 暂停播放
 *
 * @param player 播放器实例指针
 * @return gboolean 暂停是否成功
 */
gboolean gstreamer_player_pause(GstreamerPlayer *player);

/**
 * @brief 停止播放
 *
 * @param player 播放器实例指针
 * @return gboolean 停止是否成功
 */
gboolean gstreamer_player_stop(GstreamerPlayer *player);

/**
 * @brief 设置播放位置
 *
 * @param player 播放器实例指针
 * @param position 目标位置（以纳秒为单位）
 * @return gboolean 设置是否成功
 */
gboolean gstreamer_player_seek(GstreamerPlayer *player, gint64 position);

/**
 * @brief 获取当前播放位置
 *
 * @param player 播放器实例指针
 * @return gint64 当前播放位置（以纳秒为单位），如果获取失败则返回-1
 */
gint64 gstreamer_player_get_position(GstreamerPlayer *player);

/**
 * @brief 获取当前媒体的总时长
 *
 * @param player 播放器实例指针
 * @return gint64 媒体总时长（以纳秒为单位），如果获取失败则返回-1
 */
gint64 gstreamer_player_get_duration(GstreamerPlayer *player);

/**
 * @brief 设置音量
 *
 * @param player 播放器实例指针
 * @param volume 音量值（0.0到1.0之间）
 * @return gboolean 设置是否成功
 */
gboolean gstreamer_player_set_volume(GstreamerPlayer *player, gdouble volume);

/**
 * @brief 获取当前音量
 *
 * @param player 播放器实例指针
 * @return gdouble 当前音量值（0.0到1.0之间），如果获取失败则返回-1.0
 */
gdouble gstreamer_player_get_volume(GstreamerPlayer *player);

#endif
