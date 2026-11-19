#include "global.h"

#include "system.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sprite_system.h"
#include "overlay_102_easy_chat_sentence_view.h"

BgConfig *ov102_021EA268(struct UnkStruct_ov20_021D2128_t *parent);

void ov102_021EA71C(EasyChatSentenceView *view, u8 dir) {
    view->frame = 0;
    view->dir = dir;
    if (view->dir == 1) {
        reg_G2_WININ = 0x1B | (reg_G2_WININ & ~0x3F);
    }
    Main_SetHBlankIntrCB(ov102_021EA644, view);
}

s32 ov102_021EA754(EasyChatSentenceView *view) {
    BgConfig *bgConfig;
    u32 dy;
    s32 v0;

    if ((u32) view->frame > 5U) {
        if (view->dir == 0) {
            ov102_021EA678(view, 1);
        } else {
            ov102_021EA678(view, 0);
        }
        return 1;
    }
    bgConfig = ov102_021EA268(view->parent);
    if (view->dir != 0) {
        v0 = -8;
    } else {
        v0 = 8;
    }
    view->scrollY = (s16)(view->scrollY + v0);
    view->scrollX = (s16)(view->scrollX + v0);
    BgSetPosTextAndCommit(bgConfig, 0U, BG_POS_OP_SET_XSCALE, (s32) view->scrollX);
    BgSetPosTextAndCommit(bgConfig, 4U, BG_POS_OP_SET_XSCALE, (s32) view->scrollY);
    dy = 0 - v0;
    Sprite_OffsetPositionXY(view->sprites[0], 0, dy);
    Sprite_OffsetPositionXY(view->sprites[1], 0, dy);
    view->frame = (u8)(view->frame + 1);
    if ((view->dir != 0) && (view->frame == 6)) {
        Main_SetHBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
    }
    return 0;
}

void ov102_021EA80C(EasyChatSentenceView *view, NARC *narc) {
    void *ptr;
    NNSG2dPaletteData *pNStack_10;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x1c0, HEAP_ID_35);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xd, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_35);
    ptr = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 0x10, &pNStack_10, HEAP_ID_35);
    MIi_CpuCopy16(pNStack_10->pRawData, view->palette, 0x140);
    DC_FlushRange(view->palette, 0x140);
    Heap_Free(ptr);
    return;
}
