#include "global.h"

#include "math_util.h"
#include "sprite.h"

s32 ov96_021EB52C(void *, s32, s32);

Sprite *ov96_021EB5B8(void *);

s32 ov96_021F4504(void *, s32, u32);

extern s32 ov96_0221C050;

extern s32 ov96_0221C080;

void ov96_021F4CAC(void *arg0);

void ov96_021F4CAC(void *arg0) {
    VecFx32 sp0;
    Sprite *temp_r3;
    Sprite *temp_r3_2;
    int temp_r5;
    s32 var_r6;
    u16 temp_r0;
    s32 temp_r4;
    u16 temp_r0_2;
    s32 temp_r5_2;
    unsigned char var_r4;
    temp_r0 = LCRandom();
    temp_r5 = temp_r0 % 4;
    var_r4 = 0;
loop_1:
    temp_r0_2 = LCRandom();

    var_r6 = temp_r0_2 % 4;
    if (temp_r5 == var_r6) {
        var_r4 += 1;
        if (((u32)var_r4) < 0xAU) {
            goto loop_1;
        }
    }
    if (var_r4 == 0xA) {
        var_r6 = (temp_r5 + 1) % 4;
    }
    temp_r5_2 = (temp_r5 * 3) + (((s32)LCRandom()) % 3);
    temp_r4 = (var_r6 * 3) + (((s32)LCRandom()) % 3);
    ov96_021F4504(arg0, 0, (u8)temp_r5_2);
    ov96_021F4504(arg0, 1, (u8)temp_r4);
    temp_r3 = ov96_021EB5B8(*((void **)(((u8 *)arg0) + 0x90)));
    sp0 = *((VecFx32 *)((u8 *)(&ov96_0221C050)));
    sp0.y += 0x200000;
    Sprite_SetMatrix(temp_r3, &sp0);
    temp_r3_2 = ov96_021EB5B8(*((void **)(((u8 *)arg0) + 0x94)));
    sp0 = *((VecFx32 *)((u8 *)(&ov96_0221C080)));
    sp0.y += 0x200000;
    Sprite_SetMatrix(temp_r3_2, &sp0);
    ov96_021EB52C(*((void **)(((u8 *)arg0) + 0x90)), 1, 1);
    ov96_021EB52C(*((void **)(((u8 *)arg0) + 0x94)), 1, 1);
}
