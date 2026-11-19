#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "render_window.h"
#include "yes_no_prompt.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E8F6C(struct EasyChatApp *app);

u8 ov102_021E9084(struct EasyChatApp *app);

BgConfig *ov102_021EA268(struct UnkStruct_ov20_021D2128_t *parent, NARC *narc);

s32 sub_0200E948(void *, s32, s32);

void ov102_021EA41C(EasyChatSentenceView *view, NARC *narc) {
    BgConfig *temp_r6;
    s32 temp_r4;
    u32 temp_r7;
    void *temp_r0;

    temp_r6 = ov102_021EA268(view->parent, narc);
    ov102_021EA80C(view, narc);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, temp_r6, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, HEAP_ID_35);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, temp_r6, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, HEAP_ID_35);
    view->scrollY = -0xC0;
    BgSetPosTextAndCommit(temp_r6, 4U, BG_POS_OP_SET_XSCALE, (s32) view->scrollY);
    temp_r7 = GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, temp_r6, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, HEAP_ID_35);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, temp_r6, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, HEAP_ID_35);
    temp_r4 = temp_r7 >> 5;
    AddWindowParameterized(temp_r6, &view->windows[0], 0U, 3U, 1U, 0x1BU, 4U, 0U, (u16)(s32)(s32)temp_r4);
    AddWindowParameterized(temp_r6, &view->windows[1], 4U, 3U, 1U, 0x1BU, 4U, 0U, (u16)(s32)(s32)temp_r4);
    AddWindowParameterized(temp_r6, &view->windows[2], 0U, 2U, 0x15U, 0x1BU, 2U, 0xBU, (s32)(s32)(s32)(temp_r4 + 0x6C));
    AddWindowParameterized(temp_r6, &view->windows[3], 0U, 2U, 0x15U, 0x13U, 2U, 0xBU, (s32)(s32)(u32)(temp_r4 + 0x6C));
    AddWindowParameterized(temp_r6, &view->windows[4], 0U, 0x19U, 0xCU, 8U, 4U, 0xBU, (u16)(s32)(s32)(temp_r4 + 0xA2));
    view->yesNoTile = (s16)(temp_r4 + 0xA2);
    view->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_35);
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, 0, temp_r6, GF_BG_LYR_MAIN_FIRST, temp_r4 + 0xDE, 0U, 0, HEAP_ID_35);
    view->tileBase = (s32)(temp_r4 + 0xDE);
    GfGfxLoader_GXLoadPal(NARC_a_0_3_8, 0x19, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, 0x20U, HEAP_ID_35);
    view->frameTile = (s16)(temp_r4 + 0xE7);
    LoadUserFrameGfx2(temp_r6, GF_BG_LYR_MAIN_FIRST, (s32)(temp_r4 + 0xE7), 0xFU, (u8)(s32)ov102_021E9084(view->app), HEAP_ID_35);
    temp_r0 = &view->windows[2];
    view->textWindow = temp_r0;
    sub_0200E948(temp_r0, temp_r4 + 0xE7, 0xF);
    PutWindowTilemap(&view->windows[0]);
    PutWindowTilemap(&view->windows[1]);
    PutWindowTilemap(&view->windows[2]);
    PutWindowTilemap(&view->windows[3]);
    ov102_021EA8C0(view);
    ov102_021EAA3C(view);
    ov102_021EAE40(view, 0);
    ov102_021EA920(view);
    if (ov102_021E8F6C(view->app) == 2) {
        ov102_021EAC20(view);
    }
}
