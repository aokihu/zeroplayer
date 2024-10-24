/**
 * @file upnp_av_transport_actions.c
 * @brief UPnP AV Transport Actions
 * @author aokihu <aokihu@gmail.com>
 */

#include <gst/gst.h>
#include <libgupnp/gupnp.h>
#include <libgupnp-av/gupnp-av.h>
#include "upnp_callback.h"
#include "player.h"
#include "struct.h"
#include <glib.h>

/**
 * @brief 设置媒体URI的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_set_av_transport_uri_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)

{
    AppContext *appContext = (AppContext *)user_data;
    gchar *instanceId = NULL;         // 实例ID
    gchar *currentUri = NULL;         // 当前URI
    gchar *currentUriMetaData = NULL; // 当前URI元数据

    // 从动作中获取参数
    gupnp_service_action_get(action,
                             "InstanceID", G_TYPE_STRING, &instanceId,
                             "CurrentURI", G_TYPE_STRING, &currentUri,
                             "CurrentURIMetaData", G_TYPE_STRING, &currentUriMetaData,
                             NULL);

    g_print("CurrentURI: %s\n", currentUri);
    g_print("CurrentURIMetaData: %s\n", currentUriMetaData);

    // 检查参数是否有效
    if (instanceId == NULL || currentUri == NULL)
    {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // 解析MetaData
    GUPnPDIDLLiteParser *parser = gupnp_didl_lite_parser_new();
    GError *error = NULL;

    GList *items = gupnp_didl_lite_parser_parse_didl(parser, currentUriMetaData, &error);
    if (error != NULL)
    {
        g_print("Error parsing DIDL: %s\n", error->message);
        g_error_free(error);
        goto cleanup;
    }

    for (GList *item = items; item != NULL; item = g_list_next(item))
    {
        GUPnPDIDLLiteObject *object = (GUPnPDIDLLiteObject *)item->data;

        // 获取对象的资源列表
        GList *resources = gupnp_didl_lite_object_get_resources(object);

        for (GList *resource = resources; resource != NULL; resource = g_list_next(resource))
        {
            const gchar *duration = gupnp_didl_lite_resource_get_duration(resource);
            g_print("Duration: %s\n", duration);
        }
    }

    // 更新状态变量
    gupnp_service_notify(service, "AVTransportURI", G_TYPE_STRING, currentUri, NULL);
    gupnp_service_notify(service, "AVTransportURIMetaData", G_TYPE_STRING, currentUriMetaData ? currentUriMetaData : "", NULL);

    // 更新传输状态为TRANSITIONING
    gupnp_service_notify(service, "TransportState", G_TYPE_STRING, "TRANSITIONING", NULL);

    // 在这里添加实际的媒体加载逻辑
    player_set_uri(appContext->player_context, currentUri);

    // 更新传输状态为STOPPED (假设媒体已加载但尚未播放)
    gupnp_service_notify(service, "TransportState", G_TYPE_STRING, "STOPPED", NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    // 释放资源
    g_free(instanceId);
    g_free(currentUri);
    g_free(currentUriMetaData);
}

/**
 * @brief 获取设备能力的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_get_device_capabilities_action(
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
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // 设置设备能力
    const gboolean playMedia = TRUE;                  // 本设备支持播放功能
    const gboolean recMedia = FALSE;                  // 本设备不支持录制功能
    const gchar *recQualityModes = "NOT_IMPLEMENTED"; // 本设备不支持录制功能

    // 返回设备能力
    gupnp_service_action_set(action,
                             "PlayMedia", G_TYPE_BOOLEAN, playMedia,
                             "RecMedia", G_TYPE_BOOLEAN, recMedia,
                             "RecQualityModes", G_TYPE_STRING, recQualityModes,
                             NULL);
    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    // 释放资源
    g_free(instanceId);
}

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
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }

    // 检查当前是否有媒体加载
    if (player_get_uri(appContext->player_context) == NULL)
    {
        gupnp_service_action_return_error(action, 701, "转换无效");
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

/**
 * @brief 暂停的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_pause_action(
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

    // Check if media is currently playing
    if (player_get_uri(appContext->player_context) == NULL)
    {
        gupnp_service_action_return_error(action, 701, "Transition not available");
        goto cleanup;
    }

    // Pause playback
    player_pause(appContext->player_context);

    // Update transport state to PAUSED_PLAYBACK
    gupnp_service_notify(service, "TransportState", G_TYPE_STRING, "PAUSED_PLAYBACK", NULL);

    // Action completed successfully
    gupnp_service_action_return_success(action);

cleanup:
    // Free resources
    g_free(instanceId);
}

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
    if (instanceId == NULL || unit == NULL || target == NULL)
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

/**
 * @brief 获取当前Transport Actions的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_get_current_transport_actions_action(
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
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // 获取当前可用的传输操作
    const gchar *currentTransportActions = "PLAY,PAUSE,STOP,SEEK"; // 示例操作，实际操作根据需要调整

    // 返回当前传输操作
    gupnp_service_action_set(action,
                             "CurrentTransportActions", G_TYPE_STRING, currentTransportActions,
                             NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    // 释放资源
    g_free(instanceId);
}

/**
 * @brief 获取传输信息的动作回函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
void on_get_transport_info_action(
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
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // 获取当前传输状态
    const gchar *currentTransportState = player_get_transport_state(appContext->player_context);
    const gchar *currentTransportStatus = "OK";   // 假设状态正常
    const gchar *currentTransportPlaySpeed = "1"; // 假设播放速度为1

    // 返回传输信息
    gupnp_service_action_set(action,
                             "CurrentTransportState", G_TYPE_STRING, currentTransportState,
                             "CurrentTransportStatus", G_TYPE_STRING, currentTransportStatus,
                             "CurrentTransportPlaySpeed", G_TYPE_STRING, currentTransportPlaySpeed,
                             NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    // 释放资源
    g_free(instanceId);
}

/**
 * @brief 获取传输设置的动作回调函数
 */
void on_get_transport_settings_action(
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
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }

    // 设置传输设置
    const gchar *playMode = "NORMAL";                // 播放模式
    const gchar *recQualityMode = "NOT_IMPLEMENTED"; // 录制质量模式

    // 返回传输设置
    gupnp_service_action_set(action,
                             "PlayMode", G_TYPE_STRING, playMode,
                             "RecQualityMode", G_TYPE_STRING, recQualityMode,
                             NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    g_free(instanceId);
}

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
    gchar *trackDuration = player_get_duration_string(appContext->player_context); // 曲目时长
    gchar *trackMetaData = "";                                                     // 曲目元数据
    gchar *trackURI = player_get_uri(appContext->player_context);                  // 曲目URI
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

/**
 * @brief 设置下一个AV传输URI的动作回调函数
 */
void on_set_next_av_transport_uri_action(
    GUPnPService *service,
    GUPnPServiceAction *action,
    gpointer user_data)
{
    AppContext *appContext = (AppContext *)user_data;
    gchar *instanceId = NULL;      // 实例ID
    gchar *nextUri = NULL;         // 下一个URI
    gchar *nextUriMetaData = NULL; // 下一个URI元数据

    // 从动作中获取参数
    gupnp_service_action_get(action,
                             "InstanceID", G_TYPE_STRING, &instanceId,
                             "NextURI", G_TYPE_STRING, &nextUri,
                             "NextURIMetaData", G_TYPE_STRING, &nextUriMetaData,
                             NULL);

    // 检查参数是否有效
    if (instanceId == NULL)
    {
        gupnp_service_action_return_error(action, 402, "Invalid Args: InstanceID");
        goto cleanup;
    }

    // 保存下一曲的URI和元数据
    appContext->player_context->next_uri = g_strdup(nextUri);
    appContext->player_context->next_metadata = g_strdup(nextUriMetaData);

    // 更新状态变量
    gupnp_service_notify(service, "NextAVTransportURI", G_TYPE_STRING, nextUri, NULL);
    gupnp_service_notify(service, "NextAVTransportURIMetaData", G_TYPE_STRING, nextUriMetaData ? nextUriMetaData : "", NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    g_free(instanceId);
    g_free(nextUri);
    g_free(nextUriMetaData);
}

/**
 * @brief 获取媒体信息的动作回调函数
 */
void on_get_media_info_action(
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
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }

    // 获取媒体信息
    guint nrTracks = 1;                                             // 曲目数量
    gchar *mediaDuration = "00:00:00";                              // 媒体时长
    gchar *currentUri = player_get_uri(appContext->player_context); // 当前URI
    gchar *currentUriMetaData = "";                                 // 当前URI元数据
    gchar *nextUri = "";                                            // 下一个URI
    gchar *nextUriMetaData = "";                                    // 下一个URI元数据
    gchar *playMedium = "NETWORK";                                  // 播放介质
    gchar *recordMedium = "NOT_IMPLEMENTED";                        // 录制介质
    gchar *writeStatus = "NOT_IMPLEMENTED";                         // 写入状态

    // 返回媒体信息
    gupnp_service_action_set(action,
                             "NrTracks", G_TYPE_UINT, nrTracks,
                             "MediaDuration", G_TYPE_STRING, mediaDuration,
                             "CurrentURI", G_TYPE_STRING, currentUri,
                             "CurrentURIMetaData", G_TYPE_STRING, currentUriMetaData,
                             "NextURI", G_TYPE_STRING, nextUri,
                             "NextURIMetaData", G_TYPE_STRING, nextUriMetaData,
                             "PlayMedium", G_TYPE_STRING, playMedium,
                             "RecordMedium", G_TYPE_STRING, recordMedium,
                             "WriteStatus", G_TYPE_STRING, writeStatus,
                             NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    g_free(instanceId);
}

/**
 * @brief 获取媒体信息扩展的动作回调函数
 */
void on_get_media_info_ext_action(
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
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }

    // 获取媒体信息
    gchar *currentType = "TRACK_AWARE";                             // 当前类型
    guint nrTracks = 1;                                             // 曲目数量
    gchar *mediaDuration = "00:00:00";                              // 媒体时长
    gchar *currentUri = player_get_uri(appContext->player_context); // 当前URI
    gchar *currentUriMetaData = "";                                 // 当前URI元数据
    gchar *nextUri = "";                                            // 下一个URI
    gchar *nextUriMetaData = "";                                    // 下一个URI元数据
    gchar *playMedium = "NETWORK";                                  // 播放介质
    gchar *recordMedium = "NOT_IMPLEMENTED";                        // 录制介质
    gchar *writeStatus = "NOT_IMPLEMENTED";                         // 写入状态

    // 返回媒体信息
    gupnp_service_action_set(action,
                             "CurrentType", G_TYPE_STRING, currentType,
                             "NrTracks", G_TYPE_UINT, nrTracks,
                             "MediaDuration", G_TYPE_STRING, mediaDuration,
                             "CurrentURI", G_TYPE_STRING, currentUri,
                             "CurrentURIMetaData", G_TYPE_STRING, currentUriMetaData,
                             "NextURI", G_TYPE_STRING, nextUri,
                             "NextURIMetaData", G_TYPE_STRING, nextUriMetaData,
                             "PlayMedium", G_TYPE_STRING, playMedium,
                             "RecordMedium", G_TYPE_STRING, recordMedium,
                             "WriteStatus", G_TYPE_STRING, writeStatus,
                             NULL);

    // 动作成功完成
    gupnp_service_action_return_success(action);

cleanup:
    g_free(instanceId);
}

/**
 * @brief 下一曲的动作回调函数
 */
void on_next_action(
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
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }

    // TODO: 实现下一曲功能
    // 目前返回错误，因为还未实现播放列表功能
    gupnp_service_action_return_error(action, 701, "转换无效");

cleanup:
    g_free(instanceId);
}

/**
 * @brief 上一曲的动作回调函数
 */
void on_previous_action(
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
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }

    // TODO: 实现上一曲功能
    // 目前返回错误，因为还未实现播放列表功能
    gupnp_service_action_return_error(action, 701, "转换无效");

cleanup:
    g_free(instanceId);
}
