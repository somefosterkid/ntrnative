// The PC build compiles src/system/gfl_use.c through this file (tools/port_sources.py). The repeatable runs
// (port/src/bench_pin.c, docs/port-worker.md "Routes"): PortGame.pinLC reseeds the linear congruential generator this
// file keeps in a static, whichever game is running (Black, White, and Black 2's twin in port/extern_b2w2). The DS build
// never sees this file.
#include "../../../../src/system/gfl_use.c"

// the value docs/lockstep.md's anchor uses
void PortGame_PinLC(void) {
    if (data_020aa1a4 != NULL) {
        data_020aa1a4->rand.x = 0x0123456789ABCDEFULL;
    }
}
