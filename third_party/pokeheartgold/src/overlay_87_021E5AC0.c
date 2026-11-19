#include "global.h"
struct ScratchOffCardApp;

#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_020210A0.h"
#include "vram_transfer_manager.h"
#include "screen_fade.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E6780(ScratchOffCardApp *app);

s32 ScratchOffCards_Exit(OverlayManager *man, s32 *state);

s32 ov87_021E5AFC(struct ScratchOffCardApp *arg0);

s32 ScratchOffCards_Exit(OverlayManager *man, s32 *state) {
    ScratchOffCardApp *app;

    app = OverlayManager_GetData(man);
    DisableTouchPad();
    *app->dummy4 = app->dummy2;
    GF_DestroyVramTransferManager();
    ov87_021E6780(app);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_122);
    return 1;
}

s32 ov87_021E5AFC(ScratchOffCardApp *arg0) {
    u8 v0;

    v0 = arg0->subState;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 3, HEAP_ID_122);
        arg0->subState = (u8)(arg0->subState + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            *((s8 *)&arg0->currentCard) = 0;
            return 1;
        }
        break;
    }
    return 0;
}
