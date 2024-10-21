/**
 * @file upnp_util.c
 * @brief UPnP 工具函数
 */

#include "upnp_service.h"

/**
 * @brief 获取UPnP上下文的IP地址
 *
 * @param context GUPnPContext指针
 * @return const char* IP地址字符串
 */
const char *gupnp_context_get_host_ip(GUPnPContext *context)
{
  return gssdp_client_get_host_ip(GSSDP_CLIENT(context));
}