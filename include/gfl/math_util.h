// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/math_util.h.
#ifndef BWPORT_GFL_MATH_UTIL_H
#define BWPORT_GFL_MATH_UTIL_H

#include "types.h"
#include "nitro/fx/fx.h"

// The sine and cosine of an angle in whole degrees, 0 at 360 and over, or for any angle
fx16 func_0203f5fc(u16 degrees);
fx16 func_0203f63c(u16 degrees);
fx16 func_0203f684(u16 degrees);
fx16 func_0203f6d8(u16 degrees);
// An angle in whole degrees as a 16-bit angle, 0 at 360 and over
u16 func_0203f734(u16 degrees);
// The angle, as a 16-bit angle, of the arc of a circle of the radius as long as how far the point (x3, y3) is from the
// line through the other two, negative on one side
int func_0203f768(int x1, int y1, int x2, int y2, int x3, int y3, int radius);

#endif // BWPORT_GFL_MATH_UTIL_H
