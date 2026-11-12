/* port/gpu pass S: the span records built on the GPU (docs/gpu-cost.md P1). OWNED BY G2.
 *
 * The CPU keeps p16's per-polygon setup (vc_span_poly and gpu_spans_build's decisions: the grid, the widening, Dot/Line, the
 * 1-dot case) and writes a few texels a polygon (g02_spans.h, GpuSetup); shaders/g02_setup.vert runs vc_span_row for each
 * (polygon, row), one vertex per record, and transform feedback writes the records into a buffer that one copy puts into the
 * span texture pass R reads, so they are never built or uploaded by the CPU. One draw of GL_POINTS with the rasterizer off:
 * any GLES 3.0 context runs it (48 interleaved components, 64 guaranteed). BWPORT_GPU_SPANS=cpu keeps the CPU builder
 * (gpu_spans_build's records: the reference and the fallback), uploaded as they are. */
#include "g02_setup.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

enum { REC_BYTES = GPU_SPAN_TEXELS * 16, CHUNK = 1024, RANGE = 2048, SPLIT_ROWS = 32 };

static int sMode = -1, sSplit; /* gpu_g02_mode; the copy in pieces of SPLIT_ROWS rows */
static const char *const kMode[] = {"tf", "chunk", "range", "frag", "cpu"};

int gpu_g02_mode(void) {
  if (sMode < 0) {
    const char *e = getenv("BWPORT_GPU_PASSS");
    sMode = GPU_PS_TF;
    for (int i = 0; e != NULL && i <= GPU_PS_CPU; i++)
      if (!strcmp(e, kMode[i])) sMode = i;
  }
  return sMode;
}

const char *gpu_g02_mode_name(void) { return kMode[gpu_g02_mode()]; }

int gpu_g02_env(void) {
  const char *e = getenv("BWPORT_GPU_PASSS");
  if (e != NULL && !strcmp(e, "cpu")) return 1;
  e = getenv("BWPORT_GPU_SPANS");
  if (e == NULL) return 0;
  return !strcmp(e, "cpu") ? 1 : !strcmp(e, "nopass") ? 2 : 0;
}

int gpu_g02_init(GpuPassS *ps, char *err, size_t errlen) {
  GLint mx = 0;
  GL(GetIntegerv)(GL_MAX_TEXTURE_SIZE, &mx);
  ps->maxrows = mx > 0 ? mx : 2048;
  if (gpu_g02_env() != 0) return -1;
  if (ps->prog) return 0;
  static const char *const kOut[GPU_SPAN_TEXELS] = {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"};
  ps->prog = gpu_gl_program_tf("g02_setup.vert", "(transform feedback)", kOut, GPU_SPAN_TEXELS, err, errlen);
  if (ps->prog == 0) {
    fprintf(stderr, "gpu: pass S: %s; the CPU builds the span records\n", err);
    return -1;
  }
  GL(GenBuffers)(1, &ps->tfb);
  return 0;
}

void gpu_g02_release(GpuPassS *ps) {
  gpu_gl_program_done(ps->prog);
  gpu_gl_program_done(ps->prog_f);
  GLuint tex[] = {ps->t_setup[0], ps->t_setup[1], ps->t_setup[2], ps->t_find[0], ps->t_find[1], ps->t_find[2], ps->t_span[0], ps->t_span[1], ps->t_span[2],
                  ps->t_frag[0], ps->t_frag[1], ps->t_frag[2], ps->t_frag[3]};
  for (unsigned i = 0; i < sizeof tex / sizeof tex[0]; i++)
    if (tex[i]) gpu_gl_data_forget(tex[i]), GL(DeleteTextures)(1, &tex[i]);
  GLuint fbo[] = {ps->fbo_read, ps->fbo_f, ps->fbo_s};
  for (unsigned i = 0; i < 3; i++)
    if (fbo[i]) GL(DeleteFramebuffers)(1, &fbo[i]);
  if (ps->query) GL(DeleteQueries)(1, &ps->query);
  if (ps->tfb) GL(DeleteBuffers)(1, &ps->tfb);
  if (ps->cpu) gpu_spans_free(ps->cpu);
  free(ps->cpu);
  free(ps->tmp);
  memset(ps, 0, sizeof *ps);
}

static uint32_t *tmp_room(GpuPassS *ps, size_t words) {
  if (words > ps->tmpcap) {
    uint32_t *t = realloc(ps->tmp, words * 4);
    if (t == NULL) return NULL;
    ps->tmp = t;
    ps->tmpcap = words;
  }
  return ps->tmp;
}

/* pass S's inputs (the setup and the find entries) on units 0 and 1 for prog, made current; u: its uniforms (the caller adds
 * the draw's and passes them on: gpu_gl_uniforms) */
static void inputs(GpuPassS *ps, const GpuSpans *sp, GLuint prog, GpuU *u) {
  ps->ring = (ps->ring + 1) % GPU_RING;
  GL(ActiveTexture)(GL_TEXTURE0);
  gpu_gl_data_tex(&ps->t_setup[ps->ring], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, sp->su.tx, sp->su.ntx);
  GL(ActiveTexture)(GL_TEXTURE0 + 1);
  gpu_gl_data_tex(&ps->t_find[ps->ring], GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT, 8, sp->su.find, sp->su.nfind);
  GL(UseProgram)(prog);
  GL(Uniform1i)(gpu_gl_uniform(prog, "uSetup"), 0);
  GL(Uniform1i)(gpu_gl_uniform(prog, "uFind"), 1);
  memset(u, 0, sizeof *u);
  u->uNFind = sp->su.nfind;
  u->uZero = 0;
}

/* pass S's fragment form (GPU_PS_FRAG) into the span texture span (room made): texels 4j..4j+3 of each record into band j
 * of the four targets (draw j: the record built three times), then one draw puts them in the span texture's order */
static int frag_prog(GpuPassS *ps) {
  if (ps->prog_f != 0) return 0;
  char err[512];
  ps->prog_f = gpu_gl_program("g00_full.vert", "g02_setup.vert", err, sizeof err);
  if (ps->prog_f == 0) {
    fprintf(stderr, "gpu: pass S (fragment): %s\n", err);
    return -1;
  }
  GL(GenFramebuffers)(1, &ps->fbo_f);
  GL(GenFramebuffers)(1, &ps->fbo_s);
  return 0;
}
static void pass_s_frag(GpuPassS *ps, const GpuSpans *sp, GLuint vao, GLuint span) {
  int n = sp->nspan, R = (n + GPU_DATA_W - 1) / GPU_DATA_W, rows = (n * GPU_SPAN_TEXELS + GPU_DATA_W - 1) / GPU_DATA_W;
  GLenum bufs[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  GL(BindFramebuffer)(GL_FRAMEBUFFER, ps->fbo_f);
  for (int k = 0; k < 4; k++) {
    gpu_gl_data_tex(&ps->t_frag[k], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, NULL, 3 * R * GPU_DATA_W);
    GL(FramebufferTexture2D)(GL_FRAMEBUFFER, bufs[k], GL_TEXTURE_2D, ps->t_frag[k], 0);
  }
  GL(DrawBuffers)(4, bufs);
  GpuU u;
  inputs(ps, sp, ps->prog_f, &u);
  GL(BindVertexArray)(vao);
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  u.uN = n;
  u.uR = R;
  for (int j = 0; j < 3; j++) {
    GL(Viewport)(0, j * R, GPU_DATA_W, R);
    u.uPart = j;
    gpu_gl_uniforms(&u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  GL(BindFramebuffer)(GL_FRAMEBUFFER, ps->fbo_s);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, span, 0);
  GL(DrawBuffers)(1, bufs);
  static const char *const kF[4] = {"uF0", "uF1", "uF2", "uF3"};
  for (int k = 0; k < 4; k++) {
    GL(ActiveTexture)(GL_TEXTURE0 + 2 + (GLenum)k);
    GL(BindTexture)(GL_TEXTURE_2D, ps->t_frag[k]);
    GL(Uniform1i)(gpu_gl_uniform(ps->prog_f, kF[k]), 2 + k);
  }
  u.uPart = 3;
  gpu_gl_uniforms(&u);
  GL(Viewport)(0, 0, GPU_DATA_W, rows);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
}

/* pass S: sp's setup in, its records out into ps->tfb (grown by half when short) */
static void pass_s(GpuPassS *ps, const GpuSpans *sp, GLuint vao) {
  /* room for whole rows: the copy below takes the last, partial row whole (one call; each buffer-sourced upload costs a
   * Metal blit on Apple's GL), and a pixel-unpack read must lie inside the buffer. The texels past the records are
   * whatever the buffer held: pass R reads only the records. */
  size_t need = (size_t)((sp->nspan * GPU_SPAN_TEXELS + GPU_DATA_W - 1) / GPU_DATA_W) * GPU_DATA_W * 16;
  GL(BindBuffer)(GL_TRANSFORM_FEEDBACK_BUFFER, ps->tfb);
  if (need > ps->tfcap) {
    size_t cap = ps->tfcap + ps->tfcap / 2;
    if (cap < need) cap = need;
    GL(BufferData)(GL_TRANSFORM_FEEDBACK_BUFFER, (GLsizeiptr)cap, NULL, GL_DYNAMIC_COPY);
    ps->tfcap = cap;
  }
  GL(BindBuffer)(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
  GpuU u;
  inputs(ps, sp, ps->prog, &u);
  GL(BindVertexArray)(vao);
  int n = sp->nspan, mode = gpu_g02_mode();
  if (ps->diag) {
    if (ps->query == 0) GL(GenQueries)(1, &ps->query);
    GL(BeginQuery)(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, ps->query);
  }
  GL(Enable)(GL_RASTERIZER_DISCARD);
  if (mode == GPU_PS_RANGE) { /* each chunk bound at its own offset, in its own session */
    for (int f = 0; f < n; f += RANGE) {
      int c = n - f < RANGE ? n - f : RANGE;
      GL(BindBufferRange)(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ps->tfb, (GLintptr)f * REC_BYTES, (GLsizeiptr)c * REC_BYTES);
      u.uFirst = f;
      gpu_gl_uniforms(&u);
      GL(BeginTransformFeedback)(GL_POINTS);
      GL(DrawArrays)(GL_POINTS, 0, c);
      GL(EndTransformFeedback)();
    }
  } else { /* one session: one draw, or draws of CHUNK points (gl_VertexID from 0 in each) */
    int step = mode == GPU_PS_CHUNK ? CHUNK : n;
    GL(BindBufferBase)(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ps->tfb);
    GL(BeginTransformFeedback)(GL_POINTS);
    for (int f = 0; f < n; f += step) {
      u.uFirst = f;
      gpu_gl_uniforms(&u);
      GL(DrawArrays)(GL_POINTS, 0, n - f < step ? n - f : step);
    }
    GL(EndTransformFeedback)();
  }
  GL(Disable)(GL_RASTERIZER_DISCARD);
  if (ps->diag) GL(EndQuery)(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
  GL(BindBufferBase)(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

int gpu_g02_records(GpuPassS *ps, const GpuSpans *sp, const VCoreState *s, int scale, GLuint vao) {
  int n = sp->nspan, texels = n * GPU_SPAN_TEXELS;
  if ((texels + GPU_DATA_W - 1) / GPU_DATA_W > ps->maxrows) return -1;
  int mode = gpu_g02_mode();
  if (ps->prog != 0 && sp->setup && (mode == GPU_PS_CPU || (mode == GPU_PS_FRAG && frag_prog(ps) != 0))) {
    sMode = GPU_PS_CPU; /* no fragment form here either: this core builds the records (the self-check then turns pass S off) */
    gpu_gl_program_done(ps->prog);
    ps->prog = 0;
  }
  if (ps->prog == 0 || !sp->setup) { /* the CPU's records: the prep's, or built here */
    const GpuSpans *c = sp;
    if (!sp->records) {
      if (ps->cpu == NULL && (ps->cpu = calloc(1, sizeof *ps->cpu)) == NULL) return -1;
      ps->cpu->dot_block = sp->dot_block;
      ps->cpu->line_wide = sp->line_wide;
      ps->cpu->aa = sp->aa;
      ps->cpu->opt_set = 1;
      ps->cpu->setup = 0;
      ps->cpu->records = 1;
      gpu_spans_build(ps->cpu, s, scale);
      c = ps->cpu;
      if (c->nspan != n) return -1;
    }
    ps->sring = (ps->sring + 1) % GPU_RING;
    /* through the unpack ring, whole rows in one call (2026-11-03): from client memory Apple's GL converts the records (6-8 MB
     * at 3x with 4 samples) to the texture's layout on this thread inside TexSubImage2D, from a buffer the GPU does */
    int rows = (texels + GPU_DATA_W - 1) / GPU_DATA_W;
    gpu_gl_data_tex(&ps->t_span[ps->sring], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, NULL, texels); /* room, bound */
    unsigned char *m = n > 0 ? gpu_gl_unpack_map((size_t)rows * GPU_DATA_W * 16) : NULL;
    if (m != NULL) {
      memcpy(m, c->span, (size_t)texels * 16);
      if (gpu_gl_unpack_unmap() == 0) {
        GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
        GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, GPU_DATA_W, rows, GL_RGBA_INTEGER, GL_UNSIGNED_INT, (const void *)0);
        gpu_gl_unpack_done();
        return 0;
      }
    }
    gpu_gl_data_tex(&ps->t_span[ps->sring], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, c->span, texels);
    return 0;
  }
  ps->sring = 0;
  if (mode == GPU_PS_FRAG) {
    gpu_gl_data_tex(&ps->t_span[0], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, NULL, texels); /* room */
    if (n > 0) pass_s_frag(ps, sp, vao, ps->t_span[0]);
    gpu_gl_data_tex(&ps->t_span[0], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, NULL, texels); /* bound */
    return 0;
  }
  if (n > 0) pass_s(ps, sp, vao);
  gpu_gl_data_tex(&ps->t_span[0], GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, NULL, texels); /* room, bound */
  if (n < 1) return 0;
  /* the partial row whole (pass_s made room); in pieces when a self-check found the buffer right and the texture not */
  int rows = (texels + GPU_DATA_W - 1) / GPU_DATA_W, step = sSplit ? SPLIT_ROWS : rows;
  GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, ps->tfb);
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  for (int y = 0; y < rows; y += step)
    GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, y, GPU_DATA_W, rows - y < step ? rows - y : step, GL_RGBA_INTEGER, GL_UNSIGNED_INT,
                      (const void *)((size_t)y * GPU_DATA_W * 16));
  GL(BindBuffer)(GL_PIXEL_UNPACK_BUFFER, 0);
  return 0;
}

void gpu_g02_read(GpuPassS *ps, int n, uint32_t *out) {
  int rows = (n * GPU_SPAN_TEXELS + GPU_DATA_W - 1) / GPU_DATA_W;
  uint32_t *t = tmp_room(ps, (size_t)rows * GPU_DATA_W * 4);
  if (t == NULL || rows < 1 || ps->t_span[ps->sring] == 0) return;
  if (ps->fbo_read == 0) GL(GenFramebuffers)(1, &ps->fbo_read);
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, ps->fbo_read);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ps->t_span[ps->sring], 0);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(ReadPixels)(0, 0, GPU_DATA_W, rows, GL_RGBA_INTEGER, GL_UNSIGNED_INT, t);
  memcpy(out, t, (size_t)n * GPU_SPAN_TEXELS * 16);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
}

/* the fields: 0 integer, 1 an attribute (hi exact), 2 an attribute's low part (within an ulp) */
static const char *const kField[GPU_SPAN_TEXELS][4] = {
    {"xs", "xe", "den_f", "den_z"},     {"L.w", "R.w", "L.z", "R.z"},     {"L.c0", "L.c1", "L.c2", "L.s"},
    {"R.c0", "R.c1", "R.c2", "R.s"},    {"L.t", "R.t", "flags", "x0"},   {"t5.x", "t5.y", "t5.z", "t5.w"},
    {"t6.x", "t6.y", "t6.z", "t6.w"},   {"t7.x", "t7.y", "t7.z", "t7.w"}, {"t8.x", "t8.y", "t8.z", "t8.w"},
    {"t9.x", "t9.y", "t9.z", "t9.w"},   {"lo L.z", "lo R.z", "in.lo", "in.hi"}, {"lo L.t", "lo R.t", "lo L.s", "lo R.s"}};
static int kind(int t, int c) {
  if (t >= 1 && t <= 3) return 1;
  if (t == 4 && c < 2) return 1;
  if ((t == 10 && c < 2) || t == 11) return 2;
  return 0;
}

static int find_poly(const GpuSpans *sp, int rec) {
  int lo = 0, hi = sp->su.nfind - 1;
  while (lo < hi) {
    int mid = (lo + hi + 1) / 2;
    if ((int)sp->su.find[mid][0] <= rec) lo = mid;
    else hi = mid - 1;
  }
  return lo;
}

int gpu_g02_compare(const GpuSpans *sp, const uint32_t *gpu, FILE *f, int detail) {
  int cnt[GPU_SPAN_TEXELS][4] = {{0}}, nint = 0, nhi = 0, nlo = 0, nlobits = 0, shown = 0, shownlo = 0;
  for (int r = 0; r < sp->nspan; r++)
    for (int t = 0; t < GPU_SPAN_TEXELS; t++)
      for (int c = 0; c < 4; c++) {
        float a = sp->span[r].t[t][c], b;
        uint32_t ua;
        memcpy(&ua, &a, 4);
        uint32_t ub = gpu[((size_t)r * GPU_SPAN_TEXELS + t) * 4 + c];
        memcpy(&b, &ub, 4);
        if (ua == ub) continue;
        int k = kind(t, c);
        if (k == 2) {
          nlobits++;
          float m = fabsf(a) > fabsf(b) ? fabsf(a) : fabsf(b);
          if (fabsf(a - b) <= nextafterf(m, INFINITY) - m) continue;
        }
        cnt[t][c]++;
        if (k == 0) nint++;
        else if (k == 1) nhi++;
        else nlo++;
        if (detail && (k == 2 ? shownlo++ < 3 : shown++ < 10)) { /* the first ten integer or hi differences, three lo ones */
          int p = find_poly(sp, r);
          fprintf(f, "  record %d (setup %d, its record %d) %s: cpu %.9g gpu %.9g\n", r, p, r - (int)sp->su.find[p][0],
                  kField[t][c], a, b);
        }
      }
  fprintf(f, "spans-check: %d records, %d polygons: %d integer fields differ, %d attribute hi, %d attribute lo beyond an ulp "
             "(%d lo not bit-equal)\n", sp->nspan, sp->su.nfind, nint, nhi, nlo, nlobits);
  for (int t = 0; t < GPU_SPAN_TEXELS; t++)
    for (int c = 0; c < 4; c++)
      if (detail && cnt[t][c]) fprintf(f, "  %-8s %d\n", kField[t][c], cnt[t][c]);
  return nint;
}

/* gpu's records against sp's: the first whose integer fields differ, how many do, and the record from which every word is 0 */
static void wrong(const GpuSpans *sp, const uint32_t *gpu, int *first, int *count, int *zero) {
  *first = -1, *count = 0, *zero = 0;
  for (int r = 0; r < sp->nspan; r++) {
    const uint32_t *g = gpu + (size_t)r * GPU_SPAN_TEXELS * 4;
    int bad = 0, nz = 0;
    for (int t = 0; t < GPU_SPAN_TEXELS; t++)
      for (int c = 0; c < 4; c++) {
        uint32_t ua;
        memcpy(&ua, &sp->span[r].t[t][c], 4);
        nz |= g[t * 4 + c] != 0;
        bad |= kind(t, c) == 0 && ua != g[t * 4 + c];
      }
    if (bad && (*count)++ == 0) *first = r;
    if (nz) *zero = r + 1;
  }
}

int gpu_g02_step(GpuPassS *ps, const GpuSpans *sp, const uint32_t *gpu, FILE *f) {
  int mode = gpu_g02_mode(), n = sp->nspan, bf = -1, bc = -1, bz = -1, tf, tc, tz;
  GLuint written = 0;
  GLint size = 0;
  GL(Finish)();
  if (mode != GPU_PS_FRAG && ps->tfb != 0) { /* what transform feedback wrote, in its buffer */
    if (ps->query != 0) GL(GetQueryObjectuiv)(ps->query, GL_QUERY_RESULT, &written);
    GL(BindBuffer)(GL_TRANSFORM_FEEDBACK_BUFFER, ps->tfb);
    GL(GetBufferParameteriv)(GL_TRANSFORM_FEEDBACK_BUFFER, GL_BUFFER_SIZE, &size);
    const uint32_t *m = GL(MapBufferRange)(GL_TRANSFORM_FEEDBACK_BUFFER, 0, (GLsizeiptr)n * REC_BYTES, GL_MAP_READ_BIT);
    if (m != NULL) {
      wrong(sp, m, &bf, &bc, &bz);
      GL(UnmapBuffer)(GL_TRANSFORM_FEEDBACK_BUFFER);
    }
    GL(BindBuffer)(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
  }
  wrong(sp, gpu, &tf, &tc, &tz);
  fprintf(f, "pass S %s: transform feedback wrote %u of %d points; its buffer (%d bytes for %d): %d records wrong from %d, all 0 "
             "from %d; the span texture: %d wrong from %d, all 0 from %d (-1: not read)\n",
          kMode[mode], written, n, (int)size, n * REC_BYTES, bc, bf, bz, tc, tf, tz);
  static int limits; /* once */
  if (!limits++) {
    GLint lim[6] = {0};
    const GLenum kLim[6] = {GL_MAX_TEXTURE_SIZE, GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,
                            GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, GL_MAX_VERTEX_OUTPUT_COMPONENTS, GL_MAX_ELEMENTS_VERTICES,
                            GL_MAX_ELEMENT_INDEX};
    for (int i = 0; i < (gpu_gl.api == GPU_API_ES ? 6 : 5); i++) GL(GetIntegerv)(kLim[i], &lim[i]); /* (ES 3.0 has the last) */
    fprintf(f, "pass S limits: texture %d, transform feedback interleaved %d, separate %d, vertex outputs %d, elements' "
               "vertices %d, element index %d\n", lim[0], lim[1], lim[2], lim[3], lim[4], lim[5]);
  }
  if (mode != GPU_PS_FRAG && bc == 0 && tc > 0 && !sSplit) sSplit = 1; /* the buffer right, the copy not: the copy in pieces */
  else if (mode != GPU_PS_FRAG && bc == 0 && tc > 0) mode = GPU_PS_FRAG; /* still not: no copy */
  else mode++;
  sMode = mode;
  fprintf(f, "pass S: next %s%s\n", kMode[mode], sSplit && mode < GPU_PS_FRAG ? ", the copy in pieces" : "");
  return mode;
}
