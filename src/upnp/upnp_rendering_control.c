/**
 * @file upnp_rendering_control.c
 * @brief UPnP Rendering Control
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "struct.h"

void on_rendering_control_service_available(
    GUPnPRootDevice *rd,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data; // 用户数据转换为应用程序上下文

  // 创建服务
  GUPnPServiceInfo *service_info = gupnp_device_info_get_service(GUPNP_DEVICE_INFO(rd), "urn:schemas-upnp-org:service:RenderingControl:1");
  if (service_info)
  {
    GUPnPService *service = GUPNP_SERVICE(service_info);

    g_print("RenderingControl Ready.\n");
  }
  else
  {
    g_print("RenderingControl Not Ready.\n");
  }
}
