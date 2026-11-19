#include "global.h"
#include "overlay_68_move_relearner.h"

struct MoveRelearner;

void ov68_021E66A0(MoveRelearner *, u8, u8, u8, u8, u8, u8);

void ov68_021E7A18(struct MoveRelearner *arg0, s32 arg1);

s32 ov68_021E7A90(MoveRelearner *arg0, s32 arg1);
s32 ov68_021E7AB4(MoveRelearner *arg0, s32 arg1);

void ov68_021E7A18(MoveRelearner *arg0, s32 arg1) {
    u32 v1;
    u32 v0;

    v1 = 0;
    v0 = 4;
    do {
        if (v1 == arg1) {
            ov68_021E66A0(arg0, 0, v0, 0x10, 4, 0, 8);
        } else if (v1 >= (u32) arg0->numMoves) {
            ov68_021E66A0(arg0, 0, v0, 0x10, 4, 0, 4);
        } else {
            ov68_021E66A0(arg0, 0, v0, 0x10, 4, 0, 0);
        }
        v0 += 4;
        v1 += 1;
    } while (v1 < 4U);
}

s32 ov68_021E7A90(MoveRelearner *arg0, s32 arg1) {
    MoveRelearnerButtonFlash *v0;

    v0 = &arg0->buttonFlash;
    v0->x = 0x18;
    v0->y = 0x14;
    v0->width = 8;
    v0->height = 4;
    v0->timer = 0;
    v0->state = 0;
    v0->nextState = arg1;
    return 0xD;
}

s32 ov68_021E7AB4(MoveRelearner *arg0, s32 arg1) {
    MoveRelearnerButtonFlash *v0;

    v0 = &arg0->buttonFlash;
    v0->x = 0xF;
    v0->y = 0x14;
    v0->width = 8;
    v0->height = 4;
    v0->timer = 0;
    v0->state = 0;
    v0->nextState = arg1;
    return 0xD;
}
