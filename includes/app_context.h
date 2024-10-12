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
#include <libgupnp/gupnp.h>

/**
 * @struct AppContext
 * @brief 应用程序上下文结构，用于保存全局状态
 */
typedef struct {
    GMainLoop *main_loop;           /**< GLib 主循环对象 */
    GUPnPContext *gupnp_context;    /**< GUPnP 上下文对象 */
    GUPnPRootDevice *root_device;   /**< GUPnP 根设备对象 */
    GUPnPServiceInfo *av_transport; /**< AV Transport 服务对象 */
    GUPnPServiceInfo *rendering_control; /**< Rendering Control 服务对象 */
    GstElement *player;             /**< GStreamer 播放器对象 */
    gchar *device_name;             /**< 设备显示名称 */
    gchar *alsa_device;             /**< ALSA 设备名称 */
    gboolean debug_mode;            /**< 调试模式标志 */
} AppContext;

/**
 * @brief 初始化应用程序上下文
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 初始化后的 AppContext 指针，失败时返回 NULL
 */
AppContext* init_app_context(int argc, char *argv[]);

/**
 * @brief 清理应用程序上下文，释放资源
 * @param context 要清理的 AppContext 指针
 */
void cleanup_app_context(AppContext *context);
