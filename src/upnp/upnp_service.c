/**
 * @file upnp_service.c
 * @brief UPnP Service
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_util.h"
#include "upnp_service.h"
#include "app_context.h"
#include <glib.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <libgupnp/gupnp.h>
#include <libgupnp/gupnp-service-info.h>

/**
 * @brief Initialize UPnP service
 *
 * @param app_context Application context pointer
 * @return UPnPContext* UPnP context pointer
 */
void upnp_service_init(struct AppContext *app_context)
{
  g_print("Initializing UPnP service By Manager\n");

  GUPnPContextManager *manager = gupnp_context_manager_create(0);
  g_signal_connect(manager, "context-available", G_CALLBACK(upnp_on_context_available), app_context);
  g_signal_connect(manager, "context-unavailable", G_CALLBACK(upnp_on_context_unavailable), app_context);
  g_print("UPnP context manager created\n");
}

/**
 * @brief Clean up UPnP service
 *
 * @param upnp_context UPnP context pointer
 */
void upnp_service_cleanup(UPnPContext *upnp_context)
{
  // 简单的占位实现
  if (upnp_context)
  {
    free(upnp_context);
  }
}

/**
 * @brief 当UPnP上下文可用时调用
 *
 * @param manager GUPnPContextManager指针
 * @param context GUPnPContext指针
 * @param user_data 用户数据, 指向AppContext结构体指针
 */
void upnp_on_context_available(GUPnPContextManager *manager, GUPnPContext *context, gpointer user_data)
{
  const char *ip = gupnp_context_get_host_ip(context);
  g_print("UPnP context available IP: %s\n", ip);

  GError *error = NULL;

  // 创建根设备
  GUPnPRootDevice *rd = gupnp_root_device_new(context, "Description.xml", "./XML", &error);
  gupnp_context_manager_manage_root_device(manager, rd);

  // 创建 AVTransport 服务
  on_avt_service_available(rd, user_data);

  // 创建 ConnectionManager 服务
  on_cm_service_available(rd, user_data);

  // 设置根设备可用
  gupnp_root_device_set_available(rd, TRUE);

  // 释放根设备
  g_object_unref(rd);
}

/**
 * @brief 当UPnP上下文不可用时调用
 *
 * @param manager GUPnPContextManager指针
 * @param context GUPnPContext指针
 * @param user_data 用户数据, 指向AppContext结构体指针
 */
void upnp_on_context_unavailable(GUPnPContextManager *manager,
                                 GUPnPContext *context,
                                 gpointer user_data)
{
  g_print("UPnP context unavailable\n");
}
