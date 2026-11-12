// Black 2's and White 2's master-brightness fades (port/extern/lib/gfl/src/fade.c's twin; the maintainers' notes, 5.):
// pokebw2's src/gfl/fade.c compiled through this file. The wrapper lets the game set the fade up, then multiplies its
// step: 2x or 4x fewer steps, or Instant (one step to the end).
#define GFL_FadeSet GFL_FadeSet_game
#include "../../../../third_party/pokebw2/src/gfl/fade.c"
#undef GFL_FadeSet

extern int SIM_UISettingGet(const char *key);

void GFL_FadeSet(u32 mode, s32 brightnessStart, s32 brightnessEnd, s32 slowness) {
    // fadespeed 0 = the game's, 1 = 2x, 2 = 4x, 3 = Instant (a step of 32 covers any brightness range, -16..16)
    int speed = SIM_UISettingGet("fadespeed");

    GFL_FadeSet_game(mode, brightnessStart, brightnessEnd, slowness);
    if (speed >= 1 && speed <= 3) {
        s32 k = speed == 1 ? 2 : speed == 2 ? 4 : 32;

        sFadeMgr->step *= k;
        if (speed == 3) {
            sFadeMgr->wait = 0;
            sFadeMgr->waitFrames = 0;
        }
    }
}
