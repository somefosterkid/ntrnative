#include "global.h"

#include "pokepic.h"

s16 ov07_02222674(s16, s32, s32);

void ov07_022226C4(Pokepic *arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4);

void ov07_022226C4(Pokepic *pokepic, s16 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s16 v0;
    s16 v1;

    v1 = arg1;
    v0 = ov07_02222674(v1, arg2, arg3);
    if (arg4 == 1) {
        v0 *= -1;
        v1 -= arg2;
    }
    Pokepic_SetAttr(pokepic, 1, v1 + v0);
}
