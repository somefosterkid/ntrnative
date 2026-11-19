/* PORTOK hk42 */
#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
s32 ov96_021E6104(void);
s32 ov96_021E6138(void *arg0);
void *ov96_021E60C0(PokeathlonCourseData *, s32, s32);
void *ov96_021E60D8(PokeathlonCourseData *, s32, s32);
void *ov96_021E8BB0(s32);
s32 ov96_021EAA04(s32, u8);
s32 ov96_021EAA20(s32);
s32 ov96_021EABA8(s32, s32);
s32 ov96_021EABE0(s32, s32);
s32 ov96_021EABF4(s32, s32 *);
s32 ov96_021EAC0C(s32, s32);
s32 ov96_021EAF6C(s32, s32);
s32 ov96_021EAF70(s32, s32, s32);
s32 ov96_021EAF94(s32, s32, s32);
s32 ov96_021EB4F4(s32, s32, s32);
s32 ov96_021EB52C(s32, s32, s32);
Sprite *ov96_021EB5B8(s32);
s32 ov96_021FC0D0(s32, s32);

void ov96_021FA0E8(PokeathlonCourseData *arg0, s32 *arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, void *arg7) {
    s32 sp20[9];
    VecFx32 sp14;
    void *sp10;
    s32 spC;
    s32 sp8;
    Sprite *temp_r0_4;
    Sprite *temp_r0_5;
    Sprite *temp_r0_6;
    s32 temp_r0;
    s32 temp_r2;
    s32 temp_r5;
    s32 var_r1;
    void *temp_r0_3;

    *(s16 *)arg7 = 0;
    *(s16 *)((u8 *)arg7 + 2) = 0;
    *(s16 *)((u8 *)arg7 + 4) = 0x30;
    *(s16 *)((u8 *)arg7 + 6) = 0x140;
    *(s8 *)((u8 *)arg7 + 0x18) = (s8) arg6;
    *(s16 *)((u8 *)arg7 + 0x68) = 1;
    *(s32 *)((u8 *)arg7 + 0x64) = 0x3F800000;
    *(s8 *)((u8 *)arg7 + 8) = 0;
    *(s32 *)((u8 *)arg7 + 0x20) = ov96_021FC0D0(arg3, arg6);
    switch (arg6) {
    case 0:
        *(s16 *)((u8 *)arg7 + 0x14) = 0;
        *(s16 *)((u8 *)arg7 + 0x16) = 4;
        break;
    case 1:
        *(s16 *)((u8 *)arg7 + 0x14) = 1;
        *(s16 *)((u8 *)arg7 + 0x16) = 5;
        break;
    case 2:
        *(s16 *)((u8 *)arg7 + 0x14) = 2;
        *(s16 *)((u8 *)arg7 + 0x16) = 6;
        break;
    }
    switch (arg6) {
    case 0:
        var_r1 = 0;
        sp8 = 0x30;
        break;
    case 1:
        var_r1 = 1;
        sp8 = 0x80;
        break;
    case 2:
        var_r1 = 2;
        sp8 = 0xD0;
        break;
    default:
        GF_AssertFail();
        return;
    }
    spC = ov96_021EAA04(arg5, (u8) var_r1);
    temp_r0 = ov96_021EAA20(spC);
    *(s32 *)((u8 *)arg7 + 0x24) = spC;
    sp10 = ov96_021E8BB0(temp_r0);
    temp_r0_3 = ov96_021E60D8(arg0, ov96_021E5F24(arg0), arg6);
    *(s16 *)((u8 *)arg7 + 0x5C) = (s16) *(s32 *)((u8 *)arg1 + *(u8 *)((u8 *)temp_r0_3 + 2) * 4);
    *(s16 *)((u8 *)arg7 + 0x5E) = (s16) *(s32 *)((u8 *)arg1 + *(u8 *)((u8 *)temp_r0_3 + 2) * 4 + 0x14);
    *(u8 *)((u8 *)arg7 + 0x58) = *(u8 *)((u8 *)temp_r0_3 + 2);
    temp_r2 = (ov96_021E6138(ov96_021E60C0(arg0, ov96_021E5F24(arg0), arg6)) - 1) * 8;
    ov96_021EAF70(spC, *(s32 *)((u8 *)arg2 + temp_r2), *(s32 *)((u8 *)arg2 + (temp_r2 + 4)));
    ov96_021EAF6C(spC, ov96_021E6104());
    *(f32 *)((u8 *)arg7 + 0x50) = (f32) *(s32 *)((u8 *)arg1 + *(u8 *)((u8 *)temp_r0_3 + 1) * 4 + 0x3C) / 10.0f;
    *(f32 *)((u8 *)arg7 + 0x54) = (f32) *(s32 *)((u8 *)arg1 + *(u8 *)((u8 *)temp_r0_3 + 4) * 4 + 0x28) / 100.0f;
    if (*(u16 *)((u8 *)sp10 + 4) != 0) {
        sp20[6] = 0x20000;
        sp20[7] = 0x20000;
    } else {
        sp20[6] = 0x10000;
        sp20[7] = 0x10000;
    }
    *(s32 *)((u8 *)arg7 + 0x3C) = 0x78;
    ov96_021EAF94(spC, sp8, *(s32 *)((u8 *)arg7 + 0x3C) + 0x110);
    ov96_021EAC0C(spC, 1);
    ov96_021EABA8(spC, 4);
    ov96_021EABE0(spC, 2);
    ov96_021EABF4(spC, &sp20[6]);
    temp_r0 = ov96_021EB4F4(arg4, 0x66, 5);
    *(s32 *)((u8 *)arg7 + 0x44) = temp_r0;
    temp_r0_4 = ov96_021EB5B8(temp_r0);
    *(s32 *)((u8 *)arg7 + 0x40) = *(s32 *)((u8 *)arg7 + 0x3C) + 0x120;
    sp20[5] = 0;
    temp_r5 = sp8 << 0xC;
    sp20[3] = temp_r5;
    sp20[4] = *(s32 *)((u8 *)arg7 + 0x40) << 0xC;
    Sprite_SetMatrix(temp_r0_4, (VecFx32 *)&sp20[3]);
    ov96_021EB52C(*(s32 *)((u8 *)arg7 + 0x44), 1, 0);
    Sprite_SetDrawPriority(temp_r0_4, 3U);
    temp_r0 = ov96_021EB4F4(arg4, 0x68, 7);
    *(s32 *)((u8 *)arg7 + 0x48) = temp_r0;
    temp_r0_5 = ov96_021EB5B8(temp_r0);
    Sprite_SetAnimCtrlSeq(temp_r0_5, 0);
    sp20[2] = 0;
    sp20[1] = 0x1AC000;
    sp20[0] = temp_r5;
    Sprite_SetMatrix(temp_r0_5, (VecFx32 *)sp20);
    Sprite_SetDrawPriority(temp_r0_5, 4U);
    Sprite_SetAffineOverwriteMode(temp_r0_5, 2U);
    temp_r0 = ov96_021EB4F4(arg4, 0x69, 8);
    *(s32 *)((u8 *)arg7 + 0x4C) = temp_r0;
    temp_r0_6 = ov96_021EB5B8(temp_r0);
    Sprite_SetAnimCtrlSeq(temp_r0_6, 0);
    sp14.z = 0;
    sp14.y = 0x188000;
    sp14.x = temp_r5;
    Sprite_SetMatrix(temp_r0_6, &sp14);
    ov96_021EB52C(*(s32 *)((u8 *)arg7 + 0x4C), 1, 1);
    Sprite_SetDrawPriority(temp_r0_6, 2U);
    *(s32 *)((u8 *)arg7 + 0xC) = 0x3F800000;
}
