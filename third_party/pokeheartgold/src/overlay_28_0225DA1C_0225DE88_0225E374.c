#include "global.h"
#include "overlay_28.h"

extern u8 ov28_0225EA90[];

void ov28_0225E374(DowsingMchnApp *app, u16 *arg1, u16 *arg2) {
    u8 temp_r2;

    temp_r2 = ov28_0225EA90[app->proximity * 2];
    if ((s32)app->signalStrength > (s32)temp_r2) {
        app->signalStrength = (s16)temp_r2;
    }
    *arg1 = ((s16)app->signalStrength / 10) & 1;
    *arg2 = (s16)((s16)app->signalStrength / 20);
}
