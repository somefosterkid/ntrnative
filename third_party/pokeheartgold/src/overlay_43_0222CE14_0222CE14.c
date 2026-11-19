#include "global.h"

#include "overlay_43_palpad.h"
#include "system.h"
#include "touchscreen.h"

void ov43_0222A320(PalPadShared *shared);

extern const TouchscreenHitbox ov43_0222ED7A[];

BOOL ov43_0222CE14(PalPadFriendCode *friendCode, PalPadShared *shared);

BOOL ov43_0222CE14(PalPadFriendCode *friendCode, PalPadShared *shared) {
    if (TouchscreenHitbox_FindHitboxAtTouchNew(ov43_0222ED7A) == 0) {
        shared->inputState = 1;
        return TRUE;
    }
    ov43_0222A320(shared);
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0) {
        return TRUE;
    }
    return FALSE;
}
