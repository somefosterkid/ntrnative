#include "global.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221E330(void *, s32);

u32 ov65_0221FC54(s32, s32, void *, void *, s32);

extern s32 ov65_0221E600;

extern s32 ov65_0221E740;

s32 ov65_0221E354(void *arg0);

s32 ov65_0221E354(void *arg0) {
    u32 temp_r0;

    temp_r0 = (s32)(ov65_0221FC54(*(s32 *)((u8 *)arg0 + 0x180), (s32)(arg0 + 0x664), arg0 + 0x670, arg0 + 0x36C0, 1));
    switch (temp_r0) {
    case 0:
        break;
    case 1:
    case 3:
        *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E740;
        break;
    case 2:
    case 4:
        ov65_0221E330(arg0, 0x3F);
        *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E600;
        break;
    }
    return 0;
}
