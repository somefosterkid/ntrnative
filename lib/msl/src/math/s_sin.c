// fdlibm's s_sin.c, as MSL builds it: Sun's code, freely distributable under its notice below.
/* @(#)s_sin.c 1.3 95/01/18 */
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

/* sin(x)
 * Return sine function of x.
 *
 * kernel function:
 *	__kernel_sin		... sine function on [-pi/4,pi/4]
 *	__kernel_cos		... cose function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
 */

#include <math.h>

double sin(double x) {
    double y[2], z = 0.0;
    int n, ix;

    /* High word of x. */
    ix = __HI(x);

    /* |x| ~< pi/4 */
    ix &= 0x7FFFFFFF;
    if (ix <= 0x3FE921FB) {
        return __kernel_sin(x, z, 0);
    }

    /* sin(Inf or NaN) is NaN */
    else if (ix >= 0x7FF00000) {
        return x - x;
    }

    /* argument reduction needed */
    else {
        n = __ieee754_rem_pio2(x, y);
        switch (n & 3) {
            case 0:
                return __kernel_sin(y[0], y[1], 1);
            case 1:
                return __kernel_cos(y[0], y[1]);
            case 2:
                return -__kernel_sin(y[0], y[1], 1);
            default:
                return -__kernel_cos(y[0], y[1]);
        }
    }
}
