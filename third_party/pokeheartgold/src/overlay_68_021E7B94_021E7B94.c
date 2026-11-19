#include "global.h"
#include "overlay_68_move_relearner.h"

struct MoveRelearner;

s32 ov68_021E6C14(MoveRelearner *, s32);

u8 ov68_021E6CD8(MoveRelearner *);

s32 ov68_021E7B94(struct MoveRelearner *arg0);

s32 ov68_021E7B94(MoveRelearner *arg0) {
    s8 v0;

    if (ov68_021E6CD8(arg0) < 4U) {
        ov68_021E6C14(arg0, 1);
        v0 = 0;
    } else {
        ov68_021E6C14(arg0, 4);
        v0 = 2;
    }
    arg0->question = v0;
    arg0->nextState = 3;
    return 2;
}

void ov68_021E7BC8(s32 arg0);

void ov68_021E7BC8(s32 arg0) {
    if (arg0 == 1) {
        G2S_SetBlendBrightness(0x1E, -8);
        return;
    }
    reg_G2S_DB_BLDCNT = 0;
}
