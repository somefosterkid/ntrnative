#include "global.h"

#include "heap.h"
#include "pokemon_storage_system.h"
#include "overlay_14_box_funcs.h"

typedef struct UnkStruct_ov14_021E8A7C {
    u8 unk_0[0x4];
    PCStorage *unk_4;
    u8 unk_8[0x10];
    void *unk_18;
    u8 unk_1C[0x3];
    u8 unk_1F;
    u8 unk_20[0x1];
    u8 unk_21;
    u8 unk_22[0x5];
    u8 unk_27;
} UnkStruct_ov14_021E8A7C;

s32 ov14_021E8A7C(UnkStruct_ov14_021E8A7C *arg0);

s32 ov14_021E8A7C(UnkStruct_ov14_021E8A7C *arg0) {
    u8 v1;
    void *v0;

    v0 = arg0->unk_18;
    if (arg0->unk_27 == 0) {
        v1 = *(u8 *)((u8 *)v0 + 0x14);
        if ((u32) arg0->unk_21 < 0x1EU) {
            arg0->unk_21 = v1;
        } else {
            arg0->unk_21 = (u8)(v1 + 0x1E);
        }
    }
    if (((u32) arg0->unk_21 < 0x1EU) && (*(s32 *)((u8 *)v0 + 0x38) == 1)) {
        PCStorage_SetBoxModified(arg0->unk_4, arg0->unk_1F);
    }
    Heap_Free(arg0->unk_18);
    return 0;
}
