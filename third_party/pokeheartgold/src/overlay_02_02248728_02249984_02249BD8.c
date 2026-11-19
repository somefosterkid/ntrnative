#include "global.h"
#include "overlay_02_hmcut_in.h"

s32 ov02_02249420(void);

s32 ov02_0224A69C(void *, s32, s32, s32, s32);

s32 ov02_0224AB8C(void *);

s32 ov02_02249BD8(HMCutIn *arg0);

s32 ov02_02249BD8(HMCutIn *arg0) {
    s32 v0;

    arg0->updateWindow = 0;
    arg0->windowY1 = (s32)(arg0->windowY1 + arg0->windowDelta);
    arg0->windowY2 = (s32)(arg0->windowY2 - arg0->windowDelta);
    v0 = arg0->windowDelta + 0x800;
    arg0->windowDelta = v0;
    if (v0 > 0x10000) {
        arg0->windowDelta = 0x10000;
    }
    if ((s32) arg0->windowY1 >= 0x5F000) {
        arg0->windowY1 = 0x5F000;
    }
    if ((s32) arg0->windowY2 <= 0x61000) {
        arg0->windowY2 = 0x61000;
    }
    ov02_0224A69C(arg0, arg0->windowX1, arg0->windowY1, arg0->windowX2, arg0->windowY2);
    arg0->updateWindow = 1;
    if ((arg0->screenFaded == 0) && (ov02_0224AB8C(arg0) == 4)) {
        arg0->screenFaded = 1;
        ov02_02249420();
    }
    if ((arg0->windowY1 == 0x5F000) && (arg0->windowY2 == 0x61000)) {
        arg0->planeMaskInside = 0x11;
        arg0->windParticlesAnimState = 1;
        arg0->state = arg0->state + 1;
    }
    return 0;
}
