#include "global.h"
#include "overlay_65_trade_room.h"

extern s32 ov65_0221E270;

extern s32 ov65_0221E858;

s32 ov65_0221E8EC(TradeRoom *arg0);

s32 ov65_0221E8EC(TradeRoom *arg0) {
    s32 v0;
    s32 v1;

    v1 = arg0->partnerSyncStatus[0];
    if (v1 != 0) {
        v0 = arg0->partnerSyncStatus[1];
        if (v0 != 0) {
            if ((v1 == 1) && (v0 == 1)) {
                arg0->subStepCallback = &ov65_0221E858;
            } else {
                arg0->subStepCallback = &ov65_0221E270;
            }
            arg0->partnerSyncStatus[0] = 0;
            arg0->partnerSyncStatus[1] = 0;
            arg0->notificationMsgId = 0x1D;
        }
    }
    return 0;
}
