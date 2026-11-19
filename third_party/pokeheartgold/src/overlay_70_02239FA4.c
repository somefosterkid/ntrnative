#include "global.h"
struct GTSApplicationState;

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "render_window.h"
#include "sprite.h"
#include "unk_0203A3B0.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02239CF8(GTSApplicationState *);

void ov70_02239FA4(GTSApplicationState *param_1);

void ov70_0223A0C4(struct GTSApplicationState *arg0);

void ov70_02239FA4(GTSApplicationState *param_1) {
    NARC *narc;
    u32 uVar1;
    BgConfig *bgConfig;

    bgConfig = param_1->bgConfig;
    narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x100, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    uVar1 = Options_GetFrame(*(Options **)((u8 *)param_1->playerData + 0x24));
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 0xe, (u8)uVar1, HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1f, 0xb, 0, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x14, bgConfig, GF_BG_LYR_MAIN_1, 0, 0xa00, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x1c, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, 1, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
    ov70_02239CF8(param_1);
    NARC_Delete(narc);
    return;
}

void ov70_02238B54(SpriteTemplate *, GTSApplicationState *, SpriteResourcesHeader *, u32);

void ov70_0223A06C(GTSApplicationState *param_1);

void ov70_0223A06C(GTSApplicationState *param_1) {
    Sprite *pSVar1;
    SpriteTemplate SStack_38;

    ov70_02238B54(&SStack_38, param_1, &param_1->cursorSpriteResourceHeader, 1);
    SStack_38.position.x = 0xd0000;
    SStack_38.position.y = 0x3a000;
    pSVar1 = Sprite_CreateAffine(&SStack_38);
    param_1->listingCursorSprite = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->listingCursorSprite, 1);
    Sprite_SetAnimCtrlSeq(param_1->listingCursorSprite, 0x25);
    Sprite_SetPriority(param_1->listingCursorSprite, 1);
    sub_0203A880();
    return;
}

void ov70_0223A0C4(GTSApplicationState *arg0) {
    Sprite_Delete(arg0->listingCursorSprite);
}
