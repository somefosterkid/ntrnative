#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "unk_0200A090.h"

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

void ov48_0225AEA8(UnkStruct_ov69_0225ED90 *param0, UnkStruct_ov69_0225E478 *param1);

void ov48_0225B010(UnkStruct_ov69_0225EB60 *param0, u32 param1);

BOOL ov48_0225ADBC(UnkStruct_ov69_0225EB60 *param0, u32 param1, UnkStruct_ov69_0225E478 *param2);

void ov48_0225AE3C(UnkStruct_ov69_0225ED90 *param0, const UnkStruct_ov69_0225ECF0 *param1);
void ov48_0225AE58(UnkStruct_ov69_0225ED90 *param0, BOOL param1);
void ov48_0225AE5C(UnkStruct_ov69_0225ED90 *param0, BOOL param1);
void ov48_0225AEDC(UnkStruct_ov69_0225ED90 *param0);
void ov48_0225AFB4(const UnkStruct_ov69_0225ED90 *param0, UnkStruct_ov69_0225E478 *param1);

BOOL ov48_0225ADBC(UnkStruct_ov69_0225EB60 *param0, u32 param1, UnkStruct_ov69_0225E478 *param2) {
    if (param0->unk_34[param1] == 0) {
        return 0;
    }

    ov48_0225AEA8(&param0->unk_00[param1], param2);
    Sprite_SetDrawFlag(param0->unk_3C[param0->unk_38[param1]], 1);
    ov48_0225B010(param0, param1);

    return 1;
}

BOOL ov48_0225AE60(UnkStruct_ov69_0225ED90 *param0, UnkStruct_ov69_0225E478 *param1);

BOOL ov48_0225ADF8(UnkStruct_ov69_0225EB60 *param0, u32 param1, UnkStruct_ov69_0225E478 *param2);

BOOL ov48_0225ADF8(UnkStruct_ov69_0225EB60 *param0, u32 param1, UnkStruct_ov69_0225E478 *param2) {
    BOOL v0;

    if (param0->unk_34[param1] == 0) {
        return 1;
    }

    v0 = ov48_0225AE60(&param0->unk_00[param1], param2);
    ov48_0225B010(param0, param1);

    if (v0 == 1) {
        Sprite_SetDrawFlag(param0->unk_3C[param0->unk_38[param1]], FALSE);
    }

    return v0;
}

void ov48_0225AE3C(UnkStruct_ov69_0225ED90 *param0, const UnkStruct_ov69_0225ECF0 *param1) {
    param0->unk_0C = param1;
    param0->unk_00 = param1->unk_00;
    param0->unk_02 = param1->unk_02 + 16;
    param0->unk_04 = 0;
    param0->unk_06 = 0;
    param0->unk_08 = 0;
    param0->unk_0A = 0;
}

void ov48_0225AE58(UnkStruct_ov69_0225ED90 *param0, BOOL param1) {
    param0->unk_08 = param1;
}

void ov48_0225AE5C(UnkStruct_ov69_0225ED90 *param0, BOOL param1) {
    param0->unk_0A = param1;
}

BOOL ov48_0225AE60(UnkStruct_ov69_0225ED90 *param0, UnkStruct_ov69_0225E478 *param1) {
    if (param0->unk_04 > param0->unk_0C->unk_08) {
        return 1;
    }

    ov48_0225AEDC(param0);
    param0->unk_04++;

    if ((param0->unk_06 + 1) > param0->unk_0C->unk_0C) {
        param0->unk_06 = 0;
    } else {
        param0->unk_06++;
    }

    ov48_0225AFB4(param0, param1);

    return 0;
}
