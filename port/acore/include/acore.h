/* port/acore: a DS sound core (docs/acore.md). The public interface. OWNED BY THE SKELETON.
 *
 *   AcoreState *s = acore_create();
 *   if (acore_load_acs(s, "clip.acs", err, sizeof err)) ...
 *   acore_render_all(s);                     // or acore_run(s, until_cycles) in steps
 *   size_t n; const int16_t *pcm = acore_pcm(s, &n);   // n stereo samples, L R interleaved, 32728.5 Hz
 *   acore_write_wav("out.wav", pcm, n, ACORE_WAV_RATE);
 *   acore_destroy(s);
 */
#ifndef ACORE_H
#define ACORE_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ACORE_WAV_RATE 32728

typedef struct AcoreState AcoreState;

AcoreState *acore_create(void);   /* zeroed memory and registers, the driver as SND_Init leaves it */
void acore_destroy(AcoreState *s);
/* loads a .acs stream (replacing any earlier one; the state is reset first). 0 on success, else err says why */
int acore_load_acs(AcoreState *s, const char *path, char *err, size_t errlen);
int acore_load_acs_mem(AcoreState *s, const void *data, size_t size, char *err, size_t errlen);
void acore_run(AcoreState *s, uint64_t until); /* runs events, wakes and samples up to cycle `until` */
void acore_render_all(AcoreState *s);          /* to the stream's END */
uint64_t acore_end(const AcoreState *s);       /* the stream's END time (cycles) */
const int16_t *acore_pcm(const AcoreState *s, size_t *nsamples);
void acore_pcm_discard(AcoreState *s);         /* drops the samples returned so far (a live caller; time goes on) */
void acore_set_quality(AcoreState *s, int enhanced); /* 1 = Enhanced (cubic-interpolated PCM/ADPCM samples, no 10-bit DAC), 0 = the DS's exact output (the default) */
void acore_set_live(AcoreState *s);              /* a core with no stream (the port's live path): the timing a driver-mode stream sets */
void acore_set_trace(AcoreState *s, FILE *f);  /* the register trace: "T ADDR SIZE VALUE" per write; NULL = off */
uint8_t *acore_mem(AcoreState *s);             /* main memory (4 MB at 0x02000000) */
/* the music bus (docs/acore.md "Game speed and sound"): a slot's next start is music (the flag is taken by the sequence
 * that starts next on that slot), the split on/off between two acore_run calls, `frames` music samples on the music
 * clock (0 when not split), solo for measuring (0 all, 1 music channels only, 2 the others only) */
void acore_bus_slot(AcoreState *s, int slot, int music);
void acore_bus_split(AcoreState *s, int on);
int acore_bus_split_on(const AcoreState *s);
size_t acore_bus_run(AcoreState *s, unsigned frames, int16_t *out);
void acore_bus_solo(AcoreState *s, int mode);
const char *acore_info(const AcoreState *s, const char *key); /* an INFO value, or NULL */
int acore_write_wav(const char *path, const int16_t *pcm, size_t nsamples, unsigned rate); /* 0 on success */
/* the sound archive's stream (ac_strm.c; the port's NNS_SndArcStrm*): a STRM file decoded whole (malloc'd, nsamp frames of
 * nch interleaved samples; NULL if it is not one), then played on the host side beside the channels, added to the mix.
 * acore_strm_set keeps pcm (the caller frees it after a set with NULL) and leaves the stream stopped at offset_ms;
 * acore_strm_stop fades over fade_frames game frames (0: at once); the position is 0 when not playing */
typedef struct AcoreStrmInfo {
  uint32_t nsamp, nch, rate, loop, loop_start;
} AcoreStrmInfo;
int16_t *acore_strm_decode(const void *file, size_t size, AcoreStrmInfo *info);
void acore_strm_set(AcoreState *s, const int16_t *pcm, const AcoreStrmInfo *info, int volume, uint32_t offset_ms);
void acore_strm_start(AcoreState *s);
void acore_strm_stop(AcoreState *s, int fade_frames);
int acore_strm_playing(const AcoreState *s);
uint32_t acore_strm_pos_ms(const AcoreState *s);
/* reads a 16-bit stereo or mono WAV (mono is duplicated); returns malloc'd interleaved samples, NULL on error */
int16_t *acore_read_wav(const char *path, size_t *nsamples, unsigned *rate);

#endif
