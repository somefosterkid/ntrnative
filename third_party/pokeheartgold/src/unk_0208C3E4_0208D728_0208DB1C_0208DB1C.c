#include "global.h"

#include "bg_window.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

void sub_0208DB1C(PokemonSummary *summary) {
    if (summary->args->moveToLearn != 0) {
        FillWindowPixelBuffer(&summary->pageWindows[12], 0);
        sub_0208D884(summary, 4);
        ScheduleWindowCopyToVram(&summary->pageWindows[12]);
        return;
    }
    ScheduleWindowCopyToVram(&summary->windows[23]);
}
