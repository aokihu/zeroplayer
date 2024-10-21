#include "upnp_callback.h"

const gchar *SOURCE_PROTOCOL_INFO = "http-get:*:audio/mpeg:*,http-get:*:audio/aac:*,"
                                    "rtsp-rtp-udp:*:audio/mpeg:*,rtsp-rtp-udp:*:audio/aac:*,"
                                    "udp:*:audio/mpeg:*,udp:*:audio/aac:*,rtmp:*:audio/mpeg:*,"
                                    "http-get:*:application/vnd.apple.mpegurl:*";

// ----------------------------------------- //
//                                           //
//          ConnectionManager::Service       //
//                                           //
// ----------------------------------------- //

/**
 * @brief ConnectionManager 服务可用时的回调函数实现
 * @param rd GUPnPRootDevice指针
 * @param user_data 用户数据 AppContext指针
 */
void on_cm_service_available(GUPnPRootDevice *rd, gpointer user_data)
{

  // 创建服务
  GUPnPServiceInfo *service_info = gupnp_device_info_get_service(GUPNP_DEVICE_INFO(rd), "urn:schemas-upnp-org:service:ConnectionManager:3");
  if (service_info)
  {
    GUPnPService *service = GUPNP_SERVICE(service_info);

    g_signal_connect(service, "action-invoked::GetProtocolInfo", G_CALLBACK(on_get_protocol_info_action), user_data);
    g_signal_connect(service, "query-variable::SourceProtocolInfo", G_CALLBACK(on_get_source_protocol_info_query), user_data);
    g_signal_connect(service, "query-variable::SinkProtocolInfo", G_CALLBACK(on_get_sink_protocol_info_query), user_data);
    g_signal_connect(service, "query-variable::CurrentConnectionIDs", G_CALLBACK(on_get_current_connection_ids_query), user_data);

    g_print("ConnectionManager Service created\n");
  }
  else
  {
    g_print("未找到 ConnectionManager 服务\n");
  }
}

// ----------------------------------------- //
//                                           //
//          ConnectionManager::Action        //
//                                           //
// ----------------------------------------- //

/**
 * @brief 获取源协议信息的动作回调函数实现
 * @param service GUPnP服务
 * @param action GUPnP服务动作
 * @param user_data 用户数据
 */
void on_get_protocol_info_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)
{
  // 处理获取源协议信息的动作请求
  g_print("GetProtocolInfo Action invoked: %s\n", gupnp_service_action_get_name(action));

  GValue source = G_VALUE_INIT;
  g_value_init(&source, G_TYPE_STRING);

  g_value_set_string(&source, SOURCE_PROTOCOL_INFO);

  gupnp_service_action_set_value(action, "Source", &source);

  gupnp_service_action_return_success(action);

  g_value_unset(&source);
}

// ----------------------------------------- //
//                                           //
//          ConnectionManager::Query         //
//                                           //
// ----------------------------------------- //

/**
 * @brief 获取源协议信息的查询回调函数实现
 * @param service GUPnP服务
 * @param variable 查询的变量
 * @param value 查询的值
 * @param user_data 用户数据
 */
void on_get_source_protocol_info_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data)
{
  g_value_init(value, G_TYPE_STRING);
  g_value_set_string(value, SOURCE_PROTOCOL_INFO); // 返回源协议信息
}

/**
 * @brief 获取接收器协议信息的查询回调函数实现
 * @param service GUPnP服务
 * @param variable 查询的变量
 * @param value 查询的值
 * @param user_data 用户数据
 */
void on_get_sink_protocol_info_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data)
{
  // 处理获取接收器协议信息的查询请求
  g_value_init(value, G_TYPE_STRING);
  g_value_set_string(value, "SinkProtocolInfo"); // 返回接收器协议信息
}

/**
 * @brief 获取当前连接ID的查询回调函数实现
 * @param service GUPnP服务
 * @param variable 查询的变量
 * @param value 查询的值
 * @param user_data 用户数据
 */
void on_get_current_connection_ids_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data)
{
  // 处理获取当前连接ID的查询请求
  g_value_init(value, G_TYPE_STRING);
  g_value_set_string(value, "CurrentConnectionIDs"); // 返回当前连接ID
}

// ----------------------------------------- //
//                                           //
//              其他回调函数实现           //
//                                           //
// ----------------------------------------- //
