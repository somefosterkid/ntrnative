#include "global.h"
#include "overlay_28.h"

extern u8 ov28_0225EA90[];
extern u8 ov28_0225EB84[];

void ov28_0225E31C(DowsingMchnApp *app, s32 arg1) {
    s16 temp_r2;
    s32 temp_r3;

    if (arg1 >= 0) {
        temp_r3 = arg1 * ov28_0225EB84[app->proximity];
        app->signalStrength = app->signalStrength + temp_r3;
        temp_r3 = ov28_0225EA90[app->proximity * 2];
        if (app->signalStrength > (s32)temp_r3) {
            app->signalStrength = (s16)temp_r3;
        }
    } else {
        temp_r2 = app->signalStrength;
        if ((s32)temp_r2 > 0) {
            app->signalStrength = (s16)(temp_r2 - 1);
        }
    }
}
