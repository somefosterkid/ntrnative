#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221DE24(void *, s32, s32);

s32 ov65_0221F0C4(void *);

s32 ov65_0221F0D8(void *, s32, s32);

s32 ov65_0221F79C(void *);

s32 ov65_0221F7E0(void *);

u8 ov65_0221FB90(void *, s32, s32, void *, void *);

s32 ov65_0221FC54(BgConfig *, void *, s32 *, YesNoPrompt **, s32);

extern s32 ov65_0221F258;

extern s32 ov65_0221F354;

s32 ov65_0221F120(void *arg0);

s32 ov65_0221F120(void *arg0) {
    s32 temp_r0_2;
    u32 temp_r0;

    temp_r0 = ov65_0221FC54(((TradeRoom *)arg0)->bgConfig, &((TradeRoom *)arg0)->yesNoMenu, &((TradeRoom *)arg0)->yesNoMenuStep, &((TradeRoom *)arg0)->yesNoPrompt, 0);
    switch (temp_r0) {
    case 0:
        break;
    case 1:
    case 3:
        if (ov65_0221F79C(arg0) != 0) {
            temp_r0_2 = ov65_0221F7E0(arg0);
            switch (temp_r0_2) {
            case 0:
                if (*(s32 *)((u8 *)(arg0 + (*(s32 *)((u8 *)arg0 + 0x94) * 0x10)) + 0x6A8) == 0) {
                    ov65_0221F0C4(arg0);
                    ov65_0221F0D8(arg0, 0x17, 0x14);
                    *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F354;
                } else {
                    *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F258;
                }
                break;
            case 1:
                ov65_0221F0D8(arg0, 0x17, 0x25);
                *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F354;
                ov65_0221DE24(arg0, 0x18, 4);
                break;
            case 2:
                ov65_0221F0D8(arg0, 0x17, 0x26);
                *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F354;
                ov65_0221DE24(arg0, 0x18, 4);
                break;
            }
        } else {
            *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F354;
            ov65_0221DE24(arg0, 0x18, 4);
        }
        break;
    case 2:
    case 4:
        FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x180), 0U, 0U, 0U, 0U, 0x20U, 0x18U, 0U);
        ov65_0221FB90(arg0 + 0x5B4, 0x14, 1, *(void **)((u8 *)arg0 + 0x190), *(void **)((u8 *)arg0 + 0x184));
        *(s32 **)((u8 *)arg0 + 0x2220) = &ov65_0221F354;
        ov65_0221DE24(arg0, 0x18, 4);
        break;
    }
    return 0;
}
