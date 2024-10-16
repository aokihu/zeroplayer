#ifndef UPNP_SERVICE_H
#define UPNP_SERVICE_H

#include <glib.h>
#include <libgupnp/gupnp.h>

// UPnP 上下文结构体
typedef struct
{
  GUPnPRootDevice *rootDevice;   /**< GUPnP 根设备对象 */
  GUPnPDeviceInfo *deviceInfo;   /**< GUPnP 设备信息对象 */
  gchar *friendlyName;           /**< 设备的友好名称 */
  guint port;                    /**< UPnP 服务的开放端口 */
  gchar *deviceDescriptionPath;  /**< 设备描述文件路径, 设置为NULL的情况时, 使用系统默认的设备描述文件 */
  gchar *serviceDescriptionPath; /**< 服务描述文件路径, 设置为NULL的情况时, 使用系统默认的服务描述文件 */
} UPnPContext;

struct AppContext; // Forward declaration

/**
 * @brief 初始化 UPnP 服务
 *
 * @param app_context 应用程序上下文
 * @return UPnPContext* UPnP 上下文指针，如果初始化失败则返回 NULL
 */
UPnPContext *upnp_service_init(struct AppContext *app_context);

/**
 * @brief 清理 UPnP 服务
 *
 * @param upnp_context UPnP 上下文指针
 */
void upnp_service_cleanup(UPnPContext *upnp_context);

/**
 * @brief 启动 UPnP 设备
 *
 * @param upnp_context UPnP 上下文指针
 * @return gboolean 启动是否成功
 */
gboolean upnp_service_start(UPnPContext *upnp_context);

/**
 * @brief 停止 UPnP 设备
 *
 * @param upnp_context UPnP 上下文指针
 */
void upnp_service_stop(UPnPContext *upnp_context);

/**
 * @brief 设置 UPnP 设备的友好名称
 *
 * @param upnp_context UPnP 上下文指针
 * @param friendly_name 友好名称
 */
void upnp_service_set_friendly_name(UPnPContext *upnp_context, const gchar *friendly_name);

/**
 * @brief 获取 UPnP 设备的友好名称
 *
 * @param upnp_context UPnP 上下文指针
 * @return const gchar* 友好名称
 */
const gchar *upnp_service_get_friendly_name(UPnPContext *upnp_context);

#endif /* UPNP_SERVICE_H */
