#include "global.h"

#include "unk_02019BA4.h"
#include "sprite_system.h"
#include "overlay_68_move_relearner.h"
#include "move_relearner.h"
struct MoveRelearnerArgs;

void ov68_021E7910(MoveRelearner *arg0);

void ov68_021E797C(MoveRelearner *arg0, s32 arg1);

void ov68_021E7910(MoveRelearner *arg0) {
    if ((u32) arg0->numMoves < 4U) {
        GridInputHandler_ClearEnabledFlag(arg0->gridInput, 4U);
        GridInputHandler_ClearEnabledFlag(arg0->gridInput, 5U);
        return;
    }
    if (arg0->args->scrollOffset == 0) {
        GridInputHandler_ClearEnabledFlag(arg0->gridInput, 5U);
    } else {
        GridInputHandler_SetEnabledFlag(arg0->gridInput, 5U);
    }
    if ((s32)(arg0->args->scrollOffset + 4) >= (s32) arg0->numMoves) {
        GridInputHandler_ClearEnabledFlag(arg0->gridInput, 4U);
        return;
    }
    GridInputHandler_SetEnabledFlag(arg0->gridInput, 4U);
}

void ov68_021E797C(MoveRelearner *arg0, s32 arg1) {
    MoveRelearnerArgs *v0;
    ManagedSprite_SetAnimateFlag(arg0->sprites[1], 1);
    ManagedSprite_SetAnimateFlag(arg0->sprites[0], 1);
    ManagedSprite_SetAnimationFrame(arg0->sprites[1], 0U);
    ManagedSprite_SetAnimationFrame(arg0->sprites[0], 0U);
    v0 = arg0->args;
    if (arg1 > 0) {
        if (((s32)((arg0->args->scrollOffset) + 4)) < ((s32)(arg0->numMoves))) {
            ManagedSprite_SetAnim(arg0->sprites[0], 1);
        } else {
            ManagedSprite_SetAnim(arg0->sprites[0], 3);
        }
        ManagedSprite_SetAnim(arg0->sprites[1], 4);
        return;
    }
    if ((arg0->args->scrollOffset) == 0) {
        ManagedSprite_SetAnim(arg0->sprites[1], 7);
    } else {
        ManagedSprite_SetAnim(arg0->sprites[1], 5);
    }
    ManagedSprite_SetAnim(arg0->sprites[0], 0);
}
