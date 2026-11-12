// (Black 2 / White 2) The PC build compiles pokebw2 src/ov167/btlv_core.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT in a battle): it tells the route pilot which yes/no question is up (gPortRouteYesNo: set
// by BtlvCore_ShowYesNo, cleared when BtlvCore_PollYesNo has the answer), so the pilot answers the forget-a-move and
// the switch-Pokémon questions with B (No: keep the old moves, keep the Pokémon in) and the rest with A. It changes nothing else.
// The DS build never sees this file.
#pragma push_macro("BtlvCore_ShowYesNo")
#pragma push_macro("BtlvCore_PollYesNo")
#define BtlvCore_ShowYesNo BtlvCore_ShowYesNo_game
#define BtlvCore_PollYesNo BtlvCore_PollYesNo_game
#include "../../../../third_party/pokebw2/src/ov167/btlv_core.c"
#pragma pop_macro("BtlvCore_ShowYesNo")
#pragma pop_macro("BtlvCore_PollYesNo")

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
