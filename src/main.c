/**
 * @file main.c
 * @brief 主入口文件，包含主循环和初始化逻辑
 * @author aokihu <aokihu@gmail.com>
 * @version 0.1
 * @license BSD-3-Clause
 */

#include <glib.h>
#include <gst/gst.h>
#include "app_context.h"
#include "cli_parser.h"
#include "player.h"
#include "upnp_service.h"
/**
 * @brief 初始化应用程序
 * @param app_context 应用程序上下文
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return TRUE 成功，FALSE 失败
 */
static gboolean initialize_app(AppContext *app_context, int argc, char *argv[])
{
  // 初始化 GStreamer，并检查初始化是否成功
  GError *error = NULL;
  if (!gst_init_check(&argc, &argv, &error))
  {
    g_printerr("GStreamer initialization failed: %s\n", error->message);
    g_error_free(error);
    return FALSE;
  }

  // 初始化 CliOptions 结构体
  memset(&app_context->cli_options, 0, sizeof(CliOptions));

  // 解析命令行参数
  if (!parse_cli_options(argc, argv, &app_context->cli_options))
  {
    return FALSE;
  }

  // 初始化GStreamer播放器
  app_context->player_context = player_new();
  if (!app_context->player_context)
  {
    g_printerr("Failed to create GStreamer player\n");
    return FALSE;
  }

  // 初始化UPnP服务
  upnp_service_init(app_context);

  // 创建主循环
  app_context->main_loop = g_main_loop_new(NULL, FALSE);

  return TRUE;
}

/**
 * @brief 清理应用程序
 * @param app_context 应用程序上下文
 */
static void cleanup_app(AppContext *app_context)
{
  if (app_context)
  {
    g_main_loop_unref(app_context->main_loop);
    upnp_service_cleanup(app_context->upnp_context);
    player_free(app_context->player_context);
    free_cli_options(&app_context->cli_options);
  }
}

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 0 成功，其他值表示失败
 */
int main(int argc, char *argv[])
{
  // 创建应用程序上下文
  AppContext *app_context = g_new0(AppContext, 1);

  if (!initialize_app(app_context, argc, argv))
  {
    cleanup_app(app_context);
    g_printerr("Failed to initialize application\n");
    return -1;
  }

  g_print("ZeroPlayer is starting...\n");

  // 运行主循环
  g_main_loop_run(app_context->main_loop);

  g_print("ZeroPlayer is shutting down...\n");

  cleanup_app(app_context);
  return 0;
}