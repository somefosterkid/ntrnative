#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "overlay_90.h"
#include "heap.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"

void *ov90_02258E54(void *, NARC *, s32, s32, s32, s32, s32, s32, enum HeapID);

s32 ov90_0225A65C(NARC *, s32, enum HeapID);

s32 ov90_0225B254(UnkStruct_ov114_0225E854_t *, NARC *, enum HeapID);

s32 ov90_0225B59C(void *, void *, void *, void *, NARC *, enum HeapID);

void ov90_0225B330(void *);
void ov90_0225B690(void *);
void ov90_0225A960(UnkStruct_ov114_0225E854_t *param_1);

void ov90_0225A850(UnkStruct_ov114_0225E854_t *arg0, enum HeapID heapID) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_9_8, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120U, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x120U, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, arg0->unk_4C.unk_00, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, arg0->unk_4C.unk_00, GF_BG_LYR_SUB_1, 0U, 0x4000U, 0, heapID);
    arg0->unk_2B8[0] = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, 0, arg0->unk_2BC, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, arg0->unk_4C.unk_00, GF_BG_LYR_MAIN_3, 0U, 0U, 0, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, arg0->unk_4C.unk_00, GF_BG_LYR_SUB_1, 0U, 0U, 0, heapID);
    ov90_0225A65C(narc, arg0->unk_07, heapID);
    arg0->unk_248 = ov90_02258E54(&arg0->unk_230, narc, 0xB, 5, 0xC, 0xD, 0xE, 0x1388, heapID);
    ov90_0225B254(arg0, narc, heapID);
    ov90_0225B59C(&arg0->unk_26C, &arg0->unk_4C, &arg0->unk_A0, &arg0->unk_230, narc, heapID);
    NARC_Delete(narc);
}

void ov90_0225A960(UnkStruct_ov114_0225E854_t *param_1) {
    ov90_0225B690(&param_1->unk_26C);
    Heap_Free(param_1->unk_2B8[0]);
    ov90_0225B330(param_1);
    return;
}
