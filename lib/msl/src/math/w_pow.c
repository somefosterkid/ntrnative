// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/math/w_pow.c, with pow's call
// written with its arguments (zeldaret-st calls an unprototyped __ieee754_pow without them, for the same code).
// fdlibm's file, under Sun's notice below.
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

#include <math.h>

double pow(double x, double y) {
    return __ieee754_pow(x, y);
}
