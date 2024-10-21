/**
 * @file upnp_callback.h
 * @brief UPnP Callback Functions
 * @author aokihu <aokihu@gmail.com>
 */

#ifndef __UPNP_CALLBACK_H__
#define __UPNP_CALLBACK_H__

#include <libgupnp/gupnp.h>

// ----------------------------------------- //
//                                           //
//              ConnectionManager            //
//                                           //
// ----------------------------------------- //

/* Service Available Callback Functions */
void on_cm_service_available(GUPnPRootDevice *rd, gpointer user_data);

/* Action Callback Functions */
void on_get_protocol_info_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);
void on_get_current_connection_ids_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);

/* Query Callback Functions */
void on_get_source_protocol_info_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data);
void on_get_sink_protocol_info_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data);
void on_get_current_connection_ids_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data);

// ----------------------------------------- //
//                                           //
//                 AVTransport               //
//                                           //
// ----------------------------------------- //

/* Service Available Callback Functions */
void on_avt_service_available(GUPnPRootDevice *rd, gpointer user_data);

/* Action Callback Functions */

void on_set_av_transport_uri_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);
void on_play_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);
void on_pause_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);
void on_stop_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);
void on_seek_action(GUPnPService *service, GUPnPServiceAction *action, gpointer user_data);

/* Query Callback Functions */
void on_get_transport_state_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data);
void on_get_transport_status_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data);
void on_get_current_media_duration_query(GUPnPService *service, gchar *variable, const GValue *value, gpointer user_data);

// ----------------------------------------- //
//                                           //
//              RenderingControl             //
//                                           //
// ----------------------------------------- //

#endif // __UPNP_CALLBACK_H__
