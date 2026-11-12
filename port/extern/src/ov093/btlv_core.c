// (Black / White) The PC build compiles src/ov093/btlv_core.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT in a battle): it tells the route pilot which yes/no question is up (gPortRouteYesNo: set
// by func_ov093_021eabc4, BtlvCore_ShowYesNo in pokebw2; cleared when func_ov093_021eac54, BtlvCore_PollYesNo, has the answer),
// so the pilot answers the forget-a-move question and the "Will you switch Pokémon?" question of a trainer's next Pokémon
// with B (No: keep the old moves, keep the Pokémon in; the same as port/extern_b2w2/src/ov167/btlv_core.c) and the rest with A.
// It changes nothing else. The DS build never sees this file.
#pragma push_macro("func_ov093_021eabc4")
#pragma push_macro("func_ov093_021eac54")
#define func_ov093_021eabc4 func_ov093_021eabc4_game
#define func_ov093_021eac54 func_ov093_021eac54_game
#include "../../../../src/ov093/btlv_core.c"
#pragma pop_macro("func_ov093_021eabc4")
#pragma pop_macro("func_ov093_021eac54")

extern int gPortRouteYesNo;   // port/src/route_test.c: 0 none, 1 answer with A, 2 answer with B

void func_ov093_021eabc4(BtlvCore *core, const BtlvStringParam *param1, const BtlvStringParam *param2, u32 arg3) {
    // btl_client.c: the level-up move's forget-a-move question (Btlv_StringParam_Setup(&p, 6, 2)) and the shift style's
    // switch question (type 3, message 6, then the party list); arg3 is canCancel, so B = No works on both
    int b = arg3 != 0 && ((param1->type == 6 && param1->message == 2) || (param1->type == 3 && param1->message == 6));

    gPortRouteYesNo = b ? 2 : 1;
    func_ov093_021eabc4_game(core, param1, param2, arg3);
}

BOOL func_ov093_021eac54(BtlvCore *core, u32 *answer) {
    BOOL done = func_ov093_021eac54_game(core, answer);

    if (done) {
        gPortRouteYesNo = 0;
    }
    return done;
}
