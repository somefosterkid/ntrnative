// (Black / White) The PC build compiles src/ov194/namein.c through this file (tools/port_sources.py). Test only
// (port/src/route_test.c, BWPORT_PILOT's Pk token): it tells the route pilot that the naming screen is up
// (gPortRouteNameIn: 1 from the proc's init, func_ov194_021f1440, to its exit, func_ov194_021f168c). The proc table
// (data_ov194_021f5580) sits in the file with the functions, so the file's own references would skip a wrapper: the table
// is renamed too and defined again here over the wrappers. It changes nothing else. The DS build never sees this file.
#pragma push_macro("func_ov194_021f1440")
#pragma push_macro("func_ov194_021f168c")
#pragma push_macro("data_ov194_021f5580")
#define func_ov194_021f1440 func_ov194_021f1440_game
#define func_ov194_021f168c func_ov194_021f168c_game
#define data_ov194_021f5580 data_ov194_021f5580_game
#include "../../../../src/ov194/namein.c"
#pragma pop_macro("func_ov194_021f1440")
#pragma pop_macro("func_ov194_021f168c")
#pragma pop_macro("data_ov194_021f5580")

extern int gPortRouteNameIn;   // port/src/route_test.c: 1 while the naming screen's proc runs

BOOL func_ov194_021f1440(GameProc *proc, u32 *state, void *arg, void *unused) {
    BOOL r = func_ov194_021f1440_game(proc, state, arg, unused);

    gPortRouteNameIn = 1;
    return r;
}

BOOL func_ov194_021f168c(GameProc *proc, u32 *state, void *param, void *arg) {
    BOOL r = func_ov194_021f168c_game(proc, state, param, arg);

    gPortRouteNameIn = 0;
    return r;
}

const GameProcFunctions data_ov194_021f5580 = { func_ov194_021f1440, func_ov194_021f1744, func_ov194_021f168c };
