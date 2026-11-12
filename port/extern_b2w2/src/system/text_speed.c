// Black 2's and White 2's text speed option (port/extern/src/system/text_speed.c's twin; the maintainers' notes, 2. and
// "Black 2 and White 2"): pokebw2's src/system/text_speed.c compiled through this file (tools/port_sources.py --b2w2).
// Every wait a text printer is given comes from these four functions, so wrapping them at the file's boundary speeds all
// of the game's text. The file's own calls go to the game's versions (the renames below); every other file's calls reach
// the wrappers, which return the faster of the game's wait and the setting's. A negative wait is that many characters a
// frame.
#define TextSpeed_GetCurrentWait TextSpeed_GetCurrentWait_game
#define TextSpeed_GetFasterWait TextSpeed_GetFasterWait_game
#define TextSpeed_GetSlowerWait TextSpeed_GetSlowerWait_game
#define TextSpeed_GetWait TextSpeed_GetWait_game
#include "../../../../third_party/pokebw2/src/system/text_speed.c"
#undef TextSpeed_GetCurrentWait
#undef TextSpeed_GetFasterWait
#undef TextSpeed_GetSlowerWait
#undef TextSpeed_GetWait

extern int SIM_UISettingGet(const char *key);

// Text speed 0 = the game's own (the player's setting in Options), 1 = Faster (4 characters a frame), 2 = Fastest (8),
// 3 = Instant (127: a whole box a frame; it fits the s8 some callers keep the wait in)
static s32 PortTextWait(s32 wait) {
    static const s8 waits[4] = { 0, -4, -8, -127 };
    int i = SIM_UISettingGet("textspeed");

    if (i < 1 || i > 3) {
        return wait;
    }
    return waits[i] < wait ? waits[i] : wait;
}

s32 TextSpeed_GetCurrentWait(void) {
    return PortTextWait(TextSpeed_GetCurrentWait_game());
}

s32 TextSpeed_GetFasterWait(void) {
    return PortTextWait(TextSpeed_GetFasterWait_game());
}

s32 TextSpeed_GetSlowerWait(void) {
    return PortTextWait(TextSpeed_GetSlowerWait_game());
}

s32 TextSpeed_GetWait(u32 speed) {
    return PortTextWait(TextSpeed_GetWait_game(speed));
}
