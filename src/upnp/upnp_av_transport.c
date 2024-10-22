/**
 * @file upnp_av_transport.c
 * @brief UPnP AV Transport
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "struct.h"

// 实际的函数实现
void on_av_transport_service_available(
    GUPnPRootDevice *rd,
    gpointer user_data)
{
 AppContext *appContext = (AppContext *)user_data; // 用户数据转换为应用程序上下文

  // 创建服务
  GUPnPServiceInfo *service_info = gupnp_device_info_get_service(GUPNP_DEVICE_INFO(rd), "urn:schemas-upnp-org:service:AVTransport:3");
  if (service_info)
  {
    GUPnPService *service = GUPNP_SERVICE(service_info);

    g_print("AVTransport Ready.\n");
  }
  else
  {
    g_print("AVTransport Not Ready.\n");
  }
}
