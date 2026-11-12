/* port/acore: WAV write / read (16-bit PCM). OWNED BY THE SKELETON. */
#include "acore.h"
#include <stdlib.h>
#include <string.h>

static void put16(FILE *f, unsigned v) { fputc(v & 0xFF, f), fputc((v >> 8) & 0xFF, f); }
static void put32(FILE *f, unsigned long v) { put16(f, v & 0xFFFF), put16(f, (v >> 16) & 0xFFFF); }

int acore_write_wav(const char *path, const int16_t *pcm, size_t n, unsigned rate) {
  FILE *f = fopen(path, "wb");
  if (!f) return 1;
  unsigned long bytes = (unsigned long)n * 4;
  fwrite("RIFF", 1, 4, f), put32(f, 36 + bytes), fwrite("WAVEfmt ", 1, 8, f);
  put32(f, 16), put16(f, 1), put16(f, 2), put32(f, rate), put32(f, rate * 4ul), put16(f, 4), put16(f, 16);
  fwrite("data", 1, 4, f), put32(f, bytes);
  for (size_t i = 0; i < 2 * n; i++) put16(f, (unsigned)(uint16_t)pcm[i]);
  return fclose(f) ? 1 : 0;
}

static unsigned get16(const unsigned char *p) { return p[0] | p[1] << 8; }
static unsigned long get32(const unsigned char *p) { return get16(p) | (unsigned long)get16(p + 2) << 16; }

int16_t *acore_read_wav(const char *path, size_t *n, unsigned *rate) {
  FILE *f = fopen(path, "rb");
  if (!f) return NULL;
  fseek(f, 0, SEEK_END);
  long sz = ftell(f);
  fseek(f, 0, SEEK_SET);
  unsigned char *b = malloc(sz > 0 ? (size_t)sz : 1);
  if (!b || fread(b, 1, (size_t)sz, f) != (size_t)sz || sz < 12 || memcmp(b, "RIFF", 4) || memcmp(b + 8, "WAVE", 4)) {
    fclose(f), free(b);
    return NULL;
  }
  fclose(f);
  unsigned ch = 0, bits = 0;
  int16_t *out = NULL;
  for (long p = 12; p + 8 <= sz;) {
    unsigned long csz = get32(b + p + 4);
    if (!memcmp(b + p, "fmt ", 4) && csz >= 16) {
      ch = get16(b + p + 10), *rate = (unsigned)get32(b + p + 12), bits = get16(b + p + 22);
    } else if (!memcmp(b + p, "data", 4) && ch && bits == 16) {
      unsigned long frames = (csz > (unsigned long)(sz - p - 8) ? (unsigned long)(sz - p - 8) : csz) / (2 * ch);
      out = malloc(frames * 4 + 4);
      for (unsigned long i = 0; i < frames; i++) {
        const unsigned char *s = b + p + 8 + i * 2 * ch;
        out[2 * i] = (int16_t)get16(s);
        out[2 * i + 1] = (int16_t)get16(s + (ch > 1 ? 2 : 0));
      }
      *n = frames;
      break;
    }
    p += 8 + (long)((csz + 1) & ~1ul);
  }
  free(b);
  return out;
}
