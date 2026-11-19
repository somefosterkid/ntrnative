#include "global.h"
#include "unk_02005D10.h"
#include "overlay_02_hmcut_in.h"

void ov02_0224AB58(HMCutIn *);

s32 ov02_0224ADF0(HMCutIn *);

s32 ov02_02249B80(HMCutIn *arg0);

s32 ov02_0224AB8C(HMCutIn *);
u32 ov02_02249BA8(HMCutIn *param_1);

s32 ov02_02249B80(HMCutIn *arg0) {
    s32 v0;
    v0 = (arg0->subState) + 1;
    arg0->subState = v0;
    if (v0 >= 0x14) {
        arg0->subState = 0;
        arg0->state = arg0->state + 1;
        ov02_0224AB58(arg0);
        ov02_0224ADF0(arg0);
    }
    return 1;
}

u32 ov02_02249BA8(HMCutIn *param_1) {
    s32 iVar1;

    iVar1 = ov02_0224AB8C(param_1);
    if (iVar1 != 3) {
        return 0;
    }
    PlaySE(0x648);
    param_1->windowDelta = 0x800;
    param_1->windParticlesAnimState = 2;
    param_1->state = param_1->state + 1;
    return 1;
}
