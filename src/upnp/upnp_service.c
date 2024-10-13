#include "upnp_service.h"
#include <stdlib.h>

// 初始化UPnP服务
UPnPContext *upnp_service_init(struct AppContext *app_context)
{
  // 简单的占位实现
  UPnPContext *context = (UPnPContext *)malloc(sizeof(UPnPContext));

  return context;
}

// 清理UPnP服务
void upnp_service_cleanup(UPnPContext *upnp_context)
{
  // 简单的占位实现
  if (upnp_context)
  {
    free(upnp_context);
  }
}
