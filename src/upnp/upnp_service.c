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

  GUPnPContextManager *manager = gupnp_context_manager_create(5050);
  g_signal_connect(manager, "context-available", G_CALLBACK(upnp_on_context_available), NULL);
  g_signal_connect(manager, "context-unavailable", G_CALLBACK(upnp_on_context_unavailable), NULL);
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
 * @brief 启动 UPnP 设备
 *
 * @param upnp_context UPnP 上下文指针
 * @return gboolean 启动是否成功
 */
gboolean upnp_service_start(UPnPContext *upnp_context)
{

  return TRUE;
}

/**
 * @brief 当UPnP上下文可用时调用
 *
 * @param manager GUPnPContextManager指针
 * @param context GUPnPContext指针
 * @param user_data 用户数据
 */
void upnp_on_context_available(GUPnPContextManager *manager,
                               GUPnPContext *context,
                               gpointer user_data)
{
  g_print("UPnP context available\n");
}

void upnp_on_context_unavailable(GUPnPContextManager *manager,
                                 GUPnPContext *context,
                                 gpointer user_data)
{
  g_print("UPnP context unavailable\n");
}
