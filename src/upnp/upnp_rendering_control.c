/**
 * @file upnp_rendering_control.c
 * @brief UPnP Rendering Control
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"

void on_rendering_control_service_available(
    GUPnPRootDevice *rd,
    gpointer user_data)
{
  g_print("Rendering Control Service is available\n");
}
