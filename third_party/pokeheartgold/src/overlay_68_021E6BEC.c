#include "global.h"
#include "overlay_68_move_relearner.h"
#include "pokemon.h"

u16 ov68_021E6BEC(MoveRelearner *relearner);

u16 ov68_021E6BFC(MoveRelearner *relearner);

u16 ov68_021E6BEC(MoveRelearner *relearner) {
    MoveRelearnerArgs *args = relearner->args;
    return args->eligibleMoves[args->scrollOffset + args->cursorPos];
}

u16 ov68_021E6BFC(MoveRelearner *relearner) {
    MoveRelearnerArgs *args = relearner->args;
    return GetMonData(args->mon, args->moveSlot + MON_DATA_MOVE1, NULL);
}
