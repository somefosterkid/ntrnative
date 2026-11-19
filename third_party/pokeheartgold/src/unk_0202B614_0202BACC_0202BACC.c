#include "global.h"

#include "fashion_case.h"

u32 FashionCase_CountAccessories(FashionCase *fashionCase) {
    s32 a1;
    u32 v0;
    u32 v1;

    v1 = 0;
    a1 = 0;
    do {
        v0 = sub_0202BA70(fashionCase, a1);
        a1 += 1;
        v1 += v0;
    } while (a1 < 0x64);
    return v1;
}
