/**
 * @brief 跳转的动作回调函数
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"

/**
 * @brief 跳转的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_seek_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;
  gchar *instanceId = NULL; // Instance ID
  gchar *unit = NULL;       // Seek mode
  gchar *target = NULL;     // Target position

  // Get parameters from the action
  gupnp_service_action_get(action,
                           "InstanceID", G_TYPE_STRING, &instanceId,
                           "Unit", G_TYPE_STRING, &unit,
                           "Target", G_TYPE_STRING, &target,
                           NULL);

  // Check if parameters are valid
  if (unit == NULL || target == NULL)
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

  // Handle different seek modes
  if (g_strcmp0(unit, "REL_TIME") == 0)
  {
    // Relative time seek
    gint64 target_ns = player_get_timestamp_from_string(target);
    if (target_ns == -1)
    {
      gupnp_service_action_return_error(action, 402, "Invalid Args");
      goto cleanup;
    }
    player_seek(appContext->player_context, target_ns);
  }
  else if (g_strcmp0(unit, "ABS_TIME") == 0)
  {
    // Absolute time seek
    gint64 target_ns = player_get_timestamp_from_string(target);
    if (target_ns == -1)
    {
      gupnp_service_action_return_error(action, 402, "Invalid Args");
      goto cleanup;
    }
    player_set_position(appContext->player_context, target_ns);
  }
  else
  {
    // Unsupported seek mode
    gupnp_service_action_return_error(action, 710, "Seek mode not supported");
    goto cleanup;
  }

  // Update relative time position
  gint64 position = player_get_position(appContext->player_context);
  gchar *position_str = player_util_get_timestamp_string(position);
  gupnp_service_notify(service, "RelativeTimePosition", G_TYPE_STRING, position_str, NULL);
  g_free(position_str);

  // Action completed successfully
  gupnp_service_action_return_success(action);

cleanup:
  // Free resources
  g_free(instanceId);
  g_free(unit);
  g_free(target);
}