#include "global.h"

typedef struct UnkStruct_ov96_02206AC0_temp_r5 {
    u8 unk_0[0x46C];
    s32 unk_46C;
    void *unk_470;
    void *unk_474;
    u16 unk_478;
    u16 unk_47A;
} UnkStruct_ov96_02206AC0_temp_r5;

typedef struct UnkStruct_ov96_02206A24_temp_r5 {
    u8 unk_0[0x37C];
    s32 unk_37C;
    void *unk_380;
    u16 unk_384;
    u16 unk_386;
} UnkStruct_ov96_02206A24_temp_r5;

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB588(void *, s32 *);

void ov96_02206A24(void *arg0, s32 arg1, s32 arg2);

void ov96_02206A24(void *arg0, s32 arg1, s32 arg2) {
    s32 spC[3];
    s32 sp8;
    s32 sp4;
    void *sp0;
    s32 temp_r6;
    s32 temp_r4;
    u8 var_r7;
    UnkStruct_ov96_02206A24_temp_r5 *temp_r5;

    sp0 = arg0;
    sp4 = arg1;
    sp8 = arg2;
    var_r7 = 0;
loop_1:
    temp_r5 = (void *)(sp0 + (var_r7 * 0xC));
    if (temp_r5->unk_37C != 0) {
        temp_r6 = (temp_r5->unk_384 - sp4) + 0x80;
        temp_r4 = (temp_r5->unk_386 - sp8) + 0x60;
        spC[2] = 0;
        spC[0] = temp_r6 << 0xC;
        spC[1] = temp_r4 << 0xC;
        ov96_021EB588(temp_r5->unk_380, spC);
        if ((temp_r6 >= -0x20) && (temp_r6 <= 0x120) && (temp_r4 >= -0x20) && (temp_r4 <= 0xE0)) {
            ov96_021EB52C(temp_r5->unk_380, 1, 1);
        } else {
            ov96_021EB52C(temp_r5->unk_380, 1, 0);
        }
        var_r7 += 1;
        if ((u32)var_r7 < 0x14U) {
            goto loop_1;
        }
    }
}

void ov96_02206AC0(void *arg0, s32 arg1, s32 arg2);

void ov96_02206AC0(void *arg0, s32 arg1, s32 arg2) {
    s32 spC[3];
    s32 sp8;
    s32 sp4;
    void *sp0;
    s32 temp_r6;
    s32 temp_r4;
    u8 var_r7;
    UnkStruct_ov96_02206AC0_temp_r5 *temp_r5;

    sp0 = arg0;
    sp4 = arg1;
    sp8 = arg2;
    var_r7 = 0;
loop_1:
    temp_r5 = (void *)(sp0 + (var_r7 * 0x10));
    if (temp_r5->unk_46C != 0) {
        temp_r6 = (temp_r5->unk_478 - sp4) + 0x80;
        temp_r4 = (temp_r5->unk_47A - sp8) + 0x60;
        spC[2] = 0;
        spC[0] = temp_r6 << 0xC;
        spC[1] = temp_r4 << 0xC;
        ov96_021EB588(temp_r5->unk_470, spC);
        ov96_021EB588(temp_r5->unk_474, spC);
        if ((temp_r6 >= -0x20) && (temp_r6 <= 0x120) && (temp_r4 >= -0x20) && (temp_r4 <= 0xE0)) {
            ov96_021EB52C(temp_r5->unk_470, 1, 1);
            ov96_021EB52C(temp_r5->unk_474, 1, 1);
        } else {
            ov96_021EB52C(temp_r5->unk_470, 1, 0);
            ov96_021EB52C(temp_r5->unk_474, 1, 0);
        }
        var_r7 += 1;
        if ((u32)var_r7 < 0xAU) {
            goto loop_1;
        }
    }
}
