/**
 * @brief 设置音量
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"

void on_set_volume_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
  AppContext *appContext = (AppContext *)user_data;

  gint volume;

  // 从动作中获取参数
  gupnp_service_action_get(action,
                           "DesiredVolume", G_TYPE_INT, &volume,
                           NULL);

  player_set_volume(appContext->player_context, volume);

  gupnp_service_action_return_success(action);
}
