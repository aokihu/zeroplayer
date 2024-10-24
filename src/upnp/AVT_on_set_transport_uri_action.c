/**
 * @file on_set_av_transport_uri_action.c
 * @brief 设置媒体URI的动作回调函数
 * @author aokihu <aokihu@gmail.com>
 */

#include <glib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "struct.h"
#include "upnp_callback.h"
#include "upnp_service.h"
#include "player.h"



/* ----------------------------------------------- */
/*                                                 */
/*                Callback Functions               */
/*                                                 */
/* ----------------------------------------------- */


/* ----------------------------------------------- */
/*                                                 */
/*                Utility  Functions               */
/*                                                 */
/* ----------------------------------------------- */

char *add_missing_namespace(const char *xml_str) {
    const char *insert_after = "<DIDL-Lite";
    const char *p = strstr(xml_str, insert_after);
    if (!p) {
        // 找不到根元素
        return NULL;
    }

    // 移动到 '<DIDL-Lite' 的末尾
    p += strlen(insert_after);

    // 准备要插入的命名空间声明
    const char *ns_decl = " xmlns:qq=\"http://www.qq.com/music/\"";

    // 计算新字符串的长度
    size_t new_len = strlen(xml_str) + strlen(ns_decl) + 1;

    // 为新 XML 字符串分配内存
    char *new_xml = malloc(new_len);
    if (!new_xml) return NULL;

    // 复制插入点之前的内容
    size_t prefix_len = p - xml_str;
    strncpy(new_xml, xml_str, prefix_len);
    new_xml[prefix_len] = '\0';

    // 添加命名空间声明
    strcat(new_xml, ns_decl);

    // 追加剩余的 XML 内容
    strcat(new_xml, p);

    return new_xml;
}


/**
 * @brief 从XML节点获取文本内容
 * @param doc XML文档
 * @param xpath XPath表达式
 * @return 文本内容,需要用g_free释放,如果未找到返回NULL
 */
static gchar* get_node_text(xmlDocPtr doc, const char *xpath) 
{
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (!context) {
        return NULL;
    }

    // 注册所有用到的命名空间
    xmlXPathRegisterNs(context, BAD_CAST "didl", BAD_CAST "urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/");
    xmlXPathRegisterNs(context, BAD_CAST "dc", BAD_CAST "http://purl.org/dc/elements/1.1/");
    xmlXPathRegisterNs(context, BAD_CAST "qq", BAD_CAST "http://www.qq.com/music/");
    xmlXPathRegisterNs(context, BAD_CAST "upnp", BAD_CAST "urn:schemas-upnp-org:metadata-1-0/upnp/");
    xmlXPathRegisterNs(context, BAD_CAST "r", BAD_CAST "urn:schemas-rinconnetworks-com:metadata-1-0/");

    xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath, context);
    if (!result) {
        xmlXPathFreeContext(context);
        return NULL;
    }

    gchar *text = NULL;
    if (result->nodesetval && result->nodesetval->nodeNr > 0) {
        xmlNodePtr node = result->nodesetval->nodeTab[0];
        xmlChar *content = xmlNodeGetContent(node);
        if (content) {
            text = g_strdup((const char *)content);
            xmlFree(content);
        }
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
    return text;
}

/**
 * @brief Get attribute value from an XML node using XPath
 */
static gchar* get_node_attr(xmlDocPtr doc, const char *xpath, const char *attr) 
{
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (!context) {
        return NULL;
    }

    // Register namespaces
    xmlXPathRegisterNs(context, BAD_CAST "didl", BAD_CAST "urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/");
    xmlXPathRegisterNs(context, BAD_CAST "dc", BAD_CAST "http://purl.org/dc/elements/1.1/");
    xmlXPathRegisterNs(context, BAD_CAST "qq", BAD_CAST "http://www.qq.com/music/");
    xmlXPathRegisterNs(context, BAD_CAST "upnp", BAD_CAST "urn:schemas-upnp-org:metadata-1-0/upnp/");
    xmlXPathRegisterNs(context, BAD_CAST "r", BAD_CAST "urn:schemas-rinconnetworks-com:metadata-1-0/");

    xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath, context);
    if (!result) {
        xmlXPathFreeContext(context);
        return NULL;
    }

    gchar *value = NULL;
    if (result->nodesetval && result->nodesetval->nodeNr > 0) {
        xmlNodePtr node = result->nodesetval->nodeTab[0];
        xmlChar *attr_value = xmlGetProp(node, BAD_CAST attr);
        if (attr_value) {
            value = g_strdup((const char *)attr_value);
            xmlFree(attr_value);
        }
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
    return value;
}


/**
 * @brief 解析DIDL-Lite元数据
 */
static void parse_didl_lite_metadata(const char *metadata, AppContext *app_context) 
{
    if (!metadata || !app_context) {
        return;
    }

    // 预处理 XML，添加缺失的命名空间
    char *fixed_metadata = add_missing_namespace(metadata);
    if (!fixed_metadata) {
        g_print("Can't fix metadata XML\n");
        return;
    }

    // 初始化libxml2
    LIBXML_TEST_VERSION

    // 解析XML文档
    xmlDocPtr doc = xmlReadMemory(fixed_metadata, strlen(fixed_metadata), NULL, "UTF-8", 
                                    XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING |
                                    XML_PARSE_NONET | XML_PARSE_NOBLANKS | XML_PARSE_NOCDATA | 
                                    XML_PARSE_NOERROR | XML_PARSE_NOWARNING | XML_PARSE_NSCLEAN);
    if (!doc) {
        g_print("Failed to parse DIDL-Lite XML\n");
        free(fixed_metadata);  // 添加这行来释放内存
        return;
    }

    // 提取所需信息
    gchar *title = get_node_text(doc, "//dc:title");
    gchar *creator = get_node_text(doc, "//dc:creator");
    gchar *album = get_node_text(doc, "//upnp:album");
    gchar *album_art = get_node_text(doc, "//upnp:albumArtURI");
    gchar *res_url = get_node_text(doc, "//didl:res");
    gchar *duration = get_node_attr(doc, "//didl:res", "duration");
    gchar *song_id = get_node_text(doc, "//qq:songID");

    // 打印调试信息
    g_print("Parse Metadata:\n");
    g_print("Title: %s\n", title ? title : "Unknown");
    g_print("Creator: %s\n", creator ? creator : "Unknown");
    g_print("Album: %s\n", album ? album : "Unknown");
    g_print("AlbumArtURI: %s\n", album_art ? album_art : "Unknown");
    g_print("QQSongID: %s\n", song_id ? song_id : "Unknown");
    g_print("ResourceURL: %s\n", res_url ? res_url : "Unknown");
    g_print("Duration: %s\n", duration ? duration : "Unknown");

    // 更新播放器上下文
    if (app_context->player_context) {
        // 保存原始元数据
        player_set_metadata(app_context->player_context, fixed_metadata);
        
        // 更新各个元数据字段
        if (song_id) {
            player_set_qq_song_id(app_context->player_context, song_id);
        }
        if (duration) {
            player_set_current_track_duration(app_context->player_context, duration);
        }
        if (title) {
            player_set_current_track_title(app_context->player_context, title);
        }
        if (creator) {
            player_set_current_track_artist(app_context->player_context, creator);
        }
        if (album) {
            player_set_current_track_album(app_context->player_context, album);
        }
    }

    // 释放资源
    g_free(title);
    g_free(creator);
    g_free(album);
    g_free(album_art);
    g_free(song_id);
    g_free(res_url);
    g_free(duration);
    free(fixed_metadata);  // 释放修复后的元数据
    xmlFreeDoc(doc);
    xmlCleanupParser();  // 清理 XML 解析器
}


/* ----------------------------------------------- */
/*                                                 */
/*                Action Functions                 */
/*                                                 */
/* ----------------------------------------------- */


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


   

    // 检查参数是否有效
    if (instanceId == NULL || currentUri == NULL)
    {
        gupnp_service_action_return_error(action, 402, "Invalid Args");
        goto cleanup;
    }

    // 解析元数据
    if (currentUriMetaData && strlen(currentUriMetaData) > 0) {
        parse_didl_lite_metadata(currentUriMetaData, appContext);
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
