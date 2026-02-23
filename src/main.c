#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>

#include "types.h"
#include "info.h"

int main() {
  Display *display = XOpenDisplay(NULL);
  if (!display) return 1;

  Window root = DefaultRootWindow(display);
  XRRScreenResources *screen = XRRGetScreenResourcesCurrent(display, root);

  Pixels resolution = GetMonitorResolution(display, screen, 0);
  if (resolution.x < 1 || resolution.y < 1) {
    return 1;
  }

  printf("Got resolution of: %d x %d\n", resolution.x, resolution.y);

  XRRFreeScreenResources(screen);
  XCloseDisplay(display);
  return 0;
}
