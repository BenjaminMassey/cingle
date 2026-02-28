#include <xcb/xcb.h>
#include <stdlib.h>

#include "xcb_output.h"

xcb_window_t InitializeWindow(xcb_connection_t *conn, xcb_window_t root,
                               xcb_screen_t *screen, Pixels size) {
  xcb_window_t window = xcb_generate_id(conn);

  uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[] = {screen->white_pixel, XCB_EVENT_MASK_EXPOSURE};

  xcb_create_window(conn, XCB_COPY_FROM_PARENT, window, root, 0, 0, size.x,
                    size.y, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, mask, values);

  xcb_map_window(conn, window);
  xcb_flush(conn);

  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(conn)) != NULL) {
    uint8_t type = event->response_type & ~0x80;
    free(event);
    if (type == XCB_EXPOSE) break;
  }

  return window;
}

void DrawImageInWindow(xcb_connection_t *conn, xcb_window_t window,
                       xcb_gcontext_t gc, xcb_get_image_reply_t *image,
                       Pixels size) {

  // TODO: scale image to given size

  xcb_put_image(conn, XCB_IMAGE_FORMAT_Z_PIXMAP, window, gc, size.x, size.y, 0,
                0, 0, image->depth, xcb_get_image_data_length(image),
                xcb_get_image_data(image));

  xcb_flush(conn);
}
