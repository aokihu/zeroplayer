#include "cli_parser.h"
#include "version.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>

static const GOptionEntry entries[] = {
    {"debug", 'd', 0, G_OPTION_ARG_NONE, NULL, "Enable debug mode", NULL},
    {"output-alsa-device", 0, 0, G_OPTION_ARG_STRING, NULL, "Set ALSA device", "DEVICE"},
    {"name", 'n', 0, G_OPTION_ARG_STRING, NULL, "Set device display name", "NAME"},
    {"version", 'v', 0, G_OPTION_ARG_NONE, NULL, "Print current software version", NULL},
    {"xml-path", 'x', 0, G_OPTION_ARG_STRING, NULL, "Specify UPnP service XML file path", "PATH"},
    {"port", 'p', 0, G_OPTION_ARG_INT, NULL, "Specify UPnP service port", "PORT"},
    {NULL}};

gboolean parse_cli_options(int argc, char *argv[], CliOptions *options)
{
  GError *error = NULL;
  GOptionContext *context;

  if (!options)
  {
    g_printerr("Error: Invalid options pointer\n");
    return FALSE;
  }

  // 初始化选项结构体
  memset(options, 0, sizeof(CliOptions));

  // 创建 entries[] 的局部副本
  GOptionEntry local_entries[G_N_ELEMENTS(entries)];
  memcpy(local_entries, entries, sizeof(entries));

  // 设置选项数据指针
  local_entries[0].arg_data = &options->debug_mode;
  local_entries[1].arg_data = &options->alsa_device;
  local_entries[2].arg_data = &options->device_name;
  local_entries[3].arg_data = &options->show_version;
  local_entries[4].arg_data = &options->xml_path; // 新增的xml_path选项
  local_entries[5].arg_data = &options->port;

  // 创建选项上下文
  context = g_option_context_new("- ZeroPlayer UPnP Audio Renderer");
  g_option_context_add_main_entries(context, local_entries, NULL);
  g_option_context_set_help_enabled(context, TRUE);

  // 解析命令行选项
  if (!g_option_context_parse(context, &argc, &argv, &error))
  {
    g_printerr("Option parsing failed: %s\n", error->message);
    g_error_free(error);
    g_option_context_free(context);
    return FALSE;
  }

  // 如果指定了版本选项，打印版本并退出
  if (options->show_version)
  {
    print_version();
    g_option_context_free(context);
    return FALSE;
  }

  if (options->debug_mode)
  {
    g_print("Debug mode enabled\n");
  }

  if (options->alsa_device)
  {
    g_print("ALSA device: %s\n", options->alsa_device);
  }

  if (options->device_name)
  {
    g_print("Device name: %s\n", options->device_name);
  }

  if (options->xml_path)
  {
    g_print("XML file path: %s\n", options->xml_path);
  }

  if (options->port)
  {
    g_print("UPnP service port: %d\n", options->port);
  }

  g_option_context_free(context);
  return TRUE;
}

void print_help(void)
{
  GOptionContext *context;
  context = g_option_context_new("- ZeroPlayer UPnP Audio Renderer");
  g_option_context_add_main_entries(context, entries, NULL);
  g_print("%s", g_option_context_get_help(context, TRUE, NULL));
  g_option_context_free(context);
}

void print_version(void)
{
  g_print("ZeroPlayer version %s (v%d.%d.%d)\n",
          ZEROPLAYER_VERSION_STRING,
          ZEROPLAYER_VERSION_MAJOR,
          ZEROPLAYER_VERSION_MINOR,
          ZEROPLAYER_VERSION_PATCH);
}

void free_cli_options(CliOptions *options)
{
  if (options)
  {
    // 释放动态分配的内存
    g_free(options->alsa_device);
    g_free(options->device_name);
    g_free(options->xml_path); // 新增释放xml_path
    memset(options, 0, sizeof(CliOptions));
  }
}
