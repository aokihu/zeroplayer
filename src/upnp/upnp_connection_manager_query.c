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

    g_print("Source Protocol Info queried: %s\n", sourceProtocolInfo);
}
