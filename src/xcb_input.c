#include <xcb/xcb.h>
#include <xcb/randr.h>
#include <stdio.h>
#include <stdlib.h>

#include "xcb_input.h"

Pixels GetMonitorResolution(xcb_connection_t *conn, xcb_window_t root) {

  xcb_randr_get_screen_resources_current_cookie_t res_cookie =
      xcb_randr_get_screen_resources_current(conn, root);
  xcb_randr_get_screen_resources_current_reply_t *res_reply =
      xcb_randr_get_screen_resources_current_reply(conn, res_cookie, NULL);
  if (!res_reply) {
    printf("failed to get screen resources\n");
    return (Pixels){.x = 0, .y = 0};
  }
  xcb_timestamp_t config_timestamp = res_reply->config_timestamp;
  free(res_reply);

  xcb_randr_get_output_primary_cookie_t primary_cookie =
      xcb_randr_get_output_primary(conn, root);
  xcb_randr_get_output_primary_reply_t *primary_reply =
      xcb_randr_get_output_primary_reply(conn, primary_cookie, NULL);
  if (!primary_reply) {
    printf("failed to get primary monitor\n");
    return (Pixels){.x = 0, .y = 0};
  }
  xcb_randr_output_t primary = primary_reply->output;
  free(primary_reply);

  xcb_randr_get_output_info_cookie_t output_cookie =
      xcb_randr_get_output_info(conn, primary, config_timestamp);
  xcb_randr_get_output_info_reply_t *output_reply =
      xcb_randr_get_output_info_reply(conn, output_cookie, NULL);
  if (!output_reply) {
    printf("failed to get output info\n");
    return (Pixels){.x = 0, .y = 0};
  }
  if (output_reply->connection != XCB_RANDR_CONNECTION_CONNECTED ||
      !output_reply->crtc) {
    printf("primary monitor is not connected or has no crtc\n");
    free(output_reply);
    return (Pixels){.x = 0, .y = 0};
  }
  xcb_randr_crtc_t crtc = output_reply->crtc;
  free(output_reply);

  xcb_randr_get_crtc_info_cookie_t crtc_cookie =
      xcb_randr_get_crtc_info(conn, crtc, config_timestamp);
  xcb_randr_get_crtc_info_reply_t *crtc_reply =
      xcb_randr_get_crtc_info_reply(conn, crtc_cookie, NULL);
  if (!crtc_reply) {
    printf("failed to get crtc info\n");
    return (Pixels){.x = 0, .y = 0};
  }
  Pixels result = (Pixels){.x = crtc_reply->width, .y = crtc_reply->height};
  free(crtc_reply);

  return result;
}

xcb_get_image_reply_t *GetCenterSection(xcb_connection_t *conn,
                                        xcb_window_t root, Pixels resolution,
                                        Pixels size) {
  int x = (resolution.x - size.x) / 2;
  int y = (resolution.y - size.y) / 2;
  xcb_get_image_cookie_t cookie =
      xcb_get_image(conn, XCB_IMAGE_FORMAT_Z_PIXMAP, root, x, y, size.x,
                    size.y, (uint32_t)~0);
  return xcb_get_image_reply(conn, cookie, NULL);
}
