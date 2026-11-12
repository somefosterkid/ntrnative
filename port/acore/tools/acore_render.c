/* acore_render FILE.acs OUT.wav [--regs TRACE.txt] [--to CYCLES] [--info] [--enhanced]: renders a stream (docs/acore.md).
 * OWNED BY THE SKELETON (piece 17 from now on). */
#include "acore.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
  const char *in = NULL, *out = NULL, *regs = NULL;
  unsigned long long to = 0;
  int info = 0, enhanced = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--regs") && i + 1 < argc) regs = argv[++i];
    else if (!strcmp(argv[i], "--to") && i + 1 < argc) to = strtoull(argv[++i], NULL, 0);
    else if (!strcmp(argv[i], "--info")) info = 1;
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
