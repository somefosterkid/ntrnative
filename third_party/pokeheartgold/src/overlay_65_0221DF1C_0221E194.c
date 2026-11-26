#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "yes_no_prompt.h"
#include "overlay_65_trade_room.h"

void ov65_0221DE24(TradeRoom *, u32, u32);

void ov65_0221FB90(Window *, u32, u32, MsgData *, MessageFormat *);

s32 ov65_0221FC54(BgConfig *, void *, s32 *, YesNoPrompt **, s32);

void ov65_0221E050();

void ov65_0221E8EC();

u32 ov65_0221E194(TradeRoom *param_1);

u32 ov65_0221E194(TradeRoom *param_1) {
    u32 uVar1;

    uVar1 = ov65_0221FC54(param_1->bgConfig, &param_1->yesNoMenu, &param_1->yesNoMenuStep, &param_1->yesNoPrompt, 1);
    switch (uVar1) {
    case 1:
    case 3:
        FillBgTilemapRect(param_1->bgConfig, 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90(&param_1->windows[23], 0x1c, 1, param_1->msgLoader, param_1->strTemplate);
        ov65_0221DE24(param_1, 0x18, 1);
        param_1->subStepCallback = ov65_0221E8EC;
        param_1->didConfirmTrade = 0;
        break;
    case 2:
    case 4:
        FillBgTilemapRect(param_1->bgConfig, 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90(&param_1->windows[21], 0xf, 1, param_1->msgLoader, param_1->strTemplate);
        param_1->subStepCallback = ov65_0221E050;
    }
    return 0;
}
