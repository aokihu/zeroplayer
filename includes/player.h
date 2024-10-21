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

/* Function Declarations */
PlayerContext *player_new(void);                                  // Create a new player context
void player_free(PlayerContext *player);                          // Free the player context
gchar *player_get_uri(PlayerContext *player);                     // Get the URI of the player
void player_set_uri(PlayerContext *player, const gchar *uri);     // Set the URI of the player
void player_play(PlayerContext *player);                          // Play the player
void player_pause(PlayerContext *player);                         // Pause the player
void player_stop(PlayerContext *player);                          // Stop the player
void player_seek(PlayerContext *player, gint64 position);         // Seek to a specific position
void player_set_volume(PlayerContext *player, gdouble volume);    // Set the volume of the player
gdouble player_get_volume(PlayerContext *player);                 // Get the volume of the player
void player_set_position(PlayerContext *player, gint64 position); // Set the position of the player
gint64 player_get_position(PlayerContext *player);                // Get the position of the player
void player_set_mute(PlayerContext *player, gboolean mute);       // Set the mute state of the player
gboolean player_get_mute(PlayerContext *player);                  // Get the mute state of the player

#endif // __ZERO_PLAYER_PLAYER_H__