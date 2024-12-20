/**
 * @file upnp_callback.h
 * @brief UPnP Callback Functions
 * @author aokihu <aokihu@gmail.com>
 */

#ifndef __UPNP_CALLBACK_H__
#define __UPNP_CALLBACK_H__

#include <libgupnp/gupnp.h>

// ----------------------------------------- //
//                                           //
//              Common Callback              //
//                                           //
// ----------------------------------------- //

/* 服务可用回调函数类型定义 */
#define UPNP_SERVICE_AVAILABLE_CALLBACK(name) \
  void name(GUPnPRootDevice *rd, gpointer user_data)

/* 动作回调函数类型定义 */
#define UPNP_ACTION_CALLBACK(name) \
  void name(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)

/* 查询回调函数类型定义 */
#define UPNP_QUERY_CALLBACK(name) \
  void name(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data)

/* 信号连接函数统一声明 */
#define CONNECT_ACTION(action_name, callback) \
  g_signal_connect(service, "action-invoked::" #action_name, G_CALLBACK(callback), appContext)

/* 查询链接函数统一声明 */
#define CONNECT_QUERY(query_name, callback) \
  g_signal_connect(service, "query-variable::" #query_name, G_CALLBACK(callback), appContext)

// ----------------------------------------- //
//                                           //
//              ConnectionManager            //
//                                           //
// ----------------------------------------- //

/* Service Available Callback Functions */
UPNP_SERVICE_AVAILABLE_CALLBACK(on_connection_manager_service_available);

/* Action Callback Functions */

/* ConnectionManager 动作回调函数声明 */
UPNP_ACTION_CALLBACK(on_get_protocol_info_action);           // 获取协议信息的回调函数
UPNP_ACTION_CALLBACK(on_get_current_connection_ids_action);  // 获取当前连接ID的回调函数
UPNP_ACTION_CALLBACK(on_prepare_for_connection_action);      // 准备连接的回调函数
UPNP_ACTION_CALLBACK(on_get_current_connection_info_action); // 获取当前连接信息的回调函数

/* Query Callback Functions */
UPNP_QUERY_CALLBACK(on_get_source_protocol_info_query);   // 获取源协议信息的查询回调函数
UPNP_QUERY_CALLBACK(on_get_sink_protocol_info_query);     // 获取接收协议信息的查询回调函数
UPNP_QUERY_CALLBACK(on_get_current_connection_ids_query); // 获取当前连接ID的查询回调函数

// ----------------------------------------- //
//                                           //
//                 AVTransport               //
//                                           //
// ----------------------------------------- //

/* Service Available Callback Functions */
UPNP_SERVICE_AVAILABLE_CALLBACK(on_av_transport_service_available);

/* Action Callback Functions */
UPNP_ACTION_CALLBACK(on_set_av_transport_uri_action);          // 设置媒体URI的回调函数
UPNP_ACTION_CALLBACK(on_get_device_capabilities_action);       // 获取设备能力的回调函数
UPNP_ACTION_CALLBACK(on_play_action);                          // 播放的回调函数
UPNP_ACTION_CALLBACK(on_pause_action);                         // 暂停的回调函数
UPNP_ACTION_CALLBACK(on_stop_action);                          // 停止的回调函数
UPNP_ACTION_CALLBACK(on_seek_action);                          // 跳转的回调函数
UPNP_ACTION_CALLBACK(on_get_state_variables_action);           // 获取状态变量的回调函数
UPNP_ACTION_CALLBACK(on_set_state_variables_action);           // 设置状态变量的回调函数
UPNP_ACTION_CALLBACK(on_get_current_transport_actions_action); // 获取当前Transport Actions的回调函数
UPNP_ACTION_CALLBACK(on_get_transport_info_action);            // 获取传输信息的回调函数
UPNP_ACTION_CALLBACK(on_get_transport_settings_action);        // 获取传输设置的回调函数
UPNP_ACTION_CALLBACK(on_get_position_info_action);             // 获取位置信息的回调函数
UPNP_ACTION_CALLBACK(on_set_next_av_transport_uri_action);     // 设置下一个AV传输URI的回调函数
UPNP_ACTION_CALLBACK(on_get_media_info_action);                // 获取媒体信息的回调函数
UPNP_ACTION_CALLBACK(on_get_media_info_ext_action);            // 获取媒体信息扩展的回调函数
UPNP_ACTION_CALLBACK(on_next_action);                          // 下一曲的回调函数
UPNP_ACTION_CALLBACK(on_previous_action);                      // 上一曲的回调函数

/* Query Callback Functions */
UPNP_QUERY_CALLBACK(on_get_transport_state_query);        // 获取传输状态的查询回调函数
UPNP_QUERY_CALLBACK(on_get_transport_status_query);       // 获取传输状态的查询回调函数
UPNP_QUERY_CALLBACK(on_get_current_media_duration_query); // 获取当前媒体时长查询回调函数

// ----------------------------------------- //
//                                           //
//              RenderingControl             //
//                                           //
// ----------------------------------------- //

/* Service Available Callback Functions */
UPNP_SERVICE_AVAILABLE_CALLBACK(on_rendering_control_service_available);

/* Action Callback Functions */

/* RenderingControl 动作回调函数声明 */
UPNP_ACTION_CALLBACK(on_set_volume_action); // 设置音量的回调函数
UPNP_ACTION_CALLBACK(on_get_volume_action); // 获取音量的回调函数
UPNP_ACTION_CALLBACK(on_set_mute_action);   // 设置静音的回调函数
UPNP_ACTION_CALLBACK(on_get_mute_action);   // 获取静音状态的回调函数

/* Query Callback Functions */
/* RenderingControl 查询回调函数声明 */
UPNP_QUERY_CALLBACK(on_get_volume_query); // 获取音量的查询回调函数
UPNP_QUERY_CALLBACK(on_get_mute_query);   // 获取静音状态的查询回调函数

#endif // __UPNP_CALLBACK_H__
