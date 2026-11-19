#include "global.h"
#include "sprite.h"
#include "overlay_02_hmcut_in.h"

void ov02_02249D40(HMCutIn *arg0);

u32 ov02_02249AF0(HMCutIn *param_1);

void ov02_0224A690(HMCutIn *);
void ov02_0224AB58(HMCutIn *);
void ov02_0224AC38(HMCutIn *);
void ov02_0224B6B0(HMCutIn *, s32);
s32 ov02_02249B10(HMCutIn *arg0);
s32 ov02_0224AB8C(HMCutIn *);
s32 ov02_02249B38(HMCutIn *arg0);
s32 ov02_0224ADF0(HMCutIn *work);
s32 ov02_02249B60(HMCutIn *arg0);

u32 ov02_02249AF0(HMCutIn *param_1) {
    if (param_1->resourceSourcesFreed == 0) {
        return 0;
    }
    ov02_02249D40(param_1);
    param_1->state = param_1->state + 1;
    return 1;
}

s32 ov02_02249B10(HMCutIn *arg0) {
    ov02_0224AB58(arg0);
    ov02_0224AC38(arg0);
    ov02_0224A690(arg0);
    ov02_0224B6B0(arg0, 1);
    arg0->state += 1;
    return 0;
}

s32 ov02_02249B38(HMCutIn *arg0) {
    if (ov02_0224AB8C(arg0) != 2) {
        return 0;
    }
    Sprite_SetAnimCtrlSeq(arg0->playerSprite, 1);
    arg0->state = arg0->state + 1;
    return 0;
}

s32 ov02_02249B60(HMCutIn *arg0) {
    s32 v0;

    v0 = arg0->subState + 1;
    arg0->subState = v0;
    if (v0 >= 0x14) {
        arg0->subState = 0;
        arg0->state = arg0->state + 1;
        ov02_0224ADF0(arg0);
    }
    return 1;
}
