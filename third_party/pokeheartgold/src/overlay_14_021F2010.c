#include "global.h"

#include "bag.h"
#include "heap.h"
#include "unk_02005D10.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6094(void *, u32, s32, s32 *);
s32 ov14_021E64D0(void *);
s32 ov14_021F2ED0(void *, u32, u32, s32);

s32 ov14_021E6070(void *, s32, s32, void *);
void *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E71E8(void *);
s32 ov14_021E884C(UnkStruct_ov14_boxsys *);
void ov14_021F3354(UnkStruct_ov14_boxscan *param_1);
s32 ov14_021F6654(void *, s32);

s32 ov14_021F2010(UnkStruct_ov14_boxapp *arg0) {
    Heap_Free(arg0->sys->unk_C);
    return 0x73;
}

s32 ov14_021F2020(void *arg0) {
    s32 v0;
    s32 itemId;
    void *v2;
    u8 v1;

    itemId = ov14_021E6070(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, 6, 0);
    v0 = itemId;
    if (Bag_AddItem(((UnkStruct_ov14_boxapp *)arg0)->bag, (u16)itemId, 1U, HEAP_ID_10) == 1) {
        v2 = ov14_021E60C0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
        ov14_021F673C(arg0, v0, 0x25);
        v0 = 0;
        ov14_021E6094(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, 6, &v0);
        if (ov14_021E64D0(v2) == 1) {
            v1 = ((UnkStruct_ov14_boxapp *)arg0)->unk21;
            ov14_021F2ED0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, v1, ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_4094[v1]);
        }
        ov14_021E75F4(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
    } else {
        PlaySE(0x5F3U);
        ov14_021F675C(arg0, 0x25);
    }
    *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0xE;
    return 6;
}

s32 ov14_021F40DC();

s32 ov14_021F20C4(void *arg0) {
    ov14_021F40DC();
    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x25);
    ov14_021E71E8(arg0);
    ov14_021F3354(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_88DC);
    ov14_021E884C(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    return 0x1A;
}
