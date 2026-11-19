#include "global.h"

#include "overlay_67_app.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

extern u32 _021E6D4C;

u32 ov67_021E5C04(Ov67App *app) {
    BOOL BVar1;
    u32 uVar2;

    BVar1 = TouchscreenHitbox_TouchNewIsIn((TouchscreenHitbox *)&_021E6D4C);
    if ((BVar1 == 1) || (uVar2 = 3, (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0)) {
        PlaySE(0x5dd);
        ov67_021E6BB8(app);
        app->unk_49C = 6;
        uVar2 = 5;
        return uVar2;
    } else {
        return uVar2;
    }
}
