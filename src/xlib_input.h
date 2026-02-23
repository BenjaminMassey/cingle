#ifndef XLIB_INPUT_H
#define XLIB_INPUT_H

#include "types.h"

Pixels GetMonitorResolution(Display* display, XRRScreenResources* screen, int index);

XImage* GetCenterSection(Display* display, Window root, Pixels resolution, Pixels size);

#endif
