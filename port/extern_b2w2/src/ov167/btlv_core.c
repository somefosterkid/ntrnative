// (Black 2 / White 2) The PC build compiles pokebw2 src/ov167/btlv_core.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT in a battle): it tells the route pilot which yes/no question is up (gPortRouteYesNo: set
// by BtlvCore_ShowYesNo, cleared when BtlvCore_PollYesNo has the answer), so the pilot answers the forget-a-move and
// the switch-Pokémon questions with B (No: keep the old moves, keep the Pokémon in) and the rest with A. It changes nothing else.
// The DS build never sees this file.
#pragma push_macro("BtlvCore_ShowYesNo")
#pragma push_macro("BtlvCore_PollYesNo")
#pragma push_macro("BtlvCore_WaitCommandSelect")
#pragma push_macro("BtlvCore_WaitItemSelect")
#pragma push_macro("BtlvCore_WaitMoveSelect")
#define BtlvCore_WaitMoveSelect BtlvCore_WaitMoveSelect_game
#define BtlvCore_ShowYesNo BtlvCore_ShowYesNo_game
#define BtlvCore_PollYesNo BtlvCore_PollYesNo_game
#define BtlvCore_WaitCommandSelect BtlvCore_WaitCommandSelect_game
#define BtlvCore_WaitItemSelect BtlvCore_WaitItemSelect_game
#include "../../../../third_party/pokebw2/src/ov167/btlv_core.c"
#pragma pop_macro("BtlvCore_ShowYesNo")
#pragma pop_macro("BtlvCore_PollYesNo")
#pragma pop_macro("BtlvCore_WaitCommandSelect")
#pragma pop_macro("BtlvCore_WaitItemSelect")
#pragma pop_macro("BtlvCore_WaitMoveSelect")
// It also stamps the action menu's poll (BtlvCore_WaitCommandSelect) and the Bag's update (BtlvCore_WaitItemSelect, subSeq 3: the
// overlay runs) with the V-blank in gPortRouteBattle, for the pilot's C:ball (a wild catch: Bag, Poké Balls, the first ball).
// The action menu's and the move menu's (BtlvCore_WaitMoveSelect) answers go to port/extern_b2w2/src/ov167/btl_client.c's
// PortBattle_Switch and PortBattle_PickMove (the pilot's effective move).
#include "port_route.h"

void PortBattle_PickMove(BtlClient *client, BattleMon *mon, BattleAction *action);
BOOL PortBattle_Switch(BtlClient *client, BattleMon *mon);

u32 BtlvCore_WaitCommandSelect(BtlvCore *core) {
    u32 r = BtlvCore_WaitCommandSelect_game(core);

    gPortRouteBattle.menuVblank = OS_GetVBlankCount();
    if (r == BTLV_COMMAND_FIGHT && PortBattle_Switch(core->client, core->mon)) {   // no move hurts the target: the switch is set
        return 0;
    }
    return r;
}

BOOL BtlvCore_WaitMoveSelect(BtlvCore *core) {
    BOOL done = BtlvCore_WaitMoveSelect_game(core);

    if (done) {
        PortBattle_PickMove(core->client, core->mon, core->action);
    }
    return done;
}

BOOL BtlvCore_WaitItemSelect(BtlvCore *core) {
    BOOL done = BtlvCore_WaitItemSelect_game(core);

    if (core->subSeq == 3) {   // the Bag overlay (ov286) runs on core->ov286
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

extern int gPortRouteYesNo;   // port/src/route_test.c: 0 none, 1 answer with A, 2 answer with B

void BtlvCore_ShowYesNo(BtlvCore *core, const BtlvStringParam *yes, const BtlvStringParam *no, u32 canCancel) {
    // btl_client.c: the level-up move (the forget-a-move question: BTLV_STR_YES_NO 2) and the trainer's next Pokémon in the shift style
    // (the switch question: BTLV_STR_UI 6); both can be cancelled (B = No)
    int b = canCancel && ((yes->type == BTLV_STR_YES_NO && yes->message == 2) || (yes->type == BTLV_STR_UI && yes->message == 6));

    gPortRouteYesNo = b ? 2 : 1;
    BtlvCore_ShowYesNo_game(core, yes, no, canCancel);
}

BOOL BtlvCore_PollYesNo(BtlvCore *core, u32 *answer) {
    BOOL done = BtlvCore_PollYesNo_game(core, answer);

    if (done) {
        gPortRouteYesNo = 0;
    }
    return done;
}
