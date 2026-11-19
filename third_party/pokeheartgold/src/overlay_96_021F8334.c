#include "global.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"

void ov96_021F8334(UnkStruct_ov96_021F8094 *arg0, void *arg1, s32 arg2);

void ov96_021F8334(UnkStruct_ov96_021F8094 *arg0, void *arg1, s32 arg2) {
    UnkStruct_ov96_021F8128 *sum;

    sum = &arg0->extra;
    if (sum->active != 0) {
        GF_AssertFail();
    }
    sum->active = 1;
    sum->sprite = arg1;
    sum->delay = arg2;
}

void ov96_021F8354(UnkStruct_ov96_021F8128 *param_1);

void ov96_021F8354(UnkStruct_ov96_021F8128 *param_1) {
    param_1->active = 0;
    param_1->sprite = NULL;
    param_1->delay = 0;
    return;
}

s32 ov96_021F8360(UnkStruct_ov96_021F8094 *arg0);

s32 ov96_021F8360(UnkStruct_ov96_021F8094 *arg0) {
    s32 v0;

    v0 = 0;
    if ((u32)arg0->total > (u32)arg0->shown) {
        v0 = 1;
    }
    return v0;
}
