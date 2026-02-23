#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include "xlib_output.h"

Window InitializeWindow(Display *display, Window root, int screen,
                        Pixels size) {
  Window window = XCreateSimpleWindow(display, root, 0, 0, size.x, size.y, 1,
                                      BlackPixel(display, screen),
                                      WhitePixel(display, screen));

  XMapWindow(display, window);
  XFlush(display);

  XSelectInput(display, window, ExposureMask);
  XEvent event;
  XNextEvent(display, &event);

  return window;
}

void DrawImageInWindow(Display *display, Window window, GC gc, XImage *image,
                       Pixels size) {

  // TODO: scale image to given size

  XPutImage(display, window, gc, image, 0, 0, 0, 0, size.x, size.y);

  XFlush(display);
}
