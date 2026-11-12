// Black 2's and White 2's twin of port/extern/src/system/gfl_use.c: PortGame.pinLC reseeds the linear congruential
// generator pokebw2's src/system/gfl_use.c keeps in its static work (the repeatable runs, port/src/bench_pin.c).
#include "../../../../third_party/pokebw2/src/system/gfl_use.c"

void PortGame_PinLC(void) {
    if (sWork != NULL) {
        sWork->rand.x = 0x0123456789ABCDEFULL;
    }
}
