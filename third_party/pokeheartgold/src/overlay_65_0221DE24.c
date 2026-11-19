#include "global.h"
struct TradeRoom;

#include "unk_02035900.h"
#include "overlay_65_trade_room.h"

void ov65_0221DE10(int, s32, s32);

void ov65_0221DE24(struct TradeRoom *param_1, s32 param_2, s32 param_3);

s32 ov65_0221DE58(s32 arg0, s32 arg1);

void ov65_0221DE24(TradeRoom *param_1, s32 param_2, s32 param_3) {
    u16 uVar1;

    if ((param_3 != *((s32 *)&param_1->lastSyncedValue)) || (param_2 != *((s32 *)&param_1->lastSyncedCmd))) {
        uVar1 = sub_0203769C();
        ov65_0221DE10(uVar1, param_2, param_3);
        *((s32 *)&param_1->lastSyncedValue) = param_3;
        *((s32 *)&param_1->lastSyncedCmd) = param_2;
    }
    return;
}

s32 ov65_0221DE58(s32 arg0, s32 arg1) {
    return arg0 + (0x590 * arg1);
}
