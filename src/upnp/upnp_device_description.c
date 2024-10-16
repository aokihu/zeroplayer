#include "upnp_device_description.h"
#include <glib.h>

static const gchar *device_description_template = "<?xml version=\"1.0\"?>\n"
                                                  "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
                                                  "  <specVersion>\n"
                                                  "    <major>1</major>\n"
                                                  "    <minor>0</minor>\n"
                                                  "  </specVersion>\n"
                                                  "  <device>\n"
                                                  "    <deviceType>urn:schemas-upnp-org:device:MediaRenderer:1</deviceType>\n"
                                                  "    <friendlyName>%s</friendlyName>\n"
                                                  "    <manufacturer>%s</manufacturer>\n"
                                                  "    <modelName>%s</modelName>\n"
                                                  "    <UDN>uuid:%s</UDN>\n"
                                                  "    <serviceList>\n"
                                                  "      <service>\n"
                                                  "        <serviceType>urn:schemas-upnp-org:service:AVTransport:1</serviceType>\n"
                                                  "        <serviceId>urn:upnp-org:serviceId:AVTransport</serviceId>\n"
                                                  "        <SCPDURL>/AVTransport1.xml</SCPDURL>\n"
                                                  "        <controlURL>/control/AVTransport1</controlURL>\n"
                                                  "        <eventSubURL>/event/AVTransport1</eventSubURL>\n"
                                                  "      </service>\n"
                                                  "      <service>\n"
                                                  "        <serviceType>urn:schemas-upnp-org:service:RenderingControl:1</serviceType>\n"
                                                  "        <serviceId>urn:upnp-org:serviceId:RenderingControl</serviceId>\n"
                                                  "        <SCPDURL>/RenderingControl1.xml</SCPDURL>\n"
                                                  "        <controlURL>/control/RenderingControl1</controlURL>\n"
                                                  "        <eventSubURL>/event/RenderingControl1</eventSubURL>\n"
                                                  "      </service>\n"
                                                  "    </serviceList>\n"
                                                  "  </device>\n"
                                                  "</root>\n";

gchar *upnp_device_description_generate(const gchar *friendlyName, const gchar *manufacturer, const gchar *modelName, const gchar *udn)
{

  // 转义特殊字符，防止XML解析错误
  gchar *escaped_friendlyName = g_markup_escape_text(friendlyName, -1);
  gchar *escaped_manufacturer = g_markup_escape_text(manufacturer, -1);
  gchar *escaped_modelName = g_markup_escape_text(modelName, -1);
  gchar *escaped_udn = g_markup_escape_text(udn, -1);

  // 格式化字符串，替换占位符
  gchar *description = g_strdup_printf(device_description_template,
                                       escaped_friendlyName,
                                       escaped_manufacturer,
                                       escaped_modelName,
                                       escaped_udn);

  // 释放转义后的临时字符串
  g_free(escaped_friendlyName);
  g_free(escaped_manufacturer);
  g_free(escaped_modelName);
  g_free(escaped_udn);

  // 返回生成的描述字符串，调用者负责释放内存
  return description;
}
