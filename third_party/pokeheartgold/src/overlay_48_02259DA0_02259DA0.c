#include "global.h"

#include "bg_window.h"
#include "system.h"

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    Window unk_1C;
} UnkStruct_ov69_0225DC48;

s32 ov48_02258CE4(s32, s32, s32 *, s32 *, s32 *, s32 *);

void ov48_02259DA0(UnkStruct_ov69_0225DC48 *param0);

void ov48_02259DA0(UnkStruct_ov69_0225DC48 *param0) {
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    s32 temp_r0;
    s32 temp_r1;

    param0->unk_00 = 0;
    if (gSystem.touchNew != 0) {
        if (((u32)gSystem.touchX >= 0xC8U) && ((u32)gSystem.touchX <= 0xF8U) && ((u32)gSystem.touchY >= 0xA8U) && ((u32)gSystem.touchY <= 0xB8U)) {
            param0->unk_00 = 2;
            return;
        }
        param0->unk_04 = 0;
        param0->unk_10 = 0;
        param0->unk_14 = 0;
        param0->unk_18 = 0;
        param0->unk_00 = 0;
        param0->unk_08 = (s32)gSystem.touchX;
        param0->unk_0C = (s32)gSystem.touchY;
        param0->unk_18 = 4;
        goto block_7;
    }
block_7:
    if (gSystem.touchHeld != 0) {
        temp_r1 = param0->unk_04;
        switch (temp_r1) {
        case 0:
            temp_r0 = param0->unk_18;
            if (temp_r0 == 0) {
                param0->unk_04 = temp_r1 + 1;
            } else {
                param0->unk_18 = temp_r0 - 1;
            }
        case 1:
            ov48_02258CE4(param0->unk_08, param0->unk_0C, &sp14, &sp10, &spC, &sp8);
            param0->unk_00 = sp14 | spC;
            param0->unk_10 = sp10;
            param0->unk_14 = sp8;
            param0->unk_08 = (s32)gSystem.touchX;
            param0->unk_0C = (s32)gSystem.touchY;
            return;
        }
    } else {
        if (param0->unk_18 != 0) {
            param0->unk_00 = 1;
        }
        param0->unk_04 = 0;
        param0->unk_10 = 0;
        param0->unk_14 = 0;
        param0->unk_18 = 0;
    }
}
