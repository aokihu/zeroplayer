/**
 * Application Structs
 * @file struct.h
 * @author aokihu <aokihu@gmail.com>
 */
#ifndef __ZERO_PLAYER_STRUCT_H__
#define __ZERO_PLAYER_STRUCT_H__

#include <glib.h>
#include <gst/gst.h>
#include <libgupnp/gupnp.h>

/**
 * Cli Options
 * @property debug_mode - Debug Mode
 * @property alsa_device - ALSA Device
 * @property device_name - Device Name
 * @property show_version - Show Version
 * @property xml_path - XML Path
 * @property port - UPnP Service Port
 */
typedef struct _CliOptions
{
  gboolean debug_mode;
  gchar *alsa_device;
  gchar *device_name;
  gboolean show_version;
  gchar *xml_path;
  gint port;
} CliOptions;

/**
 * UPnP Context
 * @property port - UPnP Service Port
 * @property id - UPnP ID
 */
typedef struct
{
  GUPnPContextManager *upnp_manager;
  gint nextConnectionID;
  gchar *currentConnectionIDs;
} UPnPContext;

// 播放器元数据结构体
typedef struct
{
  gchar *metadata;   // 元数据
  gchar *qq_song_id; // QQ音乐歌曲ID
  gchar *duration;   // 时长
  gchar *title;      // 标题
  gchar *artist;     // 艺术家
  gchar *album;      // 专辑
} PlayerMetadata;

/**
 * Player Context
 * @property uri - URI
 * @property metadata - 当前资源元数据
 * @property qq_song_id - 当前资源QQ音乐ID
 * @property current_track_duration - 当前资源时长
 * @property current_track_title - 当前资源标题
 * @property current_track_artist - 当前资源艺术家
 * @property current_track_album - 当前资源专辑
 * @property next_uri - 下一个资源
 * @property next_metadata - 下一个资源元数据
 * @property pipeline - GStreamer Pipeline
 */
typedef struct
{
  GstElement *pipeline;                  // GStreamer管道
  guint volume;                          // 音量
  gboolean mute;                         // 静音
  gchar *uri;                            // 当前播放URI
  gchar *next_uri;                       // 下一个播放URI
  PlayerMetadata current_track_metadata; // 当前曲目元数据
  PlayerMetadata next_track_metadata;    // 下一个曲目元数据
} PlayerContext;

/**
 * App Context
 * @property main_loop - GLib Main Loop
 * @property player_context - Player Context
 * @property upnp_context - UPnP Context
 */
typedef struct
{
  CliOptions cli_options;
  GMainLoop *main_loop;
  PlayerContext *player_context;
  UPnPContext *upnp_context;
} AppContext;

#endif // __ZERO_PLAYER_STRUCT_H__