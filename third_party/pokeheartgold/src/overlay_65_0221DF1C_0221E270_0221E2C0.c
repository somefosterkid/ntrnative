#include "global.h"

#include "bg_window.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221EF0C(void *);

u8 ov65_0221FB90(void *, s32, s32, void *, void *);

extern s32 ov65_0221E050;

s32 ov65_0221E2C0(void *arg0);

s32 ov65_0221E2C0(void *arg0) {
    if ((1 & gSystem.newKeys) || (gSystem.touchNew != 0)) {
        FillBgTilemapRect(((TradeRoom *)arg0)->bgConfig, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
        ov65_0221FB90(arg0 + 0x594, 0xF, 1, *((void **)&((TradeRoom *)arg0)->msgLoader), *((void **)&((TradeRoom *)arg0)->strTemplate));
        *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E050;
        ov65_0221EF0C(arg0);
    }
    return 0;
}

void ov65_0221E330(void *arg0, s32 arg1);

void ov65_0221E330(void *arg0, s32 arg1) {
    ov65_0221FB90(arg0 + 0x5B4, arg1, 1, *((void **)&((TradeRoom *)arg0)->palPadMsgLoader), *((void **)&((TradeRoom *)arg0)->palPadStrTemplate));
}
