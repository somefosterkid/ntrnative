#include "global.h"

#include "bg_window.h"
#include "overlay_71_bg_slide_state.h"

s32 ov71_0224AFB8(SysTask *);

void ov71_0224AF58(SysTask *arg0, void *param);

void ov71_0224AF58(SysTask *arg0, void *param) {
    BgSlideState *arg1 = param;
    if (arg1->framesRemaining != 0) {
        arg1->mainBgOffset = (s32)(arg1->mainBgOffset + arg1->offsetStep);
        arg1->subBgOffset = (s32)(arg1->subBgOffset + arg1->offsetStep);
        BgSetPosTextAndCommit(arg1->bgConfig, 2U, BG_POS_OP_SET_XSCALE, (s32) arg1->mainBgOffset >> 0xC);
        BgSetPosTextAndCommit(arg1->bgConfig, 6U, BG_POS_OP_SET_XSCALE, (s32) arg1->subBgOffset >> 0xC);
        arg1->framesRemaining = (s32)(arg1->framesRemaining - 1);
        return;
    }
    BgSetPosTextAndCommit(arg1->bgConfig, 2U, BG_POS_OP_SET_XSCALE, 0);
    BgSetPosTextAndCommit(arg1->bgConfig, 6U, BG_POS_OP_SET_XSCALE, 0xC0);
    ov71_0224AFB8(arg0);
}
