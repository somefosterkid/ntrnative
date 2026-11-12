// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/math_api.c: ported as it is, with
// credit (a project decision). __msl_generic_count_bits64, which zeldaret-st lacks (its
// func_02033a80, before the file), is MSL's: a population count.
#include <math.h>

int __msl_generic_count_bits64(unsigned long long x) {
    x -= (x >> 1) & 0x5555555555555555ULL;
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    x += x >> 8;
    x += x >> 16;
    x += x >> 32;
    return (int) x & 0xFF;
}

int __signbitd(double x) {
    return (__HI(x) & 0x80000000) != 0;
}

int __fpclassifyd(double x) {
    switch (__HI(x) & 0x7FF00000) {
        case 0x7FF00000:
            if ((__HI(x) & 0x000FFFFF) || (__LO(x) & 0xFFFFFFFF)) {
                return 1;
            } else {
                return 2;
            }
            break;
        case 0:
            if ((__HI(x) & 0x000FFFFF) || (__LO(x) & 0xFFFFFFFF)) {
                return 5;
            } else {
                return 3;
            }
            break;
    }

    return 4;
}
