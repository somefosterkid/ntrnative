#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_95_egg_hatch_app.h"

void ov95_021E7388(EggHatchPrompt *, s32, s32, s32, s32);

void ov95_021E70BC(EggHatchPrompt *arg0, PaletteData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov95_021E7328(EggHatchPrompt *, Window *, u32);
void ov95_021E7410(EggHatchPrompt *);
void ov95_021E7208(EggHatchPrompt *arg0, s32 arg1, s32 arg2);

void ov95_021E70BC(EggHatchPrompt *arg0, PaletteData *arg1, s32 arg2, s32 arg3, s32 resId, s32 arg5) {
    s32 v1;
    s32 v0;
    PaletteData *data;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;

    data = arg1;
    v0 = arg2;
    v1 = arg3;
    spriteSystem = arg0->spriteSys;
    spriteManager = arg0->spriteMan;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (spriteSystem == NULL) {
        GF_AssertFail();
    }
    if (spriteManager == NULL) {
        GF_AssertFail();
    }
    narc = NARC_New(NARC_a_2_3_7, arg0->heapID);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0xB, 0, 1, 2, v1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0xC, 0, 2, v0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0xD, 0, resId);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0xE, 0, arg5);
    ov95_021E7388(arg0, v0, v1, resId, arg5);
    PaletteData_LoadNarc(data, NARC_a_2_3_7, 0, HEAP_ID_70, PLTTBUF_SUB_BG, 0xA0U, 0U);
    PaletteData_LoadNarc(data, NARC_graphic_font, 9, HEAP_ID_70, PLTTBUF_SUB_BG, 0x20U, 0xF0U);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_70);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_70);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_70);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xA, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_70);
    ToggleBgLayer(5U, 0U);
    ToggleBgLayer(6U, 0U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    NARC_Delete(narc);
}

void ov95_021E7208(EggHatchPrompt *arg0, s32 arg1, s32 arg2) {
    ov95_021E7328(arg0, &arg0->windows[0], arg1);
    ov95_021E7328(arg0, &arg0->windows[1], arg2);
    arg0->state = 0;
    arg0->selection = 1;
    ov95_021E7410(arg0);
    ToggleBgLayer(5U, 1U);
    ToggleBgLayer(6U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    ManagedSprite_SetDrawFlag(arg0->cursorSprite, 1);
}
