#include "global.h"

#include "bg_window.h"
#include "overlay_80_0223AC24.h"

static void ov80_0223AED4(Window *param0, s32 param1, s32 param2, s32 param3, s32 param4, u8 param5);

static void ov80_0223AED4(Window *param0, s32 param1, s32 param2, s32 param3, s32 param4, u8 param5);

typedef struct LinearInterpolationTaskS32 {
    int currentValue;
    int startValue;
    int delta;
    int currentStep;
    int numSteps;
} LinearInterpolationTaskS32;
typedef struct UnkStruct_ov5_021DE6BC {
    LinearInterpolationTaskS32 unk_00;
    LinearInterpolationTaskS32 unk_14;
    Window *unk_28;
    u8 unk_2C;
    u8 unk_2D;
    u8 unk_2E;
    u8 unk_2F;
} UnkStruct_ov5_021DE6BC;
void ov80_0223ADB8(LinearInterpolationTaskS32 *task, int start, int end, int numSteps);
BOOL ov80_0223ADC8(LinearInterpolationTaskS32 *task);

void ov80_0223AE14(UnkStruct_ov5_021DE6BC *param0, int param1, int param2, int param3, int param4, int param5, Window *param6, u32 param7, u32 param8, u8 param9);
BOOL ov80_0223AE6C(UnkStruct_ov5_021DE6BC *param0);

void ov80_0223AE14(UnkStruct_ov5_021DE6BC *param0, int param1, int param2, int param3, int param4, int param5, Window *param6, u32 param7, u32 param8, u8 param9) {
    ((param0->unk_2E == 0) ? (void)0 : GF_AssertFail());

    ov80_0223ADB8(&param0->unk_00, param1, param2, param5);
    ov80_0223ADB8(&param0->unk_14, param3, param4, param5);

    param0->unk_28 = param6;
    param0->unk_2C = param7;
    param0->unk_2D = param8;
    param0->unk_2F = param9;
    param0->unk_2E = 1;
}

BOOL ov80_0223AE6C(UnkStruct_ov5_021DE6BC *param0) {
    BOOL v0;
    s16 v1, v2, v3, v4;

    if (param0->unk_2E == 0) {
        return 1;
    }

    v0 = ov80_0223ADC8(&param0->unk_00);
    ov80_0223ADC8(&param0->unk_14);

    v3 = param0->unk_00.currentValue - (param0->unk_2C / 2);
    v1 = param0->unk_14.currentValue - (param0->unk_2D / 2);
    v4 = v3 + param0->unk_2C;
    v2 = v1 + param0->unk_2D;

    ov80_0223AED4(param0->unk_28, v1, v2, v3, v4, param0->unk_2F);

    return v0;
}

static void ov80_0223AED4(Window *param0, s32 param1, s32 param2, s32 param3, s32 param4, u8 param5) {
    if ((param4 <= 0) || (param2 <= 0)) {
        return;
    }

    if ((param3 == param4) || (param1 == param2)) {
        return;
    }

    if (param3 < 0) {
        param3 = 0;
    }

    if (param4 > 256) {
        param4 = 256;
    }

    if (param1 < 0) {
        param1 = 0;
    }

    if (param2 > 256) {
        param2 = 256;
    }

    FillWindowPixelRect(param0, param5, param3, param1, param4 - param3, param2 - param1);
}
