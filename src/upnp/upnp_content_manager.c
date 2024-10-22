/**
 * @file upnp_content_manager.c
 * @brief 内容管理器
 */

#include "upnp_callback.h"

void on_connection_manager_service_available(
    GUPnPRootDevice *rd,
    gpointer user_data)
{
  g_print("Connection Manager Service is available\n");
}
