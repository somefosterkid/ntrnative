#include "global.h"

#include "heap.h"
#include "item.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F1F38(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F68C0(void *, s32, s32);

s32 ov14_021EFDE4(UnkStruct_ov14_boxapp *arg0) {
    Heap_Free(arg0->sys->unk_C);
    return 0x8B;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F40DC(void *);
#else
s32 ov14_021F40DC(void *);
#endif

s32 ov14_021EFDF4(UnkStruct_ov14_boxapp *arg0) {
    u16 v3;
    UnkStruct_ov14_boxsys *v0;
    UnkStruct_ov14_boxsys *v1;
    void *v2;

    v0 = arg0->sys;
    v2 = v0->unk_C;
    if ((*(u16 *)v2 == arg0->unk21) || (v0->unk_88C8 == 0)) {
        v3 = *(u16 *)((u8 *)v2 + 2);
        ov14_021F1F38(arg0);
        v1 = arg0->sys;
        if (*((u8 *)&v1->unk_448[2]) == 0) {
            if ((ItemIdIsMail(v1->unk_88C8) == 1) && (v3 != arg0->unk21)) {
                PlaySE(0x5F3U);
                sub_0201980C(arg0->sys->unk_2F0, 7);
                ov14_021F34C8(arg0->sys, (u16) arg0->unk21, 0);
                ov14_021F68C0(arg0, 7, 0x28);
                arg0->unk30 = 0x92;
                return 6;
            }
            goto block_7;
        }
        ov14_021F6654(v1, 0x28);
        *((u8 *)&arg0->sys->unk_448[2]) = 0U;
block_7:
        if (arg0->sys->unk_88C8 != 0) {
            PlaySE(0x5EAU);
            ov14_021F34C8(arg0->sys, (u16) arg0->unk21, 0);
        }
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 8);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        return 0x8B;
    }
    ov14_021F6654(v0, 0x28);
    PlaySE(0x5EAU);
    ov14_021F34C8(arg0->sys, *(u16 *)v2, 0);
    ov14_021F34C8(arg0->sys, (u16) arg0->unk21, 1);
    ov14_021F40DC(arg0);
    ov14_021F391C(arg0->sys, 1);
    ov14_021F29E4(arg0->sys, 0xB, 2);
    return 0x8E;
}
