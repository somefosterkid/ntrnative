// Black 2's and White 2's twin of port/extern/src/ov021/fieldmap_event.c: pokebw2's src/ov036/fieldmap_event.c compiled through this file,
// its check of the player's command (a trigger that fires when the player runs onto it) sees the game's command for the port's faster steps (port/extern_b2w2/src/ov036/field_actor_acmd.c), so a
// faster run still counts as running.
#define FldAct_GetAcmd PortAcmd_GetGameAcmd
#pragma push_macro("PokeParty_GetParam")
#define PokeParty_GetParam PortCheat_PokeParty_GetParam
#include "../../../../third_party/pokebw2/src/ov036/fieldmap_event.c"
#pragma pop_macro("PokeParty_GetParam")

#include "port_route.h"

u32 PokeParty_GetParam(const PartyPkm *pkm, PkmField param, void *buffer);

// Test only (docs/port-runtime.md "Test cheat"): with BWPORT_TESTCHEAT=win the field's Surf (the A press at the water) and
// Waterfall (the step into the fall) checks find a Pokemon that knows the move: the party Pokemon read Surf and Waterfall as
// their first two moves. Those checks (GameData_FindPartyPkmByMove) are the only place this file reads a Pokemon's moves.
u32 PortCheat_PokeParty_GetParam(const PartyPkm *pkm, PkmField param, void *buffer) {
    if ((int)param == 0x36 && PortRoute_CheatFieldMove(PORT_ROUTE_GAME, 57)) {   // PKM_PARAM_MOVE1, MOVE_SURF
        return 57;
    }
    if ((int)param == 0x37 && PortRoute_CheatFieldMove(PORT_ROUTE_GAME, 127)) {   // PKM_PARAM_MOVE2, MOVE_WATERFALL
        return 127;
    }
    return PokeParty_GetParam(pkm, param, buffer);
}
