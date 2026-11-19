#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "system.h"
#include "yes_no_prompt.h"
#include "overlay_111.h"

s32 ov111_021E5C94(s32);

s32 ov111_021E5CB4(void);

s32 ov111_021E5CD4(void);

void ov111_021E5DF0(void *param_1);

void ov111_021E5F50(BugContestSwapMon *);

void ov111_021E6000(BugContestSwapMon *);

void ov111_021E60D4(BugContestSwapMon *);

void ov111_021E6180(BugContestSwapMon *);

void ov111_021E5E34(OverlayManager *arg0);

void ov111_021E5E34(OverlayManager *man) {
    BugContestSwapMonArgs *args;
    BugContestSwapMon *swapMon;
    SwapMonText *text;

    args = OverlayManager_GetArgs(man);
    if (args == NULL) {
        GF_AssertFail();
    }
    Heap_Create(HEAP_ID_3, HEAP_ID_148, 0x30000U);
    swapMon = OverlayManager_CreateAndGetData(man, sizeof(BugContestSwapMon), HEAP_ID_148);
    MI_CpuFill8(swapMon, 0U, sizeof(BugContestSwapMon));
    swapMon->heapID = HEAP_ID_148;
    swapMon->args = args;
    swapMon->state = ov111_021E5C94((u8)args->noPokemonCaught);
    ov111_021E5CD4();
    reg_GX_POWCNT &= 0xFFFF7FFF;
    ov111_021E5CB4();
    ov111_021E5F50(swapMon);
    ov111_021E6000(swapMon);
    ov111_021E60D4(swapMon);
    ov111_021E6180(swapMon);
    swapMon->yesNoPrompt = YesNoPrompt_Create(swapMon->heapID);
    text = ov111_021E67C4(swapMon->heapID);
    swapMon->text = text;
    ov111_021E67EC(text, swapMon->bgConfig, 1, args->options);
    swapMon->panels = ov111_021E66DC(swapMon->heapID, swapMon->bgConfig, swapMon->text, swapMon->spriteSystem, swapMon->spriteManager, swapMon->iconPalette);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    Main_SetVBlankIntrCB(ov111_021E5DF0, swapMon);
}

void ov111_021E5FD4(BugContestSwapMon *);

void ov111_021E6170(SpriteSystem *, SpriteManager *);

void ov111_021E5F04(OverlayManager *arg0);

void ov111_021E5F04(OverlayManager *man) {
    ManagedSprite *managedSprite;
    BugContestSwapMon *v0;

    v0 = OverlayManager_GetData(man);
    YesNoPrompt_Destroy(v0->yesNoPrompt);
    ov111_021E6710(v0->panels);
    ov111_021E685C(v0->text);
    managedSprite = v0->cursorSprite;
    if (managedSprite != NULL) {
        Sprite_DeleteAndFreeResources(managedSprite);
        v0->cursorSprite = NULL;
    }
    ov111_021E6170(v0->spriteSystem, v0->spriteManager);
    ov111_021E5FD4(v0);
    ov111_021E5CD4();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_148);
}
