#include "global.h"

#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_manager.h"

s32 ov74_02235390(s32);

s32 ov74_0223539C(s32, s32, void *, s32);

s32 ov74_0222A6C0(OverlayManager *arg0);

s32 ov74_0222A6C0(OverlayManager *man) {
    MysteryGiftAppData *v0;

    v0 = OverlayManager_GetData(man);
    if (v0->unk_5C8 == 0x1E) {
        v0->unk_5CC = 1;
    }
    ov74_02235390(1);
    ov74_0223539C(0, 0x35, v0->statePtr, 2);
    return -1;
}
