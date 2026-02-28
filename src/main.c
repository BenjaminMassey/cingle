#include <xcb/xcb.h>
#include <xcb/randr.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "xcb_input.h"
#include "xcb_output.h"

int main() {

  // TODO: sizes should be taken as arguments
  Pixels inputSize = (Pixels){.x = 200, .y = 200};
  Pixels outputSize = (Pixels){.x = 200, .y = 200};

  int screenNum;
  xcb_connection_t *conn = xcb_connect(NULL, &screenNum);
  if (xcb_connection_has_error(conn)) {
    printf("failed to open xcb connection\n");
    return 1;
  }

  const xcb_setup_t *setup = xcb_get_setup(conn);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  for (int i = 0; i < screenNum; i++) {
    xcb_screen_next(&iter);
  }
  xcb_screen_t *screen = iter.data;
  xcb_window_t root = screen->root;

  Pixels resolution = GetMonitorResolution(conn, root);
  if (resolution.x < 1 || resolution.y < 1) {
    printf("got improper monitor resolution\n");
    xcb_disconnect(conn);
    return 1;
  }

  printf("Got resolution of: %d x %d\n", resolution.x, resolution.y);

  xcb_window_t cingleWindow = InitializeWindow(conn, root, screen, outputSize);
  if (!cingleWindow) {
    printf("failed to make cingle window\n");
    xcb_disconnect(conn);
    return 1;
  }

  xcb_gcontext_t gc = xcb_generate_id(conn);
  xcb_create_gc(conn, gc, cingleWindow, 0, NULL);

  struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 66700000L};
  while (1) {
    xcb_get_image_reply_t *centerImage =
        GetCenterSection(conn, root, resolution, inputSize);
    if (centerImage) {
      DrawImageInWindow(conn, cingleWindow, gc, centerImage, outputSize);
      free(centerImage);
    }
    nanosleep(&sleepTime, NULL);
  }

  xcb_free_gc(conn, gc);
  xcb_disconnect(conn);

  printf("done.\n");
  return 0;
}
