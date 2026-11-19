#include "global.h"

#include "overlay_02_hmcut_in.h"

void ov02_0224A8D4(HMCutIn *);

s32 ov02_02249AC4(HMCutIn *arg0);

s32 ov02_02249AC4(HMCutIn *arg0) {
    ov02_0224A8D4(arg0);
    arg0->state += 1;
    return 0;
}

void ov02_02249D18(HMCutIn *);

void ov02_0224A4D0(HMCutIn *arg0);

s32 ov02_02249AD8(HMCutIn *arg0);

s32 ov02_02249AD8(HMCutIn *arg0) {
    ov02_0224A4D0(arg0);
    ov02_02249D18(arg0);
    arg0->state += 1;
    return 0;
}
