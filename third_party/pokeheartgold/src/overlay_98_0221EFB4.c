#include "global.h"

#include "overlay_98.h"

u16 ov98_0221EFA4(s32 param_1, s32 param_2);

s32 ov98_0221EFB4(s32 arg0, s32 arg1, u32 arg2) {
    u32 v0;

    v0 = ov98_0221EFA4(arg0, arg1);
    if (arg2 == 0xFFFF) {
        return 0;
    }
    if (arg0 == 0) {
        if (arg2 < v0) {
            return 1;
        }
        return 0;
    }
    if (arg2 > v0) {
        return 1;
    }
    return 0;
}

s32 ov98_0221EFE8(s32 arg0, s32 arg1, u32 arg2);

s32 ov98_0221EFE8(s32 arg0, s32 arg1, u32 arg2) {
    u32 v0;

    v0 = ov98_0221EFA4(arg0, arg1);
    if (arg2 == 0xFFFF) {
        return 0;
    }
    if (arg0 == 0) {
        if (arg2 <= v0) {
            return 1;
        }
        return 0;
    }
    if (arg2 >= v0) {
        return 1;
    }
    return 0;
}

u16 ov98_0221F01C(const Pokeathlon_RecordsSolo *param_1, s32 param_2) {
    return param_1->entries[param_2].unk_0;
}
