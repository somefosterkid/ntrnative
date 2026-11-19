#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"

void ov96_021F8128(UnkStruct_ov96_021F8128 *, s32, s32);

void ov96_021F8160(UnkStruct_ov96_021F8094 *arg0, s32 arg1, s32 arg2);

void ov96_021F8160(UnkStruct_ov96_021F8094 *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    UnkStruct_ov96_021F8128 *v1;

    v0 = 0;
    v1 = arg0->slots;
    do {
        ov96_021F8128(v1, arg1, arg2);
        v0 += 1;
        v1++;
    } while (v0 < 4);
}

void ov96_021F893C(void *, s32 *, s32 *);

void ov96_021F8948(void *, s32, s32);

s32 ov96_021F8180(UnkStruct_ov96_021F8128 *arg0, s32 arg1);

s32 ov96_021F8180(UnkStruct_ov96_021F8128 *arg0, s32 arg1) {
    s32 v1;
    s32 v0;
    s32 v2;

    v2 = 0;
    ov96_021F893C(arg0->sprite, &v1, &v0);
    if (arg1 == 0) {
        if (v1 <= 0) {
            ov96_021F8948(arg0->sprite, 0, v0);
            v2 = 1;
        }
    } else if ((arg1 == 1) && (v0 >= 0xC0)) {
        v2 = 1;
    }
    return v2;
}

void ov96_021F81C0(UnkStruct_ov96_021F8094 *param_1, u32 param_2);

void ov96_021F81C0(UnkStruct_ov96_021F8094 *param_1, u32 param_2) {
    param_1->state = 3;
    param_1->holdFrames = param_2;
    return;
}
