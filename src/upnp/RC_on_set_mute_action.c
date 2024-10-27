/**
 * @brief 设置静音
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"

void on_set_mute_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;

  gint mute;
  gupnp_service_action_get(action,
                           "DesiredMute", G_TYPE_INT, &mute,
                           NULL);

  g_print("SetMute: %d\n", mute);

  player_set_mute(appContext->player_context, mute);

  gupnp_service_action_return_success(action);
}