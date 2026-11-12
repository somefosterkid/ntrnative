// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/extras.c: ported as it is,
// with credit (a project decision).
#include <math.h>

double scalbn(double x, int n) {
    double value;
    int exp;

    value = frexp(x, &exp);
    exp += n;

    return ldexp(value, exp);
}
