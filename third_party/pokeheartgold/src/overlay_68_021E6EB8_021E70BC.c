#include "global.h"
#include "overlay_68_move_relearner.h"
#include "sprite_system.h"

void ov68_021E7028(MoveRelearner *relearner, u16 move, u16 index);

void ov68_021E70BC(MoveRelearner *relearner);

void ov68_021E70BC(MoveRelearner *relearner) {
    u32 i = 0;
    s32 y = 40;

    do {
        ManagedSprite_SetPositionXY(relearner->sprites[4 + i], 22, y);
        if (relearner->args->scrollOffset + i >= relearner->numMoves) {
            ManagedSprite_SetDrawFlag(relearner->sprites[4 + i], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(relearner->sprites[4 + i], TRUE);
            ov68_021E7028(relearner, relearner->args->eligibleMoves[relearner->args->scrollOffset + i], i);
        }
        y += 32;
        i++;
    } while (i < 4);
}
