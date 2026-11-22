/* port/gpu piece G12: the texture cache's GL half (g12_tcgl.h). OWNED BY G12. */
#include "g12_tcgl.h"
#include "gpu_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct G12Gl {
  GLuint arr[G12_NBUCKET];
  u32 cap[G12_NBUCKET];
  GLuint t_poly[GPU_RING];
  u32 serial;       /* this GL side, for GpuTexCache.owner */
  u32 gen, applied; /* the generation its arrays hold and the last frame of it applied */
  u32 (*rows)[4];
  int nrows;
  int ok;           /* the context has the units and layers (else the cache is never used) */
  int mips;         /* the arrays hold mip chains (the cache's GpuTexCache.mips when they were made) */
  int filt, texmag, aniso; /* g12_gl_filter's */
  float max_aniso;  /* GL_MAX_TEXTURE_MAX_ANISOTROPY (0: no anisotropic filtering here) */
};

static u32 sSerial;

G12Gl *g12_gl_create(void) {
  G12Gl *t = calloc(1, sizeof *t);
  if (t == NULL) return NULL;
  t->serial = ++sSerial;
  GLint units = 0, layers = 0;
  GL(GetIntegerv)(GL_MAX_TEXTURE_IMAGE_UNITS, &units);
  GL(GetIntegerv)(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
  t->ok = units >= G12_UNIT + G12_NBUCKET + 1 && layers >= 256; /* (G12Options.max_layers 0 = 256) */
  if (!t->ok) fprintf(stderr, "gpu: texture cache off: %d texture units, %d array layers\n", units, layers);
  GLint n = 0;
  GL(GetIntegerv)(GL_NUM_EXTENSIONS, &n);
  for (GLint i = 0; i < n; i++) {
    const char *x = (const char *)GL(GetStringi)(GL_EXTENSIONS, (GLuint)i);
    if (x != NULL && (!strcmp(x, "GL_EXT_texture_filter_anisotropic") || !strcmp(x, "GL_ARB_texture_filter_anisotropic")))
      GL(GetFloatv)(0x84FF /* GL_MAX_TEXTURE_MAX_ANISOTROPY */, &t->max_aniso);
  }
  t->aniso = 1;
  return t;
}

/* bucket b's sampling: exact = nearest, level 0; filtered = TexMag's magnification, trilinear over the chain with mips, Aniso */
static void params(G12Gl *t, int b) {
  int S = 32 << b, top = 0;
  for (int s = S; s > 1 && t->mips; s >>= 1) top++;
  GLint mag = t->filt && t->texmag ? GL_LINEAR : GL_NEAREST;
  GLint min = !t->filt ? GL_NEAREST : t->mips ? GL_LINEAR_MIPMAP_LINEAR : mag;
  GL(BindTexture)(GL_TEXTURE_2D_ARRAY, t->arr[b]);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, min);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, mag);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, t->filt ? top : 0);
  if (t->max_aniso >= 1.0f) {
    float a = t->filt && t->aniso > 1 ? (float)t->aniso : 1.0f;
    GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, 0x84FE /* GL_TEXTURE_MAX_ANISOTROPY */, (GLint)(a < t->max_aniso ? a : t->max_aniso));
  }
}

void g12_gl_filter(G12Gl *t, int filtered, int texmag, int aniso) {
  if (aniso < 1) aniso = 1;
  if (t->filt == filtered && t->texmag == texmag && t->aniso == aniso) return;
  t->filt = filtered, t->texmag = texmag, t->aniso = aniso;
  GL(ActiveTexture)(GL_TEXTURE0 + G12_UNIT);
  for (int b = 0; b < G12_NBUCKET; b++)
    if (t->arr[b]) params(t, b);
  GL(BindTexture)(GL_TEXTURE_2D_ARRAY, 0);
}

static void drop_arrays(G12Gl *t) {
  for (int b = 0; b < G12_NBUCKET; b++) {
    if (t->arr[b]) GL(DeleteTextures)(1, &t->arr[b]);
    t->arr[b] = 0, t->cap[b] = 0;
  }
}

void g12_gl_destroy(G12Gl *t) {
  if (t == NULL) return;
  drop_arrays(t);
  for (int i = 0; i < GPU_RING; i++)
    if (t->t_poly[i]) gpu_gl_data_forget(t->t_poly[i]), GL(DeleteTextures)(1, &t->t_poly[i]);
  free(t->rows);
  free(t);
}

/* bucket b as a new array of cap layers (the old one's layers are gone: g12_frame re-decodes their entries) */
static void grow(G12Gl *t, int b, u32 cap) {
  int S = 32 << b;
  if (t->arr[b]) GL(DeleteTextures)(1, &t->arr[b]);
  GL(GenTextures)(1, &t->arr[b]);
  GL(BindTexture)(GL_TEXTURE_2D_ARRAY, t->arr[b]);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  GL(TexParameteri)(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  for (int l = 0, s = S; s >= 1 && (l == 0 || t->mips); l++, s >>= 1)
    GL(TexImage3D)(GL_TEXTURE_2D_ARRAY, l, GL_RGBA8, s, s, (GLsizei)cap, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  t->cap[b] = cap;
  params(t, b);
}

static void apply(G12Gl *t, const G12Batch *bt) {
  for (int b = 0; b < G12_NBUCKET; b++)
    if (bt->grow_cap[b]) grow(t, b, bt->grow_cap[b]);
  for (u32 i = 0; i < bt->nup; i++) {
    const G12Upload *u = &bt->up[i];
    if (u->bucket >= G12_NBUCKET || t->arr[u->bucket] == 0 || u->layer >= t->cap[u->bucket]) continue; /* (never) */
    GL(BindTexture)(GL_TEXTURE_2D_ARRAY, t->arr[u->bucket]);
    const u8 *src = bt->bytes + u->offset;
    for (int l = 0, s = u->size; l < (t->mips ? u->levels : 1); l++, s >>= 1) {
      GL(TexSubImage3D)(GL_TEXTURE_2D_ARRAY, l, 0, 0, u->layer, s, s, 1, GL_RGBA, GL_UNSIGNED_BYTE, src);
      src += (size_t)s * s * 4;
    }
  }
}

int g12_gl_prepare(G12Gl *t, GpuTexCache *c, const GpuTcFrame *f, const GpuPrep *p, int ri) {
  if (!t->ok || f->frame == 0) return 0;
  G12Batch *todo = NULL;
  pthread_mutex_lock(&c->mu);
  if (f->gen != c->gen) { /* a frame of an older generation */
    pthread_mutex_unlock(&c->mu);
    return 0;
  }
  if (c->owner == t->serial && c->mips != t->mips) { /* the cache's layers changed form (mips): start again */
    g12_restart(c);
    pthread_mutex_unlock(&c->mu);
    drop_arrays(t);
    t->gen = 0;
    return 0;
  }
  if (c->owner != t->serial) {
    t->mips = c->mips;
    if (c->owner != 0) { /* another GL side took from this generation: its layers are not ours, start again */
      g12_restart(c);
      pthread_mutex_unlock(&c->mu);
      drop_arrays(t);
      t->gen = 0;
      return 0;
    }
    c->owner = t->serial; /* nothing of it applied anywhere: ours from its first frame */
    t->gen = c->gen, t->applied = 0;
    drop_arrays(t);
  }
  /* the batches up to f's frame, in order */
  if (c->head != NULL && c->head->frame <= f->frame) {
    G12Batch *last = c->head;
    while (last->next != NULL && last->next->frame <= f->frame) last = last->next, c->nbatch--;
    c->nbatch--;
    todo = c->head;
    c->head = last->next;
    if (c->head == NULL) c->tail = NULL;
    last->next = NULL;
  }
  pthread_mutex_unlock(&c->mu);
  if (todo != NULL) {
    GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, 0);
    GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
    GL(ActiveTexture)(GL_TEXTURE0 + G12_UNIT);
    for (G12Batch *b = todo, *n; b != NULL; b = n) {
      n = b->next;
      apply(t, b);
      if (b->frame > t->applied) t->applied = b->frame;
      free(b->up), free(b->bytes), free(b);
    }
    GL(BindTexture)(GL_TEXTURE_2D_ARRAY, 0);
  }
  if (f->frame > t->applied) t->applied = f->frame;
  if (f->frame < t->applied || f->nready == 0) return 0; /* the layers have moved past this frame's */
  /* the per-polygon table */
  int n = p->sp.npoly;
  if (n > t->nrows) {
    free(t->rows);
    t->rows = malloc((size_t)n * G12_POLY_TEXELS * sizeof *t->rows);
    t->nrows = t->rows != NULL ? n : 0;
    if (t->rows == NULL) return 0;
  }
  int any = 0;
  for (int k = 0; k < n; k++) {
    const uint32_t *P0 = p->sp.poly[k].t[0];
    u32 tp = P0[1], v = g12_tcf_find(f, g12_key(tp, P0[2]));
    int b = (int)(v >> 1) & 7;
    u32(*r)[4] = &t->rows[k * G12_POLY_TEXELS];
    if (v == 0 || t->arr[b] == 0 || (v >> 16) >= t->cap[b]) {
      memset(r, 0, G12_POLY_TEXELS * sizeof *r);
      continue;
    }
    float inv = 1.0f / (float)(32 << b);
    r[0][0] = v | (t->filt ? 16u : 0u) | (t->filt && !t->texmag ? 32u : 0u) | /* filtered, magnify-nearest */
              (t->filt && (t->mips || t->texmag || t->aniso > 1) ? 64u : 0u);   /* a sampler that blends texels: clamp its taps */
    r[0][1] = (u32)g12_tc_width(tp);
    r[0][2] = (u32)g12_tc_height(tp);
    memcpy(&r[0][3], &inv, 4);
    if (t->filt && p->sp.planes) {
      for (int i = 0; i < 3; i++) memcpy(r[1 + i], &p->sp.plane[k][i * 3], 12), r[1 + i][3] = 0;
      const int16_t *g = p->sp.range[k]; /* s range in t2's w, t range in t3's w: low and high 16 bits (12.4) */
      r[2][3] = (u32)(uint16_t)g[0] | (u32)(uint16_t)g[1] << 16;
      r[3][3] = (u32)(uint16_t)g[2] | (u32)(uint16_t)g[3] << 16;
    } else
      memset(r[1], 0, 3 * sizeof *r);
    any++;
  }
  if (!any) return 0;
  gpu_gl_data_tex(&t->t_poly[ri], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, t->rows, n * G12_POLY_TEXELS);
  return any;
}

void g12_gl_bind(G12Gl *t, GLuint prog, int ri) {
  static const char *const names[G12_NBUCKET] = {"uTc0", "uTc1", "uTc2", "uTc3", "uTc4"};
  for (int b = 0; b < G12_NBUCKET; b++) {
    GL(ActiveTexture)(GL_TEXTURE0 + G12_UNIT + b);
    GL(BindTexture)(GL_TEXTURE_2D_ARRAY, t->arr[b]);
    GL(Uniform1i)(gpu_gl_uniform(prog, names[b]), G12_UNIT + b);
  }
  GL(ActiveTexture)(GL_TEXTURE0 + G12_UNIT + G12_NBUCKET);
  GL(BindTexture)(GL_TEXTURE_2D, t->t_poly[ri]);
  GL(Uniform1i)(gpu_gl_uniform(prog, "uTcPoly"), G12_UNIT + G12_NBUCKET);
  GL(ActiveTexture)(GL_TEXTURE0);
}
