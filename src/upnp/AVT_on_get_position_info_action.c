/**
 * @file on_set_av_transport_uri_action.c
 * @brief 获取位置信息的动作回调函数
 * @author aokihu <aokihu@gmail.com>
 */

#include <glib.h>
#include "struct.h"
#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"


/**
 * @brief 获取位置信息的动作回调函数
 */
void on_get_position_info_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
    AppContext *appContext = (AppContext *)user_data;
    gchar *instanceId = NULL; // 实例ID

    // 从动作中获取参数
    gupnp_service_action_get(action,
                             "InstanceID", G_TYPE_STRING, &instanceId,
                             NULL);

    // 检查参数是否有效
    if (instanceId == NULL)
    {
        gupnp_service_action_return_error(action, 402, "Invalid Args: InstanceID");
        goto cleanup;
    }

    // 获取位置信息
    guint track = 1;                                                               // 当前曲目编号
    gchar *trackDuration = player_get_current_track_duration(appContext->player_context);     // 曲目时长
    gchar *trackMetaData = player_get_metadata(appContext->player_context);                   // 曲目元数据
    gchar *trackURI = appContext->player_context->uri;                             // 曲目URI
    gchar *relTime = "00:00:00";                                                   // 相对时间
    gchar *absTime = "00:00:00";                                                   // 绝对时间
    gint relCount = 0;                                                             // 相对计数
    gint absCount = 0;                                                             // 绝对计数

    // 返回位置信息
    gupnp_service_action_set(action,
                             "Track", G_TYPE_UINT, track,
                             "TrackDuration", G_TYPE_STRING, trackDuration,
                             "TrackMetaData", G_TYPE_STRING, trackMetaData,
                             "TrackURI", G_TYPE_STRING, trackURI,
                             "RelTime", G_TYPE_STRING, relTime,
                             "AbsTime", G_TYPE_STRING, absTime,
                             "RelCount", G_TYPE_INT, relCount,
                             "AbsCount", G_TYPE_INT, absCount,
                             NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    g_free(trackDuration);
    g_free(instanceId);
}