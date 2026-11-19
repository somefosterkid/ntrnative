#include "global.h"

#include "overlay_40.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_02233EE8(void *arg0) {
    u32 x;
    u32 y;

    if ((System_GetTouchNewCoords(&x, &y) != 0) && (x > 0x50U) && (x < 0xB0U) && (y > 0x98U) && (y < 0xB0U)) {
        PlaySE(0x57BU);
        ov40_0222BF80(arg0, 2);
    }
    return 0;
}
