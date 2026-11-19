#include "global.h"

#include "bg_window.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208DB64(PokemonSummary *summary) {
    FillWindowPixelBuffer(&summary->pageWindows[summary->moveSlot + 8], 0);
    FillWindowPixelBuffer(&summary->pageWindows[summary->swapMoveSlot + 8], 0);
    sub_0208D884(summary, summary->moveSlot);
    sub_0208D884(summary, summary->swapMoveSlot);
    ScheduleWindowCopyToVram(&summary->pageWindows[summary->moveSlot + 8]);
    ScheduleWindowCopyToVram(&summary->pageWindows[summary->swapMoveSlot + 8]);
}
