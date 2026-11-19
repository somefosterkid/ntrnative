#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite_system.h"
s32 ov96_0220A254(PokeathlonCourseData *, u8, s32);
ManagedSprite *ov96_0220D1A0(s32, s32, s16, s16, s32, s32);
extern s32 ov96_0221CCC8;

void ov96_02209F8C(PokeathlonCourseData *arg0) {
    s16 sp20;
    s32 sp1C;
    void *sp18;
    s32 sp14;
    s32 *sp10;
    s32 spC;
    PokeathlonCourseData *sp8;
    ManagedSprite *temp_r0_2;
    ManagedSprite *temp_r0_3;
    ManagedSprite *temp_r0_4;
    s16 temp_r4;
    s32 temp_r0_5;
    s32 var_r4;
    s32 var_r6;
    s32 var_r6_2;
    void *temp_r0;
    void *var_r4_2;
    void *var_r5;

    sp8 = arg0;
    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    if (temp_r0 == NULL) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)temp_r0 + 8) == 0) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)temp_r0 + 0xC) == 0) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)temp_r0 + 0x44) == 0) {
        GF_AssertFail();
    }
    sp1C = 0;
    sp18 = temp_r0;
    sp18 = temp_r0 + 0x54;
    sp14 = 0x78;
    sp10 = &ov96_0221CCC8;
loop_9:
    temp_r4 = (s16) sp14;
    *(ManagedSprite **)((u8 *)sp18 + 4) = ov96_0220D1A0(*(s32 *)((u8 *)temp_r0 + 8), *(s32 *)((u8 *)temp_r0 + 0xC), temp_r4, 0x98, (u16) (sp1C + 0xD), 3);
    ov96_0220A254(sp8, (u8) sp1C, 0);
    temp_r0_2 = ov96_0220D1A0(*(s32 *)((u8 *)temp_r0 + 8), *(s32 *)((u8 *)temp_r0 + 0xC), temp_r4, 0x98, 0x12, 3);
    *(ManagedSprite **)((u8 *)sp18 + 8) = temp_r0_2;
    ManagedSprite_SetDrawFlag(temp_r0_2, 0);
    var_r5 = sp18;
    var_r4 = 0;
    var_r6 = 0;
    sp20 = (s16) sp14 + 6;
    do {
        temp_r0_3 = ov96_0220D1A0(*(s32 *)((u8 *)temp_r0 + 8), *(s32 *)((u8 *)temp_r0 + 0xC), (s16) sp20, (s16) ((s16) (0x88 - var_r4) - 0x14), (s32) (u16) *(s32 *)((u8 *)sp10 + 8), (s32) (u8) (0x2C - var_r6));
        *(ManagedSprite **)((u8 *)var_r5 + 0x5C) = temp_r0_3;
        ManagedSprite_SetDrawFlag(temp_r0_3, 0);
        var_r6 += 1;
        var_r4 += 0x10;
        var_r5 += 4;
    } while (var_r6 < 5);
    var_r4 = 0;
    var_r4_2 = sp18;
    spC = (s32) sp14;
    var_r6_2 = 0;
    do {
        temp_r0_4 = ov96_0220D1A0(*(s32 *)((u8 *)temp_r0 + 8), *(s32 *)((u8 *)temp_r0 + 0xC), (s16) spC, (s16) (0x88 - var_r6_2), (s32) (u16) *(s32 *)((u8 *)sp10 + 4), (s32) (u8) (0x18 - var_r4));
        *(ManagedSprite **)((u8 *)var_r4_2 + 0xC) = temp_r0_4;
        ManagedSprite_SetDrawFlag(temp_r0_4, 0);
        var_r4 += 1;
        var_r6_2 += 4;
        var_r4_2 += 4;
    } while (var_r4 < 0x14);
    sp18 += 0x70;
    sp14 += 0x20;
    sp10 += 3;
    temp_r0_5 = sp1C + 1;
    sp1C = temp_r0_5;
    if (temp_r0_5 < 4) {
        goto loop_9;
    }
}
