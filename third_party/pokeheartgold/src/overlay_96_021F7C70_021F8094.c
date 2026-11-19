#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "unk_02005D10.h"

void *ov96_021F86E8(enum HeapID heapID, BgConfig *bgConfig, PokeathlonCourseData *data);

void ov96_021F8094(UnkStruct_ov96_021F8094 *param_1, enum HeapID param_2, BgConfig *param_3, PokeathlonCourseData *param_4);

void ov96_021F8094(UnkStruct_ov96_021F8094 *param_1, enum HeapID param_2, BgConfig *param_3, PokeathlonCourseData *param_4) {
    void *pHVar1;

    pHVar1 = ov96_021F86E8(param_2, param_3, param_4);
    param_1->scene = pHVar1;
    return;
}

void ov96_021F82E4(UnkStruct_ov96_021F8094 *, void *, s32);

void ov96_021F8334(UnkStruct_ov96_021F8094 *, void *, s32);

void *ov96_021F8378(UnkStruct_ov96_021F8094 *, PokeathlonCourseData *, s32, s32);

void ov96_021F87E0(void *, s32);

u16 ov96_021F8978(void *);

void ov96_021F80A8(UnkStruct_ov96_021F8094 *arg0, PokeathlonCourseData *arg1, u8 arg2, s32 arg3);

void ov96_021F80A8(UnkStruct_ov96_021F8094 *arg0, PokeathlonCourseData *arg1, u8 arg2, s32 arg3) {
    s32 v3;
    void *v2;
    u32 v1;
    PokeathlonCourseData *v0;
    s32 v4;
    s32 v5;
    s32 v6;
    s32 v7;

    v1 = arg2;
    v0 = arg1;
    arg0->total = (u8)v1;
    ov96_021F87E0(arg0->scene, arg3);
    v6 = 0;
    v3 = 0;
    v7 = 0;
    do {
        v5 = v6 + 0xA;
        v2 = ov96_021F8378(arg0, v0, 0x100, 0x9C - v7);
        if (ov96_021F8978(v2) == 0) {
            v5 += 0xA;
        }
        ov96_021F82E4(arg0, v2, v5);
        v6 += 0xA;
        v4 = v3 + 1;
        v7 += 0x28;
        v3 = v4;
    } while (v4 < 4);
    if (v1 > 4U) {
        ov96_021F8334(arg0, ov96_021F8378(arg0, v0, 0x100, 0x28), 0);
    }
    arg0->state = 1;
}

void ov96_021F895C(void *, s32, s32);

void ov96_021F8128(UnkStruct_ov96_021F8128 *arg0, s32 arg1, s32 arg2);

void ov96_021F8128(UnkStruct_ov96_021F8128 *arg0, s32 arg1, s32 arg2) {
    s32 v0;

    if (arg0->active != 0) {
        v0 = arg0->delay;
        if (v0 == 0) {
            ov96_021F895C(arg0->sprite, arg1, arg2);
            if (arg0->played == 0) {
                ov96_021F8978(arg0->sprite);
                PlaySE(0x8D7U);
                arg0->played = 1;
            }
        } else {
            arg0->delay = (s32)(v0 - 1);
        }
    }
}
