/**
 * @file upnp_av_transport.c
 * @brief UPnP AV Transport
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"

// 实际的函数实现
void on_av_transport_service_available(
    GUPnPRootDevice *rd,
    gpointer user_data)
{
  g_print("AV Transport Service is available\n");
}
