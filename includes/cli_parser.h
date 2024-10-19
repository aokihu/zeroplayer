#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <glib.h>

typedef struct _CliOptions
{
  gboolean debug_mode;
  gchar *config_file_path;
  gchar *alsa_device;
  gchar *device_name;
  gboolean show_version;
  gchar *xml_path;
  gint port;
} CliOptions;

/**
 * @brief 解析命令行参数
 *
 * @param argc 参数数量
 * @param argv 参数数组
 * @param options 用于存储解析结果的CliOptions结构体指针
 * @return gboolean 解析是否成功
 */
gboolean parse_cli_options(int argc, char *argv[], CliOptions *options);

/**
 * @brief 打印帮助信息
 */
void print_help(void);

/**
 * @brief 打印版本信息
 */
void print_version(void);

/**
 * @brief 释放CliOptions结构体中的动态分配内存
 *
 * @param options 需要清理的CliOptions结构体指针
 */
void free_cli_options(CliOptions *options);

#endif /* CLI_PARSER_H */
