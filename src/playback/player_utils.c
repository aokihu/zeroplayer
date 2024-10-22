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
gint64 *player_get_timestamp_from_string(gchar* timestamp)
{
    gint hours, minutes, seconds, milliseconds = 0;
    gint64 *result = g_malloc(sizeof(gint64)); // 分配内存

    if (sscanf(timestamp, "%d:%d:%d.%d", &hours, &minutes, &seconds, &milliseconds) >= 3) {
        *result = (gint64)hours * 3600000 + 
                  (gint64)minutes * 60000 + 
                  (gint64)seconds * 1000 + 
                  (gint64)milliseconds;
        return result;
    }

    g_free(result); // 如果转换失败,释放内存
    return NULL;
}

/**
 * 将gint64类型的时间戳转换为字符串
 * 
 * @param timestamp gint64类型的时间戳,单位为毫秒
 * @return gchar* 转换后的时间戳字符串,格式为"HH:MM:SS.mmm"
 */
gchar *player_util_get_timestamp_string(gint64 timestamp)
{
    gint hours, minutes, seconds, milliseconds;
    
    hours = (gint)(timestamp / 3600000);
    minutes = (gint)((timestamp % 3600000) / 60000);
    seconds = (gint)((timestamp % 60000) / 1000);
    milliseconds = (gint)(timestamp % 1000);

    return g_strdup_printf("%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
}

