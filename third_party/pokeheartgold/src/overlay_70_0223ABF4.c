#include "global.h"
struct GTSApplicationState;

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "sprite.h"
#include "unk_0203A3B0.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_0223ABF4(u8 **param_1);

void ov70_0223ACE4(struct GTSApplicationState *arg0);

void ov70_0223ABF4(u8 **param_1) {
    u32 uVar1;
    BgConfig *bgConfig;

    bgConfig = (BgConfig *)param_1[1];
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    uVar1 = Options_GetFrame(*(Options **)(*param_1 + 0x24));
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_CNT, 1, 2, (u8)uVar1, HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 0x14, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xc00, 1, HEAP_ID_61);
    GfGfxLoader_GXLoadPal(NARC_a_1_0_0, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 0x12, bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 0x2a, bgConfig, GF_BG_LYR_SUB_2, 0, 0, 1, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
    return;
}

void ov70_02238B54(SpriteTemplate *template, struct GTSApplicationState *appState, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen);

void ov70_0223AC98(GTSApplicationState *param_1);

void ov70_0223AC98(GTSApplicationState *param_1) {
    Sprite *pSVar1;
    SpriteTemplate SStack_38;

    ov70_02238B54(&SStack_38, param_1, &param_1->cursorSpriteResourceHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    SStack_38.position.x = 0xd0000;
    SStack_38.position.y = 0x3a000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->listingCursorSprite = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->listingCursorSprite, 1);
    Sprite_SetAnimCtrlSeq(param_1->listingCursorSprite, 0x25);
    sub_0203A880();
    return;
}

void ov70_0223ACE4(GTSApplicationState *arg0) {
    Sprite_Delete(arg0->listingCursorSprite);
}
