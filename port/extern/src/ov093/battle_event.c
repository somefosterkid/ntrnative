// The PC build compiles src/ov093/battle_event.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, docs/port-worker.md "Test cheat"): with BWPORT_TESTCHEAT=win the battles still play in full, but
// the player's side (mon IDs 0-11: clients 0 and 2) always moves first, its attacks do 3000 damage (they one-shot), use no
// PP, and the attacks on it do none. It rewrites event results after the game's own handlers ran: CALC_SPEED's ratio,
// CALC_DAMAGE's damage, CHECK_ALWAYS_HIT's flag and the PP amount (0x4e). Off by default: then the game's own function runs and nothing else. The DS build never sees this file.
#pragma push_macro("BattleEvent_CallHandlers")
#define BattleEvent_CallHandlers BattleEvent_CallHandlers_game
#include "../../../../src/ov093/battle_event.c"
#pragma pop_macro("BattleEvent_CallHandlers")

#include <stdio.h>
#include "port_route.h"

void BattleEvent_CallHandlers(BtlServerFlow *flow, u32 event) {
    BattleEvent_CallHandlers_game(flow, event);
    if ((event == 0x47 || event == 0x13 || event == 0x32 || event == 0x4e) && PortRoute_CheatWin(PORT_ROUTE_GAME)) {
        if (event == 0x4e) {   // the PP a move uses (func_ov093_021c07d4): attacker var 3, amount var 0x20: the player's
            // moves use none, so a long leg never runs its first move dry (the pilot only presses A: a move with no PP
            // left looped on "no PP" until the hang check)
            if (BattleEventVar_GetValue(3) < 12) {
                BattleEventVar_SetValue(0x20, 0);
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
