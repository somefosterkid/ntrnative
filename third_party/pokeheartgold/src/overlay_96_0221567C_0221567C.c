#include "global.h"

u32 ov96_02215614(s32 param_1, s32 param_2, s32 param_3, s32 param_4);

u32 ov96_0221567C(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6);

u32 ov96_0221567C(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u8 param_5, u8 param_6) {
    s32 iVar1;

    iVar1 = ov96_02215614(param_1, param_2, param_3, param_4);
    if ((u32)param_6 == (u32)param_5) {
        if (3 - (u32)param_6 != iVar1) {
            return 1;
        }
        return 0;
    }
    return 0;
}
