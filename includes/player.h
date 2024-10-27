/**
 * @file player.h
 * @brief 播放器接口
 * @author aokihu <aokihu@gmail.com>
 */
#ifndef __ZERO_PLAYER_PLAYER_H__
#define __ZERO_PLAYER_PLAYER_H__

#include <glib.h>
#include <gst/gst.h>
#include "struct.h"

/* Constant Variables Definitions */
#define PLAYBIN_NAME "playbin"

/* 工具方法定义 */
gint64 player_get_timestamp_from_string(gchar *timestamp);
gchar *player_util_get_timestamp_string(gint64 timestamp);

/* Function Declarations */
PlayerContext *player_new(void);                                  // Create a new player context
void player_free(PlayerContext *player);                          // Free the player context
gchar *player_get_uri(PlayerContext *player);                     // Get the URI of the player
void player_set_uri(PlayerContext *player, const gchar *uri);     // Set the URI of the player
void player_play(PlayerContext *player);                          // Play the player
void player_pause(PlayerContext *player);                         // Pause the player
void player_stop(PlayerContext *player);                          // Stop the player
void player_seek(PlayerContext *player, gint64 position);         // Seek to a specific position
void player_set_volume(PlayerContext *player, gint volume);       // Set the volume of the player
gdouble player_get_volume(PlayerContext *player);                 // Get the volume of the player
void player_set_position(PlayerContext *player, gint64 position); // Set the position of the player
gint64 player_get_position(PlayerContext *player);                // Get the position of the player
gchar *player_get_position_string(PlayerContext *player);         // Get the position string of the player
gint64 player_get_duration(PlayerContext *player);                // Get the duration of the player
gchar *player_get_duration_string(PlayerContext *player);         // Get the duration string of the player
void player_set_mute(PlayerContext *player, gboolean mute);       // Set the mute state of the player
gboolean player_get_mute(PlayerContext *player);                  // Get the mute state of the player
gchar *player_get_transport_state(PlayerContext *player);         // Get the transport state of the player

/* 元数据相关函数声明 */
void player_set_qq_song_id(PlayerContext *player, const gchar *song_id);              // 设置当前播放的QQ音乐歌曲ID
gchar *player_get_qq_song_id(PlayerContext *player);                                  // 获取当前播放的QQ音乐歌曲ID
void player_set_current_track_duration(PlayerContext *player, const gchar *duration); // 设置当前曲目的时长
gchar *player_get_current_track_duration(PlayerContext *player);                      // 获取当前曲目的时长
void player_set_current_track_title(PlayerContext *player, const gchar *title);       // 设置当前曲目的标题
void player_set_current_track_artist(PlayerContext *player, const gchar *artist);     // 设置当前曲目的艺术家
void player_set_current_track_album(PlayerContext *player, const gchar *album);       // 设置当前曲目的专辑名
void player_set_metadata(PlayerContext *player, const gchar *metadata);               // 设置原始元数据
gchar *player_get_metadata(PlayerContext *player);                                    // 获取原始元数据
void player_set_next_uri(PlayerContext *player, const gchar *uri);                    // 设置下一个播放URI
gchar *player_get_next_uri(PlayerContext *player);                                    // 获取下一个播放URI
void player_set_next_metadata(PlayerContext *player, const gchar *metadata);          // 设置下一个曲目元数据

#endif // __ZERO_PLAYER_PLAYER_H__
