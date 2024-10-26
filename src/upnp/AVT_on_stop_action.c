
#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"

/**
 * @brief 停止的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_stop_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;
  gchar *instanceId = NULL; // Instance ID

  // Get parameters from the action
  gupnp_service_action_get(action,
                           "InstanceID", G_TYPE_STRING, &instanceId,
                           NULL);

  // Check if parameters are valid
  if (instanceId == NULL)
  {
    gupnp_service_action_return_error(action, 402, "Invalid Args");
    goto cleanup;
  }

  // Check if media is currently loaded
  if (player_get_uri(appContext->player_context) == NULL)
  {
    gupnp_service_action_return_error(action, 701, "Transition not available");
    goto cleanup;
  }

  // Stop playback
  player_stop(appContext->player_context);

  // Update transport state to STOPPED
  gupnp_service_notify(service, "TransportState", G_TYPE_STRING, "STOPPED", NULL);

  // Reset current track to 0
  gupnp_service_notify(service, "CurrentTrack", G_TYPE_UINT, 0, NULL);

  // Action completed successfully
  gupnp_service_action_return_success(action);

cleanup:
  // Free resources
  g_free(instanceId);
}