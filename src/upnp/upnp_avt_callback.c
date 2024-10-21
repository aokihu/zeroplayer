#include "upnp_service.h"
#include "upnp_callback.h"
#include "app_context.h"
#include "gstreamer_player.h"
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

    g_signal_connect(service, "action-invoked::SetAVTransportURI", G_CALLBACK(on_set_av_transport_uri_action), user_data);
    g_signal_connect(service, "action-invoked::Play", G_CALLBACK(on_play_action), user_data);
    g_signal_connect(service, "action-invoked::Pause", G_CALLBACK(on_pause_action), user_data);
    g_signal_connect(service, "action-invoked::Stop", G_CALLBACK(on_stop_action), user_data);

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
 * @brief 设置AVTransportURI的回调函数实现
 * @param service GUPnP服务
 * @param action GUPnP服务动作
 * @param user_data 用户数据
 */
void on_set_av_transport_uri_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)
{
  g_print("SetAVTransportURI Action invoked: %s\n", gupnp_service_action_get_name(action));

  // 初始化GValue
  GValue uri = G_VALUE_INIT;
  g_value_init(&uri, G_TYPE_STRING);

  // 获取CurrentURI参数
  gupnp_service_action_get_value(action, "CurrentURI", &uri);
  g_print("URI: %s\n", g_value_get_string(&uri));

  // 保存到AppContext中
  AppContext *app_context = (AppContext *)user_data;
  gstreamer_player_set_uri(app_context->player, g_value_get_string(&uri));

  // 清理GValue
  g_value_unset(&uri);

  // 设置动作结果
  gupnp_service_action_return_success(action);
}

/**
 * @brief 播放的回调函数实现
 * @param service GUPnP服务
 * @param action GUPnP服务动作
 * @param user_data 用户数据
 */
void on_play_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data)
{
  AppContext *app_context = (AppContext *)user_data;
  struct GstreamerPlayer *player = app_context->player;

  gstreamer_player_play(player);

  // 设置动作结果
  gupnp_service_action_return_success(action);

  g_print("Play Action invoked: %s\n", gupnp_service_action_get_name(action));

  // 更新 TransportState 状态变量
  // gupnp_service_notify(service, "TransportState", G_TYPE_STRING, "PLAYING", NULL);
  GValue value = G_VALUE_INIT;
  g_value_init(&value, G_TYPE_STRING);
  g_value_set_string(&value, "PLAYING");
  gupnp_service_notify_value(service, "TransportState", &value);
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