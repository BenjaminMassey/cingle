#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include "xlib_input.h"

Pixels GetMonitorResolution(Display* display, XRRScreenResources* screen, int index) {

  XRROutputInfo *output = XRRGetOutputInfo(display, screen, screen->outputs[index]);

  int width = 0;
  int height = 0;
  if (output->connection == RR_Connected && output->crtc) {
    XRRCrtcInfo *crtc = XRRGetCrtcInfo(display, screen, output->crtc);
    width = crtc->width;
    height = crtc->height;
    XRRFreeCrtcInfo(crtc);
  }
  XRRFreeOutputInfo(output);

  return (Pixels){ .x = width, .y = height };
}

XImage* GetCenterSection(Display* display, Window root, Pixels resolution, Pixels size) {
  int x = (resolution.x - size.x) * 0.5;
  int y = (resolution.y - size.y) * 0.5;
  return XGetImage(display, root, x, y, size.x, size.y, AllPlanes, ZPixmap);
}
