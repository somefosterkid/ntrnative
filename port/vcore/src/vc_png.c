/* port/vcore: a PNG writer without a compression library (stored deflate blocks), for vcore_render and the tests.
 * The skeleton's. */
#include "vcore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t crc_table[256];

static void crc_init(void) {
  if (crc_table[1]) return;
  for (uint32_t n = 0; n < 256; n++) {
    uint32_t c = n;
    for (int k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
    crc_table[n] = c;
  }
}

static uint32_t crc(uint32_t c, const uint8_t *p, size_t n) {
  c ^= 0xFFFFFFFFu;
  while (n--) c = crc_table[(c ^ *p++) & 0xFF] ^ (c >> 8);
  return c ^ 0xFFFFFFFFu;
}

static void be32(uint8_t *p, uint32_t v) {
  p[0] = (uint8_t)(v >> 24);
  p[1] = (uint8_t)(v >> 16);
  p[2] = (uint8_t)(v >> 8);
  p[3] = (uint8_t)v;
}

static int chunk(FILE *f, const char *type, const uint8_t *data, uint32_t n) {
  uint8_t h[8];
  be32(h, n);
  memcpy(h + 4, type, 4);
  uint32_t c = crc(0, h + 4, 4);
  c = crc(c, data, n);
  uint8_t t[4];
  be32(t, c);
  return fwrite(h, 1, 8, f) == 8 && (n == 0 || fwrite(data, 1, n, f) == n) && fwrite(t, 1, 4, f) == 4;
}

int vcore_write_png(const char *path, const uint8_t *rgba, int w, int h) {
  crc_init();
  size_t row = (size_t)w * 4 + 1, raw = row * (size_t)h;
  size_t nblocks = (raw + 65534) / 65535;
  size_t zlen = 2 + raw + nblocks * 5 + 4;
  uint8_t *z = (uint8_t *)malloc(zlen), *q = z;
  uint8_t *img = (uint8_t *)malloc(raw);
  if (z == NULL || img == NULL) {
    free(z);
    free(img);
    return -1;
  }
  for (int y = 0; y < h; y++) {
    img[y * row] = 0; /* filter: none */
    memcpy(img + y * row + 1, rgba + (size_t)y * w * 4, (size_t)w * 4);
  }
  *q++ = 0x78;
  *q++ = 0x01;
  uint32_t a = 1, b = 0;
  for (size_t i = 0; i < raw; i++) {
    a = (a + img[i]) % 65521;
    b = (b + a) % 65521;
  }
  for (size_t at = 0; at < raw || at == 0;) {
    size_t n = raw - at > 65535 ? 65535 : raw - at;
    *q++ = (uint8_t)(at + n >= raw ? 1 : 0);
    *q++ = (uint8_t)n;
    *q++ = (uint8_t)(n >> 8);
    *q++ = (uint8_t)~n;
    *q++ = (uint8_t)(~n >> 8);
    memcpy(q, img + at, n);
    q += n;
    at += n;
    if (raw == 0) break;
  }
  be32(q, (b << 16) | a);
  q += 4;
  FILE *f = fopen(path, "wb");
  int ok = f != NULL;
  if (ok) {
    static const uint8_t sig[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'};
    uint8_t ihdr[13];
    be32(ihdr, (uint32_t)w);
    be32(ihdr + 4, (uint32_t)h);
    ihdr[8] = 8;  /* bit depth */
    ihdr[9] = 6;  /* RGBA */
    ihdr[10] = 0; /* deflate */
    ihdr[11] = 0;
    ihdr[12] = 0;
    ok = fwrite(sig, 1, 8, f) == 8 && chunk(f, "IHDR", ihdr, 13) && chunk(f, "IDAT", z, (uint32_t)(q - z)) &&
         chunk(f, "IEND", NULL, 0);
    ok = (fclose(f) == 0) && ok;
  }
  free(z);
  free(img);
  return ok ? 0 : -1;
}
