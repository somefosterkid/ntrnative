// The PC build compiles lib/gfl/src/fade.c through this file (tools/port_sources.py). The port's Screen fades setting
// (the maintainers' notes, 5.): every master-brightness fade starts in GFL_FadeSet (the field's map changes, doors and
// warps through src/ov021/field_transition.c and field_fade.c, and most apps); the wrapper lets the game set the fade
// up, then multiplies its step: 2x or 4x fewer steps, or Instant (one step to the end). The fade's start and end
// brightness and the callers' waits for it (GFL_FadeIsRunning) are unchanged. The screen wipes are
// port/extern/src/system/wipe.c. The DS build never sees this file.
#pragma push_macro("GFL_FadeSet")
#define GFL_FadeSet GFL_FadeSet_game
#include "../../../../../lib/gfl/src/fade.c"
#pragma pop_macro("GFL_FadeSet")

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
