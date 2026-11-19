#include "global.h"

#include "bg_window.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

extern const WindowTemplate _02104D94[36];

void sub_0208C3E4(PokemonSummary *summary) {
    for (u16 window = 0; window < 34; window++) {
        AddWindow(summary->bgConfig, &summary->windows[window], &_02104D94[window]);
        FillWindowPixelBuffer(&summary->windows[window], 0);
    }

    sub_0208C9A4(summary);
    sub_0208C57C(summary);
    sub_0208C614(summary);
    sub_0208C6B4(summary);
}
