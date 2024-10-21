#ifndef __ZERO_PLAYER_UPNP_SERVICE_H__
#define __ZERO_PLAYER_UPNP_SERVICE_H__

#include <glib.h>
#include <libgupnp/gupnp.h>
#include <libgupnp/gupnp.h>
#include <libgupnp/gupnp-service-info.h>
#include "struct.h"

/* ------------ 工具方法 ------------ */

/* 获取IP地址 */
const char *gupnp_context_get_host_ip(
    GUPnPContext *context // UPnP上下文
);

/* ------------ 服务方法 ------------ */

/* UPnP服务初始化 */
void upnp_service_init(
    AppContext *app_context // 应用程序上下文
);

/* UPnP服务清理 */
void upnp_service_cleanup(
    AppContext *app_context // 应用程序上下文
);

/* UPnP服务停止 */
void upnp_service_stop(
    AppContext *app_context // 应用程序上下文
);

/* 设置UPnP设备的友好名称 */
void upnp_service_set_friendly_name(
    AppContext *app_context,   // 应用程序上下文
    const gchar *friendly_name // 友好名称
);

/* 获取UPnP设备的友好名称 */
const gchar *upnp_service_get_friendly_name(
    AppContext *app_context // 应用程序上下文
);

/* 当UPnP上下文可用时调用 */
void upnp_on_context_available(
    GUPnPContextManager *manager, // GUPnP上下文管理器
    GUPnPContext *context,        // GUPnP上下文
    gpointer user_data            // 用户数据
);

/* 当UPnP上下文不可用时调用 */
void upnp_on_context_unavailable(
    GUPnPContextManager *manager, // GUPnP上下文管理器
    GUPnPContext *context,        // GUPnP上下文
    gpointer user_data            // 用户数据
);

#endif /* __ZERO_PLAYER_UPNP_SERVICE_H__ */
