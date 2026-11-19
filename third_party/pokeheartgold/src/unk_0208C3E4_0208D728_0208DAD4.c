#include "global.h"

#include "bg_window.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208DAD4(PokemonSummary *param_1) {
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[24]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[25]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[26]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[13]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[14]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->pageWindows[15]);
    return;
}
