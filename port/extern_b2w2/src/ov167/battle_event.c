// (Black 2 / White 2 twin of port/extern/src/ov093/battle_event.c) The PC build compiles pokebw2 src/ov167/battle_event.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, docs/port-worker.md "Test cheat"): with BWPORT_TESTCHEAT=win the battles still play in full, but
// the player's side (mon IDs 0-11: clients 0 and 2) always moves first, its attacks do 3000 damage (they one-shot), never miss,
// use no PP, and the attacks on it do none. It rewrites event results after the game's own handlers ran: CALC_SPEED's ratio,
// CALC_DAMAGE's damage, CHECK_ALWAYS_HIT's flag and CALC_PP_DECREMENT's amount. Off by default: then the game's own function runs
// and nothing else. The DS build never sees this file.
#pragma push_macro("BattleEvent_CallHandlers")
#define BattleEvent_CallHandlers BattleEvent_CallHandlers_game
#include "../../../../third_party/pokebw2/src/ov167/battle_event.c"
#pragma pop_macro("BattleEvent_CallHandlers")

#include <stdio.h>
#include <stdlib.h>
#include "battle/btl_main.h"
#include "port_route.h"

extern BtlPokeCon *gPortSweepPokeCon;   // the server's Pokemon (port/include/port_bw_sweep_battle.h; BtlServerFlow is private to its file)

void BattleEvent_CallHandlers(BtlServerFlow *flow, u32 event) {
    BattleEvent_CallHandlers_game(flow, event);
    if (getenv("BWPORT_BW_TESTLIST") != NULL) {   // the battle sweep (port/include/port_bw_sweep_battle.h): both sides always hit, use no PP,
        if (event == 0x32) {                       // and no hit takes more than an eighth of the defender's HP (at least 1)
            BattleEventVar_SetValue(0x51, 1);
        } else if (event == 0x4e) {
            BattleEventVar_SetValue(0x20, 0);
        } else if (event == 0x47 && gPortSweepPokeCon != NULL) {
            BattleMon *def = GetPokeParam(gPortSweepPokeCon, (u8)BattleEventVar_GetValue(4));
            int cap = def->core.hp / 8 > 0 ? def->core.hp / 8 : 1;

            if ((int)BattleEventVar_GetValue(0x32) > cap) {
                BattleEventVar_SetValue(0x32, (uintptr_t)cap);
            }
        }
        return;
    }
    if ((event == 0x47 || event == 0x13 || event == 0x32 || event == 0x4e || event == 0x80) && PortRoute_CheatWin(PORT_ROUTE_GAME)) {
        if (event == 0x4e) {   // BTL_EVENT_CALC_PP_DECREMENT (btl_server_flow.c): attacker BTL_EVAR_ATTACKER 3, amount BTL_EVAR_AMOUNT
            // 0x20: the player's moves use none, as in Black (the pilot only presses A, and the move menu refuses a move with no PP
            // left: Black 2's frigate battle hung on a lead whose first move had run dry)
            if (BattleEventVar_GetValue(3) < 12) {
                BattleEventVar_SetValue(0x20, 0);
            }
        } else if (event == 0x80) {   // BTL_EVENT_CHECK_SIMPLE_DAMAGE_ENABLED: mon var 4, result var 0x51 (Magic Guard's own rewrite):
            // the player's side takes no damage outside a hit either (burn, poison, weather, Leech Seed, recoil, Life Orb; int5 11-19)
            if (BattleEventVar_GetValue(4) < 12) {
                BattleEventVar_SetValue(0x51, 0);
            }
        } else if (event == 0x32) {   // CHECK_ALWAYS_HIT: attacker var 3, result flag var 0x51: the player's moves never miss
            if (BattleEventVar_GetValue(3) < 12) {
                BattleEventVar_SetValue(0x51, 1);
            }
        } else if (event == 0x13) {   // CALC_SPEED: ratio applied to the speed (var 0x35); mon is var 2
            uintptr_t mon = BattleEventVar_GetValue(2);

            BattleEventVar_SetValue(0x35, mon < 12 ? 0x20000 : 0x19a);
            fprintf(stderr, "cheat: speed mon %u ratio %s\n", (unsigned)mon, mon < 12 ? "32x" : "0.1x");
        } else {               // CALC_DAMAGE: attacker var 3, defender var 4, damage var 0x32
            uintptr_t atk = BattleEventVar_GetValue(3);
            uintptr_t def = BattleEventVar_GetValue(4);

            if (def < 12) {
                BattleEventVar_SetValue(0x32, 0);
            } else if (atk < 12 && !gPortRouteCatch) {   // (C:ball does not attack: a one-shot would end a catch before it)
                BattleEventVar_SetValue(0x32, 3000);
            }
            fprintf(stderr, "cheat: damage atk %u def %u -> %u\n", (unsigned)atk, (unsigned)def,
                    (unsigned)BattleEventVar_GetValue(0x32));
        }
    }
}
