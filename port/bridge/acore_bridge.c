/* port/bridge/acore_bridge.c: port/acore behind the game's sound path (docs/acore.md piece 18).
 *
 * The acore side of the live path: one AcoreState, driven from the audio callback at the DS rate (33513982 / 1024 =
 * 32728.5 Hz) and resampled to the device's rate (cubic). port/src/acore_glue.c is the game's side: it translates the
 * ARM9's command lists (acapture.c) and calls these functions with plain integers and pointers, so that this file,
 * which includes acore's headers (their own u8 / s32 ...), never meets the game's types. Built apart from the game's C
 * (port/meson.build), as vcore_bridge.c is.
 *
 * Threads: the game's side (libntr's PXI thread: memory writes, command lists, wakeups) and the audio callback share
 * the state. The caller holds ab_lock() around every other ab_* call; the callback holds it for the whole chunk.
 *
 * Time (frame clock, libntr-soundclock.patch, the default): libntr's SIM_Audio_FrameTick calls ab_run once per game
 * frame with the ARM7 cycle the frame ends at (560190 more each frame); acore renders the DS-rate samples up to there
 * and libntr's ring plays them. Commands and memory that arrive during the frame are applied at the next ab_run.
 * ab_wake counts from acore's `now`, the end of the last ab_run.
 *
 * Time (device clock, BWPORT_SOUNDCLOCK=device): acore has no clock of its own; the audio callback advances it. Commands and memory that arrive between two
 * callbacks are applied at the next chunk's start (acore's `now` is the end of the last chunk), so a command takes
 * effect within one callback (~5 ms) of its arrival, and the driver's own periodic alarm (every 5.2 ms) picks it up.
 *
 * BWPORT_AUDIO_WAV=FILE writes acore's samples at the DS rate (before the resampling) as a 16-bit stereo WAV, from the
 * first frame (libntr's sim_audio.cpp, frame clock) or callback (here, device clock) on: the form tools/acore_compare.py reads. BWPORT_ACORE_REGS=FILE writes the register trace.
 */
#include "acore.h"
#include "ac_shared.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static AcoreState *sCore;
static pthread_mutex_t sLock = PTHREAD_MUTEX_INITIALIZER;
static double sStep;      /* DS samples per output frame */
static double sFrac;      /* position between sHist[1] and sHist[2] */
static int16_t sHist[4][2];
static FILE *sWav;
static unsigned long long sWavFrames;
static unsigned sWavSince;
static FILE *sRegs;

void ab_lock(void) { pthread_mutex_lock(&sLock); }
void ab_unlock(void) { pthread_mutex_unlock(&sLock); }

static void WavHeader(FILE *f, unsigned long long frames) {
  unsigned char h[44];
  unsigned data = (unsigned)(frames * 4);
  memcpy(h, "RIFF", 4);
  unsigned v = 36 + data;
  memcpy(h + 4, &v, 4);
  memcpy(h + 8, "WAVEfmt ", 8);
  v = 16;
  memcpy(h + 16, &v, 4);
  unsigned short s = 1;
  memcpy(h + 20, &s, 2);
  s = 2;
  memcpy(h + 22, &s, 2);
  v = ACORE_WAV_RATE;
  memcpy(h + 24, &v, 4);
  v = ACORE_WAV_RATE * 4;
  memcpy(h + 28, &v, 4);
  s = 4;
  memcpy(h + 32, &s, 2);
  s = 16;
  memcpy(h + 34, &s, 2);
  memcpy(h + 36, "data", 4);
  memcpy(h + 40, &data, 4);
  fseek(f, 0, SEEK_SET);
  fwrite(h, 1, 44, f);
  fseek(f, 0, SEEK_END);
}

static void WavClose(void) {
  if (!sWav) return;
  WavHeader(sWav, sWavFrames);
  fclose(sWav);
  sWav = NULL;
}

void ab_wav_close(void) { WavClose(); }

/* creates the core; the output rate is the device's (SDL converts from it). 1 on success */
int ab_init(unsigned out_rate) {
  if (sCore) return 1;
  sCore = acore_create();
  if (!sCore) return 0;
  acore_set_live(sCore);
  sStep = (33513982.0 / 1024.0) / (double)out_rate;
  sFrac = 1.0; /* the first output frame fetches its samples */
  const char *solo = getenv("BWPORT_ACORE_SOLO"); /* measuring: music = only the music players' channels are heard, rest = only the others */
  if (solo && !strcmp(solo, "music")) acore_bus_solo(sCore, 1);
  else if (solo && !strcmp(solo, "rest")) acore_bus_solo(sCore, 2);
  const char *r = getenv("BWPORT_ACORE_REGS");
  if (r && *r && (sRegs = fopen(r, "w")) != NULL) acore_set_trace(sCore, sRegs);
  return 1;
}

/* the in-process restart (port/src/machine.c; the caller holds ab_lock): a new core, as at the start (zeroed memory and
 * registers, the driver as SND_Init leaves it); the output's resampler starts again with silence */
void ab_reset(void) {
  if (!sCore) return;
  acore_destroy(sCore);
  sCore = acore_create();
  if (!sCore) return;
  acore_set_live(sCore);
  const char *solo = getenv("BWPORT_ACORE_SOLO");
  if (solo && !strcmp(solo, "music")) acore_bus_solo(sCore, 1);
  else if (solo && !strcmp(solo, "rest")) acore_bus_solo(sCore, 2);
  if (sRegs) acore_set_trace(sCore, sRegs);
  sFrac = 1.0;
  memset(sHist, 0, sizeof sHist);
}

uint8_t *ab_mem(void) { return acore_mem(sCore); }

/* the music bus (docs/acore.md "Game speed and sound"): split the music players off the frame clock, make `frames` frames
 * of the music alone (the music clock), say what the next start on a driver slot is */
void ab_split(int on) {
  if (sCore) acore_bus_split(sCore, on);
}

int ab_run_music(int16_t *out, int frames) {
  return sCore && frames > 0 ? (int)acore_bus_run(sCore, (unsigned)frames, out) : 0;
}

void ab_set_quality(int enhanced) { acore_set_quality(sCore, enhanced); }

void ab_slot_music(int slot, int music) {
  if (sCore) acore_bus_slot(sCore, slot, music);
}

/* main memory at a DS address (the plan of docs/acore.md keeps to 0x02000000-0x0237FFFF) */
void ab_mem_write(uint32_t ds, const uint8_t *buf, uint32_t len) {
  uint8_t *m = acore_mem(sCore);
  for (uint32_t i = 0; i < len; i++) m[(ds + i) & (AC_MAIN_SIZE - 1)] = buf[i];
}

/* a command list ({id, a0..a3} x n) for the driver's next wake; 0 when the queue is full */
int ab_queue(const uint32_t (*cmd)[5], uint32_t n) {
  if (sCore->drv.nqueue >= AC_MAX_QUEUE) return 0;
  AC_List *l = &sCore->drv.queue[sCore->drv.nqueue++];
  l->n = n < AC_MAX_LIST ? n : AC_MAX_LIST;
  memcpy(l->cmd, cmd, (size_t)l->n * sizeof l->cmd[0]);
  return 1;
}

/* the ARM9's wakeup word */
void ab_wake(void) {
  if (sCore->drv.on && sCore->drv.npend < 16) sCore->drv.pend[sCore->drv.npend++] = sCore->now + sCore->drv.lag_word;
}

/* acore's clock: the cycle the last ab_run ended at = where a list queued now is taken (the next alarm after it) */
uint64_t ab_now(void) { return sCore->now; }

uint32_t ab_nqueue(void) { return sCore->drv.nqueue; }
/* a command wait with no reply (libntr-ios-audiowait.patch): acore's finished lists, and one more counted finished (the
 * driver writes the tag to the shared work as after a list) */
uint32_t ab_finish_tag(void) { return sCore->p15.finish_tag; }
void ab_finish_bump(void) {
  sCore->p15.finish_tag++;
  if (sCore->drv.shared) ac_wr32(sCore, sCore->drv.shared + AC_SW_FINISH_TAG, sCore->p15.finish_tag);
}

static int16_t Cubic(int a, int b, int c, int d, double t) {
  double v = b + 0.5 * t * (c - a + t * (2.0 * a - 5.0 * b + 4.0 * c - d + t * (3.0 * (b - c) + d - a)));
  return (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
}

/* the frame clock: run acore to the cycle `to` and hand over the DS-rate frames that made (interleaved s16 L R, at
 * most `max`); the rest of them are dropped (libntr asks for a frame at a time: ~548) */
int ab_run(uint64_t to, int16_t *out, int max) {
  if (to > sCore->now) acore_run(sCore, to);
  size_t n;
  const int16_t *pcm = acore_pcm(sCore, &n);
  if (n > (size_t)max) n = (size_t)max;
  memcpy(out, pcm, n * 4);
  if (sRegs) fflush(sRegs);
  acore_pcm_discard(sCore);
  return (int)n;
}

/* `frames` output frames (interleaved s16 L R) at the device rate; vol = libntr's master volume (0-127) */
void ab_render(int16_t *out, int frames, unsigned vol) {
  /* the device clock writes the WAV here; with the frame clock libntr's sim_audio.cpp does */
  static int wavTried;
  if (!wavTried) {
    wavTried = 1;
    const char *w = getenv("BWPORT_AUDIO_WAV");
    if (w && *w) {
      sWav = fopen(w, "wb");
      if (sWav) {
        WavHeader(sWav, 0);
        atexit(WavClose);
      } else {
        fprintf(stderr, "acore: cannot write %s\n", w);
      }
    }
  }
  /* how many DS samples the chunk consumes */
  double f = sFrac;
  int need = 0;
  for (int i = 0; i < frames; i++) {
    while (f >= 1.0) f -= 1.0, need++;
    f += sStep;
  }
  if (need > 0) acore_run(sCore, (sCore->nsamples + (uint64_t)need) * AC_CYCLES_PER_SAMPLE + sCore->sample_phase);
  size_t n;
  const int16_t *pcm = acore_pcm(sCore, &n);
  size_t q = 0;
  for (int i = 0; i < frames; i++) {
    while (sFrac >= 1.0) {
      memmove(sHist[0], sHist[1], 3 * sizeof sHist[0]);
      if (q < n) {
        sHist[3][0] = pcm[2 * q], sHist[3][1] = pcm[2 * q + 1];
        q++;
      }
      sFrac -= 1.0;
    }
    for (int c = 0; c < 2; c++) {
      int v = Cubic(sHist[0][c], sHist[1][c], sHist[2][c], sHist[3][c], sFrac);
      out[2 * i + c] = (int16_t)(vol >= 127 ? v : v * (int)vol / 127);
    }
    sFrac += sStep;
  }
  if (sWav && n) {
    fwrite(pcm, 4, n, sWav);
    sWavFrames += n;
    if (++sWavSince >= 100) {
      WavHeader(sWav, sWavFrames);
      fflush(sWav);
      sWavSince = 0;
    }
  }
  if (sRegs && sWavSince == 0) fflush(sRegs);
  acore_pcm_discard(sCore);
}

/* The sound archive's stream (port/src/strm_arc.c, the port's NNS_SndArcStrm*; acore's ac_strm.c). The decoded samples are
 * kept here, apart from the core, so that a restart's new core (ab_reset) can play them again. ab_strm_load and ab_strm_free
 * take the lock themselves (the decode runs outside it: tens of ms for a long stream); the others want it held. With no
 * core (Sound=libntr) the stream is silent: nothing is decoded and the position stays 0. */
static int16_t *sStrmPcm;
static AcoreStrmInfo sStrmInfo;

int ab_strm_load(const uint8_t *file, size_t size) {
  ab_lock();
  int live = sCore != NULL;
  ab_unlock();
  if (!live) return 0;
  AcoreStrmInfo in;
  int16_t *pcm = acore_strm_decode(file, size, &in);
  if (!pcm) return 0;
  ab_lock();
  if (sCore) acore_strm_set(sCore, NULL, NULL, 0, 0);
  int16_t *old = sStrmPcm;
  sStrmPcm = pcm;
  sStrmInfo = in;
  ab_unlock();
  free(old);
  return 1;
}

void ab_strm_free(void) {
  ab_lock();
  if (sCore) acore_strm_set(sCore, NULL, NULL, 0, 0);
  int16_t *old = sStrmPcm;
  sStrmPcm = NULL;
  ab_unlock();
  free(old);
}

/* stopped at offset_ms with the archive's volume (0-127); 0 when nothing is loaded */
int ab_strm_set(int volume, uint32_t offset_ms) {
  if (!sCore || !sStrmPcm) return 0;
  acore_strm_set(sCore, sStrmPcm, &sStrmInfo, volume, offset_ms);
  return 1;
}

void ab_strm_start(void) {
  if (sCore) acore_strm_start(sCore);
}

void ab_strm_stop(int fade_frames) {
  if (sCore) acore_strm_stop(sCore, fade_frames);
}

uint32_t ab_strm_pos(void) { return sCore ? acore_strm_pos_ms(sCore) : 0; }

int ab_strm_playing(void) { return sCore ? acore_strm_playing(sCore) : 0; }
