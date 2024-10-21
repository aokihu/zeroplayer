#ifndef UPNP_SERVICE_H
#define UPNP_SERVICE_H

#include <glib.h>
#include <libgupnp/gupnp.h>
#include "upnp_callback.h"

// UPnP 上下文结构体
typedef struct
{
  GUPnPContext *upnp_context;    /**< GUPnP 上下文对象 */
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
void upnp_service_init(struct AppContext *app_context);

/**
 * @brief 清理 UPnP 服务
 *
 * @param upnp_context UPnP 上下文指针
 */
void upnp_service_cleanup(UPnPContext *upnp_context);

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

/**
 * @brief 当UPnP上下文可用时调用
 *
 * @param manager GUPnPContextManager指针
 * @param context GUPnPContext指针
 * @param user_data 用户数据
 *
 * @details 当UPnP上下文可用时调用, 可以在这里进行一些初始化操作,
 *          比如创建UPnP设备, 创建UPnP服务等.
 */
void upnp_on_context_available(GUPnPContextManager *manager,
                               GUPnPContext *context,
                               gpointer user_data);

/**
 * @brief 当UPnP上下文不可用时调用
 *
 * @param manager GUPnPContextManager指针
 * @param context GUPnPContext指针
 * @param user_data 用户数据
 */
void upnp_on_context_unavailable(GUPnPContextManager *manager,
                                 GUPnPContext *context,
                                 gpointer user_data);

#endif /* UPNP_SERVICE_H */
