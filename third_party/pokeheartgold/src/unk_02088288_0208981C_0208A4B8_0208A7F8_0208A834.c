#include "global.h"
#include "sprite_resources_header.h"

#include "pokemon.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

enum SummaryDataType {
    SUMMARY_DATA_MON = 0,
    SUMMARY_DATA_PARTY_MON,
    SUMMARY_DATA_BOX_MON,
};

void *sub_0208A520(PokemonSummary *summaryScreen);

void sub_0208A834(PokemonSummary *summaryScreen);

void sub_0208A834(PokemonSummary *summaryScreen) {
    void *monData = sub_0208A520(summaryScreen);

    if (summaryScreen->args->unk11 == SUMMARY_DATA_BOX_MON) {
        BoxMonSwapMoves(monData, summaryScreen->moveSlot, summaryScreen->swapMoveSlot);
    } else {
        MonSwapMoves(monData, summaryScreen->moveSlot, summaryScreen->swapMoveSlot);
    }

    u16 move = summaryScreen->mon.moves[summaryScreen->moveSlot];
    summaryScreen->mon.moves[summaryScreen->moveSlot] = summaryScreen->mon.moves[summaryScreen->swapMoveSlot];
    summaryScreen->mon.moves[summaryScreen->swapMoveSlot] = move;

    u8 curPP = summaryScreen->mon.pp[summaryScreen->moveSlot];
    summaryScreen->mon.pp[summaryScreen->moveSlot] = summaryScreen->mon.pp[summaryScreen->swapMoveSlot];
    summaryScreen->mon.pp[summaryScreen->swapMoveSlot] = curPP;

    u8 maxPP = summaryScreen->mon.maxPP[summaryScreen->moveSlot];
    summaryScreen->mon.maxPP[summaryScreen->moveSlot] = summaryScreen->mon.maxPP[summaryScreen->swapMoveSlot];
    summaryScreen->mon.maxPP[summaryScreen->swapMoveSlot] = maxPP;

    summaryScreen->args->unk38 = TRUE;
}
