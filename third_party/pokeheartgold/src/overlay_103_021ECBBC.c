#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ECBBC(MailBoxWork *arg0);

void ov103_021ECC1C(MailBoxWork *param_1);
void ov103_021ECD18(MailBoxWork *param_1);

void ov103_021ECBBC(MailBoxWork *arg0) {
    FreeBgTilemapBuffer(arg0->view->bgConfig, 3U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 2U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 1U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 0U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 7U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 6U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 5U);
    FreeBgTilemapBuffer(arg0->view->bgConfig, 4U);
    Heap_Free(arg0->view->bgConfig);
}

void ov103_021ECC1C(MailBoxWork *param_1) {
    NARC *narc;
    void *ptr;
    void *pvVar1;
    NNSG2dCharacterData *pNStack_18;

    narc = NARC_New(NARC_a_2_5_0, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, param_1->view->bgConfig, GF_BG_LYR_SUB_3, 0, 0, 1, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, param_1->view->bgConfig, GF_BG_LYR_SUB_3, 0, 0, 1, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, param_1->view->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 1, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, param_1->view->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 1, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, param_1->view->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 1, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_157);
    ptr = GfGfxLoader_GetCharDataFromOpenNarc(narc, 1, 1, &pNStack_18, HEAP_ID_157);
    pvVar1 = pNStack_18->pRawData;
    MIi_CpuCopy32((void *)((u8 *)pvVar1 + 0xe0), param_1->view->tileA, 0x20);
    MIi_CpuCopy32((void *)((u8 *)pvVar1 + 0x160), param_1->view->tileB, 0x20);
    Heap_Free(ptr);
    NARC_Delete(narc);
    return;
}

void ov103_021ECD18(MailBoxWork *param_1) {
    MailBoxView *view = param_1->view;
    u32 uVar1;
    uVar1 = Options_GetFrame(param_1->options);
    LoadUserFrameGfx2(view->bgConfig, GF_BG_LYR_MAIN_0, 0x3e2, 0xd, (u8)uVar1, HEAP_ID_157);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_157);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_157);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_157);
    return;
}
