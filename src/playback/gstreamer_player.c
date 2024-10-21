#include "gstreamer_player.h"
#include <stdlib.h>

/**
 * @brief 创建新的GstreamerPlayer实例
 * @return GstreamerPlayer* 返回GstreamerPlayer实例指针
 */
struct GstreamerPlayer *gstreamer_player_new(void)
{
  // 简单的占位实现
  struct GstreamerPlayer *player = (struct GstreamerPlayer *)malloc(sizeof(struct GstreamerPlayer));
  if (!player) // 检查内存分配是否成功
  {
    return NULL; // 返回NULL表示创建失败
  }

  // 初始化GstElement
  player->pipeline = gst_element_factory_make("playbin3", PLAYBIN_NAME);
  if (!player->pipeline)
  {
    g_printerr("Failed to create playbin3 element\n");
    free(player);
    return NULL;
  }

  // 设置Bus
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(player->pipeline));

  return player;
}

// 释放GstreamerPlayer实例
void gstreamer_player_free(struct GstreamerPlayer *player)
{
  // 简单的占位实现
  if (player)
  {
    // 这里应该添加清理GstElement的代码
    free(player); // 释放内存
  }
}

gboolean gstreamer_player_set_uri(struct GstreamerPlayer *player, const gchar *uri)
{
  // 保存URI
  player->uri = g_strdup(uri);

  // 检查pipeline是否已经设置
  if (player->pipeline)
  {
    g_object_set(player->pipeline, "uri", uri, NULL);
  }

  return TRUE;
}

/**
 * @brief 开始播放
 *
 * @param player 播放器实例指针
 * @return gboolean 播放是否成功启动
 */
gboolean gstreamer_player_play(struct GstreamerPlayer *player)
{
  gst_element_set_state(player->pipeline, GST_STATE_PLAYING);
  return TRUE;
}
