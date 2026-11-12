// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/math/s_fabs.c: ported as it is,
// with credit (a project decision). fdlibm's file, under Sun's notice below.

/* @(#)s_fabs.c 1.3 95/01/18 */
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
 * fabs(x) returns the absolute value of x.
 */

#include <math.h>

#ifdef __STDC__
double fabs(double x)
#else
double fabs(x)
double x;
#endif
{
    __HI(x) &= 0x7FFFFFFF;
    return x;
}
