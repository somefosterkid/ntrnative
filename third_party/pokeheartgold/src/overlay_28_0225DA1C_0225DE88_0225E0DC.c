#include "global.h"
#include "overlay_28.h"

void ov28_0225E0DC(DowsingMchnNeedle *needle) {
    needle->nextAngle = (u32)((u32)(needle->angle + (needle->targetAngle * 2)) / 3U);
}

void ov28_0225E0F4(DowsingMchnApp *app) {
    s32 v0;
    s32 v1;

    v1 = (app->nearestX * 8) + 0x54;
    v0 = (app->nearestY * 8) + 0x64;
    app->needles[0].targetAngle = ov28_0225DE88(ov28_0225DE78(v1 - app->touchX, v0 - app->touchY));
    app->needles[1].targetAngle = ov28_0225DE88(ov28_0225DE78((v1 + 0xA) - app->touchX, (v0 + 0xC) - app->touchY));
    app->needles[2].targetAngle = ov28_0225DE88(ov28_0225DE78((v1 - 0xC) - app->touchX, (v0 - 4) - app->touchY));
    app->needles[3].targetAngle = ov28_0225DE88(ov28_0225DE78((v1 - 8) - app->touchX, (v0 - 8) - app->touchY));
    app->needleTimer = 3;
}
