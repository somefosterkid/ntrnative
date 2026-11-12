/* port/acore: constants, register offsets, command ids and the DS's data layouts (docs/acore.md). OWNED BY THE
 * SKELETON: frozen while the pieces run (ask the main session for a change).
 *
 * The layouts are the protocol between the ARM9 and the ARM7 (and the files' formats): they are written out here
 * from the SDK's interface and GBATEK 63-68, not taken from SDK code. Everything is little-endian, DS addresses are
 * u32. No C bitfields: the flags bytes have their bit masks below.
 */
#ifndef AC_TYPES_H
#define AC_TYPES_H
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

/* ---- clocks and time (GBATEK 57, 58; the SDK's SND_PROC_INTERVAL) ---- */
#define AC_ARM7_HZ 33513982u          /* ARM7 cycles per second: every time in acore is in these cycles */
#define AC_CYCLES_PER_SAMPLE 1024u    /* the mixer's output rate: 32728.5 Hz */
#define AC_TICKS_PER_SAMPLE 512u      /* the channel timers count at ARM7/2 = 16.76 MHz: 512 counts per sample */
#define AC_CYCLES_PER_FRAME 560190u   /* 263 lines x 2130 cycles: a capture's frame f is at f * this */
#define AC_OS_TICK_CYCLES 64u         /* an OS tick (the SDK's OSTick) */
#define AC_DRIVER_INTERVAL 0xAA8u     /* the driver's periodic alarm in OS ticks */
#define AC_DRIVER_PERIOD (AC_DRIVER_INTERVAL * AC_OS_TICK_CYCLES) /* 174592 cycles = 170.5 samples = 5.2095 ms */
#define AC_LAG_PERIODIC 0u            /* extra lag of the driver thread after the alarm (INFO lag_p); the wake's own latency is AC_T_PRO */
#define AC_LAG_WORD 10600u            /* the first write of a wake that a WAKE word starts: this long after the word (oracle) */
/* the driver thread's timing inside a wake, in cycles (measured on the oracle's register traces: each
 * number is the median of the oracle's gaps between two writes, see docs/acore.md "Write timing") */
#define AC_T_PRO_WORD 1595u           /* a WAKE word's pass: the PXI interrupt wakes the thread (no alarm jitter) */
#define AC_T_PRO 1707u                /* grid alarm -> the first sync-loop iteration: the thread wakes ~1694 after the alarm interrupt, which itself
                                         fires 13 late when the ARM7 is idle (the oracle's DEV mode; other values scatter -22..+40) */
#define AC_WAV_RATE 32728u            /* the rate a WAV header says */

/* ---- memory ---- */
#define AC_MAIN_BASE 0x02000000u
#define AC_MAIN_SIZE 0x00400000u /* 4 MB; addresses wrap (0x02400000-0x02FFFFFF mirror it) */
/* the address plan of docs/acore.md (the capture and the synthetic streams keep to it) */
#define AC_PLAN_CMD_BASE 0x02040000u   /* command lists (the oracle writes CMDS lists here) */
#define AC_PLAN_CMD_END 0x02100000u
#define AC_PLAN_HEAP_BASE 0x02100000u  /* the port's sound heap, mapped linearly */
#define AC_PLAN_WAVE_BASE 0x02230000u  /* wave-out samples (a ring) */
#define AC_PLAN_WAVE_END 0x0237D000u
#define AC_PLAN_DRVINFO 0x0237D000u    /* READ_DRIVER_INFO's buffer */
#define AC_PLAN_SHARED 0x0237F000u     /* SNDSharedWork */
#define AC_PLAN_ARM7_BASE 0x02380000u  /* the ROM's ARM7 binary (the oracle's): never write here */

/* ---- sound registers (ARM7 I/O, GBATEK 57-59): offsets from 0x04000000 ---- */
#define AC_IO_BASE 0x04000000u
#define AC_REG_CH(n) (0x400u + 0x10u * (u32)(n)) /* channel n's block */
#define AC_CH_CNT 0x0u                           /* SOUNDxCNT (32 bits) */
#define AC_CH_SAD 0x4u                           /* SOUNDxSAD (32) */
#define AC_CH_TMR 0x8u                           /* SOUNDxTMR (16) */
#define AC_CH_PNT 0xAu                           /* SOUNDxPNT (16) */
#define AC_CH_LEN 0xCu                           /* SOUNDxLEN (32) */
#define AC_REG_SOUNDCNT 0x500u                   /* SOUNDCNT (16) */
#define AC_REG_SOUNDBIAS 0x504u                  /* SOUNDBIAS (16) */
#define AC_REG_SNDCAP0CNT 0x508u                 /* SNDCAP0CNT (8) */
#define AC_REG_SNDCAP1CNT 0x509u                 /* SNDCAP1CNT (8) */
#define AC_REG_SNDCAP0DAD 0x510u                 /* (32) */
#define AC_REG_SNDCAP0LEN 0x514u                 /* (16) */
#define AC_REG_SNDCAP1DAD 0x518u
#define AC_REG_SNDCAP1LEN 0x51Cu
#define AC_IO_FIRST 0x400u
#define AC_IO_END 0x520u /* the register file covers 0x04000400-0x0400051F */

/* SOUNDxCNT fields */
#define AC_CNT_VOL(c) ((c) & 0x7Fu)
#define AC_CNT_DIV(c) (((c) >> 8) & 3u) /* 0 /1, 1 /2, 2 /4, 3 /16 */
#define AC_CNT_HOLD 0x8000u
#define AC_CNT_PAN(c) (((c) >> 16) & 0x7Fu)
#define AC_CNT_DUTY(c) (((c) >> 24) & 7u)
#define AC_CNT_REPEAT(c) (((c) >> 27) & 3u) /* 0 manual, 1 loop, 2 one-shot */
#define AC_CNT_FORMAT(c) (((c) >> 29) & 3u) /* 0 PCM8, 1 PCM16, 2 ADPCM, 3 PSG/noise */
#define AC_CNT_START 0x80000000u
enum { AC_FMT_PCM8, AC_FMT_PCM16, AC_FMT_ADPCM, AC_FMT_PSG };
enum { AC_REPEAT_MANUAL, AC_REPEAT_LOOP, AC_REPEAT_ONESHOT };

/* ---- the command ids (the SDK's SNDCommandID, 0-33) and their argument packing ---- */
enum {
  AC_CMD_START_SEQ,             /* a0 player, a1 seq base (DS addr), a2 offset, a3 bank (DS addr) */
  AC_CMD_STOP_SEQ,              /* a0 player */
  AC_CMD_PREPARE_SEQ,           /* as START_SEQ */
  AC_CMD_START_PREPARED_SEQ,    /* a0 player */
  AC_CMD_PAUSE_SEQ,             /* a0 player, a1 flag */
  AC_CMD_SKIP_SEQ,              /* a0 player, a1 ticks */
  AC_CMD_PLAYER_PARAM,          /* a0 player, a1 offset in AC_Player, a2 data, a3 size (1, 2, 4) */
  AC_CMD_TRACK_PARAM,           /* a0 player | size << 24, a1 track mask, a2 offset in AC_Track, a3 data */
  AC_CMD_MUTE_TRACK,            /* a0 player, a1 track mask, a2 mute (AC_MUTE_*) */
  AC_CMD_ALLOCATABLE_CHANNEL,   /* a0 player, a1 track mask, a2 channel mask */
  AC_CMD_PLAYER_LOCAL_VAR,      /* a0 player, a1 var no, a2 value */
  AC_CMD_PLAYER_GLOBAL_VAR,     /* a0 var no, a1 value */
  AC_CMD_START_TIMER,           /* a0 channel mask, a1 capture mask, a2 alarm mask, a3 flags */
  AC_CMD_STOP_TIMER,            /* a0 channel mask, a1 capture mask, a2 alarm mask, a3 flags (hold) */
  AC_CMD_SETUP_CHANNEL_PCM,     /* a0 ch | timer << 16, a1 data (DS addr), a2 loopLen | shift << 22 | volume << 24,
                                   a3 loopStart | pan << 16 | format << 24 | loop << 26 */
  AC_CMD_SETUP_CHANNEL_PSG,     /* a0 ch, a1 duty, a2 volume | shift << 8 (see piece 15), a3 timer | pan << 16 */
  AC_CMD_SETUP_CHANNEL_NOISE,   /* a0 ch, a1 volume | shift << 8, a2 timer | pan << 16 (see piece 15) */
  AC_CMD_SETUP_CAPTURE,         /* the capture unit's setup (piece 15 documents the packing) */
  AC_CMD_SETUP_ALARM,           /* a0 alarm no, a1 tick, a2 period, a3 id */
  AC_CMD_CHANNEL_TIMER,         /* a0 channel mask, a1 timer */
  AC_CMD_CHANNEL_VOLUME,        /* a0 channel mask, a1 volume, a2 shift */
  AC_CMD_CHANNEL_PAN,           /* a0 channel mask, a1 pan */
  AC_CMD_SURROUND_DECAY,        /* a0 decay */
  AC_CMD_MASTER_VOLUME,         /* a0 volume */
  AC_CMD_MASTER_PAN,            /* a0 pan, -1 = reset */
  AC_CMD_OUTPUT_SELECTOR,       /* a0 left, a1 right, a2 channel 1 out, a3 channel 3 out */
  AC_CMD_LOCK_CHANNEL,          /* a0 channel mask, a1 flags */
  AC_CMD_UNLOCK_CHANNEL,        /* a0 channel mask, a1 flags */
  AC_CMD_STOP_UNLOCKED_CHANNEL, /* a0 channel mask, a1 flags */
  AC_CMD_SHARED_WORK,           /* a0 SNDSharedWork (DS addr) */
  AC_CMD_INVALIDATE_SEQ,        /* a0 start, a1 end */
  AC_CMD_INVALIDATE_BANK,       /* a0 start, a1 end */
  AC_CMD_INVALIDATE_WAVE,       /* a0 start, a1 end */
  AC_CMD_READ_DRIVER_INFO,      /* a0 SNDDriverInfo buffer (DS addr) */
  AC_CMD_COUNT
};
extern const char *const ac_cmd_names[AC_CMD_COUNT]; /* ac_state.c */
enum { AC_MUTE_OFF, AC_MUTE_NO_STOP, AC_MUTE_RELEASE, AC_MUTE_STOP };

/* ---- counts ---- */
#define AC_NCH 16
#define AC_NPLAYER 16
#define AC_NTRACK 32
#define AC_TRACKS_PER_PLAYER 16
#define AC_NALARM 8
#define AC_NVAR 16
#define AC_CALL_DEPTH 3
#define AC_NO_TRACK 0xFF

/* ---- the ARM7's work in the DS layout (READ_DRIVER_INFO copies it; PLAYER_PARAM / TRACK_PARAM write into it by
 * offset). Pointers are u32 DS addresses. ---- */
typedef struct AC_LfoParam {
  u8 target; /* 0 pitch, 1 volume, 2 pan */
  u8 speed;
  u8 depth;
  u8 range;
  u16 delay;
} AC_LfoParam; /* 6 */

typedef struct AC_Lfo {
  AC_LfoParam param;
  u16 delay_counter;
  u16 counter;
} AC_Lfo; /* 10 */

typedef struct AC_WaveParam { /* a SWAV's header (GBATEK 68) */
  u8 format;
  u8 loopflag;
  u16 rate;
  u16 timer;
  u16 loopstart; /* words */
  u32 looplen;   /* words */
} AC_WaveParam; /* 12 */

/* AC_ExChannel.flags */
#define AC_EXF_ACTIVE 0x01
#define AC_EXF_START 0x02
#define AC_EXF_AUTO_SWEEP 0x04
#define AC_EXF_SYNC_SHIFT 3 /* bits 3-7: the sync flags (AC_SYNC_*) */
#define AC_SYNC_START 0x01
#define AC_SYNC_STOP 0x02
#define AC_SYNC_TIMER 0x04
#define AC_SYNC_VOLUME 0x08
#define AC_SYNC_PAN 0x10
enum { AC_EX_PCM, AC_EX_PSG, AC_EX_NOISE }; /* AC_ExChannel.type */
enum { AC_ENV_ATTACK, AC_ENV_DECAY, AC_ENV_SUSTAIN, AC_ENV_RELEASE };

typedef struct AC_ExChannel {
  u8 myNo;          /* 0x00 */
  u8 type;          /* 0x01 AC_EX_* */
  u8 env_status;    /* 0x02 AC_ENV_* */
  u8 flags;         /* 0x03 AC_EXF_* */
  u8 pan_range;     /* 0x04 */
  u8 original_key;  /* 0x05 */
  s16 user_decay2;  /* 0x06 */
  u8 key;           /* 0x08 */
  u8 velocity;      /* 0x09 */
  s8 init_pan;      /* 0x0A */
  s8 user_pan;      /* 0x0B */
  s16 user_decay;   /* 0x0C */
  s16 user_pitch;   /* 0x0E */
  s32 env_decay;    /* 0x10 */
  s32 sweep_counter;/* 0x14 */
  s32 sweep_length; /* 0x18 */
  u8 attack;        /* 0x1C */
  u8 sustain;       /* 0x1D */
  u16 decay;        /* 0x1E */
  u16 release;      /* 0x20 */
  u8 prio;          /* 0x22 */
  u8 pan;           /* 0x23 */
  u16 volume;       /* 0x24 */
  u16 timer;        /* 0x26 */
  AC_Lfo lfo;       /* 0x28 */
  s16 sweep_pitch;  /* 0x32 */
  s32 length;       /* 0x34 */
  AC_WaveParam wave;/* 0x38 */
  u32 data;         /* 0x44 the wave's samples (DS addr), or the PSG duty */
  u32 callback;     /* 0x48 (ARM7 code address on the DS; acore: an index, see piece 11) */
  u32 callback_data;/* 0x4C (the track, piece 11) */
  u32 nextLink;     /* 0x50 the next channel of the track (DS layout: an AC_ExChannel's DS address) */
} AC_ExChannel;     /* 0x54 */

/* AC_Player.flags */
#define AC_PLF_ACTIVE 0x01
#define AC_PLF_PREPARED 0x02
#define AC_PLF_PAUSE 0x04
typedef struct AC_Player {
  u8 flags;        /* 0x00 */
  u8 myNo;         /* 0x01 */
  u8 pad2_, pad3_; /* 0x02 */
  u8 prio;         /* 0x04 */
  u8 volume;       /* 0x05 */
  s16 extFader;    /* 0x06 */
  u8 tracks[AC_TRACKS_PER_PLAYER]; /* 0x08 indices into the track pool, AC_NO_TRACK = none */
  u16 tempo;       /* 0x18 */
  u16 tempo_ratio; /* 0x1A */
  u16 tempo_counter; /* 0x1C */
  u16 pad__;       /* 0x1E */
  u32 bank;        /* 0x20 the bank (DS addr) */
} AC_Player;       /* 0x24 */

/* AC_Track.flags */
#define AC_TRF_ACTIVE 0x01
#define AC_TRF_NOTE_WAIT 0x02
#define AC_TRF_MUTE 0x04
#define AC_TRF_TIE 0x08
#define AC_TRF_NOTE_FINISH_WAIT 0x10
#define AC_TRF_PORTA 0x20
#define AC_TRF_CMP 0x40
#define AC_TRF_CHANNEL_MASK 0x80
typedef struct AC_Track {
  u8 flags;        /* 0x00 */
  u8 pan_range;    /* 0x01 */
  u16 prgNo;       /* 0x02 */
  u8 volume;       /* 0x04 */
  u8 volume2;      /* 0x05 expression */
  s8 pitch_bend;   /* 0x06 */
  u8 bend_range;   /* 0x07 */
  s8 pan;          /* 0x08 */
  s8 ext_pan;      /* 0x09 */
  s16 extFader;    /* 0x0A */
  s16 ext_pitch;   /* 0x0C */
  u8 attack;       /* 0x0E */
  u8 decay;        /* 0x0F */
  u8 sustain;      /* 0x10 */
  u8 release;      /* 0x11 */
  u8 prio;         /* 0x12 */
  s8 transpose;    /* 0x13 */
  u8 porta_key;    /* 0x14 */
  u8 porta_time;   /* 0x15 */
  s16 sweep_pitch; /* 0x16 */
  AC_LfoParam mod; /* 0x18 */
  u16 channel_mask;/* 0x1E */
  s32 wait;        /* 0x20 */
  u32 base;        /* 0x24 the sequence data's base (DS addr) */
  u32 cur;         /* 0x28 the next command (DS addr) */
  u32 call_stack[AC_CALL_DEPTH]; /* 0x2C */
  u8 loop_count[AC_CALL_DEPTH];  /* 0x38 */
  u8 call_stack_depth;           /* 0x3B */
  u32 channel_list; /* 0x3C the first channel (DS layout: its AC_ExChannel's address in the work image) */
} AC_Track;         /* 0x40 */

/* the alarms as the DS keeps them (READ_DRIVER_INFO copies them too): enable, id, the setting; the OSAlarm part is
 * opaque here (zeros) */
typedef struct AC_Alarm {
  u8 enable;
  u8 id;
  u16 padding;
  u32 tick_lo, tick_hi;     /* 0x04 OSTick (u64, 4-aligned on the DS) */
  u32 period_lo, period_hi; /* 0x0C */
  u8 os_alarm[0x2C];        /* 0x14 the OSAlarm: its size is to be checked against the oracle (piece 15) */
} AC_Alarm;                 /* 0x40 */

typedef struct AC_Work { /* the SDK's SNDWork */
  AC_ExChannel channel[AC_NCH];  /* 0x000 */
  AC_Player player[AC_NPLAYER];  /* 0x540 */
  AC_Track track[AC_NTRACK];     /* 0x780 */
  AC_Alarm alarm[AC_NALARM];     /* 0xF80 */
} AC_Work;                       /* 0x1180 */

/* SNDSharedWork in main memory (the ARM9 reads it): byte offsets */
#define AC_SW_FINISH_TAG 0x00     /* u32 */
#define AC_SW_PLAYER_STATUS 0x04  /* u32, bit n = player n active */
#define AC_SW_CHANNEL_STATUS 0x08 /* u16, bit n = channel n busy */
#define AC_SW_CAPTURE_STATUS 0x0A /* u16 */
#define AC_SW_PLAYER(n) (0x20u + 0x24u * (u32)(n)) /* s16 variable[16], u32 tickCounter */
#define AC_SW_PLAYER_VAR(n, i) (AC_SW_PLAYER(n) + 2u * (u32)(i))
#define AC_SW_PLAYER_TICKS(n) (AC_SW_PLAYER(n) + 0x20u)
#define AC_SW_GLOBAL_VAR(i) (0x260u + 2u * (u32)(i))
#define AC_SW_SIZE 0x280u

/* SNDDriverInfo (READ_DRIVER_INFO's buffer): the work, the channels' SOUNDxCNT, the work's ARM7 address, the locked
 * channels */
#define AC_DI_WORK 0x0u
#define AC_DI_CHCTRL (sizeof(AC_Work))
#define AC_DI_WORKADDR (AC_DI_CHCTRL + 4u * AC_NCH)
#define AC_DI_LOCKED (AC_DI_WORKADDR + 4u)
#define AC_DI_SIZE (AC_DI_LOCKED + 4u + 24u)

/* ---- file formats (GBATEK 63-68): the offsets the driver reads ---- */
#define AC_SSEQ_DATA_OFFSET 0x18 /* u32 in an SSEQ: the sequence data's offset in the file (0x1C) */
#define AC_SBNK_INST_COUNT 0x38  /* u32 */
#define AC_SBNK_INST_RECORDS 0x3C /* u32 per instrument: type (bits 0-7) | offset << 8 */
#define AC_SBNK_WAVEARC_LINK(i) (0x18u + 8u * (u32)(i)) /* {u32 wave archive (DS addr), u32 next} */
#define AC_SWAR_WAVE_COUNT 0x38  /* u32 */
#define AC_SWAR_WAVE_OFFSETS 0x3C /* u32 per wave: an offset in the file, or an absolute address >= 0x02000000 */
enum { AC_INST_INVALID, AC_INST_PCM, AC_INST_PSG, AC_INST_NOISE, AC_INST_DIRECTPCM, AC_INST_NULL,
       AC_INST_DRUM_SET = 0x10, AC_INST_KEY_SPLIT = 0x11 };

#endif
