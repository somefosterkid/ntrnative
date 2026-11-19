#include "global.h"
#include "pokemon.h"
#include "overlay_68_move_relearner.h"

u8 ov68_021E6CD8(MoveRelearner *arg0);

u8 ov68_021E6CD8(MoveRelearner *arg0) {
    u8 v0;

    v0 = 0;
loop_1:
    if (GetMonData(arg0->args->mon, v0 + MON_DATA_MOVE1, NULL) != 0) {
        v0 += 1;
        if ((u32)v0 < 4U) {
            goto loop_1;
        }
    }
    return v0;
}

s32 ov68_021E6C14(MoveRelearner *, s32);

s32 ov68_021E6D00(MoveRelearner *arg0);

s32 ov68_021E6D00(MoveRelearner *arg0) {
    ov68_021E6C14(arg0, 3);
    arg0->args->moveSlot = ov68_021E6CD8(arg0);
    arg0->nextState = 5;
    return 2;
}

s32 ov68_021E6C74(MoveRelearner *);

s32 ov68_021E7A18(MoveRelearner *, s32);

s32 ov68_021E6D20(MoveRelearner *arg0);

s32 ov68_021E6D20(MoveRelearner *arg0) {
    ov68_021E6C74(arg0);
    if (arg0->learnButtonActive == 0) {
        ov68_021E7A18(arg0, 5);
    }
    return 1;
}

s32 ov68_021E6D40(MoveRelearner *arg0);

s32 ov68_021E6D40(MoveRelearner *arg0) {
    arg0->args->cancelled = 1;
    return 8;
}

s32 ov68_021E6D4C(MoveRelearner *arg0);

s32 ov68_021E6D4C(MoveRelearner *arg0) {
    ov68_021E6C74(arg0);
    return 1;
}

s32 ov68_021E6D58(MoveRelearner *arg0);

s32 ov68_021E6D58(MoveRelearner *arg0) {
    arg0->nextState = 0xA;
    return 0;
}

s32 ov68_021E6D64(MoveRelearner *arg0);

s32 ov68_021E6D64(MoveRelearner *arg0) {
    ov68_021E6C14(arg0, 7);
    arg0->question = 3;
    arg0->nextState = 3;
    return 2;
}

s32 ov68_021E6D80(MoveRelearner *arg0);

s32 ov68_021E6D80(MoveRelearner *arg0) {
    ov68_021E6C14(arg0, 8);
    arg0->nextState = 8;
    arg0->args->cancelled = 1;
    return 2;
}
