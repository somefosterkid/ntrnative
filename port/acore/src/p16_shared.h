/* port/acore piece 16: Shared work and variables (docs/acore.md). OWNED BY PIECE 16.
 * p16_shared.c/.h, tests/t16_shared.c. `ac_shared_update(s)` (UpdateSharedWork: player status bits, channel status
 * bits (the busy bits), capture status, each player's tick counter, at the shared work's DS address), `ac_var_get /
 * ac_var_set(s, p, n)` (local variables per player, global ones: they live in the shared work, as on the DS; before
 * SHARED_WORK a fallback store).
 *
 * Seen on the oracle (probe streams, 10-29), what the other pieces must do for the shared work to match:
 *  - a player's tick counter is NOT in AC_Player: the player piece (9) calls ac_p16_tick(s, p) once per sequencer
 *    tick it runs and ac_p16_tick_reset(s, p) when the player starts (the count restarts at 0 on every START_SEQ and
 *    stays put after STOP_SEQ).
 *  - START_SEQ / PREPARE_SEQ set the player's 16 local variables back to -1 (a PLAYER_LOCAL_VAR sent before the start
 *    is lost, one sent after it in the same list is kept): piece 9 calls ac_var_reset_player(s, p) at the start.
 */
#ifndef AC_P16_H
#define AC_P16_H

#define AC_P16_DEFAULT_VAR (-1) /* SND_DEFAULT_VARIABLE */

typedef struct AC_P16 {
  short local[16][16]; /* the fallback store (before SHARED_WORK) */
  short global[16];
  unsigned char init;  /* the stores hold -1 */
  unsigned tick[16];   /* the players' tick counters (written to the shared work by ac_shared_update) */
} AC_P16;

struct AcoreState;
void ac_p16_tick(struct AcoreState *s, int p);        /* one sequencer tick of player p */
void ac_p16_tick_reset(struct AcoreState *s, int p);  /* player p (re)started */
void ac_var_reset_player(struct AcoreState *s, int p);/* player p's locals back to -1 */

#endif
