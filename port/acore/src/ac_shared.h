/* port/acore: the state every piece shares, memory and register access, and every cross-piece function with its
 * owner (docs/acore.md). OWNED BY THE SKELETON: frozen while the pieces run.
 *
 * A piece's own state is its AC_PNN struct (in its header), reached as s->pNN. The stubs of the pieces compile, do
 * nothing and return 0.
 */
#ifndef AC_SHARED_H
#define AC_SHARED_H
#include "ac_types.h"
#include <stdio.h>

#include "p01_pcm.h"
#include "p02_adpcm.h"
#include "p03_psg_noise.h"
#include "p04_mixer.h"
#include "p05_capture.h"
#include "p06_tables.h"
#include "p07_seq_flow.h"
#include "p08_seq_param.h"
#include "p09_player.h"
#include "p10_bank.h"
#include "p11_alloc.h"
#include "p12_envelope.h"
#include "p13_lfo_sweep.h"
#include "p14_exchannel.h"
#include "p15_commands.h"
#include "p16_shared.h"
#include "p17_driver.h"

/* one stream event (ac_state.c's loader) */
enum { AC_EV_MEM, AC_EV_CMDS, AC_EV_WAKE, AC_EV_REGS, AC_EV_END };
typedef struct AC_Event {
  u64 time;
  int type;
  u32 frame;      /* CMDS */
  u32 addr;       /* MEM */
  u32 n;          /* CMDS: commands, REGS: writes, MEM: bytes */
  const u8 *data; /* into the loaded file: MEM bytes, CMDS {id, a0-a3} x n, REGS {addr, value, size} x n */
} AC_Event;

/* a command list waiting for the driver (CMDS queued; the driver runs them at the next wake) */
#define AC_MAX_QUEUE 64
#define AC_MAX_LIST 256
typedef struct AC_List {
  u32 n;
  u32 cmd[AC_MAX_LIST][5]; /* id, a0, a1, a2, a3 */
} AC_List;

typedef struct AC_Driver { /* the skeleton's part of the driver's state; the pieces' parts are in s->pNN */
  int on;                   /* 0 for mode=hw streams: no periodic wakes */
  u64 tick0;                /* the first periodic wake (cycles) */
  u64 next_tick;            /* the next periodic wake */
  u32 nqueue;
  AC_List queue[AC_MAX_QUEUE];
  u32 shared;               /* SNDSharedWork's DS address (SHARED_WORK), 0 = none yet */
  u32 work_addr;            /* the DS address READ_DRIVER_INFO reports for the work (INFO work_addr=) */
  u32 wakes;                /* wakes so far (periodic and not) */
  u64 lag_periodic;         /* the driver thread runs this many cycles after the alarm (INFO lag_p) */
  u64 lag_word;             /* ... and after a WAKE word (INFO lag_w) */
  u64 pend[16];             /* WAKE words waiting to run (arrival times, ascending) */
  u8 pendk[16];             /* 0 = a word's first pass, 1 = the pass that follows it */
  u32 npend;
  u64 free_at;              /* the thread is busy until here: the end of the last pass */
  /* write timing inside a wake (ac_state.c): with `timed` the driver's register writes are queued at
   * wake_t + cur instead of landing at the wake's instant; the pieces advance `cur` by the cycles the real driver
   * spends between writes (ac_drv_wait) */
  int timed;                /* set when a stream is loaded (the unit tests keep writes immediate unless they ask) */
  int in_wake;
  int autogap;              /* commands: consecutive writes to one channel's registers are spaced like the sync loop's */
  u32 last_addr;
  int pass_word;            /* the running pass was started by a WAKE word */
  u64 wake_t;               /* the wake's start (the thread's wake-up) */
  u64 cur;                  /* cycles since wake_t */
  struct AC_PendWrite { u64 due; u32 addr, value; int size; } pq[2048];
  u32 npq;
} AC_Driver;

/* The music bus (docs/acore.md "Game speed and sound"). At game Speed S > 1 the port runs the sound side S times as fast
 * as the device plays. With the bus split on, the sequences the game started on its music players (BGM, fanfares) run on
 * a clock of their own: their sequencer, envelopes, LFOs and sweeps are stepped by the music wake (a periodic alarm on
 * the music clock), their hardware channels (music_ch) are mixed by the music samples, and the caller asks for as many
 * music samples as the real time needs (acore_bus_run). Everything else (the commands, the other players, the shared
 * work) stays on the main clock (acore_run). With the split off (the default) nothing of this changes the core. */
typedef struct AC_Bus {
  int split;               /* 1: the music players have their own clock and samples */
  int dom;                 /* what the running wake / sample covers: 0 everything, 1 all but the music bus, 2 the music bus */
  int solo;                /* measuring: 0 every channel is heard, 1 only the music's, 2 only the others */
  u16 music_players;       /* players (driver slots) whose running sequence is music */
  u16 music_ch;            /* hardware channels playing a music player's note */
  u8 fifo[AC_NPLAYER][8];  /* the flag of the next starts of each slot (ac_bus_slot) */
  u8 nfifo[AC_NPLAYER];
  u8 last[AC_NPLAYER];
  u64 mnsamples;           /* music samples made (sample k at k * 1024 + sample_phase on the music clock) */
  u64 mnext_tick;          /* the music clock's next periodic wake */
} AC_Bus;

/* The sound archive's stream (ac_strm.c; the port's NNS_SndArcStrm*, port/src/strm_arc.c): decoded samples played on the
 * host side as the DS's two (or one) stream channels would, added to the mixer's sum. pcm NULL: none (Black, White) */
typedef struct AC_Strm {
  const s16 *pcm;    /* nsamp frames of nch interleaved samples (the caller's buffer, kept while set) */
  u32 nsamp, nch, rate, loop, loop_start;
  u32 timer;         /* the channel's timer period in 16.76 MHz counts (16756991 / rate) */
  u32 count;         /* counts run since the current sample started (512 a mixer sample) */
  u32 pos;           /* the current sample */
  int playing;
  int vol;           /* the archive's volume, 0-127 */
  int fade, fade_len;/* the fade's frames left and its length (0: none) */
  u32 fade_count;    /* cycles into the current game frame of the fade */
  u16 cnt;           /* volume | divider << 8, from vol and the fade */
} AC_Strm;

typedef struct AcoreState {
  /* memory and registers */
  u8 *mem;                  /* main memory, AC_MAIN_SIZE bytes (0x02000000) */
  u8 io[AC_IO_END];         /* the sound registers as written (0x04000400-0x0400051F at io[0x400..]) */
  /* time */
  u64 now;                  /* cycles */
  u64 sample_phase;         /* sample k is taken at k * 1024 + sample_phase */
  u64 nsamples;             /* samples rendered */
  u64 pcm_off;              /* ... of which the first pcm_off were discarded from s->pcm (acore_pcm_discard) */
  /* the stream */
  u8 *file;                 /* the loaded .acs */
  size_t file_size;
  AC_Event *ev;
  u32 nev, next_ev;
  u64 end_time;             /* END, or the last event */
  char info[4096];          /* INFO text */
  /* the output */
  s16 *pcm;                 /* interleaved L R */
  size_t pcm_cap;
  FILE *trace;              /* the register trace (acore_set_trace), NULL = off */
  /* the driver */
  AC_Work work;             /* the ARM7's work in the DS layout */
  AC_Driver drv;
  AC_Bus bus;
  AC_Strm strm;
  int hq;                   /* 1: Enhanced output (acore_set_quality): interpolated channel samples, the mix at 16 bits; 0 = the DS's exact output */
  int hq_fade;              /* mixer samples left of the cross-fade from the other quality's channel output (a switch mid-note) */
  /* the pieces' own state */
  AC_P01 p01; AC_P02 p02; AC_P03 p03; AC_P04 p04; AC_P05 p05; AC_P06 p06; AC_P07 p07; AC_P08 p08; AC_P09 p09;
  AC_P10 p10; AC_P11 p11; AC_P12 p12; AC_P13 p13; AC_P14 p14; AC_P15 p15; AC_P16 p16; AC_P17 p17;
} AcoreState;

/* ---- memory (main memory only: other addresses read 0, writes are dropped) ---- */
static inline int ac_in_main(u32 a) { return (a >> 24) == 0x02; }
static inline u8 ac_rd8(const AcoreState *s, u32 a) { return ac_in_main(a) ? s->mem[a & (AC_MAIN_SIZE - 1)] : 0; }
static inline u16 ac_rd16(const AcoreState *s, u32 a) {
  return (u16)(ac_rd8(s, a & ~1u) | ac_rd8(s, (a & ~1u) + 1) << 8);
}
static inline u32 ac_rd32(const AcoreState *s, u32 a) {
  a &= ~3u;
  return (u32)ac_rd8(s, a) | (u32)ac_rd8(s, a + 1) << 8 | (u32)ac_rd8(s, a + 2) << 16 | (u32)ac_rd8(s, a + 3) << 24;
}
static inline void ac_wr8(AcoreState *s, u32 a, u8 v) {
  if (ac_in_main(a)) s->mem[a & (AC_MAIN_SIZE - 1)] = v;
}
static inline void ac_wr16(AcoreState *s, u32 a, u16 v) {
  a &= ~1u;
  ac_wr8(s, a, (u8)v);
  ac_wr8(s, a + 1, (u8)(v >> 8));
}
static inline void ac_wr32(AcoreState *s, u32 a, u32 v) {
  a &= ~3u;
  ac_wr16(s, a, (u16)v);
  ac_wr16(s, a + 2, (u16)(v >> 16));
}

/* ---- the music bus (ac_state.c; p17 for the wake): is channel ch / player p part of what the running wake covers ---- */
static inline int ac_dom_ch(const AcoreState *s, int ch) {
  return s->bus.dom == 0 || ((s->bus.music_ch >> ch & 1) == (s->bus.dom == 2));
}
static inline int ac_dom_pl(const AcoreState *s, int p) {
  return s->bus.dom == 0 || ((s->bus.music_players >> p & 1) == (s->bus.dom == 2));
}
void ac_bus_slot(AcoreState *s, int slot, int music); /* the next start on `slot` is music (or not) */
void ac_bus_player_started(AcoreState *s, int p);     /* p9: a player (re)starts: its bus is the oldest flag of its slot */
void ac_music_wake(AcoreState *s);                    /* p17: one wake of the music clock */

/* ---- the sound registers (ac_state.c): ofs = 0x400-0x51F or the full 0x04000400 address; size 1, 2, 4.
 * A write stores the bytes, traces them (when on), and calls ac_ch_start / ac_ch_stop (1) on a SOUNDxCNT start-bit
 * edge (0 -> 1 start, 1 -> 0 stop) and ac_cap_start / ac_cap_stop (5) on a SNDCAPxCNT bit 7 edge. A piece clearing
 * the busy bit itself (a one-shot's end) uses ac_hw_set_busy, which neither traces nor calls back. ---- */
void ac_hw_write(AcoreState *s, u32 ofs, u32 value, int size);
u32 ac_hw_read(const AcoreState *s, u32 ofs, int size);
void ac_hw_set_busy(AcoreState *s, int ch, int busy);
/* the driver spends `cycles` before its next register write (only meaningful inside a wake) */
static inline void ac_drv_wait(AcoreState *s, u32 cycles) { s->drv.cur += cycles; }
/* a channel's register: outside a wake (the mixer's ~40 reads a sample) straight from the file, as ac_hw_read would */
static inline u32 ac_ch_reg32(const AcoreState *s, int ch, u32 r) {
  if (s->drv.in_wake) return ac_hw_read(s, AC_REG_CH(ch) + r, 4);
  const u8 *p = &s->io[AC_REG_CH(ch) + r];
  return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24;
}
static inline u16 ac_ch_reg16(const AcoreState *s, int ch, u32 r) {
  if (s->drv.in_wake) return (u16)ac_hw_read(s, AC_REG_CH(ch) + r, 2);
  const u8 *p = &s->io[AC_REG_CH(ch) + r];
  return (u16)(p[0] | p[1] << 8);
}
/* one output sample (ac_state.c): 5's premix, each channel's output (1), the mix (4), the capture (5), the channels
 * stepped (1); appended to s->pcm */
void ac_hw_sample(AcoreState *s);

/* the work's DS addresses (the DS layout's pointers inside the work: nextLink, channel_list) */
#define AC_EXCH_ADDR(s, ch) ((s)->drv.work_addr + (u32)offsetof(AC_Work, channel) + (u32)(ch) * (u32)sizeof(AC_ExChannel))
#define AC_EXCH_NO(s, a) ((a) ? (int)(((a) - (s)->drv.work_addr - (u32)offsetof(AC_Work, channel)) / sizeof(AC_ExChannel)) : -1)

/* ======== the pieces' functions (bodies in the maintainer's file; stubs do nothing / return 0) ======== */

/* 1 PCM channels (p01_pcm.c) */
void ac_ch_start(AcoreState *s, int ch);  /* SOUNDxCNT's start bit set: latch SAD/TMR/PNT/LEN, the start delay */
void ac_ch_stop(AcoreState *s, int ch);   /* the start bit cleared */
s32 ac_ch_output(AcoreState *s, int ch);  /* the channel's current sample (-0x8000..0x7FFF, before volume) */
void ac_ch_step(AcoreState *s, int ch);   /* advance one output sample (512 timer counts) */
void ac_ch_run(AcoreState *s, int ch, s32 *out, int k); /* k samples in a row: out[i] = ac_ch_output, then ac_ch_step */
/* 2 IMA-ADPCM (p02_adpcm.c), called by 1 */
void ac_adpcm_start(AcoreState *s, int ch);          /* read the header word at SAD */
s32 ac_adpcm_next(AcoreState *s, int ch, u32 nib);   /* decode nibble nib (0 = the first after the header), in order */
void ac_adpcm_loop_save(AcoreState *s, int ch);      /* at the loop start */
void ac_adpcm_loop_restore(AcoreState *s, int ch);   /* at the loop end */
s32 ac_adpcm_step(s32 *pred, u32 *idx, u32 d);       /* one code on a (sample, index) pair; also the stream's (ac_strm.c) */
/* 3 PSG and noise (p03_psg_noise.c), called by 1 */
void ac_psg_start(AcoreState *s, int ch);
s32 ac_psg_next(AcoreState *s, int ch);              /* the next duty step / noise bit: +-0x7FFF */
/* 4 the mixer (p04_mixer.c): mix = the mixer's left/right sums (for capture), out = the final s16 pair */
void ac_mix(AcoreState *s, const s32 chout[AC_NCH], s32 mix[2], s16 out[2]);
#define AC_BLOCK 128 /* ac_state.c's block of samples between events */
/* the stream (ac_strm.c): one mixer sample of it added to sum (16.8, as the channels'), and its time advanced */
void ac_strm_sample(AcoreState *s, s32 sum[2]);
void ac_mix_block(AcoreState *s, const s32 (*buf)[AC_BLOCK], u32 live, int n, s16 *pcm);
/* 5 capture (p05_capture.c) */
void ac_cap_start(AcoreState *s, int n);
void ac_cap_stop(AcoreState *s, int n);
void ac_cap_premix(AcoreState *s, s32 chout[AC_NCH]);           /* the add mode's effect on channels 1 / 3 */
void ac_cap_sample(AcoreState *s, const s32 mix[2], const s32 chout[AC_NCH]);

/* 6 tables (p06_tables.c) */
s16 ac_decibel_sq(int x);               /* a note's velocity 0-127 -> dB*10 (the square law) */
s16 ac_decibel(int x);                  /* 0-127 -> the driver's dB*10 value (a negative number; 0 at 127) */
u16 ac_volume_table(int i);             /* BIOS GetVolumeTable, 0-723 */
u16 ac_pitch_table(int i);              /* BIOS GetPitchTable, 0-767 */
s8 ac_sine(int i);                      /* the LFO's sine, i in 0-127 (a full period) */
u16 ac_calc_timer(int timer, int pitch);/* pitch in 1/64 semitones (+ up): the new timer value */
u16 ac_calc_volume(int db);             /* dB*10 -> volume | shift << 8 for SOUNDxCNT */
u16 ac_random(AcoreState *s);           /* the driver's random generator (advanced every wake) */
/* 7 sequence flow (p07_seq_flow.c) */
int ac_track_tick(AcoreState *s, int p, int t);  /* one tick of track t of player p; 1 when the track ended */
/* 8 track parameters (p08_seq_param.c) */
void ac_track_init(AcoreState *s, int t);        /* a new track's defaults */
void ac_track_param(AcoreState *s, int p, int t, int cmd, s32 value);
void ac_track_update_channels(AcoreState *s, int p, int t, int release);
/* 9 players (p09_player.c) */
void ac_seq_main(AcoreState *s, int periodic);
void ac_player_start(AcoreState *s, int p, u32 base, u32 offset, u32 bank);
void ac_player_prepare(AcoreState *s, int p, u32 base, u32 offset, u32 bank);
void ac_player_start_prepared(AcoreState *s, int p);
void ac_player_stop(AcoreState *s, int p);
void ac_player_pause(AcoreState *s, int p, int flag);
void ac_player_skip(AcoreState *s, int p, u32 ticks);
void ac_player_param(AcoreState *s, int p, u32 ofs, u32 data, int size);
void ac_track_param_raw(AcoreState *s, int p, u32 mask, u32 ofs, u32 data, int size);
void ac_track_mute(AcoreState *s, int p, u32 mask, int mute);
void ac_track_allocatable(AcoreState *s, int p, u32 mask, u32 chmask);
int ac_player_open_track(AcoreState *s, int p, int no, u32 addr); /* SSEQ 0x93 */
/* 10 banks and note-on (p10_bank.c) */
int ac_note_on(AcoreState *s, int p, int t, int key, int velocity, s32 length); /* 1 when a channel started */
/* 11 channel allocation (p11_alloc.c) */
int ac_exch_alloc(AcoreState *s, u32 chmask, int prio, int strong, int t);      /* a channel or -1 */
void ac_exch_start(AcoreState *s, int ch, s32 length);
void ac_exch_release(AcoreState *s, int ch);
void ac_exch_free(AcoreState *s, int ch);
void ac_exch_lock(AcoreState *s, u32 chmask, u32 flags);
void ac_exch_unlock(AcoreState *s, u32 chmask, u32 flags);
u32 ac_exch_locked(const AcoreState *s);
void ac_exch_stop_unlocked(AcoreState *s, u32 chmask, u32 flags);
void ac_exch_sync(AcoreState *s);                                     /* UpdateExChannel */
void ac_track_release_channels(AcoreState *s, int p, int t, int release);
void ac_track_free_channels(AcoreState *s, int t);
/* 12 envelopes (p12_envelope.c) */
void ac_env_init(AcoreState *s, int ch);
void ac_env_set_attack(AcoreState *s, int ch, int v);
void ac_env_set_decay(AcoreState *s, int ch, int v);
void ac_env_set_sustain(AcoreState *s, int ch, int v);
void ac_env_set_release(AcoreState *s, int ch, int v);
void ac_env_update(AcoreState *s, int ch);
s32 ac_env_level(const AcoreState *s, int ch);
/* 13 LFO, sweep, portamento (p13_lfo_sweep.c) */
void ac_lfo_init(AC_LfoParam *p);
void ac_lfo_start(AC_Lfo *l);
void ac_lfo_update(AcoreState *s, AC_Lfo *l, int periodic);
s32 ac_lfo_value(AcoreState *s, const AC_Lfo *l);
s32 ac_sweep_value(AcoreState *s, int ch);
void ac_sweep_update(AcoreState *s, int ch, int periodic);
/* 14 channel update (p14_exchannel.c) */
void ac_exch_main(AcoreState *s, int periodic);
void ac_exch_length_tick(AcoreState *s, int ch);   /* once per channel of a track on every sequencer tick (p07) */
int ac_exch_pan_value(const AC_ExChannel *c, int lfo_pan);
void ac_porta_setup(AcoreState *s, int ch, AC_Track *t, int key, s32 note_length); /* 13, at note-on (p10) */
void ac_p16_tick(AcoreState *s, int p);
void ac_p16_tick_reset(AcoreState *s, int p);
void ac_var_reset_player(AcoreState *s, int p);
/* 15 commands (p15_commands.c) */
void ac_cmd_proc(AcoreState *s);
void ac_cmd_exec(AcoreState *s, const u32 cmd[5]);
/* 16 shared work and variables (p16_shared.c); p < 0: a global variable */
void ac_shared_update(AcoreState *s);
s16 ac_var_get(AcoreState *s, int p, int n);
void ac_var_set(AcoreState *s, int p, int n, s16 v);
/* 17 the driver (p17_driver.c) */
void ac_driver_init(AcoreState *s);
void ac_driver_wake(AcoreState *s, int periodic);
void ac_drv_cmd_time(AcoreState *s, u32 id); /* p17: the driver's time to reach a command (before its writes) */

#endif
