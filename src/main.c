#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include "types.h"
#include "xlib_input.h"
#include "xlib_output.h"

int main() {

  // TODO: sizes should be taken as arguments
  Pixels inputSize = (Pixels){ .x = 200, .y = 200 };
  Pixels outputSize = (Pixels){ .x = 200, .y = 200 };

  Display *display = XOpenDisplay(NULL);
  if (!display) return 1;

  Window root = DefaultRootWindow(display);
  XRRScreenResources *screen = XRRGetScreenResourcesCurrent(display, root);

  Pixels resolution = GetMonitorResolution(display, screen, 0);
  if (resolution.x < 1 || resolution.y < 1) {
    return 1;
  }

  printf("Got resolution of: %d x %d\n", resolution.x, resolution.y);

  Window cingleWindow = InitializeWindow(display, root, DefaultScreen(display), outputSize);
  if (cingleWindow <= 0) {
    return 1;
  }

  GC graphicsContext = XCreateGC(display, cingleWindow, 0, NULL);

  struct timespec SleepTime = { .tv_sec = 0, .tv_nsec = 66700000L };
  while (1) {
    XImage* centerImage = GetCenterSection(display, root, resolution, inputSize);
    DrawImageInWindow(display, cingleWindow, graphicsContext, centerImage, outputSize);
    nanosleep(&SleepTime, NULL);
    XDestroyImage(centerImage);
  }

  XFreeGC(display, graphicsContext);
  XRRFreeScreenResources(screen);
  XCloseDisplay(display);
  return 0;
}
