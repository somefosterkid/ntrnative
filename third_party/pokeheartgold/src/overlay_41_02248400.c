#include "global.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
struct UnkStruct_ov22_02257F50;

struct UnkStruct_ov22_02259560_t;
void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
void ov41_02249B94(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
void ov41_02249BAC(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_02248400(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

s32 ov41_0224888C(void *, s32);
void ov41_022489A8(void *, void *);
void ov41_02248488(struct UnkStruct_ov22_02257F50 *arg0, void *arg1);

void ov41_02248400(UnkStruct_ov22_02259560 *param0, int *param1, int *param2) {
    int v0, v1;
    int v2, v3;
    int v4, v5;
    int v6, v7;
    int v8, v9, v10, v11;
    int v12, v13, v14, v15;

    ov41_02249B94(param0, &v0, &v1);
    ov41_02249B44(param0, &v2, &v3);
    ov41_02249BAC(param0, &v4, &v6, &v5, &v7);

    v8 = v3 + v5;
    v9 = v3 + v1 - v7;
    v10 = v2 + v4;
    v11 = v2 + v0 - v6;
    v14 = (136 + 2) - v10;
    v15 = v11 - ((136 + 2) + (112 - (2 * 2)));
    v12 = (16 + 2) - v8;
    v13 = v9 - ((16 + 2) + (129 - (2 * 2)));

    if (v14 > 0) {
        *param1 = v14;
    } else if (v15 > 0) {
        *param1 = -v15;
    } else {
        *param1 = 0;
    }

    if (v12 > 0) {
        *param2 = v12;
    } else if (v13 > 0) {
        *param2 = -v13;
    } else {
        *param2 = 0;
    }
}

void ov41_02248488(UnkStruct_ov22_02257F50 *arg0, void *arg1) {
    *((s32 *)&arg0->unk_44.unk_00) = (s32) * (s32 *)arg1;
    *((s32 *)&arg0->unk_44.unk_04) = (s32) * (s32 *)((u8 *)arg1 + 4);
    *((s32 *)&arg0->unk_44.unk_08) = (s32) * (s32 *)((u8 *)arg1 + 8);
    *((s32 *)&arg0->unk_44.unk_0C) = (s32) * (s32 *)((u8 *)arg1 + 0xC);
    *((s32 *)&arg0->unk_44.unk_10) = (s32) * (s32 *)((u8 *)arg1 + 0x10);
    *((s32 *)&arg0->unk_44.unk_14) = (s32) * (s32 *)((u8 *)arg1 + 0x14);
    *((s32 *)&arg0->unk_00.unk_08) = (s32) * (s32 *)((u8 *)arg1 + 0x18);
    *((s32 *)&arg0->unk_00.unk_04) = (s32) * (s32 *)((u8 *)arg1 + 0x28);
    ov41_0224888C(arg0, 0);
    ov41_022489A8(arg0, arg1);
}
