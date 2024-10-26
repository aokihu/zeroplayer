/**
 * @file player_utils.c
 * @brief 播放器工具函数实现
 * @author AI助手
 */

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"

/**
 * 将时间戳字符串转换为gint64类型的时间戳
 *
 * @param timestamp 时间戳字符串,格式为"HH:MM:SS"或"HH:MM:SS.mmm"
 * @return gint64* 指向转换后的时间戳的指针,如果转换失败则返回NULL
 */
gint64 player_get_timestamp_from_string(gchar *timestamp)
{
    gint hours, minutes, seconds, milliseconds = 0;
    gint64 result = 0;

    if (sscanf(timestamp, "%d:%d:%d.%d", &hours, &minutes, &seconds, &milliseconds) >= 3)
    {
        result = (gint64)hours * 3600000 +
                 (gint64)minutes * 60000 +
                 (gint64)seconds * 1000 +
                 (gint64)milliseconds;
        return result;
    }

    return -1; // 如果转换失败,返回-1表示错误
}

/**
 * 将gint64类型的时间戳转换为字符串
 *
 * @param timestamp gint64类型的时间戳,单位为毫秒
 * @return gchar* 转换后的时间戳字符串,格式为"HH:MM:SS"
 */
gchar *player_util_get_timestamp_string(gint64 timestamp)
{
    gint hours, minutes, seconds, milliseconds;

    hours = (gint)(timestamp / 3600000);
    minutes = (gint)((timestamp % 3600000) / 60000);
    seconds = (gint)((timestamp % 60000) / 1000);

    return g_strdup_printf("%02d:%02d:%02d", hours, minutes, seconds);
}