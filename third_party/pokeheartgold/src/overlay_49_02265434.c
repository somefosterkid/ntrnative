#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

typedef struct {
    s32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    fx32 unk_1C;
    fx32 unk_20;
    fx32 unk_24;
} UnkStruct_ov70_022692B4;

BOOL ov49_02265434(UnkStruct_ov70_022692B4 *param0, s32 param1);

void ov49_022655E0(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3);

BOOL ov49_02265434(UnkStruct_ov70_022692B4 *param0, s32 param1) {
    s32 v0;
    BOOL v1 = 0;

    if (param1 > param0->unk_00) {
        v0 = param0->unk_00;
        v1 = 1;
    } else {
        v0 = param1;
    }

    if (param0->unk_10 != 0) {
        param0->unk_04 = param0->unk_14 + FX_Div(FX_Mul(FX32_CONST(v0), param0->unk_10), FX32_CONST(param0->unk_00));
    }

    if (param0->unk_18 != 0) {
        param0->unk_08 = param0->unk_1C + FX_Div(FX_Mul(FX32_CONST(v0), param0->unk_18), FX32_CONST(param0->unk_00));
    }

    if (param0->unk_20 != 0) {
        param0->unk_0C = param0->unk_24 + FX_Div(FX_Mul(FX32_CONST(v0), param0->unk_20), FX32_CONST(param0->unk_00));
    }

    return v1;
}

void ov49_022655E0(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    *arg1 = *(s32 *)((u8 *)arg0 + 4);
    *arg2 = *(s32 *)((u8 *)arg0 + 8);
    *arg3 = *(s32 *)((u8 *)arg0 + 0xC);
}
