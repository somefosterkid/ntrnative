#include "global.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

#include "error_handling.h"
#include "font.h"
#include "overlay_45_thumb.h"
#include "overlay_49.h"
#include "pm_string.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

u8 ov45_0222F294(s32);

s32 ov45_0222F2D4(s32);

s32 ov49_0225A24C(UnkStruct_ov70_0225DEE8 *, u16, u16, u16, u16);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *, int, u32);

s32 ov49_0225A31C(UnkStruct_ov70_0225DEE8 *, u32, s32, s32, s32);

String *ov49_0225A37C(UnkStruct_ov70_0225DEE8 *, int, s32);

void ov49_02262E10(UnkStruct_ov70_02266C68 *param0, UnkStruct_ov70_0225DEE8 *param1, s32 param2, s32 param3) {
    s32 sp4;
    UnkStruct_ov70_0225DEE8 *var_r4;
    UnkStruct_ov70_02266C68 *var_r5;
    s32 var_r6;
    String *str;
    s32 var_r7;
    u8 temp_r0;
    u8 temp_r0_2;
    unsigned char new_var;
    s32 var_r0;
    var_r5 = param0;
    var_r4 = param1;
    sp4 = param3;
    if (param2 != 0) {
        var_r5->unk_07 = 0xFF;
    }
    temp_r0 = var_r5->unk_03;
    switch (temp_r0) {
    case 0:
        var_r6 = 0;
        break;

    case 1:
        var_r6 = 1;
        break;

    case 2:
        var_r6 = 2;
        break;

    default:
        GF_AssertFail();
        var_r6 = 2;
        break;
    }

    var_r7 = ov45_0222F274(var_r6);
    if (var_r5->unk_00 != var_r7) {
        var_r5->unk_07 |= 4;
    }
    var_r5->unk_00 = var_r7;
    temp_r0_2 = ov45_0222F294(var_r6);
    if (var_r5->unk_02 != temp_r0_2) {
        var_r5->unk_07 |= 4;
    }
    var_r5->unk_02 = temp_r0_2;
    if (!(var_r7 == 1)) {
        var_r0 = 4;
    } else {
        var_r0 = ov45_0222F2D4(var_r6);
    }
    if (var_r5->unk_01 != var_r0) {
        var_r5->unk_07 |= 2;
    }
    var_r5->unk_01 = var_r0;
    if (1 & var_r5->unk_07) {
        ov49_0225A24C(var_r4, 0, 0, 0x68, 0x10);
        ov49_0225A37C(var_r4, var_r5->unk_03, 0);
        ov49_0225A23C(var_r4, ov49_0225A30C(var_r4, 1, 0x36), 0, 0);
    }
    if (4 & var_r5->unk_07) {
        ov49_0225A24C(var_r4, 0, 0x10, 0x68, 0x10);
        if (((sp4 == 1) && (var_r5->unk_02 == 0)) && (var_r5->unk_00 == 1)) {
            str = ov49_0225A30C(var_r4, 1, 0x45);
            new_var = 0x68 - FontID_String_GetWidth(FONT_SYSTEM, str, 0U);
            ov49_0225A23C(var_r4, str, new_var, 0x10);
        }
    }
    if (2 & var_r5->unk_07) {
        ov49_0225A24C(var_r4, 0, 0x20, 0x68, 0x20);
        ov49_0225A31C(var_r4, var_r5->unk_01, 1, 1, 2);
        ov49_0225A31C(var_r4, 4 - var_r5->unk_01, 1, 0, 2);
        ov49_0225A23C(var_r4, ov49_0225A30C(var_r4, 1, 0x44), 0, 0x20);
    }
    if ((var_r5->unk_06 == 1) && (8 & var_r5->unk_07)) {
        ov49_0225A24C(var_r4, 0, 0x40, 0x68, 0x10);
        ov49_0225A31C(var_r4, var_r5->unk_04, 2, 0, 2);
        ov49_0225A23C(var_r4, ov49_0225A30C(var_r4, 0, 0xF), 0, 0x40);
    }
    var_r5->unk_07 = 0;
}
