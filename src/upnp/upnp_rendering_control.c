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

    // 设置服务回调
    CONNECT_ACTION(GetVolume, on_get_volume_action);
    CONNECT_ACTION(SetVolume, on_set_volume_action);
    CONNECT_ACTION(GetMute, on_get_mute_action);
    CONNECT_ACTION(SetMute, on_set_mute_action);

    g_print("RenderingControl Ready.\n");
  }
  else
  {
    g_print("RenderingControl Not Ready.\n");
  }
}
