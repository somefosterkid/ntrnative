#include "global.h"
#include "unk_02031B0C.h"

u8 sub_02032024(SaveApricornBox *arg0);

s32 sub_02031BEC(SaveApricornBox *, s32);
s32 sub_02032340(SaveApricornBox *, s32);
s32 sub_02032354(SaveApricornBox *);

u8 sub_02032024(SaveApricornBox *arg0) {
    u8 v0;

    v0 = arg0->juiceServings;
    if (v0 == 0) {
        return 0U;
    }
    arg0->juiceServings = (u8)(v0 - 1);
    if (arg0->juiceServings == 0) {
        sub_02031B5C(&arg0->juice);
        arg0->juiceAgingSteps = 0;
        arg0->mixStartSteps = 0;
        arg0->mixWord = (s32)(0xC0000000 & arg0->mixWord);
    }
    return arg0->juiceServings;
}

void sub_02032058(SaveApricornBox *saveApricornBox, u32 steps) {
    s32 v0;
    u32 v1;

    if ((((u32)(*(u16 *)((u8 *)saveApricornBox + 0xA) << 0x10) >> 0x1F) != 0) || (*(u8 *)((u8 *)saveApricornBox + 9) != 0)) {
        v1 = steps - *(s32 *)((u8 *)saveApricornBox + 0x18);
        if (v1 >= 0x64U) {
            sub_02032340(saveApricornBox, (u8)(v1 / 100U));
            *(s32 *)((u8 *)saveApricornBox + 0x18) = (s32)(steps - (v1 % 100U));
        }
        if (((u32)(*(u16 *)((u8 *)saveApricornBox + 0xA) << 0x10) >> 0x1F) != 0) {
            v0 = (0x3FFFFFFF & (steps - *(s32 *)((u8 *)saveApricornBox + 0x14))) | (*(s32 *)((u8 *)saveApricornBox + 0x1C) & 0xC0000000);
            *(s32 *)((u8 *)saveApricornBox + 0x1C) = v0;
            if ((u32)((u32)(v0 * 4) >> 2) >= 0x64U) {
                if (*(u8 *)((u8 *)saveApricornBox + 9) != 0) {
                    sub_02031BEC(saveApricornBox, 3);
                } else {
                    sub_02031BEC(saveApricornBox, 2);
                }
                sub_02032354(saveApricornBox);
            }
        }
    }
}
