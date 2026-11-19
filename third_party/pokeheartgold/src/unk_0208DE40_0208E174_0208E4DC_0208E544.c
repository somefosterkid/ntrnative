#include "global.h"

#include "pokemon_summary_app.h"

int sub_0208E4DC(PokemonSummary *summaryScreen);

u8 sub_0208E544(PokemonSummary *summaryScreen);

u8 sub_0208E544(PokemonSummary *summaryScreen) {
    int button = sub_0208E4DC(summaryScreen);

    if (button != -1) {
        return button;
    }

    return 0xFF;
}
