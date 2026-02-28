#ifndef XCB_OUTPUT_H
#define XCB_OUTPUT_H

#include <xcb/xcb.h>

#include "types.h"

xcb_window_t InitializeWindow(xcb_connection_t *conn, xcb_window_t root,
                               xcb_screen_t *screen, Pixels size);

void DrawImageInWindow(xcb_connection_t *conn, xcb_window_t window,
                       xcb_gcontext_t gc, xcb_get_image_reply_t *image,
                       Pixels size);

#endif
