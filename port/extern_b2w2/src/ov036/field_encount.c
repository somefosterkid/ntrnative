// Black 2's and White 2's twin of port/extern/src/ov021/field_encount.c: pokebw2's src/ov036/field_encount.c compiled
// through this file. Test only: BWPORT_NOENCOUNTER=1 stops the wild encounters of steps and effect spots (see there).
#define EventWildBattleCall_CreateRandom EventWildBattleCall_CreateRandom_game
#include "../../../../third_party/pokebw2/src/ov036/field_encount.c"
#undef EventWildBattleCall_CreateRandom

#include <stdio.h>

#include "port_route.h"

GameEvent *EventWildBattleCall_CreateRandom(EncountSystem *system, u32 mode) {
    if ((mode == ENCOUNT_MODE_NORMAL || mode == ENCOUNT_MODE_EFFECT) && PortRoute_NoEncounter(PORT_ROUTE_GAME)) {
        return NULL;
    }
    GameEvent *event = EventWildBattleCall_CreateRandom_game(system, mode);

    if (event != NULL) {   // the route tests count them (tools/route.py)
        fprintf(stderr, "bwport: wild encounter (mode %u, V-blank %u)\n", (unsigned)mode, (unsigned)OS_GetVBlankCount());
    }
    return event;
}
