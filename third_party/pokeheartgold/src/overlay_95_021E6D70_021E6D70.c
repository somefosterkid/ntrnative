#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "options.h"
#include "overlay_95_egg_hatch_app.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pokepic.h"
#include "render_text.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_02016EDC.h"
#include "vram_transfer_manager.h"

void ov95_021E5900(void);

void ov95_021E5928(void);

GF3DVramMan *ov95_021E5954(void);

void ov95_021E59F8(void);

void ov95_021E5A38(BgConfig *);

void ov95_021E5B24(void *);

void ov95_021E619C(EggHatchGraphics *);

void ov95_021E6FC4(BgConfig *);

EggHatchPrompt *ov95_021E7020(BgConfig *, SpriteSystem *, SpriteManager *, enum HeapID);

BOOL HatchEggApp_Init(OverlayManager *manager, int *state);

BOOL HatchEggApp_Init(OverlayManager *manager, int *state) {
    PaletteData *data;
    EggHatchCutscene *eggHatch;
    EggHatchGraphics *graphics;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Heap_Create(HEAP_ID_3, HEAP_ID_70, 0x40000U);
    eggHatch = OverlayManager_CreateAndGetData(manager, sizeof(EggHatchCutscene), HEAP_ID_70);
    memset(eggHatch, 0, sizeof(EggHatchCutscene));
    eggHatch->app = OverlayManager_GetArgs(manager);
    graphics = &eggHatch->graphics;
    graphics->renderDelay = Options_GetTextFrameDelay(eggHatch->app->args.options);
    graphics->frame = Options_GetFrame(eggHatch->app->args.options);
    graphics->vramMan = ov95_021E5954();
    graphics->monSpriteMan = PokepicManager_Create(HEAP_ID_70);
    graphics->monDataNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_70);
    graphics->bgConfig = BgConfig_Alloc(HEAP_ID_70);
    GF_CreateVramTransferManager(0x40U, HEAP_ID_70);
    graphics->animMan = sub_02016EDC(HEAP_ID_70, 1, 0);
    data = PaletteData_Init(HEAP_ID_70);
    graphics->plttData = data;
    PaletteData_SetAutoTransparent(data, 1);
    PaletteData_AllocBuffers(graphics->plttData, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_70);
    PaletteData_AllocBuffers(graphics->plttData, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_70);
    PaletteData_AllocBuffers(graphics->plttData, PLTTBUF_SUB_BG, 0x200U, HEAP_ID_70);
    PaletteData_LoadNarc(graphics->plttData, NARC_graphic_font, 9, HEAP_ID_70, PLTTBUF_SUB_BG, 0x20U, 0xF0U);
    ov95_021E5900();
    ov95_021E5928();
    ov95_021E59F8();
    ov95_021E5A38(graphics->bgConfig);
    ov95_021E6FC4(graphics->bgConfig);
    ov95_021E619C(graphics);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    eggHatch->prompt = ov95_021E7020(graphics->bgConfig, graphics->spriteSys, graphics->spriteMan, HEAP_ID_70);
    Sound_Stop();
    Main_SetVBlankIntrCB(ov95_021E5B24, eggHatch);
    return 1;
}
