// The PC build compiles src/system/wipe.c through this file (tools/port_sources.py). The port's Screen fades setting
// (the maintainers' notes, 5.): every screen wipe (the field menu, each app's open and close, the battle's screens, the
// field's map-change patterns) starts in GFL_WipeSet, which runs the wipe over `division` steps of `sync` frames. The
// wrapper passes 1/2 or 1/4 of the divisions (at least 1), or 1 for Instant; each pattern spreads its whole move over
// the divisions it gets (src/system/wipe_sub.c), so it ends where it would, sooner. The fades are
// port/extern/lib/gfl/src/fade.c. The DS build never sees this file.
#pragma push_macro("GFL_WipeSet")
#define GFL_WipeSet GFL_WipeSet_game
#include "../../../../src/system/wipe.c"
#pragma pop_macro("GFL_WipeSet")

extern int SIM_UISettingGet(const char *key);

void GFL_WipeSet(int mode, int typeMain, int typeSub, u16 color, int division, int sync, u32 heapId) {
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
