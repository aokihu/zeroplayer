/**
 * @file app_context.h
 * @brief 应用程序上下文
 * @author aokihu <aokihu@gmail.com>
 * @version 0.1
 * @license BSD-3-Clause
 */

#pragma once

#include <glib.h>
#include <gst/gst.h>
#include "cli_parser.h"
#include "upnp_service.h"
// #include "gstreamer_player.h"

struct GstreamerPlayer GstreamerPlayer; // Forward declaration

/**
 * @struct AppContext
 * @brief 应用程序上下文结构，用于保存全局状态
 */
typedef struct _AppContext
{
    GMainLoop *main_loop;           /**< GLib 主循环对象 */
    struct GstreamerPlayer *player; /**< GStreamer 播放器对象 */
    gchar *alsa_device;             /**< ALSA 设备名称 */
    gboolean debug_mode;            /**< 调试模式标志 */
    CliOptions cli_options;         /**< 命令行选项 */
    UPnPContext *upnp_context;      /**< UPnP 服务上下文 */
} AppContext;

/**
 * @brief 初始化应用程序上下文
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 初始化后的 AppContext 指针，失败时返回 NULL
 */
AppContext *initAppContext(int argc, char *argv[]);

/**
 * @brief 清理应用程序上下文，释放资源
 * @param context 要清理的 AppContext 指针
 */
void cleanupAppContext(AppContext *context);
