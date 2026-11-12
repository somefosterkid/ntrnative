// The PC build compiles src/system/text_speed.c through this file (tools/port_sources.py). The port's Text speed
// setting (the maintainers' notes, 2.): every wait a text printer is given comes from these four functions (103 callers
// of the Options speed, the battle's messages through func_02013080, the printer's own speed commands), so wrapping
// them at the file's boundary speeds all of the game's text. The file's own calls go to the game's versions (the
// renames below); every other file's calls reach the wrappers, which return the faster of the game's wait and the
// setting's. A negative wait is that many characters a frame (src/system/printsys.c's func_0201cdf4). The printer still
// stops at a page break, a stream command or a callback, so a continue prompt still waits for a press.
// The DS build never sees this file.
#pragma push_macro("func_02012ffc")
#define func_02012ffc func_02012ffc_game
#pragma push_macro("func_02013020")
#define func_02013020 func_02013020_game
#pragma push_macro("func_02013050")
#define func_02013050 func_02013050_game
#pragma push_macro("func_02013080")
#define func_02013080 func_02013080_game
#include "../../../../src/system/text_speed.c"
#pragma pop_macro("func_02012ffc")
#pragma pop_macro("func_02013020")
#pragma pop_macro("func_02013050")
#pragma pop_macro("func_02013080")

extern int SIM_UISettingGet(const char *key);

// Text speed 0 = the game's own (the player's slow / mid / fast in Options), 1 = Faster (4 characters a frame, twice the
// game's fast), 2 = Fastest (8), 3 = Instant (127: a whole box a frame; it fits the s8 some callers keep the wait in)
static s32 PortTextWait(s32 wait) {
    static const s8 waits[4] = { 0, -4, -8, -127 };
    int i = SIM_UISettingGet("textspeed");

    if (i < 1 || i > 3) {
        return wait;
    }
    return waits[i] < wait ? waits[i] : wait;
}

s32 func_02012ffc(void) {
    return PortTextWait(func_02012ffc_game());
}

s32 func_02013020(void) {
    return PortTextWait(func_02013020_game());
}

s32 func_02013050(void) {
    return PortTextWait(func_02013050_game());
}

s32 func_02013080(u32 speed) {
    return PortTextWait(func_02013080_game(speed));
}
