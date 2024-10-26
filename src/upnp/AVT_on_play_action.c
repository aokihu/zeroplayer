/**
 * @file AVT_on_play_action.c
 * @brief 播放动作处理函数
 */

#include <libgupnp/gupnp.h>
#include "struct.h"
#include "player.h"

/**
 * @brief 播放的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_play_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;
  gchar *instanceId = NULL; // 实例ID
  gchar *speed = NULL;      // 播放速度

  // 从动作中获取参数
  gupnp_service_action_get(action,
                           "InstanceID", G_TYPE_STRING, &instanceId,
                           "Speed", G_TYPE_STRING, &speed,
                           NULL);

  // 检查参数是否有效
  if (instanceId == NULL || speed == NULL)
  {
    gupnp_service_action_return_error(action, 402, "Invalid Args: InstanceID or Speed");
    goto cleanup;
  }

  // 检查当前是否有媒体加载
  if (player_get_uri(appContext->player_context) == NULL)
  {
    gupnp_service_action_return_error(action, 701, "No media loaded");
    goto cleanup;
  }

  // 开始播放
  player_play(appContext->player_context);

  // 更新传输状态为PLAYING
  gupnp_service_notify(service, "TransportState", G_TYPE_STRING, "PLAYING", NULL);

  // 更新播放速度
  gupnp_service_notify(service, "TransportPlaySpeed", G_TYPE_STRING, speed, NULL);

  // 动作成功完成
  gupnp_service_action_return_success(action);

cleanup:
  // 释放资源
  g_free(instanceId);
  g_free(speed);
}