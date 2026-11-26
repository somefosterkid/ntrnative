#include "global.h"

#include "bg_window.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221DE24(void *, s32, s32);

u8 ov65_0221FB90(void *, s32, s32, void *, void *);

extern s32 ov65_0221EBF8;

s32 ov65_0221EB9C(TradeRoom *arg0);

s32 ov65_0221EB9C(TradeRoom *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
    ov65_0221FB90(&arg0->windows[23], 0x14, 1, arg0->msgLoader, arg0->strTemplate);
    ov65_0221DE24(arg0, 0x18, 2);
    arg0->subStepCallback = &ov65_0221EBF8;
    return 0;
}
