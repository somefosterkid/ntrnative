#include "global.h"

#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_74_mystery_gift_statics.h"
#include "overlay_manager.h"

void ov74_02229F60(Window *window, u8 skipTransfer);

void ov74_0222AA18(OverlayManager *man, Window *window, u32 msgno);

s32 ov74_0222A504(OverlayManager *arg0);

s32 ov74_0222A504(OverlayManager *man) {
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    if ((u32)(v0->canReceiveGiftStatus - 2) <= 2U) {
        return 0x31;
    }
    ov74_02229F60(&v0->menuWindows[2], 0);
    ov74_0222AA18(man, &v0->messageBox, 7);
    return 0x21;
}

s32 ov74_022365FC(void);

s32 ov74_0222A538(void);

s32 ov74_0222A538(void) {
    if (((ov74_0223D0A8.state == WIRELESS_DISTRIBUTION_STATE_40) || (ov74_0223D0A8.state == WIRELESS_DISTRIBUTION_STATE_41) || (ov74_0223D0A8.state == WIRELESS_DISTRIBUTION_STATE_48)) && (ov74_022365FC() != 0)) {
        ov74_0223D0A8.state = WIRELESS_DISTRIBUTION_STATE_43;
    }
    return 0x24;
}
