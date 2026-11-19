#include "global.h"

#include "unk_02019BA4.h"
#include "unk_02005D10.h"
#include "overlay_68_move_relearner.h"
#include "move_relearner.h"
struct MoveRelearnerArgs;

void ov68_021E7604(MoveRelearner *arg0);

void ov68_021E7614(void);
s32 ov68_021E67E0(MoveRelearner *);
s32 ov68_021E68D4(MoveRelearner *, s32);
u16 ov68_021E6BEC(MoveRelearner *);
s32 ov68_021E73A4(MoveRelearner *, u32, s32);
s32 ov68_021E7898(MoveRelearner *, s32);
s32 ov68_021E7910(MoveRelearner *);
s32 ov68_021E797C(MoveRelearner *, s32);
s32 ov68_021E7A18(MoveRelearner *, s32);
void ov68_021E7618(MoveRelearner *arg0, s32 arg1, s32 arg2);

void ov68_021E7604(MoveRelearner *arg0) {
    GridInputHandler_Free(arg0->gridInput);
}

void ov68_021E7614(void) {
}

void ov68_021E7618(MoveRelearner *arg0, s32 arg1, s32 arg2) {
    s32 v3;
    MoveRelearnerArgs *v0;
    MoveRelearnerArgs *v1;
    MoveRelearnerArgs *v2;

    v3 = arg1;
    ov68_021E7898(arg0, 0);
    ov68_021E7A18(arg0, 5);
    if (v3 <= 3) {
        PlaySE(0x5DDU);
        arg0->args->cursorPos = (u16)v3;
        v0 = arg0->args;
        if ((s32)(v0->scrollOffset + v0->cursorPos) < (s32) arg0->numMoves) {
            ov68_021E68D4(arg0, ov68_021E6BEC(arg0));
        } else {
            ov68_021E68D4(arg0, -2);
        }
    } else if (v3 == 6) {
        if ((arg2 == 3) && ((s32)(arg0->args->scrollOffset + 4) < (s32) arg0->numMoves)) {
            PlaySE(0x5DDU);
            v3 = 3;
            GridInputHandler_SetNextInput(arg0->gridInput, 3);
            v1 = arg0->args;
            v1->scrollOffset = (u16)(v1->scrollOffset + 1);
            ov68_021E67E0(arg0);
            ov68_021E68D4(arg0, ov68_021E6BEC(arg0));
            ov68_021E7910(arg0);
            ov68_021E797C(arg0, 1);
        } else if (arg2 == 0) {
            if (arg0->args->scrollOffset != 0) {
                PlaySE(0x5DDU);
                v2 = arg0->args;
                v2->scrollOffset = (u16)(v2->scrollOffset - 1);
                ov68_021E67E0(arg0);
                ov68_021E68D4(arg0, ov68_021E6BEC(arg0));
                ov68_021E7910(arg0);
                ov68_021E797C(arg0, -1);
            }
            v3 = 0;
            GridInputHandler_SetNextInput(arg0->gridInput, 0);
        } else {
            PlaySE(0x5DDU);
            ov68_021E68D4(arg0, -2);
        }
    } else {
        PlaySE(0x5DDU);
    }
    ov68_021E73A4(arg0, (u8)v3, 3);
}
