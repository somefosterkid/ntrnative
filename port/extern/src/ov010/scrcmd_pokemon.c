// The PC build compiles src/ov010/scrcmd_pokemon.c through this file (tools/port_sources.py). Test only
// (docs/port-runtime.md "Test cheat"): with BWPORT_TESTCHEAT=win the party "knows" the field moves (HM01-HM06 and the other
// moves a field script asks for: PortRoute_CheatFieldMove): the script command 0x116 PokePartyHasMoveAny, which gates Cut,
// Surf, Strength, Waterfall and Dive, finds them in party slot 0 (its var gets 0), so the scripts go on to their "use it?"
// question and the move's animation with that Pokemon. 0x115 (a given slot) is left alone. Off by default: then the game's own
// function runs and nothing else. The DS build never sees this file.
#pragma push_macro("s0116_PokePartyHasMoveAny")
#define s0116_PokePartyHasMoveAny s0116_PokePartyHasMoveAny_game
#include "../../../../src/ov010/scrcmd_pokemon.c"
#pragma pop_macro("s0116_PokePartyHasMoveAny")

#include "port_route.h"

// With the cheat on: the game's function with the field moves found in the first slot (it reads the same arguments)
BOOL s0116_PokePartyHasMoveAny(VM *vm, FieldScriptEnv *env) {
    PokeParty *party;
    u16 *result;
    u16 move;
    int i;
    int count;

    if (!PortRoute_CheatWin(PORT_ROUTE_GAME)) {
        return s0116_PokePartyHasMoveAny_game(vm, env);
    }
    party = GameData_GetParty(FieldScriptEnv_GetGameData(env));
    result = ScriptReadVar(vm, env);
    move = ScriptReadAny(vm, env);
    count = PokeParty_GetPkmCount(party);
    *result = 6;
    for (i = 0; i < count; i++) {
        if (PortRoute_CheatFieldMove(PORT_ROUTE_GAME, move) ||
            CheckPokeMoveLearned_NonEgg(PokeParty_GetPkm(party, i), move) == TRUE) {
            *result = i;
            break;
        }
    }
    return FALSE;
}
