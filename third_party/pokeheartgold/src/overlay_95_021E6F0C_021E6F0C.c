#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_95_egg_hatch_app.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pokepic.h"
#include "render_text.h"
#include "system.h"
#include "unk_02016EDC.h"
#include "unk_02087E70.h"
#include "vram_transfer_manager.h"

void ov95_021E6228(EggHatchGraphics *);

void ov95_021E7078(EggHatchPrompt *);

BOOL HatchEggApp_Exit(OverlayManager *manager, int *state);

BOOL HatchEggApp_Exit(OverlayManager *manager, int *state) {
    EggHatchCutscene *eggHatch;
    EggHatchGraphics *graphics;

    eggHatch = OverlayManager_GetData(manager);
    graphics = &eggHatch->graphics;
    ov95_021E7078(eggHatch->prompt);
    GF_3DVramMan_Delete(graphics->vramMan);
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 0U);
    GfGfx_EngineATogglePlanes(3U, 0U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
    PaletteData_FreeBuffers(graphics->plttData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(graphics->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(graphics->plttData, PLTTBUF_SUB_BG);
    PaletteData_Free(graphics->plttData);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    FreeBgTilemapBuffer(graphics->bgConfig, 1U);
    FreeBgTilemapBuffer(graphics->bgConfig, 2U);
    FreeBgTilemapBuffer(graphics->bgConfig, 3U);
    Heap_Free(graphics->bgConfig);
    GF_DestroyVramTransferManager();
    PokepicManager_Delete(graphics->monSpriteMan);
    sub_02016F2C(graphics->animMan);
    NARC_Delete(graphics->monDataNarc);
    ov95_021E6228(graphics);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_70);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Field_SetEnvironmentSoundState_None_Unk2();
    return 1;
}
