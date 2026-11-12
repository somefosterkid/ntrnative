// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/arith.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <arith.h>

int abs(int __x) {
    return __x < 0 ? -__x : __x;
}
