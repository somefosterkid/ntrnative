#include "global.h"

#include "constants/heap.h"

#include "message_format.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_02037C94.h"
#include "overlay_65_trade_room.h"

s32 ov65_0221E330(void *, s32);

u32 ov65_0221FC54(s32, s32, void *, void *, s32);

s32 sub_0202C338(WiFiList *, s32);

s32 sub_020311AC(FrontierSave *, s32);

extern s32 ov65_0221E354;

extern s32 ov65_0221E740;

s32 ov65_0221E3BC(void *arg0);

s32 ov65_0221E3BC(void *arg0) {
    u32 v0;

    v0 = (s32)(ov65_0221FC54(*((s32 *)&((TradeRoom *)arg0)->bgConfig), (s32)(arg0 + 0x664), arg0 + 0x670, arg0 + 0x36C0, 1));
    switch (v0) {
    case 0:
        break;
    case 1:
    case 3:
        sub_020311AC(Save_Frontier_GetStatic(*(SaveData **)((u8 *)arg0 + 4)), *((s32 *)&((TradeRoom *)arg0)->friendToDeleteSlot));
        sub_0202C338(*((void **)&((TradeRoom *)arg0)->wifiList), *((s32 *)&((TradeRoom *)arg0)->friendToDeleteSlot));
        sub_0203A280(*(SaveData **)((u8 *)arg0 + 4), *((s32 *)&((TradeRoom *)arg0)->registrationTargetNetId), 0x1F, HEAP_ID_26, 0U);
        *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E740;
        break;
    case 2:
    case 4:
        BufferPlayersName(((MessageFormat *)((TradeRoom *)arg0)->palPadStrTemplate), 0U, sub_02034818((u32) *((s32 *)&((TradeRoom *)arg0)->registrationTargetNetId)));
        ov65_0221E330(arg0, 0x40);
        *((s32 **)&((TradeRoom *)arg0)->subStepCallback) = &ov65_0221E354;
        break;
    }
    return 0;
}
