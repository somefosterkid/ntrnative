/* port/acore piece 5: Capture (docs/acore.md). OWNED BY PIECE 5.
 * p05_capture.c/.h, tests/t05_capture.c. ac_cap_start(s, n), ac_cap_stop(s, n), ac_cap_sample(s, mix, chout):
 * SNDCAPxCNT (add mode, source: mixer or channel 0/2, one-shot/loop, PCM8/16, start), DAD, LEN (words), the
 * capture timer (it runs on channel 1 / 3 timer), the clipping and the PCM8 rounding (GBATEK 61 "capture
 * clipping"), the writes into s->mem, the add mode (channel 1/3 plus the capture input); the documented capture
 * bugs (GBATEK 59). Not used by the game (no NNS_SndCapture): synthetic streams only. GBATEK 59, 60, 61.
 *
 * Interface assumptions (piece 4 / piece 1): the mix array of ac_cap_sample is the mixer left/right sums in
 * GBATEK 61 20.8 fixed point (the 16.8 values of the channels added: PCM16 units << 8); chout[ch] is the channel
 * sample before volume (-0x8000..0x7FFF), 0 for a channel that is off. ac_cap_premix does nothing (see p05_capture.c).
 */
#ifndef AC_P05_H
#define AC_P05_H

/* one capture unit */
typedef struct AC_P05_Unit {
  int running;        /* started (SNDCAPxCNT bit 7 edge) and not finished */
  int ticks;          /* timer ticks (16.76 MHz clock) until the next capture sample */
  unsigned pos;       /* words written in this pass of the buffer */
  unsigned word;      /* the word being assembled */
  int nbytes;         /* bytes of it filled */
  unsigned pend[4];   /* completed words not written yet (the hardware writes 4-word blocks) */
  int npend;
} AC_P05_Unit;

/* the piece own state, s->p05 */
typedef struct AC_P05 {
  AC_P05_Unit u[2];
} AC_P05;

#endif
