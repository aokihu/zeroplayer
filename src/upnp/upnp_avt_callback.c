#include "upnp_service.h"
#include "upnp_callback.h"
#include <glib.h>

// ----------------------------------------- //
//                                           //
//              AVTransport                  //
//                                           //
// ----------------------------------------- //

/**
 * @brief AVTransport 服务可用时的回调函数
 * @param rd GUPnPRootDevice指针
 * @param user_data 用户数据 AppContext指针
 */
void on_avt_service_available(GUPnPRootDevice *rd, gpointer user_data)
{

  // 创建服务
  GUPnPServiceInfo *service_info = gupnp_device_info_get_service(GUPNP_DEVICE_INFO(rd), "urn:schemas-upnp-org:service:AVTransport:3");
  if (service_info)
  {
    GUPnPService *service = GUPNP_SERVICE(service_info);

    g_signal_connect(service, "query-variable::TransportStatus", G_CALLBACK(on_get_transport_status_query), user_data);
    g_signal_connect(service, "query-variable::TransportState", G_CALLBACK(on_get_transport_state_query), user_data);

    g_print("AVTransport Service created\n");
  }
  else
  {
    g_print("未找到 AVTransport 服务\n");
  }
}

/**
 * @brief 播放的回调函数实现
 * @param service GUPnP服务
 * @param action GUPnP服务动作
 * @param user_data 用户数据
 */
void on_play_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)
{
  // 处理播放请求的基本逻辑
  // ... 这里添加播放逻辑 ...

  // 设置动作结果
  gupnp_service_action_return_success(action);
}

/**
 * @brief 暂停的回调函数实现
 * @param service GUPnP服务
 * @param action GUPnP服务动作
 * @param user_data 用户数据
 */
void on_pause_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)
{
  // 处理暂停请求的基本逻辑
  // ... 这里添加暂停逻辑 ...

  // 设置动作结果
  gupnp_service_action_return_success(action);
}

/**
 * @brief 停止的回调函数实现
 * @param service GUPnP服务
 * @param action GUPnP服务动作
 * @param user_data 用户数据
 */
void on_stop_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)
{
  // 处理停止请求的基本逻辑
  // ... 这里添加停止逻辑 ...

  // 设置动作结果
  gupnp_service_action_return_success(action);
}

/**
 * @brief 获取传输状态的查询回调函数
 * @param service GUPnP服务
 * @param variable 查询的变量
 * @param value 查询的值
 * @param user_data 用户数据
 */
void on_get_transport_state_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data)
{
  g_value_init(value, G_TYPE_STRING);
  g_value_set_string(value, "NO_MEDIA_PRESENT");
}

void on_get_transport_status_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data)
{
  g_value_init(value, G_TYPE_STRING);
  g_value_set_string(value, "OK");
}