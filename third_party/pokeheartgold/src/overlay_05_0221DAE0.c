#include "global.h"

#include "gf_3d_vramman.h"
#include "heap.h"
#include "unk_02014DA0.h"
#include "unk_02026E30.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

s32 ov05_0221DAE0(UnkStruct_ov10_0221FB28 *arg0);

s32 ov05_0221DAE0(UnkStruct_ov10_0221FB28 *arg0) {
    if (arg0->trainerIntroData->mode != 1) {
        return 0;
    }
    Thunk_G3X_Reset();
    if (sub_020154B0(arg0->unk_B5C) == 0) {
        return 0;
    }
    sub_0201543C();
    sub_02015460();
    return 1;
}

void ov05_0221DB18(UnkStruct_ov10_0221FB28 *arg0);

void ov05_0221DB18(UnkStruct_ov10_0221FB28 *arg0) {
    if (arg0->trainerIntroData->mode == 1) {
        sub_02014EBC(arg0->unk_B5C);
        Heap_Free(arg0->unk_B60);
        GF_3DVramMan_Delete(arg0->unk_B58);
    }
}

u32 ov05_0221DB4C(u32 param0, BOOL param1) {
    u32 v0;
    NNSGfdTexKey v1;

    v1 = NNS_GfdAllocTexVram(param0, param1, 0);
    sub_02015354(v1);

    ((v1 != (u32)0x0) ? (void)0 : GF_AssertFail());

    v0 = NNS_GfdGetTexKeyAddr(v1);

    return v0;
}

u32 ov05_0221DB70(u32 param0, BOOL param1) {
    NNSGfdPlttKey v0;
    u32 v1;

    v0 = NNS_GfdAllocPlttVram(param0, param1, NNS_GFD_ALLOC_FROM_LOW);
    sub_02015394(v0);

    if (v0 == (u32)0x0) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    v1 = NNS_GfdGetPlttKeyAddr(v0);

    return v1;
}
