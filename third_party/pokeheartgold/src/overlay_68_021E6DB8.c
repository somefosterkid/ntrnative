#include "global.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E6C14(MoveRelearner *, s32);

s32 ov68_021E6DB8(MoveRelearner *arg0);

s32 ov68_021E6DB8(MoveRelearner *arg0) {
    ov68_021E6C14(arg0, 5);
    arg0->nextState = 6;
    return 2;
}

s32 ov68_021E6DD0(MoveRelearner *arg0);

s32 ov68_021E6DD0(MoveRelearner *arg0) {
    arg0->nextState = 0xA;
    return 0;
}
