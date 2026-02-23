#ifndef XLIB_OUTPUT_H
#define XLIB_OUTPUT_H

#include <X11/Xlib.h>

#include "types.h"

Window InitializeWindow(Display *display, Window root, int screen, Pixels size);

void DrawImageInWindow(Display *display, Window window, GC gc, XImage *image,
                       Pixels size);

#endif
