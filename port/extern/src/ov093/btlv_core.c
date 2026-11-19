// (Black / White) The PC build compiles src/ov093/btlv_core.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT in a battle): it tells the route pilot which yes/no question is up (gPortRouteYesNo: set
// by func_ov093_021eabc4, BtlvCore_ShowYesNo in pokebw2; cleared when func_ov093_021eac54, BtlvCore_PollYesNo, has the answer),
// so the pilot answers the forget-a-move question and the "Will you switch Pokémon?" question of a trainer's next Pokémon
// with B (No: keep the old moves, keep the Pokémon in; the same as port/extern_b2w2/src/ov167/btlv_core.c) and the rest with A.
// It also stamps the action menu's poll (func_ov093_021e97a4: 1 Fight, 2 Bag, 3 Pokémon, 4 Run when it has an answer) and the Bag's
// update (func_ov093_021e9e80, subSeq 3: the overlay runs) with the V-blank in gPortRouteBattle, for the pilot's C:ball
// (a wild catch: Bag, Poké Balls, the first ball). The action menu's and the move menu's (func_ov093_021e98b4) answers go to
// port/extern/src/ov093/btl_client.c's PortBattle_Switch and PortBattle_PickMove (the pilot's effective move). It changes nothing
// else. The DS build never sees this file.
#pragma push_macro("func_ov093_021eabc4")
#pragma push_macro("func_ov093_021eac54")
#pragma push_macro("func_ov093_021e97a4")
#pragma push_macro("func_ov093_021e9e80")
#pragma push_macro("func_ov093_021e98b4")
#define func_ov093_021e98b4 func_ov093_021e98b4_game
#define func_ov093_021e97a4 func_ov093_021e97a4_game
#define func_ov093_021e9e80 func_ov093_021e9e80_game
#define func_ov093_021eabc4 func_ov093_021eabc4_game
#define func_ov093_021eac54 func_ov093_021eac54_game
#include "../../../../src/ov093/btlv_core.c"
#pragma pop_macro("func_ov093_021eabc4")
#pragma pop_macro("func_ov093_021eac54")
#pragma pop_macro("func_ov093_021e97a4")
#pragma pop_macro("func_ov093_021e9e80")
#pragma pop_macro("func_ov093_021e98b4")

#include <stdio.h>
#include <stdlib.h>
#include "port_route.h"

extern int gPortRouteYesNo;   // port/src/route_test.c: 0 none, 1 answer with A, 2 answer with B
// port/extern/src/ov093/btl_client.c: the pilot's move and switch (BWPORT_PILOT)
void PortBattle_PickMove(BtlClient *client, BattleMon *mon, BattleAction *action);
BOOL PortBattle_Switch(BtlClient *client, BattleMon *mon);

void func_ov093_021eabc4(BtlvCore *core, const BtlvStringParam *param1, const BtlvStringParam *param2, u32 arg3) {
    // btl_client.c: the level-up move's forget-a-move question (Btlv_StringParam_Setup(&p, 6, 2)) and the shift style's
    // switch question (type 3, message 6, then the party list); arg3 is canCancel, so B = No works on both
    int b = arg3 != 0 && ((param1->type == 6 && param1->message == 2) || (param1->type == 3 && param1->message == 6));

    gPortRouteYesNo = b ? 2 : 1;
    if (getenv("BWPORT_PILOT") != NULL) {
        fprintf(stderr, "pilot: battle yes/no: type %d message %d mode %d canCancel %u (V-blank %u)\n", param1->type, param1->message,
                param1->mode, (unsigned)arg3, (unsigned)OS_GetVBlankCount());
    }
    func_ov093_021eabc4_game(core, param1, param2, arg3);
}

BOOL func_ov093_021eac54(BtlvCore *core, u32 *answer) {
    BOOL done = func_ov093_021eac54_game(core, answer);

    if (done) {
        gPortRouteYesNo = 0;
    }
    return done;
}

u32 func_ov093_021e97a4(BtlvCore *core) {
    u32 r = func_ov093_021e97a4_game(core);

    gPortRouteBattle.menuVblank = OS_GetVBlankCount();
    if (r == 1 && PortBattle_Switch(core->client, core->mon)) {   // Fight, but no move hurts the target: the switch is set
        return 0;
    }
    return r;
}

BOOL func_ov093_021e98b4(BtlvCore *core) {
    BOOL done = func_ov093_021e98b4_game(core);

    if (done) {   // the move menu's answer, core->unkB8 the client's action
        PortBattle_PickMove(core->client, core->mon, core->unkB8);
    }
    return done;
}

BOOL func_ov093_021e9e80(BtlvCore *core) {
    BOOL done = func_ov093_021e9e80_game(core);

    if (core->subSeq == 3) {   // the Bag overlay (ov197) runs on core->ov286
        gPortRouteBattle.bagVblank = OS_GetVBlankCount();
        gPortRouteBattle.bagItem = core->ov286.item;
        gPortRouteBattle.bagPocket = core->ov286.pocket;
        gPortRouteBattle.bagDone = core->ov286.done;
        gPortRouteBattle.bagBallError = core->ov286.ballError;
        gPortRouteBattle.ballRow = core->ov286.rows[2];
        gPortRouteBattle.ballPage = core->ov286.pages[2];
    }
    return done;
}
