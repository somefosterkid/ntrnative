// Which game runs (docs/launcher.md, "More than one game"). The exe holds Black and the games the build had: White (when
// its assets were there: port/meson.build's `white`), Black 2 and White 2 (pokebw2's C, the maintainers' notes). Each game's C
// carries its own PortGame table (port/game/game_table.c; port/include/port_game.h): bwgame, w_bwgame, b2_bwgame,
// w2_bwgame. White's is w_bwgame either way it is linked: renamed by tools/port_rename.py's header on the PC, the one
// global of its relocatable object on macOS and iOS. PORT_SetGame makes one of them PORT_Game, which the host reads for
// what it calls in the game (machine.c's NitroMain, acapture.c's sound heap, bench_pin.c's MT, ovl.c's overlay numbers).
// Plain C, no platform calls: every target builds it.
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"
#include "port_game.h"
#include "launcher_core.h"

const PortGame *PORT_Game = &bwgame;   // a flat folder (BWPORT_DIR) is Black's until header.bin says otherwise

static const PortGame *const sBuilt[] = {
    &bwgame,
#ifdef BWPORT_WHITE
    &w_bwgame,
#endif
#ifdef BWPORT_B2
    &b2_bwgame,
#endif
#ifdef BWPORT_W2
    &w2_bwgame,
#endif
};

// The game of this code in the exe; NULL: not built (a NULL code is Black's)
const PortGame *PORT_GameByCode(const char *code) {
    size_t i;

    if (code == NULL) {
        return &bwgame;
    }
    for (i = 0; i < sizeof(sBuilt) / sizeof(sBuilt[0]); i++) {
        if (strcmp(sBuilt[i]->code, code) == 0) {
            return sBuilt[i];
        }
    }
    return NULL;
}

// The game the boot starts (PORT_EnterGame, the flat layout's header.bin): its table and its overlays' (ovl.c)
void PORT_SetGame(const char *code) {
    const PortGame *g = PORT_GameByCode(code);

    if (g == NULL) {
        fprintf(stderr, "bwport: this build has no game %s (port/meson.build: its ROM was not there): Black runs\n", code);
        g = &bwgame;
    }
    if (g != PORT_Game) {
        const LC_Game *lc = LC_GameByCode(g->code);
        fprintf(stderr, "bwport: game: %s\n", lc != NULL ? lc->title : g->code);
    }
    PORT_Game = g;
    PortOvl_SetGame(g);
}
