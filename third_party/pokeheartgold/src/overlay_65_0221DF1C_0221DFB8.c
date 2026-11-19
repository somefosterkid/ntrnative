#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221CB5C(void *);

s32 ov65_0221DD34(s32, void *, s32);

extern s32 ov65_0221E144;

extern s32 ov65_0221E9A8;

extern s32 ov65_0221F3F4;

extern TouchscreenHitbox ov65_0221FE6C;

static s32 ov65_0221DFB8(void *arg0);

static s32 ov65_0221DFB8(void *arg0) {
    s32 v0;
    s32 v1;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov65_0221FE6C);
    if (v0 != -1) {
        if (v0 == 0xC) {
            PlaySE(0x5DCU);
            *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E144;
            *((s32 *)&((TradeRoom *)arg0)->selectedSlot[0]) = 0xC;
        } else if (((TradeRoom *)arg0)->monDisplayData[v0].species != 0) {
            *((s32 *)&((TradeRoom *)arg0)->selectedSlot[0]) = v0;
            v1 = *((s32 *)&((TradeRoom *)arg0)->selectedSlot[0]);
            if (v1 < 6) {
                *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E9A8;
            } else if ((v1 >= 6) && (v1 < 0xC)) {
                *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221F3F4;
            }
        }
        ov65_0221DD34(*((s32 *)&((TradeRoom *)arg0)->selectedSlot[0]), *((void **)&((TradeRoom *)arg0)->cursorSprites[0]), 0);
        ov65_0221CB5C(arg0);
        return 1;
    }
    return 0;
}

void ov65_0221DC34(TradeRoom *);

s32 ov65_0221DF1C(void *);

s32 ov65_0221E050(void *arg0);

s32 ov65_0221E050(void *arg0) {
    if (ov65_0221DFB8(arg0) == 0) {
        ov65_0221DF1C(arg0);
    }
    ov65_0221DC34(arg0);
    return 0;
}
