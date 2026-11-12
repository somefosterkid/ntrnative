// MSL's float.c: the constants of math.h's INFINITY, NAN and HUGE_VAL, as Black has them (zeldaret-st's float.c,
// CC0-1.0, has only __float_nan).
int __float_nan[]   = {0x7FFFFFFF};
int __float_huge[]  = {0x7F800000};
int __double_huge[] = {0, 0x7FF00000};
