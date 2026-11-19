#include "global.h"
#include "overlay_28.h"
#include "touchscreen.h"

s32 ov28_0225E7D4(DowsingMchnApp *app) {
    u32 x;
    u32 y;

    System_GetTouchHeldCoords(&x, &y);
    if ((app->touchX != x) || (app->touchY != y)) {
        app->touchX = x;
        app->touchY = y;
        return 0;
    }
    return 1;
}
