#include "global.h"
#include "overlay_14.h"
struct UnkStruct_ov14_boxapp;
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8944(void *);

void ov14_021E76D0(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, u32 arg3);

s32 ov14_021E895C(void *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021E765C(void *arg0);
s32 ov14_021F5718(void *, void *, s32, s32, s32);
void ov14_021F40E8(void *param_1, s32 param_2);
void ov14_021E765C(void *arg0) {
    void *v0;

    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 2, 0);
    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 3, 0);
    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0xD, 0);
    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0xE, 0);
    ov14_021F53C0(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021E895C(arg0, 0);
    v0 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 3) {
        ov14_021E8944(((void *)((UnkStruct_ov14_boxsys *)v0)->unk_2F0));
        return;
    }
    ov14_021E88A4(((void *)((UnkStruct_ov14_boxsys *)v0)->unk_2F0));
}

void ov14_021E76B8(UnkStruct_ov14_boxapp *param_1);

void ov14_021E76B8(UnkStruct_ov14_boxapp *param_1) {
    ov14_021E765C(param_1);
    ov14_021F40E8(param_1, 0);
    param_1->unk21 = 0xff;
    return;
}

void ov14_021E76D0(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, u32 tileStart) {
    NNSG2dCharacterData *ppCharData;
    void *ptr;

    ptr = GfGfxLoader_GetCharData(NARC_a_0_1_9, arg1 + 0x10, 1, &ppCharData, HEAP_ID_10);
    BG_LoadCharTilesData(arg0->sys->unk_14, 3U, ppCharData->pRawData, ppCharData->szByte, tileStart);
    ov14_021F5718(arg0, ppCharData->pRawData + 0x2A0, tileStart + 0x15, 0x15, 3);
    Heap_Free(ptr);
}
