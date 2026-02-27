#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>

#include "xlib_input.h"

Pixels GetMonitorResolution(Display *display, XRRScreenResources *screen,
                            Window root) {

  RROutput primary = XRRGetOutputPrimary(display, root);
  if (!primary) {
    printf("failed to get primary monitor\n");
    return (Pixels){.x = 0, .y = 0};
  }

  XRROutputInfo *output = XRRGetOutputInfo(display, screen, primary);
  if (output->connection != RR_Connected || !output->crtc) {
    printf("primary monitor is not connected or has no crtc\n");
    XRRFreeOutputInfo(output);
    return (Pixels){.x = 0, .y = 0};
  }

  XRRCrtcInfo *crtc = XRRGetCrtcInfo(display, screen, output->crtc);
  int width = crtc->width;
  int height = crtc->height;
  XRRFreeCrtcInfo(crtc);
  XRRFreeOutputInfo(output);

  return (Pixels){.x = width, .y = height};
}

XImage *GetCenterSection(Display *display, Window root, Pixels resolution,
                         Pixels size) {
  int x = (resolution.x - size.x) * 0.5;
  int y = (resolution.y - size.y) * 0.5;
  return XGetImage(display, root, x, y, size.x, size.y, AllPlanes, ZPixmap);
}
