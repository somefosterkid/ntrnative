/* port/gpu piece G4: the texture slots' upload (docs/gpu.md, G4). OWNED BY G4.
 * The texture unit sees VRAM through two regions (vcore p12): VC_RGN_TEX, the OR of the banks per 16 KB page of the
 * 512 KB of slots 0-3 (the address wraps at npages pages), and VC_RGN_TEXPAL, 6 pages. Both are flattened to words
 * (gpu_texdata_build, on any thread: the port's worker does it once per frame); the shader (shaders/g04_texture.glsl)
 * decodes the 7 formats from them. */
#include "g04_texture.h"

#include <stdlib.h>
#include <string.h>

enum { TEX_PAGES = 32, PAL_PAGES = 6, TOON_WORDS = 16, PAL_WORDS = PAL_PAGES * VC_PAGE_SIZE / 4 };

struct GpuTexData {
  u8 tex[TEX_PAGES * VC_PAGE_SIZE];
  u8 pal[PAL_WORDS * 4 + TOON_WORDS * 4];
  unsigned serial; /* this build's number (gpu_g04_upload skips a build it uploaded last) */
};

GpuTexData *gpu_texdata_create(void) { return calloc(1, sizeof(GpuTexData)); }
void gpu_texdata_destroy(GpuTexData *t) { free(t); }

static void page(u8 *d, const VC_Page *pg) {
  if (pg->n == 1) {
    memcpy(d, pg->src[0], VC_PAGE_SIZE);
    return;
  }
  memset(d, 0, VC_PAGE_SIZE);
  for (int i = 0; i < pg->n; i++)
    for (int o = 0; o < VC_PAGE_SIZE; o++) d[o] |= pg->src[i][o];
}

void gpu_texdata_build(GpuTexData *t, const VCoreState *s) {
  static unsigned serial; /* the builds' count over all GpuTexData: a serial names one build (atomic: two workers may build) */
  const VC_Region *r = &s->rgn[VC_RGN_TEX];
  for (int p = 0; p < TEX_PAGES; p++) {
    u8 *d = t->tex + (size_t)p * VC_PAGE_SIZE;
    if (r->npages == 0) memset(d, 0, VC_PAGE_SIZE);
    else page(d, &r->page[(u32)p % r->npages]);
  }
  /* the palette: no wrap (an address past the mapped pages reads 0) */
  const VC_Region *pr = &s->rgn[VC_RGN_TEXPAL];
  for (int p = 0; p < PAL_PAGES; p++) {
    u8 *d = t->pal + (size_t)p * VC_PAGE_SIZE;
    if ((u32)p >= pr->npages) memset(d, 0, VC_PAGE_SIZE);
    else page(d, &pr->page[p]);
  }
  for (int i = 0; i < 32; i++) {
    u32 v = vc_io16(s, VC_REG_TOON_TABLE + i * 2);
    t->pal[PAL_WORDS * 4 + i * 2] = (u8)v;
    t->pal[PAL_WORDS * 4 + i * 2 + 1] = (u8)(v >> 8);
  }
  t->serial = __atomic_add_fetch(&serial, 1, __ATOMIC_RELAXED);
}

void gpu_g04_upload(GLuint *t_tex, GLuint *t_pal, unsigned *up, int *cur, const GpuTexData *t) {
  int i = *cur;
  if (t_tex[i] != 0 && t_pal[i] != 0 && t->serial == up[i]) return; /* this build is up (an in-between picture of its frame) */
  *cur = i = (i + 1) % GPU_RING; /* each pair keeps its own copy for the diff: the rows that differ from what it holds */
  gpu_gl_data_tex_diff(&t_tex[i], GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, t->tex, TEX_PAGES * VC_PAGE_SIZE / 4);
  gpu_gl_data_tex_diff(&t_pal[i], GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, t->pal, PAL_WORDS + TOON_WORDS);
  up[i] = t->serial;
}

void gpu_texdata_bytes(const GpuTexData *t, const void **tex, int *ntex, const void **pal, int *npal, unsigned *serial) {
  *tex = t->tex;
  *ntex = TEX_PAGES * VC_PAGE_SIZE / 4;
  *pal = t->pal;
  *npal = PAL_WORDS + TOON_WORDS;
  *serial = t->serial;
}

void gpu_g04_bind(GLuint prog, int unit, GLuint t_tex, GLuint t_pal) {
  GL(ActiveTexture)(GL_TEXTURE0 + unit);
  GL(BindTexture)(GL_TEXTURE_2D, t_tex);
  GL(Uniform1i)(gpu_gl_uniform(prog, "uTex"), unit);
  GL(ActiveTexture)(GL_TEXTURE0 + unit + 1);
  GL(BindTexture)(GL_TEXTURE_2D, t_pal);
  GL(Uniform1i)(gpu_gl_uniform(prog, "uTexPal"), unit + 1);
}
