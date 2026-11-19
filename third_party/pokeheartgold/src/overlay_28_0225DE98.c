#include "global.h"
#include "overlay_28.h"

void ov28_0225DE98(DowsingMchnNeedle *needle) {
    needle->nextAngle = (u32)((u32)(needle->targetAngle + (needle->angle * 5)) / 6U);
}

void ov28_0225DEB0(DowsingMchnApp *app) {
    s32 v0;

    v0 = ov28_0225DE88(ov28_0225DE64(app->touchX, app->touchY));
    app->needles[0].targetAngle = v0;
    app->needles[1].targetAngle = (s32)(v0 + 0x12C);
    app->needles[2].targetAngle = (s32)(v0 + 0x78);
    app->needles[3].targetAngle = (s32)(v0 + 0xD8);
    app->needleTimer = 6;
}

void ov28_0225DF04(DowsingMchnNeedle *needle) {
    needle->nextAngle = (u32)((u32)(needle->targetAngle + (needle->angle * 3)) >> 2);
}

void ov28_0225DF14(DowsingMchnApp *app) {
    s32 v0;
    s32 v1;

    v1 = ov28_0225DE88(ov28_0225DE64(app->touchX, app->touchY));
    v0 = ov28_0225DE88(ov28_0225DE78(app->nearestX, app->nearestY));
    app->needles[0].targetAngle = (u32)((u32)(v0 + (v1 * 3)) >> 2);
    app->needles[1].targetAngle = (u32)((u32)(v0 + 0x78 + ((v1 + 0x1E) * 3)) >> 2);
    app->needles[2].targetAngle = (u32)((u32)((v0 - 0x87) + ((v1 - 0x24) * 3)) >> 2);
    app->needles[3].targetAngle = (u32)((u32)(v0 + 0x5A + ((v1 + 0x5A) * 3)) >> 2);
    app->needleTimer = 4;
}

void ov28_0225DFA4(DowsingMchnNeedle *needle) {
    needle->nextAngle = (u32)((u32)(needle->targetAngle + (needle->angle * 2)) / 3U);
}

void ov28_0225DFBC(DowsingMchnApp *app) {
    s32 v0;
    s32 v1;

    v1 = ov28_0225DE88(ov28_0225DE64(app->touchX, app->touchY));
    v0 = ov28_0225DE88(ov28_0225DE78(app->nearestX, app->nearestY));
    app->needles[0].targetAngle = (u32)((u32)(v0 + (v1 * 2)) / 3U);
    app->needles[1].targetAngle = (u32)((u32)(v0 + 0x87 + ((v1 + 0x1E) * 2)) / 3U);
    app->needles[2].targetAngle = (u32)((u32)((v0 - 0x78) + ((v1 - 0x24) * 2)) / 3U);
    app->needles[3].targetAngle = (u32)((u32)(v0 + 0x3C + ((v1 + 0x5A) * 2)) / 3U);
    app->needleTimer = 3;
}

void ov28_0225E054(DowsingMchnNeedle *needle) {
    needle->nextAngle = (u32)((u32)(needle->targetAngle + needle->angle) >> 1);
}

void ov28_0225E060(DowsingMchnApp *app) {
    s32 v0;
    s32 v1;

    v1 = ov28_0225DE88(ov28_0225DE64(app->touchX, app->touchY));
    v0 = ov28_0225DE88(ov28_0225DE78(app->nearestX, app->nearestY));
    app->needles[0].targetAngle = (u32)((u32)(v1 + v0) / 3U);
    app->needles[1].targetAngle = (u32)((u32)(v0 + 0x3C + v1) >> 1);
    app->needles[2].targetAngle = (u32)((u32)((v0 - 0x48) + v1) >> 1);
    app->needles[3].targetAngle = (u32)((u32)(v1 + 0x5A + (v0 + 0x2D)) >> 1);
    app->needleTimer = 2;
}
