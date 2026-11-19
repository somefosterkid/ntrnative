#include "global.h"
struct ScratchOffCardApp;

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E6AF4(struct ScratchOffCardApp *param_1);

void ov87_021E6B28(struct ScratchOffCardApp *arg0);

s32 ov87_021E6C40(void);
s32 ov87_021E6C60(BgConfig *);
s32 ov87_021E6DBC(void *, s32);
s32 ov87_021E6E00(void *, s32);
s32 ov87_021E6E44(void);
s32 ov87_021E6E78(void);
s32 ov87_021E6EB0(void *, s32);
s32 ov87_021E6EF4(void *, s32);
void ov87_021E6B38(struct ScratchOffCardApp *arg0);
void ov87_021E7AF8(ScratchOffCardsAppSpriteManager *spriteMan);
void ov87_021E6BA8(ScratchOffCardApp *app);
void ov87_021E6BB8(BgConfig *arg0);

void ov87_021E6AF4(ScratchOffCardApp *param_1) {
    YesNoPromptTemplate YStack_18;

    YStack_18.bgConfig = param_1->bgConfig;
    YStack_18.bgId = 2;
    YStack_18.tileStart = 0x380;
    YStack_18.plttSlot = 9;
    YStack_18.x = 0x18;
    YStack_18.y = 8;
    YStack_18.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(param_1->yesNoMenu, &YStack_18);
    return;
}

void ov87_021E6B28(ScratchOffCardApp *arg0) {
    YesNoPrompt_Destroy(arg0->yesNoMenu);
}

void ov87_021E6B38(ScratchOffCardApp *arg0) {
    ov87_021E6C40();
    ov87_021E6C60(*((void **)&arg0->bgConfig));
    arg0->plttData = PaletteData_Init(HEAP_ID_122);
    PaletteData_AllocBuffers(arg0->plttData, PLTTBUF_MAIN_OBJ, 0x200U, HEAP_ID_122);
    PaletteData_AllocBuffers(arg0->plttData, PLTTBUF_MAIN_BG, 0x200U, HEAP_ID_122);
    ov87_021E6DBC(arg0, 7);
    ov87_021E6E44();
    ov87_021E6E00(arg0, 6);
    ov87_021E6EB0(arg0, 1);
    ov87_021E6E78();
    ov87_021E6EF4(arg0, 0);
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineBTogglePlanes(1U, 0U);
}

void ov87_021E6BA8(ScratchOffCardApp *app) {
    ov87_021E7AF8(&app->spriteMan);
}

void ov87_021E6BB8(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 7U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    Heap_Free(bgConfig);
}
