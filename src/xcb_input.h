#ifndef XCB_INPUT_H
#define XCB_INPUT_H

#include <xcb/xcb.h>
#include <xcb/randr.h>

#include "types.h"

Pixels GetMonitorResolution(xcb_connection_t *conn, xcb_window_t root);

xcb_get_image_reply_t *GetCenterSection(xcb_connection_t *conn,
                                        xcb_window_t root, Pixels resolution,
                                        Pixels size);

#endif
