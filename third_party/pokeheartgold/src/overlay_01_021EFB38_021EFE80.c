#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_01_unk_struct_ov83_0223_d9_ec.h"
#include "overlay_01_unk_struct_ov83_0223_da94.h"

BOOL ov01_021EFE80(UnkStruct_ov83_0223D9EC *param0);

enum Screen {
    SCREEN_TOP = 1,
    SCREEN_BOTTOM,
};
void ov01_021EFF90(enum Screen screen, int brightness);

BOOL ov01_021EFE80(UnkStruct_ov83_0223D9EC *param0) {
    fx32 v0;

    v0 = FX_Mul(param0->unk_08, param0->unk_0C << FX32_SHIFT);
    v0 = FX_Div(v0, param0->unk_10 << FX32_SHIFT);

    param0->unk_00 = v0 + param0->unk_04;

    if ((param0->unk_0C + 1) <= param0->unk_10) {
        param0->unk_0C++;
        return 0;
    }

    param0->unk_0C = param0->unk_10;

    return 1;
}

void ov01_021EFEC8(UnkStruct_ov83_0223DA94 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

void ov01_021EFEC8(UnkStruct_ov83_0223DA94 *param0, fx32 param1, fx32 param2, fx32 param3, int param4) {
    fx32 v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;

    v2 = param2 - param1;

    v0 = (param4 * param4) << FX32_SHIFT;
    v1 = FX_Mul(param3, param4 * FX32_ONE);
    v1 = v2 - v1;
    v1 = FX_Mul(v1, 2 * FX32_ONE);
    v3 = FX_Div(v1, v0);

    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_08 = param3;
    param0->unk_0C = v3;
    param0->unk_10 = 0;
    param0->unk_14 = param4;
}

BOOL ov01_021EFF28(UnkStruct_ov83_0223DA94 *param0);

BOOL ov01_021EFF28(UnkStruct_ov83_0223DA94 *param0) {
    fx32 v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;

    v3 = FX_Mul(param0->unk_08, param0->unk_10 << FX32_SHIFT);
    v1 = (param0->unk_10 * param0->unk_10) << FX32_SHIFT;
    v2 = FX_Mul(param0->unk_0C, v1);
    v2 = FX_Div(v2, 2 * FX32_ONE);
    v0 = v3 + v2;

    param0->unk_00 = param0->unk_04 + v0;

    if ((param0->unk_10 + 1) <= param0->unk_14) {
        param0->unk_10++;
        return 0;
    }

    param0->unk_10 = param0->unk_14;
    return 1;
}

void ov01_021EFF90(enum Screen screen, int brightness) {
    screen == SCREEN_TOP
        ? GX_SetMasterBrightness(brightness)
        : GXS_SetMasterBrightness(brightness);
}
