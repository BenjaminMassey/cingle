#ifndef XLIB_OUTPUT_H
#define XLIB_OUTPUT_H

#include "types.h"

Window InitializeWindow(Display* display, Window root, int screen, Pixels size);

void DrawImageInWindow(Display* display, Window window, GC gc, XImage* image, Pixels size);

#endif
