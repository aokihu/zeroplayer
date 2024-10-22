/**
 * @file upnp_connection_manager_actions.c
 * @brief UPnP连接管理动作实现
 */

#include "upnp_service.h"
#include "struct.h"


/**
 * @brief 获取协议信息动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_get_protocol_info_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  // 定义支持的协议信息
  const char *sourceProtocolInfo = g_strjoinv(",", UPNP_SERVICE_SINK_PROTOCOL);
  const char *sinkProtocolInfo = ""; // 我们的设备不支持接收，所以为空

  // 设置动作的返回值
  gupnp_service_action_set(action,
                           "Source", G_TYPE_STRING, sourceProtocolInfo,
                           "Sink", G_TYPE_STRING, sinkProtocolInfo,
                           NULL);

  // 返回成功
  gupnp_service_action_return_success(action);
}

/**
 * @brief 获取当前连接ID动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_get_current_connection_ids_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  // 假设我们只有一个连接，ID为0
  const char *connectionIDs = "0";

  // 设置动作的返回值
  gupnp_service_action_set(action,
                           "ConnectionIDs", G_TYPE_STRING, connectionIDs,
                           NULL);

  // 返回成功
  gupnp_service_action_return_success(action);
}


/**
 * @brief 获取当前连接信息动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_get_current_connection_info_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  // 获取输入参数
  gint connectionID;
  gupnp_service_action_get(action,
                           "ConnectionID", G_TYPE_INT, &connectionID,
                           NULL);

  // 检查连接ID是否有效（在这个例子中，我们只支持ID为0的连接）
  if (connectionID != 0)
  {
    gupnp_service_action_return_error(action, 706, "无效的连接ID");
    return;
  }

  // 设置动作的返回值
  gupnp_service_action_set(action,
                           "RcsID", G_TYPE_INT, -1,
                           "AVTransportID", G_TYPE_INT, 0,
                           "ProtocolInfo", G_TYPE_STRING, "http-get:*:audio/mpeg:*",
                           "PeerConnectionManager", G_TYPE_STRING, "",
                           "PeerConnectionID", G_TYPE_INT, -1,
                           "Direction", G_TYPE_STRING, "Output",
                           "Status", G_TYPE_STRING, "OK",
                           NULL);

  // 返回成功
  gupnp_service_action_return_success(action);
}

/**
 * @brief Prepare for connection action callback function
 * @param service Service
 * @param action Action
 * @param user_data User data
 */
void on_prepare_for_connection_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;
  gchar *remoteProtocolInfo;
  gchar *peerConnectionManager;
  gint peerConnectionID;
  gchar *direction;

  // Get input parameters
  gupnp_service_action_get(action,
                           "RemoteProtocolInfo", G_TYPE_STRING, &remoteProtocolInfo,
                           "PeerConnectionManager", G_TYPE_STRING, &peerConnectionManager,
                           "PeerConnectionID", G_TYPE_INT, &peerConnectionID,
                           "Direction", G_TYPE_STRING, &direction,
                           NULL);

  g_print("Preparing connection: RemoteProtocolInfo=%s, PeerConnectionManager=%s, PeerConnectionID=%d, Direction=%s\n",
          remoteProtocolInfo, peerConnectionManager, peerConnectionID, direction);

  // Check if protocol is compatible
  gboolean protocolCompatible = FALSE;
  for (gint i = 0; UPNP_SERVICE_SINK_PROTOCOL[i] != NULL; i++) {
    if (g_str_has_prefix(remoteProtocolInfo, UPNP_SERVICE_SINK_PROTOCOL[i])) {
      protocolCompatible = TRUE;
      break;
    }
  }

  if (!protocolCompatible) {
    gupnp_service_action_return_error(action, 701, "Incompatible protocol");
    return;
  }

  // Check if direction is correct
  if (g_strcmp0(direction, "Input") != 0) {
    gupnp_service_action_return_error(action, 702, "Unsupported flow direction");
    return;
  }

  // Generate new connection ID
  gint newConnectionID = appContext->upnp_context->nextConnectionID++;
  gint rcsID = 0;  // Assume RCS ID is always 0
  gint avTransportID = newConnectionID;  // Assume AVTransport ID is the same as Connection ID

  // Set action return values
  gupnp_service_action_set(action,
                           "ConnectionID", G_TYPE_INT, newConnectionID,
                           "AVTransportID", G_TYPE_INT, avTransportID,
                           "RcsID", G_TYPE_INT, rcsID,
                           NULL);

  // Update current connection ID list
  gchar *currentConnectionIDs = g_strdup_printf("%s%s%d", 
                                                appContext->upnp_context->currentConnectionIDs,
                                                (appContext->upnp_context->currentConnectionIDs[0] != '\0') ? "," : "",
                                                newConnectionID);
  g_free(appContext->upnp_context->currentConnectionIDs);
  appContext->upnp_context->currentConnectionIDs = currentConnectionIDs;

  // Notify that ConnectionIDs have changed
  gupnp_service_notify(service, "CurrentConnectionIDs", G_TYPE_STRING, currentConnectionIDs, NULL);

  // Return success
  gupnp_service_action_return_success(action);

  g_free(remoteProtocolInfo);
  g_free(peerConnectionManager);
  g_free(direction);
}