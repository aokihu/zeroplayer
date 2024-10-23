/**
 * @file upnp_service.c
 * @brief UPnP Service
 * @author aokihu <aokihu@gmail.com>
 */

#include "upnp_callback.h"
#include "upnp_service.h"
#include "app_context.h"
#include <glib.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <libgupnp/gupnp.h>
#include <libgupnp/gupnp-service-info.h>

static void on_service_available(GUPnPContextManager *manager, GUPnPContext *context, gpointer user_data);
static void on_service_unavailable(GUPnPContextManager *manager, GUPnPContext *context, gpointer user_data);

/**
 * @brief 支持的协议信息
 */
const gchar *UPNP_SERVICE_SINK_PROTOCOL[] = {
    "http-get:*:application/dash+xml:*",
    "http-get:*:application/kate:*",
    "http-get:*:application/ogg:*",
    "http-get:*:application/vnd.ms-sstr+xml:*",
    "http-get:*:application/x-3gp:*",
    "http-get:*:application/x-apetag:*",
    "http-get:*:application/x-ass:*",
    "http-get:*:application/x-hls:*",
    "http-get:*:application/x-icy:*",
    "http-get:*:application/x-id3:*",
    "http-get:*:application/x-kate:*",
    "http-get:*:application/x-navi-animation:*",
    "http-get:*:application/x-ogg-avi:*",
    "http-get:*:application/x-ogm-audio:*",
    "http-get:*:application/x-ogm-text:*",
    "http-get:*:application/x-ogm-video:*",
    "http-get:*:application/x-rtcp:*",
    "http-get:*:application/x-rtcp-stream:*",
    "http-get:*:application/x-rtp:*",
    "http-get:*:application/x-rtp-stream:*",
    "http-get:*:application/x-srtcp:*",
    "http-get:*:application/x-srtcp-stream:*",
    "http-get:*:application/x-srtp:*",
    "http-get:*:application/x-srtp-stream:*",
    "http-get:*:application/x-ssa:*",
    "http-get:*:audio/*:*",
    "http-get:*:audio/AMR:*",
    "http-get:*:audio/AMR-WB:*",
    "http-get:*:audio/G722:*",
    "http-get:*:audio/G723:*",
    "http-get:*:audio/G729:*",
    "http-get:*:audio/L16;rate=44100;channels=2:*",
    "http-get:*:audio/ac3:*",
    "http-get:*:audio/alac:*",
    "http-get:*:audio/isac:*",
    "http-get:*:audio/m4a:*",
    "http-get:*:audio/mp4:*",
    "http-get:*:audio/mpeg:*",
    "http-get:*:audio/mp3:*",
    "http-get:*:audio/ogg:*",
    "http-get:*:audio/webm:*",
    "http-get:*:audio/x-ac3:*",
    "http-get:*:audio/x-adpcm:*",
    "http-get:*:audio/x-alac:*",
    "http-get:*:audio/x-alaw:*",
    "http-get:*:audio/x-amr-nb-sh:*",
    "http-get:*:audio/x-amr-wb-sh:*",
    "http-get:*:audio/x-au:*",
    "http-get:*:audio/x-bv:*",
    "http-get:*:audio/x-celt:*",
    "http-get:*:audio/x-dsd:*",
    "http-get:*:audio/x-dts:*",
    "http-get:*:audio/x-eac3:*",
    "http-get:*:audio/x-flac:*",
    "http-get:*:audio/x-gsm:*",
    "http-get:*:audio/x-iLBC:*",
    "http-get:*:audio/x-ldac:*",
    "http-get:*:audio/x-m4a:*",
    "http-get:*:audio/x-matroska:*",
    "http-get:*:audio/x-mpeg:*",
    "http-get:*:audio/x-mulaw:*",
    "http-get:*:audio/x-nellymoser:*",
    "http-get:*:audio/x-opus:*",
    "http-get:*:audio/x-pn-realaudio:*",
    "http-get:*:audio/x-private1-ac3:*",
    "http-get:*:audio/x-private1-dts:*",
    "http-get:*:audio/x-raw:*",
    "http-get:*:audio/x-rf64:*",
    "http-get:*:audio/x-sbc:*",
    "http-get:*:audio/x-scpls:*",
    "http-get:*:audio/x-siren:*",
    "http-get:*:audio/x-speex:*",
    "http-get:*:audio/x-tta:*",
    "http-get:*:audio/x-unaligned-raw:*",
    "http-get:*:audio/x-vorbis:*",
    "http-get:*:audio/x-wav:*",
    "http-get:*:audio/x-wavpack:*",
    "http-get:*:audio/x-wavpack-correction:*",
    "http-get:*:audio/x-wma:*",
    "http-get:*:closedcaption/x-cea-608:*",
    "http-get:*:closedcaption/x-cea-708:*",
    "http-get:*:meta/x-klv:*",
    "http-get:*:multipart/x-mixed-replace:*",
    "http-get:*:text/x-cmml:*",
    "http-get:*:text/x-raw:*",
    NULL};

/**
 * @brief 创建UPnP上下文
 * @return UPnP上下文
 */
void upnp_service_init(AppContext *app_context)
{
  UPnPContext *upnp_context = g_new0(UPnPContext, 1);
  app_context->upnp_context = upnp_context;
  app_context->upnp_context->nextConnectionID = 0;
  app_context->upnp_context->currentConnectionIDs = g_strdup("");

  // 初始化UPnP
  GUPnPContextManager *manager = gupnp_context_manager_create(0);
  app_context->upnp_context->upnp_manager = manager;

  g_signal_connect(manager, "context-available", G_CALLBACK(on_service_available), app_context);
  g_signal_connect(manager, "context-unavailable", G_CALLBACK(on_service_unavailable), app_context);

  g_print("UPnP Service initialized\n");
}

/**
 * @brief 释放UPnP上下文
 * @param app_context 应用程序上下文
 */
void upnp_service_cleanup(AppContext *app_context)
{
  g_object_unref(app_context->upnp_context->upnp_manager);
  g_free(app_context->upnp_context);
}

/**
 * @brief 服务可用回调函数
 * @param manager UPnP上下文管理器
 * @param context UPnP上下文
 * @param user_data 用户数据(AppContext 应用程序上下文)
 */
static void on_service_available(
    GUPnPContextManager *manager,
    GUPnPContext *context,
    gpointer user_data)
{
  // 打印监听的端口
  const char *host_ip = gupnp_context_get_host_ip(context);
  g_print("Service is available, listening on port %s\n", host_ip);

  GError *error = NULL;

  GUPnPRootDevice *root_device = gupnp_root_device_new(context, "description.xml", "./XML", &error);
  if (error)
  {
    g_printerr("Error creating root device: %s\n", error->message);
    g_error_free(error);
    return;
  }
  gupnp_context_manager_manage_root_device(manager, root_device);

  // 服务设置
  on_connection_manager_service_available(root_device, user_data);
  on_av_transport_service_available(root_device, user_data);
  on_rendering_control_service_available(root_device, user_data);

  gupnp_root_device_set_available(root_device, TRUE);

  g_object_unref(root_device);
}

/**
 * @brief 服务不可用回调函数
 * @param manager UPnP上下文管理器
 * @param context UPnP上下文
 * @param user_data 用户数据
 */
static void on_service_unavailable(
    GUPnPContextManager *manager,
    GUPnPContext *context,
    gpointer user_data)
{
  g_print("Service is unavailable\n");
}