#include "global.h"

extern u32 ov98_0221F1F8;

u16 ov98_0221EFA4(s32 param_1, s32 param_2);

u16 ov98_0221EFA4(s32 param_1, s32 param_2) {
    return *(u16 *)((u8 *)(&ov98_0221F1F8 + param_1) + param_2 * 2);
}
