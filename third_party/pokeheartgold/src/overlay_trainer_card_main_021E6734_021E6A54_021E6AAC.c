#include "global.h"

#include "system.h"
#include "touchscreen.h"
#include "trainer_card_main.h"
#include "unk_02005D10.h"

extern const TouchscreenHitbox _021E7DB8;

extern const TouchscreenHitbox ov51_021E7DBC;

extern const TouchscreenHitbox ov51_021E7DC0;

u32 ov51_021E6AAC(TrainerCardMain *cardMain, u32 *param_2);

u32 ov51_021E6AAC(TrainerCardMain *cardMain, u32 *param_2) {
    BOOL BVar1;
    BVar1 = System_GetTouchNew();
    if (BVar1 == 0) {
        return 0;
    }
    BVar1 = TouchscreenHitbox_TouchNewIsIn(&_021E7DB8);
    if (BVar1 != 0) {
        *param_2 = 1;
        PlaySE(0x940);
        return 5;
    }
    if ((cardMain->showingBack != 0 && !cardMain->isConnected) && (BVar1 = TouchscreenHitbox_TouchNewIsIn(&ov51_021E7DBC), BVar1 != 0)) {
        *param_2 = 1;
        PlaySE(0x5dc);
        return 4;
    }
    BVar1 = TouchscreenHitbox_TouchNewIsIn(&ov51_021E7DC0);
    if (BVar1 != 0) {
        cardMain->touchX = gSystem.touchX;
        cardMain->touchY = gSystem.touchY;
        *param_2 = 1;
        return 3;
    }
    return 0;
}
