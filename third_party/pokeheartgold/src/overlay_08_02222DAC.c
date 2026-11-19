#include "global.h"

#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_party.h"
#include "palette.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

u8 ov08_02222DAC(BattleBag *battleBag);
u8 ov08_02222DC4(BattleBag *battleBag);
u8 ov08_02222DEC(BattleBag *battleBag);
u8 ov08_02222E04(BattleBag *battleBag);

u8 ov08_02222DAC(BattleBag *battleBag) {
    if (TextPrinterCheckActive(battleBag->textPrinterID) == 0) {
        return 10;
    }

    return 9;
}

u8 ov08_02222DC4(BattleBag *battleBag) {
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == 1) {
        return battleBag->queuedState;
    }

    return 10;
}

u8 ov08_02222DEC(BattleBag *battleBag) {
    if (battleBag->pressedButtonState == BATTLE_SUB_MENU_BUTTON_STATE_PRESSED) {
        return battleBag->queuedState;
    }

    return 11;
}

u8 ov08_02222E04(BattleBag *battleBag) {
    PaletteData_BeginPaletteFade(battleBag->palette, (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_SUB_OBJ), 0xffff, -8, 0, 16, 0);
    return 14;
}
