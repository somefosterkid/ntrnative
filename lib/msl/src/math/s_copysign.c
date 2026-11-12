// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/math/s_copysign.c: ported as it is,
// with credit (a project decision). fdlibm's file, under Sun's notice below.

/* @(#)s_copysign.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * copysign(double x, double y)
 * copysign(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#include <math.h>

#ifdef __STDC__
double copysign(double x, double y)
#else
double copysign(x, y)
double x, y;
#endif
{
    __HI(x) = (__HI(x) & 0x7FFFFFFF) | (__HI(y) & 0x80000000);
    return x;
}
