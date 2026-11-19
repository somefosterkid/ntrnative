#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "sprite.h"

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
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

void ov48_0225AEDC(UnkStruct_ov69_0225ED90 *param0);

s32 ov48_0225AEC4(void *, VecFx32 *);
void ov48_0225B010(u8 *arg0, s32 arg1);

void ov48_0225AEDC(UnkStruct_ov69_0225ED90 *param0) {
    u16 v0;
    s16 v1, v2;

    if (param0->unk_0A == 0) {
        v2 = param0->unk_0C->unk_06 - 16;
    } else {
        v2 = param0->unk_0C->unk_06;
    }

    if (param0->unk_08 == 0) {
        v1 = param0->unk_0C->unk_02 + 16;
    } else {
        v1 = param0->unk_0C->unk_02;
    }

    param0->unk_00 = param0->unk_0C->unk_00 + (((param0->unk_0C->unk_04 - param0->unk_0C->unk_00) * param0->unk_04) / param0->unk_0C->unk_08);
    param0->unk_02 = v1 + (((v2 - v1) * param0->unk_04) / param0->unk_0C->unk_08);

    v0 = (param0->unk_06 * 0xffff) / param0->unk_0C->unk_0C;
    param0->unk_00 += FX_Mul(FX32_CONST(param0->unk_0C->unk_0A), FX_SinIdx(v0)) >> FX32_SHIFT;
}

void ov48_0225A650(UnkStruct_ov69_0225E478 *param0, s32 param1, s32 param2);

void ov48_0225AFB4(const UnkStruct_ov69_0225ED90 *param0, UnkStruct_ov69_0225E478 *param1);

void ov48_0225AFB4(const UnkStruct_ov69_0225ED90 *param0, UnkStruct_ov69_0225E478 *param1) {
    ov48_0225A650(param1, 0, 0);

    if (param0->unk_08 == 0) {
        if (param0->unk_02 > (param0->unk_0C->unk_02 - 16)) {
            ov48_0225A650(param1, param0->unk_0C->unk_02, (s16)(param0->unk_0C->unk_02 + 32));
        }
    }

    if (param0->unk_0A == 0) {
        if ((param0->unk_02 - 16) <= param0->unk_0C->unk_06) {
            ov48_0225A650(param1, (s16)(param0->unk_0C->unk_06 - 32), param0->unk_0C->unk_06);
        }
    }
}

void ov48_0225B010(u8 *arg0, s32 arg1) {
    VecFx32 vec;

    ov48_0225AEC4(arg0 + (arg1 * 0x10), &vec);
    Sprite_SetMatrix(*(Sprite **)((u8 *)(arg0 + (*(u8 *)((u8 *)(arg0 + arg1) + 0x38) * 4)) + 0x3C), &vec);
}
