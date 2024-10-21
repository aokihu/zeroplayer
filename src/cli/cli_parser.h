#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <glib.h>

typedef struct
{
  gboolean debug_mode;     // 调试模式标志
  gchar *config_file_path; // 配置文件路径
  gchar *alsa_device;      // ALSA设备名称
  gchar *device_name;      // 设备显示名称
  gboolean show_version;   // 显示版本标志
  gchar *xml_path;         // UPnP服务XML文件路径
  gint port;               // UPnP服务端口
} CliOptions;

#endif // CLI_PARSER_H
