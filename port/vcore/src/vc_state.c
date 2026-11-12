/* port/vcore: the state, the .vcs loader and the outputs (the skeleton's; docs/vcore.md). The .vcs format:
 * <ref>/VCS_FORMAT.md (g50a's design): "VCST", version 1, then chunks (4-byte tag, u32 size, data, padded to 4):
 * INFO text, IO9 0x700, IO9B 0x70, VRAM 0xA4000 (banks A-I), PLTT 0x800, OAM 0x800, GX3D words, LSHT 256x384 RGBA,
 * L3D 256x192 RGBA. Unknown chunks are skipped; a missing chunk leaves its memory zero. */
#include "vcore.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>

const u32 vc_bank_size[VC_NBANKS] = {0x20000, 0x20000, 0x20000, 0x20000, 0x10000, 0x4000, 0x4000, 0x8000, 0x4000};
const u32 vc_bank_offset[VC_NBANKS] = {0x00000, 0x20000, 0x40000, 0x60000, 0x80000,
                                       0x90000, 0x94000, 0x98000, 0xA0000};

static void free_input(VCoreState *s) {
  free(s->gx);
  free(s->info);
  free(s->lsht);
  free(s->l3d);
  s->gx = NULL;
  s->gx_words = 0;
  s->info = NULL;
  s->lsht = s->l3d = NULL;
}

VCoreState *vcore_create(void) {
  VCoreState *s = (VCoreState *)calloc(1, sizeof(VCoreState));
  if (s != NULL) vc_reset_work(s);
  return s;
}

void vcore_destroy(VCoreState *s) {
  if (s == NULL) return;
  vc_p20_destroy(s);
  vc_p21_destroy(s);
  vc_ip_tape_free(s);
  free_input(s);
  free(s);
}

void vc_reset_geo(VCoreState *s) {
  memset(&s->geo, 0, sizeof s->geo);
  for (u32 i = 0; i < VC_MAX_POLYS; i++) s->geo.order[i] = (u16)i;
}

void vc_reset_work(VCoreState *s) {
  memset(s->rgn, 0, sizeof s->rgn);
  memset(&s->geo, 0, sizeof s->geo);
  memset(&s->f3d, 0, sizeof s->f3d);
  memset(&s->line, 0, sizeof s->line);
  memset(s->eng_out, 0, sizeof s->eng_out);
  memset(s->screen, 0, sizeof s->screen);
  for (u32 i = 0; i < VC_MAX_POLYS; i++) s->geo.order[i] = (u16)i;
  s->plot = vc_3d_plot;
}

static u32 rd32(const u8 *p) { return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24; }

static int fail(char *err, size_t len, const char *msg) {
  if (err != NULL && len) snprintf(err, len, "%s", msg);
  return -1;
}

static void *dup_bytes(const u8 *p, u32 n, int nul) {
  u8 *d = (u8 *)malloc(n + (nul ? 1 : 0) + 4);
  if (d == NULL) return NULL;
  memcpy(d, p, n);
  if (nul) d[n] = 0;
  return d;
}

int vcore_load_vcs_mem(VCoreState *s, const void *data, size_t size, char *err, size_t errlen) {
  const u8 *b = (const u8 *)data;
  if (size < 8 || memcmp(b, "VCST", 4) != 0) return fail(err, errlen, "not a .vcs file (no VCST magic)");
  if (rd32(b + 4) != 1) return fail(err, errlen, "unknown .vcs version (want 1)");
  free_input(s);
  memset(s->io9, 0, sizeof s->io9);
  memset(s->io9b, 0, sizeof s->io9b);
  memset(s->vram, 0, sizeof s->vram);
  memset(s->pltt, 0, sizeof s->pltt);
  memset(s->oam, 0, sizeof s->oam);
  size_t at = 8;
  while (at + 8 <= size) {
    const u8 *tag = b + at;
    u32 n = rd32(b + at + 4);
    at += 8;
    if (n > size - at) return fail(err, errlen, "truncated chunk");
    const u8 *p = b + at;
#define TAG(t) (memcmp(tag, t, 4) == 0)
#define COPY(dst) memcpy(dst, p, n < sizeof(dst) ? n : sizeof(dst))
    if (TAG("INFO")) s->info = (char *)dup_bytes(p, n, 1);
    else if (TAG("IO9 ")) COPY(s->io9);
    else if (TAG("IO9B")) COPY(s->io9b);
    else if (TAG("VRAM")) COPY(s->vram);
    else if (TAG("PLTT")) COPY(s->pltt);
    else if (TAG("OAM ")) COPY(s->oam);
    else if (TAG("GX3D")) {
      s->gx = (u32 *)malloc(n + 4);
      if (s->gx == NULL) return fail(err, errlen, "out of memory");
      for (u32 i = 0; i < n / 4; i++) s->gx[i] = rd32(p + i * 4);
      s->gx_words = n / 4;
    } else if (TAG("LSHT") && n >= 256 * 384 * 4) s->lsht = (u8 *)dup_bytes(p, 256 * 384 * 4, 0);
    else if (TAG("L3D ") && n >= 256 * 192 * 4) s->l3d = (u8 *)dup_bytes(p, 256 * 192 * 4, 0);
#undef TAG
#undef COPY
    at += n;
    at = (at + 3) & ~(size_t)3;
  }
  /* POWCNT1 as the oracle reads it by default (<ref>/oracle/README): the LCD bit on, and a file with 0 (a
   * capture that never saw the register) gets both engines and the 3D on, engine A on top */
  u16 pow = (u16)(s->io9[VC_REG_POWCNT1] | s->io9[VC_REG_POWCNT1 + 1] << 8);
  pow = pow == 0 ? 0x820F : (u16)(pow | 1);
  s->io9[VC_REG_POWCNT1] = (u8)pow;
  s->io9[VC_REG_POWCNT1 + 1] = (u8)(pow >> 8);
  vc_reset_work(s);
  return 0;
}

int vcore_load_vcs(VCoreState *s, const char *path, char *err, size_t errlen) {
  FILE *f = fopen(path, "rb");
  if (f == NULL) return fail(err, errlen, "can't open the file");
  fseek(f, 0, SEEK_END);
  long n = ftell(f);
  fseek(f, 0, SEEK_SET);
  if (n < 0) {
    fclose(f);
    return fail(err, errlen, "can't size the file");
  }
  u8 *buf = (u8 *)malloc((size_t)n + 1);
  if (buf == NULL || fread(buf, 1, (size_t)n, f) != (size_t)n) {
    free(buf);
    fclose(f);
    return fail(err, errlen, "can't read the file");
  }
  fclose(f);
  int r = vcore_load_vcs_mem(s, buf, (size_t)n, err, errlen);
  free(buf);
  return r;
}

const char *vc_info(const VCoreState *s, const char *key, char *buf, size_t len) {
  if (s->info == NULL || len == 0) return NULL;
  size_t kl = strlen(key);
  for (const char *l = s->info; *l;) {
    const char *e = strchr(l, '\n');
    size_t ll = e ? (size_t)(e - l) : strlen(l);
    if (ll > kl && memcmp(l, key, kl) == 0 && l[kl] == '=') {
      size_t vl = ll - kl - 1 < len - 1 ? ll - kl - 1 : len - 1;
      memcpy(buf, l + kl + 1, vl);
      buf[vl] = 0;
      return buf;
    }
    if (e == NULL) break;
    l = e + 1;
  }
  return NULL;
}

void vcore_render(VCoreState *s) {
  vc_reset_work(s);
  s->p20.stage_ok = 0;
  if (vc_p21_active(s)) vc_render_frame_n(s);
  else vc_render_frame(s);
}

/* vcore_render again for the same state with only the 3D redone when the last vcore_render left the 2D reusable (the state's
 * inputs unchanged since, p23's blend may differ): in-between pictures. Same picture as vcore_render. */
void vcore_render_again(VCoreState *s) {
  if (!vc_render_3d_again(s)) vcore_render(s);
}

void vcore_forget_2d(VCoreState *s) { s->p20.stage_ok = 0; }

const uint32_t *vcore_screen(const VCoreState *s, int screen) { return &s->screen[screen ? 1 : 0][0][0]; }

static inline u8 c6to8(u32 c) { return (u8)((c << 2) | (c >> 4)); }

void vcore_screen_rgba(const VCoreState *s, int screen, uint8_t *rgba) {
  const u32 *px = vcore_screen(s, screen);
  for (int i = 0; i < VC_W * VC_H; i++) {
    u32 c = px[i];
    rgba[i * 4 + 0] = c6to8(VC_R6(c));
    rgba[i * 4 + 1] = c6to8(VC_G6(c));
    rgba[i * 4 + 2] = c6to8(VC_B6(c));
    rgba[i * 4 + 3] = 255;
  }
}

void vcore_frame_rgba(const VCoreState *s, uint8_t *rgba) {
  vcore_screen_rgba(s, 0, rgba);
  vcore_screen_rgba(s, 1, rgba + VC_W * VC_H * 4);
}

const uint8_t *vcore_libntr_shot(const VCoreState *s) { return s->lsht; }
