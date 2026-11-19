#include "global.h"
#include "overlay_02_hmcut_in.h"

static void ov02_0224A66C(HMCutIn *arg0);

void ov02_0224A674(HMCutIn *);

void ov02_0224A67C(HMCutIn *);

void ov02_0224A6A8(HMCutIn *arg0);

void ov02_0224A648(HMCutIn *arg0);

void ov02_0224A648(HMCutIn *arg0) {
    ov02_0224A6A8(arg0);
    arg0->updateWindow = 0;
    ov02_0224A674(arg0);
    ov02_0224A67C(arg0);
    ov02_0224A66C(arg0);
    arg0->updateWindow = 1;
}

static void ov02_0224A66C(HMCutIn *arg0) {
    arg0->window = 1;
}
