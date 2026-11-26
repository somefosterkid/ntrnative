#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "yes_no_prompt.h"
#include "overlay_65_trade_room.h"

void ov65_0221DE24(TradeRoom *, u32, u32);

void ov65_0221F0C4(TradeRoom *);

void ov65_0221F0D8(TradeRoom *, u32, u32);

void ov65_0221FB90(Window *, u32, u32, MsgData *, MessageFormat *);

s32 ov65_0221FC54(BgConfig *, void *, s32 *, YesNoPrompt **, s32);

void ov65_0221F354();

u32 ov65_0221F2A8(TradeRoom *param_1);

u32 ov65_0221F2A8(TradeRoom *param_1) {
    u32 uVar1;

    uVar1 = ov65_0221FC54(param_1->bgConfig, &param_1->yesNoMenu, &param_1->yesNoMenuStep, &param_1->yesNoPrompt, 0);
    switch (uVar1) {
    case 1:
    case 3:
        ov65_0221F0C4(param_1);
        ov65_0221F0D8(param_1, 0x17, 0x14);
        param_1->subStepCallback = ov65_0221F354;
        break;
    case 2:
    case 4:
        FillBgTilemapRect(param_1->bgConfig, 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90(&param_1->windows[23], 0x14, 1, param_1->msgLoader, param_1->strTemplate);
        param_1->subStepCallback = ov65_0221F354;
        ov65_0221DE24(param_1, 0x18, 4);
    }
    return 0;
}
