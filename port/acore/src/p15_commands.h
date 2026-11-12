/* port/acore piece 15: Commands (docs/acore.md). OWNED BY PIECE 15.
 * p15_commands.c/.h, tests/t15_commands.c. `ac_cmd_proc(s)` runs each queued list (`s->drv.queue`, in order) and
 * each command of it (ids 0-33 of ac_types.h), then finishCommandTag++ (one per list, as the oracle's shared work
 * shows); `ac_cmd_exec(s, {id, a0..a3})` runs one command.
 *
 * Register writes follow the oracle's trace (the ROM's own ARM7 driver in melonDS), command by command:
 *   SETUP_CHANNEL_PCM  CNT (32), TMR (16, -timer), PNT, LEN, SAD     (the start bit is not set)
 *   SETUP_CHANNEL_PSG / NOISE  CNT (32), TMR (16)
 *   START_TIMER        per channel CNT byte 3 | 0x80; both captures: one 16-bit write of 0x508, one: a byte
 *   STOP_TIMER         per channel CNT (32) = readback with the start bit cleared (| 0x8000 when flags & 1);
 *                      per capture a byte 0
 *   CHANNEL_TIMER/VOLUME/PAN  TMR (16, -timer) / CNT (16, volume | shift << 8) / CNT byte 2
 *   MASTER_PAN         every channel's CNT byte 2 (the master pan, or with a negative value each channel's own pan)
 *   MASTER_VOLUME / OUTPUT_SELECTOR  SOUNDCNT byte 0 / byte 1
 *   SETUP_CAPTURE      SNDCAPxCNT (8), DAD (32), LEN (16)
 * Alarms (SETUP_ALARM, START_TIMER / STOP_TIMER's alarm mask) have no register: they are recorded for the port.
 */
#ifndef AC_P15_H
#define AC_P15_H

#define AC_P15_MAX_ALARM_RUNS 64

/* one start of an alarm (START_TIMER's alarm mask) until its stop */
typedef struct AC_AlarmRun {
  unsigned char no, id;
  unsigned long long start;  /* cycles: the command's time */
  unsigned long long first;  /* the first firing (start + tick * 64) */
  unsigned long long period; /* cycles, 0 = one shot */
  unsigned long long stop;   /* ~0 while running */
} AC_AlarmRun;

typedef struct AC_AlarmFire {
  unsigned long long time;
  int no, id;
} AC_AlarmFire;

/* the piece's own state, s->p15 */
typedef struct AC_P15 {
  unsigned int finish_tag;           /* finishCommandTag */
  int master_on;                     /* 0 = no master pan (a zeroed state); else master_pan (0-255) is every channel pan */
  int master_pan;
  unsigned char chan_pan[16];        /* each channel's own pan (what MASTER_PAN -1 restores) */
  int nalarm_run;
  AC_AlarmRun alarm_run[AC_P15_MAX_ALARM_RUNS]; /* the alarm log: starts and stops in time order */
} AC_P15;

struct AcoreState;
/* the alarm firings up to `until` (cycles, inclusive), in time order; returns how many were stored (<= max) */
int ac_p15_alarm_fires(const struct AcoreState *s, unsigned long long until, AC_AlarmFire *out, int max);

#endif
