#include "global.h"
struct UnkStruct_ov22_0225A0E4;
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "pokepic.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

s32 ov41_02246D54(void *, s32, s32, s32);

void ov41_022464AC(void *arg0, s32 arg1);

int ov41_022464BC(NNSG2dCharacterData *param0, int param1, int param2, int param3);

s32 ov41_022467E4();
s32 ov41_02246C90(void *, s32);
s32 ov41_02246CC0(void *, s32, s32, s32);
void ov41_02246518(struct UnkStruct_ov22_0225A0E4 *arg0, void *arg1, s32 arg2);

void ov41_022464AC(void *arg0, s32 arg1) {
    ov41_02246D54(arg0, 0x76, 0x13, arg1);
}

int ov41_022464BC(NNSG2dCharacterData *param0, int param1, int param2, int param3) {
    u32 *v0;
    int v1, v2;
    int v3;
    int v4;

    v1 = param0->W;
    v2 = param0->H;
    v1 *= 8;
    v2 *= 8;

    if ((param1 < 0) || (param2 < 0) || (param1 >= v1) || (param2 >= v2)) {
        return 2;
    }

    v0 = param0->pRawData;
    v3 = (param2 * v1) + param1;
    v4 = (v3 % 8);
    v3 /= 8;

    if ((v0[v3] & (0xf << (v4 * 4))) == (param3 << (v4 * 4))) {
        return 1;
    }

    return 0;
}

void ov41_02246518(UnkStruct_ov22_0225A0E4 *arg0, void *arg1, s32 arg2) {
    ov41_022467E4();
    ov41_02246CC0(arg0, arg2, 0x2800, 0x20);
    PokepicManager_SetNeedG3IdentityFlag(*((PokepicManager * *)&arg0->unk_20), 1);
    ov41_02246C90(arg0, arg2);
}
