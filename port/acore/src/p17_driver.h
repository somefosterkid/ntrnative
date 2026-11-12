/* port/acore piece 17: The driver loop and integration (docs/acore.md). OWNED BY PIECE 17.
 * p17_driver.c/.h, tests/t17_driver.c (and ac_state.c, tools/acore_render.c from now on). `ac_driver_wake` (the
 * order above), `ac_driver_init` (SND_Init's state: channels off, master volume 0x7F, the output selector 0), the
 * periodic alarm's phase (`tick0`: from the oracle's trace of the stream), the register trace, and the whole-core
 * score: every corpus clip through acore and the oracle, register traces and PCM, triaged to the pieces. Tests: the
 * wake order (a command and a tick in one wake), tick0, the end of a stream, an end-to-end note.
 */
#ifndef AC_P17_H
#define AC_P17_H

/* the piece's own state, s->p17 (per channel / player / track arrays as it needs) */
typedef struct AC_P17 {
  int unused;
} AC_P17;

#endif
