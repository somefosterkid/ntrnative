#include "global.h"

#include "math_util.h"

extern u32 ov96_0221AF54;

u8 ov96_021EE040(u8 *values, u8 *rank);

u8 ov96_021EE040(u8 *values, u8 *rank) {
    u8 candidates[29] = { 0 };
    u8 max = 0;
    u32 count = 0;
    int i;
    u8 value;

    for (i = 0; i < 29; i++) {
        value = values[i];
        if (max < value) {
            max = value;
        }
    }
    for (i = 0; i < 29; i++) {
        if (max == values[i]) {
            candidates[count] = i;
            count++;
        }
    }
    for (i = 0; i < 4; i++) {
        if (max >= (&ov96_0221AF54)[i]) {
            *rank = 3 - i;
            break;
        }
    }
    return candidates[MTRandom() % count];
}
