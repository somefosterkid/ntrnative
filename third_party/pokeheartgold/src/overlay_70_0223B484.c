#include "global.h"
#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
void ov70_02245084(Window *, void *, u32, u32, u32, u32);

void ov70_0223B484(Window *param_1, void *param_2, void *param_3);

void ov70_0223B4D4(GTSApplicationState *arg0);

void ov70_0223B484(Window *param_1, void *param_2, void *param_3) {
    FillWindowPixelBuffer(param_1, 0);
    FillWindowPixelBuffer(param_1 + 1, 0);
    if (param_2 != 0) {
        ov70_02245084(param_1, param_2, 0, 0, 0, 0x10200);
    }
    if (param_3 != 0) {
        ov70_02245084(param_1 + 1, param_3, 0, 0, 0, 0x10200);
    }
    return;
}

void ov70_0223B4D4(GTSApplicationState *arg0) {
    BgSetPosTextAndCommit(arg0->bgConfig, 0U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 1U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 2U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 3U, BG_POS_OP_SET_XSCALE, arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 4U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 5U, BG_POS_OP_SET_XSCALE, -0x20 - arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 6U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
    BgSetPosTextAndCommit(arg0->bgConfig, 7U, BG_POS_OP_SET_XSCALE, 0 - arg0->unk_F14);
}
