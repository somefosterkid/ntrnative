/* acore_render FILE.acs OUT.wav [--regs TRACE.txt] [--to CYCLES] [--info] [--enhanced]: renders a stream (docs/acore.md).
 * OWNED BY THE SKELETON (piece 17 from now on). */
#include "acore.h"
#include "ac_shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
  const char *in = NULL, *out = NULL, *regs = NULL;
  unsigned long long to = 0;
  int info = 0, enhanced = 0;
  long stf0 = 0, stf1 = 0, stfs = 1;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--regs") && i + 1 < argc) regs = argv[++i];
    else if (!strcmp(argv[i], "--to") && i + 1 < argc) to = strtoull(argv[++i], NULL, 0);
    else if (!strcmp(argv[i], "--info")) info = 1;
    else if (!strcmp(argv[i], "--state") && i + 3 < argc) { stf0 = atol(argv[i + 1]); stf1 = atol(argv[i + 2]); stfs = atol(argv[i + 3]); i += 3; }
    else if (!strcmp(argv[i], "--enhanced")) enhanced = 1; /* acore_set_quality(1): not the oracle-exact output */
    else if (!in) in = argv[i];
    else if (!out) out = argv[i];
  }
  if (!in || !out) {
    fprintf(stderr, "usage: acore_render FILE.acs OUT.wav [--regs TRACE.txt] [--to CYCLES] [--info] [--enhanced]\n");
    return 2;
  }
  AcoreState *s = acore_create();
  char err[256];
  if (acore_load_acs(s, in, err, sizeof err)) {
    fprintf(stderr, "acore_render: %s: %s\n", in, err);
    return 1;
  }
  acore_set_quality(s, enhanced);
  FILE *tf = regs ? fopen(regs, "w") : NULL;
  if (tf) acore_set_trace(s, tf);
  clock_t t0 = clock();
  if (stf1 > 0) {   /* --state F0 F1 STEP: dump the driver's players, tracks and channels every STEP game frames (measuring) */
    for (long f = stf0; f <= stf1; f += stfs) {
      acore_run(s, (uint64_t)f * AC_CYCLES_PER_FRAME);
      printf("== frame %ld  SOUNDCNT %04x\n", f, ac_hw_read(s, AC_REG_SOUNDCNT, 2));
      for (int p = 0; p < AC_NPLAYER; p++) {
        const AC_Player *pl = &s->work.player[p];
        if (!(pl->flags & AC_PLF_ACTIVE)) continue;
        printf(" player %d flags %02x prio %d vol %d fader %d tempo %d\n", p, pl->flags, pl->prio, pl->volume, pl->extFader, pl->tempo);
        for (int k = 0; k < AC_TRACKS_PER_PLAYER; k++) {
          if (pl->tracks[k] == AC_NO_TRACK) continue;
          const AC_Track *t = &s->work.track[pl->tracks[k]];
          if (!(t->flags & AC_TRF_ACTIVE)) continue;
          printf("   trk %d(%d) fl %02x vol %d expr %d fader %d chmask %04x prg %d wait %d\n", k, pl->tracks[k], t->flags, t->volume, t->volume2,
                 t->extFader, t->channel_mask, t->prgNo, t->wait);
        }
      }
      for (int c = 0; c < AC_NCH; c++) {
        const AC_ExChannel *e = &s->work.channel[c];
        u32 cnt = ac_hw_read(s, AC_REG_CH(c) + AC_CH_CNT, 4);
        if (!(e->flags & AC_EXF_ACTIVE) && !(cnt & AC_CNT_START)) continue;
        printf(" ch %2d exfl %02x env %d prio %d key %d vel %d vol %d | cnt %08x vol %d div %d pan %d busy %d\n", c, e->flags, e->env_status, e->prio, e->key,
               e->velocity, e->volume, cnt, AC_CNT_VOL(cnt), AC_CNT_DIV(cnt), AC_CNT_PAN(cnt), cnt >> 31);
      }
    }
    return 0;
  }
  acore_run(s, to ? to : acore_end(s));
  double cpu = (double)(clock() - t0) / CLOCKS_PER_SEC;
  size_t n;
  const int16_t *pcm = acore_pcm(s, &n);
  if (acore_write_wav(out, pcm, n, ACORE_WAV_RATE)) {
    fprintf(stderr, "acore_render: can't write %s\n", out);
    return 1;
  }
  if (tf) fclose(tf);
  if (info)
    fprintf(stderr, "acore_render: %s: %zu samples (%.3f s), end %llu cycles, cpu %.3f s (%.2f ms per second of audio)\n", in, n,
            n / 32728.5, (unsigned long long)acore_end(s), cpu, 1000.0 * cpu / (n / 32728.5));
  acore_destroy(s);
  return 0;
}
