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
  GUPnPServiceInfo *service_info = gupnp_device_info_get_service(GUPNP_DEVICE_INFO(rd),
                                                                 "urn:schemas-upnp-org:service:AVTransport:3");

  if (service_info)
  {
    GUPnPService *service = GUPNP_SERVICE(service_info);
    CONNECT_ACTION(SetAVTransportURI, on_set_av_transport_uri_action);
    CONNECT_ACTION(GetDeviceCapabilities, on_get_device_capabilities_action);
    CONNECT_ACTION(Play, on_play_action);
    CONNECT_ACTION(Pause, on_pause_action);
    CONNECT_ACTION(Stop, on_stop_action);
    CONNECT_ACTION(Seek, on_seek_action);
    CONNECT_ACTION(GetCurrentTransportActions, on_get_current_transport_actions_action);
    CONNECT_ACTION(GetTransportInfo, on_get_transport_info_action);
    CONNECT_ACTION(GetTransportSettings, on_get_transport_settings_action);
    CONNECT_ACTION(GetPositionInfo, on_get_position_info_action);
    CONNECT_ACTION(SetNextAVTransportURI, on_set_next_av_transport_uri_action);
    CONNECT_ACTION(GetMediaInfo, on_get_media_info_action);
    CONNECT_ACTION(GetMediaInfoExt, on_get_media_info_ext_action);
    CONNECT_ACTION(Next, on_next_action);
    CONNECT_ACTION(Previous, on_previous_action);
    g_print("AVTransport Ready.\n");
  }
  else
  {
    g_print("AVTransport Not Ready.\n");
  }
}
