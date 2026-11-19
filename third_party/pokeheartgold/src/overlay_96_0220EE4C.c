#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

extern s32 ov96_0221CF58;

void ov96_0220EE4C(UnkStruct_ov96_0220E3F0 *arg0);

void ov96_0220EE8C(UnkStruct_ov96_0220E3F0 *arg0);
void ov96_0220EFD0(u8 *arg0, BgConfig *arg1, enum HeapID arg2);

void ov96_0220EE4C(UnkStruct_ov96_0220E3F0 *arg0) {
    s32 v0;
    s32 *v1;

    RemoveWindow(&arg0->window);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->msgData);
    v1 = &ov96_0221CF58;
    v0 = 0;
    do {
        FreeBgTilemapBuffer(arg0->bgConfig, (u8)*v1);
        v0 += 1;
        v1 += 1;
    } while (v0 < 7);
    FontID_Release(FONT_UNK_4);
    Heap_Free(arg0->bgConfig);
}

void ov96_0220EE8C(UnkStruct_ov96_0220E3F0 *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 4, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 2, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 5, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 1, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_5, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
    ScheduleSetBgPosText(arg0->bgConfig, 1U, BG_POS_OP_ADD_XSCALE, 8);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 4, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 0xB, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 0xB, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 5, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 0xC, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 0xD, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_5, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapId);
}

void ov96_0220EFD0(u8 *arg0, BgConfig *bgConfig, enum HeapID heapID) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (bgConfig == NULL) {
        GF_AssertFail();
    }
    arg0[0] = (u8)(arg0[0] + 1);
    if ((u32)arg0[0] >= 4U) {
        GfGfxLoader_LoadScrnData(NARC_a_2_3_5, arg0[1] + 5, bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, heapID);
        GfGfxLoader_LoadScrnData(NARC_a_2_3_5, arg0[1] + 5, bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, heapID);
        arg0[1] = (u8)((s32)(arg0[1] + 1) % 6);
        arg0[0] = 0U;
    }
}
