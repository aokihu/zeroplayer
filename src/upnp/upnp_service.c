/**
 * @file upnp_service.c
 * @brief UPnP Service
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "app_context.h"
#include <glib.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <libgupnp/gupnp.h>
#include <libgupnp/gupnp-service-info.h>

static void on_service_available(GUPnPContextManager *manager, GUPnPContext *context, gpointer user_data);
static void on_service_unavailable(GUPnPContextManager *manager, GUPnPContext *context, gpointer user_data);

/**
 * @brief 支持的协议信息
 */
const gchar *UPNP_SERVICE_SINK_PROTOCOL[] = {
    "http-get:*:audio/mpeg:*",
    "http-get:*:audio/mp3:*",
    "http-get:*:audio/aac:*",
    "http-get:*:audio/m4a:*",
    "http-get:*:audio/wav:*",
    "http-get:*:audio/x-wav:*",
    "http-get:*:audio/flac:*",
    "http-get:*:audio/x-flac:*",
    "http-get:*:audio/ogg:*",
    NULL
};

/**
 * @brief 创建UPnP上下文
 * @return UPnP上下文
 */
void upnp_service_init(AppContext *app_context)
{
  UPnPContext *upnp_context = g_new0(UPnPContext, 1);
  app_context->upnp_context = upnp_context;
  app_context->upnp_context->nextConnectionID = 0;
  app_context->upnp_context->currentConnectionIDs = g_strdup("");


  // 初始化UPnP
  GUPnPContextManager *manager = gupnp_context_manager_create(0);
  app_context->upnp_context->upnp_manager = manager;

  g_signal_connect(manager, "context-available", G_CALLBACK(on_service_available), app_context);
  g_signal_connect(manager, "context-unavailable", G_CALLBACK(on_service_unavailable), app_context);

  g_print("UPnP Service initialized\n");
}

/**
 * @brief 释放UPnP上下文
 * @param app_context 应用程序上下文
 */
void upnp_service_cleanup(AppContext *app_context)
{
  g_object_unref(app_context->upnp_context->upnp_manager);
  g_free(app_context->upnp_context);
}

/**
 * @brief 服务可用回调函数
 * @param manager UPnP上下文管理器
 * @param context UPnP上下文
 * @param user_data 用户数据(AppContext 应用程序上下文)
 */
static void on_service_available(
    GUPnPContextManager *manager,
    GUPnPContext *context,
    gpointer user_data)
{
  // 打印监听的端口
  const char *host_ip = gupnp_context_get_host_ip(context);
  g_print("Service is available, listening on port %s\n", host_ip);

  GError *error = NULL;

  GUPnPRootDevice *root_device = gupnp_root_device_new(context, "description.xml", "./XML", &error);
  if (error)
  {
    g_printerr("Error creating root device: %s\n", error->message);
    g_error_free(error);
    return;
  }
  gupnp_context_manager_manage_root_device(manager, root_device);

  // 服务设置
  on_connection_manager_service_available(root_device, user_data);

  gupnp_root_device_set_available(root_device, TRUE);

  g_object_unref(root_device);
}

/**
 * @brief 服务不可用回调函数
 * @param manager UPnP上下文管理器
 * @param context UPnP上下文
 * @param user_data 用户数据
 */
static void on_service_unavailable(
    GUPnPContextManager *manager,
    GUPnPContext *context,
    gpointer user_data)
{
  g_print("Service is unavailable\n");
}