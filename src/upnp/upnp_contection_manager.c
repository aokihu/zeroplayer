/**
 * @file upnp_content_manager.c
 * @brief 内容管理器
 */

#include "app_context.h"
#include "upnp_service.h"
#include "upnp_callback.h"
#include <libgupnp/gupnp.h>
#include <string.h>

/**
 * @brief 服务可用回调函数
 * @param rd 根设备
 * @param user_data 用户数据
 */
void on_connection_manager_service_available(
    GUPnPRootDevice *rd,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data; // 用户数据转换为应用程序上下文

  // 创建服务
  GUPnPServiceInfo *service_info = gupnp_device_info_get_service(GUPNP_DEVICE_INFO(rd), "urn:schemas-upnp-org:service:ConnectionManager:1");
  if (service_info)
  {
    GUPnPService *service = GUPNP_SERVICE(service_info);

    CONNECT_ACTION(PrepareForConnection, on_prepare_for_connection_action);
    CONNECT_ACTION(GetProtocolInfo, on_get_protocol_info_action);
    CONNECT_ACTION(GetCurrentConnectionIDs, on_get_current_connection_ids_action);
    CONNECT_ACTION(GetCurrentConnectionInfo, on_get_current_connection_info_action);
    g_print("ConnectionManager Ready.\n");
  }
  else
  {
    g_print("ConnectionManager Not Ready.\n");
  }
}
