#include "global.h"

#include "constants/heap.h"

#include "message_format.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_02037C94.h"
#include "yes_no_prompt.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221E330(void *, s32);

s32 ov65_0221FC54(BgConfig *, void *, s32 *, YesNoPrompt **, s32);

s32 sub_0202C338(WiFiList *, s32);

s32 sub_020311AC(FrontierSave *, s32);

extern s32 ov65_0221E354;

extern s32 ov65_0221E740;

s32 ov65_0221E3BC(TradeRoom *arg0);

s32 ov65_0221E3BC(TradeRoom *arg0) {
    u32 v0;

    v0 = ov65_0221FC54(arg0->bgConfig, &arg0->yesNoMenu, &arg0->yesNoMenuStep, &arg0->yesNoPrompt, 1);
    switch (v0) {
    case 0:
        break;
    case 1:
    case 3:
        sub_020311AC(Save_Frontier_GetStatic(arg0->saveData), arg0->friendToDeleteSlot);
        sub_0202C338(arg0->wifiList, arg0->friendToDeleteSlot);
        sub_0203A280(arg0->saveData, arg0->registrationTargetNetId, 0x1F, HEAP_ID_26, 0U);
        arg0->subStepCallback = &ov65_0221E740;
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
