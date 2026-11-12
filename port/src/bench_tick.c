// The game's OS_GetTick under the bench pin (BWPORT_BENCH_CLOCK, port/src/bench_pin.c): a pure function of game frames.
// libntr's OS_GetTick is the host's wall clock (SDL_GetTicks64), and the game reads it for logic: the play time in the
// save (src/system/playtime_ctrl.c: whole seconds of wall time, so a leg's save differed between identical runs), the
// text printer's per-frame budget (src/system/printsys.c: 0x1130 ticks, then the rest of the string waits a frame), the
// trainer AI's per-frame budget (src/ov096/tr_ai.c: 500 ticks, then it resumes next frame), cutscene movies' pacing
// (src/ov179/cdemo_control.c) and the battle timer. So pacing (Max, a shot window, frame skip, the machine's load)
// changed what the game did. Pinned, the tick is the V-blank count times a DS frame (560190 cycles / 64) plus one tick a
// call within the frame (a budget loop still ends, after as many calls on every run); the alarms, sound and the IR/Wi-Fi
// code inside libntr keep the real clock. port_runtime.h sends the game's OS_GetTick() calls here.
#define BWPORT_PORT_RUNTIME_IMPL
#include <nitro.h>
#include <stdlib.h>
#include "port_runtime.h"

#define FRAME_CYCLES 560190u   // 263 lines x 2130 cycles: one DS frame at 33.51 MHz
#define FRAME_TICKS (FRAME_CYCLES / 64u)   // OS ticks are 64 cycles

OSTick PORT_GetTick(void) {
    static int sPin = -1;
    static u32 sFrame;
    static u32 sCalls;
    u32 v;

    if (sPin < 0) {
        sPin = getenv("BWPORT_BENCH_CLOCK") != NULL;
    }
    if (!sPin) {
        return (OS_GetTick)();   // the parentheses: libntr's function, not port_runtime.h's macro (it would call us)
    }
    v = OS_GetVBlankCount();
    if (v != sFrame) {
        sFrame = v;
        sCalls = 0;
    } else if (sCalls < FRAME_TICKS - 1) {
        sCalls++;
    }
    return (OSTick)v * FRAME_CYCLES / 64u + sCalls;
}
