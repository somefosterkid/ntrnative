#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "screen_fade.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

s32 ov14_021F01D8(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

s32 ov14_021F01D8(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_10);
    arg0->sys->unk_440 = arg1;
    return 2;
}

s32 ov14_021F0204(void *arg0, s32 arg1);

s32 ov14_021F0204(void *arg0, s32 arg1) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_10);
    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_440 = arg1;
    return 2;
}

void ov14_021E5A44(UnkStruct_ov14_boxsys *param_1, UnkFuncPtr_ov14_boxtask param_2);

s32 ov14_021F0234(struct UnkStruct_ov14_boxapp *param_1, UnkFuncPtr_ov14_boxtask param_2, s32 param_3);

s32 ov14_021F0234(UnkStruct_ov14_boxapp *param_1, UnkFuncPtr_ov14_boxtask param_2, s32 param_3) {
    param_1->unk30 = param_3;
    ov14_021E5A44(param_1->sys, param_2);
    return 5;
}

s32 ov14_021E5A54(UnkStruct_ov14_boxsys *);

s32 ov14_021F0244(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

s32 ov14_021F0244(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    *((s32 *)&arg0->unk30) = arg1;
    ov14_021E5A54(arg0->sys);
    return 5;
}

s32 ov14_021E5EFC(void *, s8);

s32 ov14_021F0254(struct UnkStruct_ov14_boxapp *arg0, s16 arg1);

s32 ov14_021F0254(UnkStruct_ov14_boxapp *arg0, s16 arg1) {
    if (arg1 == 1) {
        ov14_021E5EFC(arg0, 1);
    } else {
        ov14_021E5EFC(arg0, 0);
    }
    *((s16 *)&arg0->sys->unk_438) = arg1;
    return 7;
}

void ov14_021F027C(void *arg0, s8 arg1);

void ov14_021F027C(void *arg0, s8 arg1) {
    *((s8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk_1E[0]) = arg1;
    *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 9;
    ov14_021F0204(arg0, 1);
}
