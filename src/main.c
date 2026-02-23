#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Pixels;

Pixels getMonitorResolution(Display* display, XRRScreenResources* screen, int index) {

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

int main() {
  Display *display = XOpenDisplay(NULL);
  if (!display) return 1;

  Window root = DefaultRootWindow(display);
  XRRScreenResources *screen = XRRGetScreenResourcesCurrent(display, root);

  Pixels resolution = getMonitorResolution(display, screen, 0);
  if (resolution.x < 1 || resolution.y < 1) {
    return 1;
  }

  printf("Got resolution of: %d x %d\n", resolution.x, resolution.y);

  XRRFreeScreenResources(screen);
  XCloseDisplay(display);
  return 0;
}
