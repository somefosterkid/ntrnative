// MSL's math for ARM: only nan is in Black. The file's name is a guess, from pokediamond's object's (MSL_ARM_math.o),
// its function's pokebw2's.
#include <math.h>

double nan(const char *x) {
    return NAN;
}
