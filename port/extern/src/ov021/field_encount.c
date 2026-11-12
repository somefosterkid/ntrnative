// The PC build compiles src/ov021/field_encount.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c): with BWPORT_NOENCOUNTER=1 the wild encounter of a step in grass, a cave or water, and of an
// effect spot (rustling grass, a dust cloud, rippling water), never starts, as with a Repel; Sweet Scent still does.
// Trainers, scripts and static encounters don't come through here. Off by default: the game's own function runs. The DS
// build never sees this file.
#pragma push_macro("EventWildBattleCall_CreateRandom")
#define EventWildBattleCall_CreateRandom EventWildBattleCall_CreateRandom_game
#include "../../../../src/ov021/field_encount.c"
#pragma pop_macro("EventWildBattleCall_CreateRandom")

#include <stdio.h>

#include "port_route.h"

// mode 0 is a step's check, 1 an effect spot's, 2 Sweet Scent (pokebw2's ENCOUNT_MODE_*)
GameEvent *EventWildBattleCall_CreateRandom(EncountSystem *system, u32 mode) {
    if (mode < 2 && PortRoute_NoEncounter(PORT_ROUTE_GAME)) {
        return NULL;
    }
    GameEvent *event = EventWildBattleCall_CreateRandom_game(system, mode);

    if (event != NULL) {   // the route tests count them (tools/route.py)
        fprintf(stderr, "bwport: wild encounter (mode %u, V-blank %u)\n", (unsigned)mode, (unsigned)OS_GetVBlankCount());
    }
    return event;
}
