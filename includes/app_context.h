#ifndef __ZERO_PLAYER_APP_CONTEXT_H__
#define __ZERO_PLAYER_APP_CONTEXT_H__

#include <glib.h>
#include <gst/gst.h>
#include "struct.h"

/* Function prototypes */
AppContext *initAppContext(int argc, char *argv[]);
void cleanupAppContext(AppContext *context);

#endif /* __ZERO_PLAYER_APP_CONTEXT_H__ */