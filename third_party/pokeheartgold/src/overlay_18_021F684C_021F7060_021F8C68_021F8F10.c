#include "global.h"

#include "bg_window.h"
#include "overlay_18_pokedex_entry_popup.h"


void ov18_021F8F10(PokedexEntryPopup *param_1) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = param_1->windows;
    do {
        RemoveWindow(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 9);
    return;
}
