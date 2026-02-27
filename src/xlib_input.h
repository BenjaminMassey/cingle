#ifndef XLIB_INPUT_H
#define XLIB_INPUT_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include "types.h"

Pixels GetMonitorResolution(Display *display, XRRScreenResources *screen,
                            Window root);

XImage *GetCenterSection(Display *display, Window root, Pixels resolution,
                         Pixels size);

#endif
