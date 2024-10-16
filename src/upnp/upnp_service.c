#include "upnp_service.h"
#include "upnp_device_description.h"
#include <glib.h>
#include <stdlib.h>
#include <uuid/uuid.h>

/**
 * @brief Initialize UPnP service
 *
 * @param app_context Application context pointer
 * @return UPnPContext* UPnP context pointer
 */
UPnPContext *upnp_service_init(struct AppContext *app_context)
{
  UPnPContext *upnp_context = g_new0(UPnPContext, 1);

  if (!upnp_context)
  {
    return NULL;
  }

  upnp_context->port = 0;
  upnp_context->friendlyName = g_strdup("UPnP Media Renderer");
  upnp_context->deviceDescriptionPath = NULL;
  upnp_context->serviceDescriptionPath = NULL;

  // 生成UUID
  uuid_t uuid;
  gchar uuid_str[37];
  uuid_generate(uuid);
  uuid_unparse_lower(uuid, uuid_str);

  // 使用宏生成设备描述XML
  const gchar *device_description = upnp_device_description_generate(upnp_context->friendlyName,
                                                                     "Unknown",
                                                                     "Unknown",
                                                                     uuid_str);
  // 创建临时文件存储设备描述XML
  GError *error = NULL;
  gchar *temp_filename = NULL;
  GFileIOStream *iostream = NULL;
  GFile *file = g_file_new_tmp("upnp_device_description.xml", &iostream, &error);
  if (error)
  {
    g_printerr("Failed to create temporary file: %s\n", error->message);
    g_error_free(error);
    upnp_service_cleanup(upnp_context);
    return NULL;
  }

  temp_filename = g_file_get_path(file);
  g_object_unref(file);

  GOutputStream *output_stream = g_io_stream_get_output_stream(G_IO_STREAM(iostream));

  if (!g_output_stream_write(output_stream, device_description, strlen(device_description), NULL, &error))
  {
    g_printerr("Failed to write to temporary file: %s\n", error->message);
    g_error_free(error);
    g_object_unref(iostream);
    g_free(temp_filename);
    upnp_service_cleanup(upnp_context);
    return NULL;
  }

  g_output_stream_close(output_stream, NULL, &error);
  g_object_unref(iostream);
  g_object_unref(output_stream);

  // 设置设备描述路径
  upnp_context->deviceDescriptionPath = g_strdup(temp_filename);

  // 创建GUPnP上下文
  GUPnPContext *gupnp_context = gupnp_context_new_for_address(NULL, upnp_context->port, GSSDP_UDA_VERSION_1_1, &error);
  if (error)
  {
    g_printerr("Failed to create GUPnP context: %s\n", error->message);
    g_error_free(error);
    upnp_service_cleanup(upnp_context);
    return NULL;
  }

  // 设置GUPnP上下文
  upnp_context->upnp_context = gupnp_context;

  // 创建GUPnP根设备
  GUPnPRootDevice *root_device = gupnp_root_device_new(gupnp_context, upnp_context->deviceDescriptionPath, NULL, &error);
  if (error)
  {
    g_printerr("Failed to create GUPnP root device: %s\n", error->message);
    g_error_free(error);
    upnp_service_cleanup(upnp_context);
    return NULL;
  }

  // 设置根设备
  upnp_context->rootDevice = root_device;

  return upnp_context;
}

/**
 * @brief Clean up UPnP service
 *
 * @param upnp_context UPnP context pointer
 */
void upnp_service_cleanup(UPnPContext *upnp_context)
{
  // 简单的占位实现
  if (upnp_context)
  {
    free(upnp_context);
  }
}
