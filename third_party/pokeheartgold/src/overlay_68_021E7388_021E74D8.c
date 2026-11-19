#include "global.h"
#include "overlay_68_move_relearner.h"
#include "overlay_manager.h"
#include "unk_02088288.h"

void ov68_021E5B14(MoveRelearner *relearner);
u16 ov68_021E6BEC(MoveRelearner *relearner);

extern const u8 _021E7BE8[];

s32 ov68_021E74D8(MoveRelearner *relearner);

s32 ov68_021E74D8(MoveRelearner *relearner) {
    u8 pages[2];

    pages[0] = _021E7BE8[0];
    pages[1] = _021E7BE8[1];
    ov68_021E5B14(relearner);
    relearner->summary.party = (Party *)relearner->args->mon; // the summary shows this one Pokemon (partyCount 1)
    relearner->summary.options = relearner->args->options;
    relearner->summary.unk11 = 0;
    relearner->summary.partySlot = 0;
    relearner->summary.partyCount = 1;
    relearner->summary.moveToLearn = ov68_021E6BEC(relearner);
    relearner->summary.unk12 = 2;
    relearner->summary.unk2C = 1;
    relearner->summary.unk28 = 0;
    relearner->summary.menuInputStatePtr = NULL;
    sub_02089D40(&relearner->summary, pages);
    relearner->summaryManager = OverlayManager_New(&gOverlayTemplate_PokemonSummary, &relearner->summary, HEAP_ID_66);
    return 0xC;
}
