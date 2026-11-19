#include "global.h"

#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_74_mystery_gift_statics.h"
#include "overlay_manager.h"

void ov74_02229E28(MysteryGiftAppData *appData, BOOL show);

void ov74_02229F60(Window *window, u8 skipTransfer);

void ov74_0222AA18(OverlayManager *man, Window *window, u32 msgno);

u32 ov74_0222A4CC(OverlayManager *param_1);

u32 ov74_0222A4CC(OverlayManager *param_1) {
    MysteryGiftAppData *pvVar1;

    pvVar1 = OverlayManager_GetData(param_1);
    ov74_0222AA18(param_1, &pvVar1->messageBox, 3);
    ov74_02229F60(&pvVar1->menuWindows[2], 0);
    ov74_02229E28(pvVar1, 1);
    ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_INIT;
    return 0x20;
}
