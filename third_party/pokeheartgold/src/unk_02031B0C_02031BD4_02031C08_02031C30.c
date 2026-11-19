#include "global.h"

#include "error_handling.h"

void ApricornBox_TakeApricorn(u8 *, s32, u32);

u32 sub_02031C30(u8 *p, s32 param_2, u32 param_3);

u32 sub_02031C30(u8 *p, s32 param_2, u32 param_3) {
    u16 uVar2;
    s32 i;

    uVar2 = *(u16 *)(p + 10);
    if (param_2 >= 7) {
        GF_AssertFail();
        return 0;
    }
    for (i = 0; i < 5; i++) {
        if ((uVar2 & 7) != 0) {
            uVar2 = uVar2 >> 3;
        } else {
            *(u16 *)(p + 10) |= (u16)(((param_2 + 1) & 7) << (i * 3));
            *(u16 *)(p + 10) |= 0x8000;
            *(u32 *)(p + 20) = param_3;
            if (*(u8 *)(p + 9) == 0) {
                *(u32 *)(p + 24) = param_3;
            }
            *(u32 *)(p + 28) &= 0xc0000000;
            ApricornBox_TakeApricorn(p, param_2, 1);
            return 1;
        }
    }
    return 0;
}
