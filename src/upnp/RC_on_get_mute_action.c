/**
 * @brief 获取静音状态
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"

void on_get_mute_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;

  gint mute = appContext->player_context->mute;

  gupnp_service_action_set(action,
                           "Channel", G_TYPE_STRING, "Master",
                           "CurrentMute", G_TYPE_INT, mute,
                           NULL);

  g_print("GetMute: %d\n", mute);

  gupnp_service_action_return_success(action);
}
