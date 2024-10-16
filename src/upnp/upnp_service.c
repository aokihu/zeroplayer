#include "upnp_service.h"
#include <stdlib.h>

/**
 * @brief Initialize UPnP service
 *
 * @param app_context Application context pointer
 * @return UPnPContext* UPnP context pointer
 */
UPnPContext *upnp_service_init(struct AppContext *app_context)
{
  UPnPContext *context = (UPnPContext *)malloc(sizeof(UPnPContext));

  return context;
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
