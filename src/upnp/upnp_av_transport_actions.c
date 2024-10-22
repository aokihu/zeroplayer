/**
 * @file upnp_av_transport_actions.c
 * @brief UPnP AV Transport Actions
 * @author aokihu <aokihu@gmail.com>
 */

#include <gst/gst.h>
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
    gchar *instanceId = NULL; // 实例ID
    gchar *currentUri = NULL; // 当前URI
    gchar *currentUriMetaData = NULL; // 当前URI元数据
    
    // 从动作中获取参数
    gupnp_service_action_get(action,
                             "InstanceID", G_TYPE_STRING, &instanceId,
                             "CurrentURI", G_TYPE_STRING, &currentUri,
                             "CurrentURIMetaData", G_TYPE_STRING, &currentUriMetaData,
                             NULL);
    
    // 检查参数是否有效
    if (instanceId == NULL || currentUri == NULL) {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
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
    if (instanceId == NULL) {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }
    
    // 设置设备能力
    const gboolean playMedia = TRUE; // 本设备支持播放功能
    const gboolean recMedia = FALSE; // 本设备不支持录制功能
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
    gchar *speed = NULL; // 播放速度
    
    // 从动作中获取参数
    gupnp_service_action_get(action,
                             "InstanceID", G_TYPE_STRING, &instanceId,
                             "Speed", G_TYPE_STRING, &speed,
                             NULL);
    
    // 检查参数是否有效
    if (instanceId == NULL || speed == NULL) {
        gupnp_service_action_return_error(action, 402, "无效参数");
        goto cleanup;
    }
    
    // 检查当前是否有媒体加载
    if (player_get_uri(appContext->player_context) == NULL) {
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
    if (instanceId == NULL) {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // Check if media is currently playing
    if (player_get_uri(appContext->player_context) == NULL) {
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
    if (instanceId == NULL) {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // Check if media is currently loaded
    if (player_get_uri(appContext->player_context) == NULL) {
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
    gchar *unit = NULL; // Seek mode
    gchar *target = NULL; // Target position

    // Get parameters from the action
    gupnp_service_action_get(action,
                             "InstanceID", G_TYPE_STRING, &instanceId,
                             "Unit", G_TYPE_STRING, &unit,
                             "Target", G_TYPE_STRING, &target,
                             NULL);

    // Check if parameters are valid
    if (instanceId == NULL || unit == NULL || target == NULL) {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // Check if media is currently loaded
    if (player_get_uri(appContext->player_context) == NULL) {
        gupnp_service_action_return_error(action, 701, "Transition not available");
        goto cleanup;
    }

    // Handle different seek modes
    if (g_strcmp0(unit, "REL_TIME") == 0) {
        // Relative time seek
        gint64 target_ns = player_get_timestamp_from_string(target);
        if (target_ns == -1) {
            gupnp_service_action_return_error(action, 402, "Invalid Args");
            goto cleanup;
        }
        player_seek(appContext->player_context, target_ns);
    } else if (g_strcmp0(unit, "ABS_TIME") == 0) {
        // Absolute time seek
        gint64 target_ns = player_get_timestamp_from_string(target);
        if (target_ns == -1) {
            gupnp_service_action_return_error(action, 402, "Invalid Args");
            goto cleanup;
        }
        player_set_position(appContext->player_context, target_ns);
    } else {
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
 * @brief 获取状态变量的动作回调函数
 * @param service 服务
 * @param action 动作
 * @param user_data 用户数据
 */
// void on_get_state_variables_action(
//     GUPnPService *service,
//     GUPnPServiceAction *action,
//     gpointer user_data)
// {
//     AppContext *appContext = (AppContext *)user_data;
//     gchar *instanceId = NULL; // Instance ID
//     gchar *stateVariableList = NULL; // List of state variables to retrieve

//     // Get parameters from the action
//     gupnp_service_action_get(action,
//                              "InstanceID", G_TYPE_STRING, &instanceId,
//                              "StateVariableList", G_TYPE_STRING, &stateVariableList,
//                              NULL);

//     // Check if parameters are valid
//     if (instanceId == NULL || stateVariableList == NULL) {
//         gupnp_service_action_return_error(action, 402, "Invalid Args");
//         goto cleanup;
//     }

//     // Parse the state variable list
//     gchar **variables = g_strsplit(stateVariableList, ",", -1);
//     GString *stateVariableValuePairs = g_string_new("");

//     for (int i = 0; variables[i] != NULL; i++) {
//         const gchar *value = gupnp_service_get_state_variable(service, g_strstrip(variables[i]));
//         if (value != NULL) {
//             g_string_append_printf(stateVariableValuePairs, "%s=%s,", variables[i], value);
//         }
//     }

//     // Remove the trailing comma
//     if (stateVariableValuePairs->len > 0) {
//         g_string_truncate(stateVariableValuePairs, stateVariableValuePairs->len - 1);
//     }

//     // Set the action return value
//     gupnp_service_action_set(action,
//                              "StateVariableValuePairs", G_TYPE_STRING, stateVariableValuePairs->str,
//                              NULL);

//     // Action completed successfully
//     gupnp_service_action_return_success(action);

//     g_string_free(stateVariableValuePairs, TRUE);
//     g_strfreev(variables);

// cleanup:
//     // Free resources
//     g_free(instanceId);
//     g_free(stateVariableList);
// }

// /**
//  * @brief 设置状态变量的动作回调函数
//  * @param service 服务
//  * @param action 动作
//  * @param user_data 用户数据
//  */
// void on_set_state_variables_action(
//     GUPnPService *service,
//     GUPnPServiceAction *action,
//     gpointer user_data)
// {
//     AppContext *appContext = (AppContext *)user_data;
//     gchar *instanceId = NULL; // Instance ID
//     gchar *stateVariableValuePairs = NULL; // State variable value pairs to set

//     // Get parameters from the action
//     gupnp_service_action_get(action,
//                              "InstanceID", G_TYPE_STRING, &instanceId,
//                              "StateVariableValuePairs", G_TYPE_STRING, &stateVariableValuePairs,
//                              NULL);

//     // Check if parameters are valid
//     if (instanceId == NULL || stateVariableValuePairs == NULL) {
//         gupnp_service_action_return_error(action, 402, "Invalid Args");
//         goto cleanup;
//     }

//     // Parse the state variable value pairs
//     gchar **pairs = g_strsplit(stateVariableValuePairs, ",", -1);
//     GString *stateVariableList = g_string_new("");

//     for (int i = 0; pairs[i] != NULL; i++) {
//         gchar **pair = g_strsplit(pairs[i], "=", 2);
//         if (pair[0] != NULL && pair[1] != NULL) {
//             gchar *variable = g_strstrip(pair[0]);
//             gchar *value = g_strstrip(pair[1]);

//             // Attempt to set the state variable
//             if (gupnp_service_set_state_variable(service, variable, value)) {
//                 g_string_append_printf(stateVariableList, "%s,", variable);
//             }
//         }
//         g_strfreev(pair);
//     }

//     // Remove the trailing comma
//     if (stateVariableList->len > 0) {
//         g_string_truncate(stateVariableList, stateVariableList->len - 1);
//     }

//     // Set the action return value
//     gupnp_service_action_set(action,
//                              "StateVariableList", G_TYPE_STRING, stateVariableList->str,
//                              NULL);

//     // Action completed successfully
//     gupnp_service_action_return_success(action);

//     g_string_free(stateVariableList, TRUE);
//     g_strfreev(pairs);

// cleanup:
//     // Free resources
//     g_free(instanceId);
//     g_free(stateVariableValuePairs);
// }
