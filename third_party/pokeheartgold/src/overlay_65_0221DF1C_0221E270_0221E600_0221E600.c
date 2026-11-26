#include "global.h"

#include "message_format.h"
#include "unk_02034354.h"
#include "yes_no_prompt.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221E330(void *, s32);

s32 ov65_0221FC54(BgConfig *, void *, s32 *, YesNoPrompt **, s32);

extern s32 ov65_0221E354;

extern s32 ov65_0221E558;

s32 ov65_0221E600(TradeRoom *arg0);

s32 ov65_0221E600(TradeRoom *arg0) {
    u32 v0;

    v0 = ov65_0221FC54(arg0->bgConfig, &arg0->yesNoMenu, &arg0->yesNoMenuStep, &arg0->yesNoPrompt, 1);
    switch (v0) {
    case 0:
        break;
    case 1:
    case 3:
        arg0->subStepCallback = &ov65_0221E558;
        break;
    case 2:
    case 4:
        BufferPlayersName(arg0->palPadStrTemplate, 0U, sub_02034818(arg0->registrationTargetNetId));
        ov65_0221E330(arg0, 0x40);
        arg0->subStepCallback = &ov65_0221E354;
        break;
    }
    return 0;
}
