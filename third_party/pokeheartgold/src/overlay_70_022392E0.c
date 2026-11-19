#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "options.h"
#include "render_window.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_022392BC(BgConfig *bgConfig);

void ov70_022392E0(BgConfig *arg0);

void ov70_02239C6C(void *arg0);
void ov70_02239CF8(void *arg0);
void ov70_02239330(GTSApplicationState *arg0);

void ov70_022392E0(BgConfig *bgConfig) {
    ov70_022392BC(bgConfig);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
}

void ov70_02239304(struct GTSApplicationState *arg0);

void ov70_02239304(GTSApplicationState *arg0) {
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 0U);
    GfGfx_EngineATogglePlanes(4U, 0U);
    Sprite_SetDrawFlag(arg0->cursorSprite, 0);
}

void ov70_02239330(GTSApplicationState *arg0) {
    BgConfig *bgConfig;
    NARC *narc;

    bgConfig = arg0->bgConfig;
    narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60U, HEAP_ID_61);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_61);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_FIRST, 1U, 0xEU, (u8)(u8)Options_GetFrame(*(Options **)((u8 *)(arg0->playerData) + 0x24)), HEAP_ID_61);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_FIRST, 0x1FU, 0xBU, 0U, HEAP_ID_61);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x10, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0xC00U, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x27, bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0x600U, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x26, bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0x600U, 1, HEAP_ID_61);
    ov70_02239C6C(arg0);
    if (arg0->hasPlayerDescended != 0) {
        ov70_02239CF8(arg0);
        GfGfx_EngineBTogglePlanes(1U, 1U);
    } else {
        GfGfx_EngineBTogglePlanes(1U, 0U);
    }
    NARC_Delete(narc);
}
