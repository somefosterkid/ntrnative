// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/dpc_ui.h: the lines in use so far.
#ifndef BWPORT_APP_DPC_UI_H
#define BWPORT_APP_DPC_UI_H

#include "types.h"
#include "app/dpc_main.h"

// What the player did
enum {
    DPCUI_LEFT,
    DPCUI_RIGHT,
    DPCUI_CLOSE,
    DPCUI_RETURN,
    DPCUI_DECIDE,
    // A touched Pokémon: DPCUI_POKE plus its place in the team
    DPCUI_POKE,
    DPCUI_NONE = 0xff,
};

// pokebw2's DpcUi_PageMain
// The input of a page, and of the page while a Pokémon's data is shown
u32 func_ov171_021de8a8(DendouPCWork *wk);
// pokebw2's DpcUi_PokeMain
u32 func_ov171_021de914(DendouPCWork *wk);

#endif // BWPORT_APP_DPC_UI_H
