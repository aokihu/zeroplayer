/**
 * @brief 获取音量
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"

void on_get_volume_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;

  player_get_volume(appContext->player_context);
  gint volume = appContext->player_context->volume;

  gupnp_service_action_set(action,
                           "Channel", G_TYPE_STRING, "Master",
                           "CurrentVolume", G_TYPE_INT, volume,
                           NULL);

  g_print("GetVolume: %d\n", volume);

  gupnp_service_action_return_success(action);
}
