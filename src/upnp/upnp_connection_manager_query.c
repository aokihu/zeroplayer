/**
 * @file upnp_connection_manager_query.c
 * @brief Connection Manager Query Functions
 * @author aokihu <aokihu@gmail.com>
 */

#include <glib.h>
#include <libgupnp/gupnp.h>
#include "struct.h"
#include "upnp_service.h"

/**
 * @brief Query callback for getting source protocol info
 * @param service The UPnP service
 * @param variable The state variable being queried
 * @param value The GValue to store the result
 * @param user_data User data (AppContext)
 */
void on_get_source_protocol_info_query(GUPnPService *service,
                                       gchar *variable,
                                       GValue *value,
                                       gpointer user_data)
{
    AppContext *appContext = (AppContext *)user_data;

    // Define the supported source protocols
    const char *sourceProtocolInfo = g_strjoinv(",", UPNP_SERVICE_SINK_PROTOCOL);

    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, sourceProtocolInfo);
}

/**
 * @brief Query callback for getting sink protocol info
 * @param service The UPnP service
 * @param variable The state variable being queried
 * @param value The GValue to store the result
 * @param user_data User data (AppContext)
 */
void on_get_sink_protocol_info_query(GUPnPService *service,
                                     gchar *variable,
                                     GValue *value,
                                     gpointer user_data)
{
    AppContext *appContext = (AppContext *)user_data;

    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, "");
}

/**
 * @brief Query callback for getting current connection IDs
 * @param service The UPnP service
 * @param variable The state variable being queried
 * @param value The GValue to store the result
 * @param user_data User data (AppContext)
 */
void on_get_current_connection_ids_query(GUPnPService *service,
                                         gchar *variable,
                                         GValue *value,
                                         gpointer user_data)
{
    AppContext *appContext = (AppContext *)user_data;

    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, appContext->upnp_context->currentConnectionIDs);

    g_print("Current Connection IDs queried: %s\n", "");
}