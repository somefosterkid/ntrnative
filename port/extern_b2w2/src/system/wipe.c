// Black 2's and White 2's screen wipes (port/extern/src/system/wipe.c's twin; the maintainers' notes, 5.): pokebw2's
// src/system/wipe.c compiled through this file. Every screen wipe starts in GFL_WipeSet, which runs it over `division`
// steps of `sync` frames; the wrapper passes 1/2 or 1/4 of the divisions (at least 1), or 1 for Instant.
#define GFL_WipeSet GFL_WipeSet_game
#include "../../../../third_party/pokebw2/src/system/wipe.c"
#undef GFL_WipeSet

extern int SIM_UISettingGet(const char *key);

void GFL_WipeSet(int mode, int typeMain, int typeSub, u16 color, s32 division, s32 sync, u32 heapId) {
    // fadespeed 0 = the game's, 1 = 2x, 2 = 4x, 3 = Instant
    int speed = SIM_UISettingGet("fadespeed");

    if (speed >= 1 && speed <= 3 && division > 1) {
        division = speed == 3 ? 1 : division / (speed == 1 ? 2 : 4);
        if (division < 1) {
            division = 1;
        }
    }
    GFL_WipeSet_game(mode, typeMain, typeSub, color, division, sync, heapId);
}
