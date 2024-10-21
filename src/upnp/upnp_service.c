/**
 * @file upnp_service.c
 * @brief UPnP Service
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_service.h"
#include "app_context.h"
#include <glib.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <libgupnp/gupnp.h>
#include <libgupnp/gupnp-service-info.h>

/**
 * @brief 创建UPnP上下文
 * @return UPnP上下文
 */
void upnp_service_init(AppContext *app_context)
{
  UPnPContext *upnp_context = g_new0(UPnPContext, 1);
  app_context->upnp_context = upnp_context;
  app_context->upnp_context->id = 0;

  // 初始化UPnP
  GUPnPContextManager *upnp_manager = gupnp_context_manager_create_full(GSSDP_UDA_VERSION_1_1, G_SOCKET_FAMILY_IPV4, 0);
  app_context->upnp_context->upnp_manager = upnp_manager;
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