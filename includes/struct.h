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

/**
 * Player Context
 * @property uri - URI
 * @property next_uri - Next URI
 * @property pipeline - GStreamer Pipeline
 */
typedef struct
{
  gchar *uri;
  gchar *next_uri;
  GstElement *pipeline;
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