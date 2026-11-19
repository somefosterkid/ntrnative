// (Black 2 / White 2 twin of port/extern/src/ov093/btl_server_flow_sub.c) The PC build compiles pokebw2 src/ov167/btl_server_flow_sub.c
// through this file (tools/port_sources.py). Test only (port/src/route_test.c's C:ball, docs/port-worker.md "Routes"): with
// BWPORT_TESTCHEAT=win a Poke Ball always catches (the capture value the game tests against 255 is PassPower_ApplyCapture's result:
// the cheat returns 255 itself, so the first throw catches: three shakes, no critical). Off by default: the game's own function runs.
// The DS build never sees this file.
#include "save/high_link.h"
#include "port_route.h"

// (a function, not a macro over the name: White's build renames PassPower_ApplyCapture to w_... by a macro of its own)
static s32 PortCapture(s32 rate) {
    return PortRoute_CheatWin(PORT_ROUTE_GAME) ? FX32_CONST(255) : PassPower_ApplyCapture(rate);
}
#undef PassPower_ApplyCapture
#define PassPower_ApplyCapture(rate) PortCapture(rate)
#include "../../../../third_party/pokebw2/src/ov167/btl_server_flow_sub.c"
