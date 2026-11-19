#include "global.h"

#include "bg_window.h"
#include "unk_0208C3E4.h"
#include "pokemon_summary_app.h"

void sub_0208C560(PokemonSummary *param_1) {
    u32 uVar1;
    Window *window;

    sub_0208C4E0(param_1);
    uVar1 = 0;
    window = param_1->windows;
    do {
        RemoveWindow(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 0x22);
    return;
}
