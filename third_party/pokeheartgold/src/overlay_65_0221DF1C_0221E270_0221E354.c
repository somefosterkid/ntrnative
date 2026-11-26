#include "global.h"
#include "yes_no_prompt.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221FC54(BgConfig *, void *, s32 *, YesNoPrompt **, s32);

s32 ov65_0221E330(void *, s32);

extern s32 ov65_0221E600;

extern s32 ov65_0221E740;

s32 ov65_0221E354(TradeRoom *arg0);

s32 ov65_0221E354(TradeRoom *arg0) {
    u32 temp_r0;

    temp_r0 = ov65_0221FC54(arg0->bgConfig, &arg0->yesNoMenu, &arg0->yesNoMenuStep, &arg0->yesNoPrompt, 1);
    switch (temp_r0) {
    case 0:
        break;
    case 1:
    case 3:
        arg0->subStepCallback = &ov65_0221E740;
        break;
    case 2:
    case 4:
        ov65_0221E330(arg0, 0x3F);
        arg0->subStepCallback = &ov65_0221E600;
        break;
    }
    return 0;
}
