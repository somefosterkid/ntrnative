/* port/gpu G7: engine A's compositor at N x, the CPU side (docs/gpu.md, G7; shaders/g07_compose.frag). OWNED BY G7.
 *
 * Per frame: the 2D layers of engine A (vcore, 1x) packed into one RGBA32UI texture, the 1x lines of what the shader
 * does not compose (engine A in display modes 0/2/3, forced blank or powered off; engine B) into an R32UI texture (g07_pack:
 * the port's worker does it, and a frame's in-between pictures reuse the upload),
 * the registers as uniforms (vcore applies them per frame, not per line), one full-screen pass into an RGBA8 texture. */
#include "g07_compose.h"
#include "gpu_gl.h"
#include "gpu_mtl.h"
#include "vc_shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* what one screen of pass C's output was drawn from (g07_set_reuse): the engine, its 2D (the serial of the pack it was made in),
 * the uniforms, the 3D layer (its number, the texture names, its scale), the 2D filter's style; cleared first, compared whole */
typedef struct G07Key {
  int eng, up, sc;
  unsigned aserial, bserial, id3d, tex3d, smaa;
  GpuU u;
} G07Key;

struct G07 {
  int scale, w, h; /* scale: the grid pass C draws on (N); w, h: the output (256 N x 384 N) */
  GLuint prog, vao, fbo, t_out, t_stage, t_src, t_3d, t_none; /* t_none: 1 x 1 RGBA8, cSmaa's when unused */
  GLuint prog_plan, fbo_plan, t_plan; /* the plan (g07_compose.frag's C_PLAN), 256 x 192 RG32UI */
  GLuint fbo_cap, t_cap; /* g07_compose_capsrc's: engine A without master brightness, 256 N x 192 N RGBA8 (made on first use) */
  unsigned plan_serial;               /* the pack the plan is made from (0: none) and the uniforms it read */
  uint32_t plan_key[9];
  int sc3d;
  VC_Line *own;    /* g07_stage_build's lines when the caller gives none */
  G07Pack *pack;   /* g07_compose's */
  unsigned up;     /* the pack in t_stage / t_src (0: none) */
  uint8_t *rb;
  GpuMtlC *mtl; /* g07_create_metal's: the GL fields are unused */
  /* the 2D filter (g07_set_up2d; shaders/g11_up.glsl): the style, its programs (analysis, pass C's N x variant; built on
   * first use, failed = drawn Off), the info texture and what it was made from */
  int up2d;
  int hiprec; /* g07_set_hiprec */
  struct { unsigned tex, ser; uint32_t bg; int ox, oy, y0, y1; } cap; /* g07_set_capbg */
  GLuint up_prog[G07_UP_N][3], fbo_info, t_info; /* [2]: a second analysis stage (MMPX at N >= 3: into t_info from t_info1) */
  GLuint fbo_info1, t_info1;                     /* the first stage's info when there are two */
  int up_failed[G07_UP_N];
  unsigned info_serial;
  uint32_t info_key[3]; /* style, cCompose, cHave3d */
  /* the screens' reuse (g07_set_reuse): what each half of the output was last drawn from (0 the top, the first 192 N rows; 1 the
   * bottom), the 3D layer's number for the next compose (g07_set_3d_id), the check mode's readbacks and the counts */
  int reuse; /* 0 off (the default), 1 on, 2 check */
  unsigned id3d;
  int key_ok[2];
  G07Key key[2];
  uint8_t *chk[2];
  unsigned long long n_comp, n_half, n_none, n_checked, n_differ;
  unsigned long long out_gen; /* g07_out_gen */
};

struct G07Pack {
  uint32_t stage[VC_H * VC_W * 4];
  uint32_t src[VC_H * 2 * VC_W]; /* engine A's lines (when not composed), then engine B's */
  unsigned serial;               /* this pack's number */
  unsigned aserial;              /* the number of the pack engine A's stage was made in (reuse bit 0: an earlier one's) */
  unsigned bserial;              /* the same for engine B's lines (reuse bit 1) */
};

G07Pack *g07_pack_create(void) { return calloc(1, sizeof(G07Pack)); }
void g07_pack_destroy(G07Pack *k) { free(k); }

/* engine A composed on the GPU only in display mode 1, powered on and not forced blank (vc_display_line) */
static int composed(VCoreState *s) {
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  int mode = (dispcnt >> 16) & 3, on = (vc_io16(s, VC_REG_POWCNT1) >> 1) & 1;
  return mode == 1 && on && !(dispcnt & 0x80);
}

void g07_pack(G07Pack *k, VCoreState *s, const VC_Line *stage) { g07_pack_reuse(k, s, stage, NULL, 0); }

void g07_pack_reuse(G07Pack *k, VCoreState *s, const VC_Line *stage, const G07Pack *from, int reuse) {
  static unsigned serial; /* the packs' count over all G07Packs (atomic: two workers may pack) */
  int compose = composed(s);
  if (reuse & 1) {
    memcpy(k->stage, from->stage, sizeof k->stage);
    if (!compose) memcpy(k->src, from->src, sizeof k->src / 2);
  }
  if (reuse & 2) memcpy(&k->src[VC_H * VC_W], &from->src[VC_H * VC_W], sizeof k->src / 2);
  for (int y = 0; y < VC_H; y++) {
    if (!(reuse & 2)) memcpy(&k->src[(size_t)(VC_H + y) * VC_W], s->eng_out[VC_ENG_B][y], VC_W * 4);
    if (reuse & 1) continue;
    const VC_Line *ln = &stage[y];
    uint32_t *o = &k->stage[(size_t)y * VC_W * 4];
    uint32_t bgon = (uint32_t)(ln->bg_on[0] | ln->bg_on[1] << 1 | ln->bg_on[2] << 2 | ln->bg_on[3] << 3);
    for (int x = 0; x < VC_W; x++) {
      o[x * 4 + 0] = ln->obj[x];
      o[x * 4 + 1] = (ln->bg[0][x] & 0xFFFFu) | (ln->bg[1][x] & 0xFFFFu) << 16;
      o[x * 4 + 2] = (ln->bg[2][x] & 0xFFFFu) | (ln->bg[3][x] & 0xFFFFu) << 16;
      o[x * 4 + 3] = ln->winmask[x] | bgon << 8 | (uint32_t)(ln->bg3d != 0) << 12;
    }
    if (!compose) vc_display_line(s, VC_ENG_A, y, ln, &k->src[(size_t)y * VC_W]);
  }
  k->serial = __atomic_add_fetch(&serial, 1, __ATOMIC_RELAXED);
  k->aserial = (reuse & 1) ? from->aserial : k->serial;
  k->bserial = (reuse & 2) ? from->bserial : k->serial;
}

int g07_pack_differs(const G07Pack *a, const G07Pack *b, VCoreState *s, int mask) {
  int d = 0;
  if ((mask & 1) && (memcmp(a->stage, b->stage, sizeof a->stage) != 0 || (!composed(s) && memcmp(a->src, b->src, sizeof a->src / 2) != 0)))
    d |= 1;
  if ((mask & 2) && memcmp(&a->src[VC_H * VC_W], &b->src[VC_H * VC_W], sizeof a->src / 2) != 0) d |= 2;
  return d;
}

static GLuint make_tex(GLenum ifmt, int w, int h) {
  GLuint t;
  GL(GenTextures)(1, &t);
  GL(BindTexture)(GL_TEXTURE_2D, t);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL(TexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  (void)ifmt; (void)w; (void)h;
  return t;
}

G07 *g07_create(int scale, char *err, size_t errlen) {
  if (scale < 1 || scale > GPU_MAX_SCALE) {
    snprintf(err, errlen, "scale %d: 1..%d", scale, GPU_MAX_SCALE);
    return NULL;
  }
  G07 *c = calloc(1, sizeof *c);
  if (c == NULL) {
    snprintf(err, errlen, "out of memory");
    return NULL;
  }
  c->scale = scale;
  c->w = VC_W * scale;
  c->h = VC_H * 2 * scale;
  c->rb = malloc((size_t)c->w * c->h * 4);
  c->prog = gpu_gl_program("g00_full.vert", "g07_compose.frag", err, errlen);
  c->prog_plan = c->prog ? gpu_gl_program("g00_full.vert", "g07_compose.frag#1+C_PLAN", err, errlen) : 0;
  if (!c->rb || !c->prog || !c->prog_plan) {
    g07_destroy(c);
    return NULL;
  }
  GL(GenVertexArrays)(1, &c->vao);
  c->t_out = make_tex(GL_RGBA8, c->w, c->h);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, c->w, c->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  c->t_stage = make_tex(GL_RGBA32UI, VC_W, VC_H);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA32UI, VC_W, VC_H, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
  c->t_src = make_tex(GL_R32UI, VC_W, VC_H * 2);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_R32UI, VC_W, VC_H * 2, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  c->t_none = make_tex(GL_RGBA8, 1, 1);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  c->t_plan = make_tex(GL_RG32UI, VC_W, VC_H);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RG32UI, VC_W, VC_H, 0, GL_RG_INTEGER, GL_UNSIGNED_INT, NULL);
  GL(GenFramebuffers)(1, &c->fbo);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, c->t_out, 0);
  GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GL(GenFramebuffers)(1, &c->fbo_plan);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_plan);
  GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, c->t_plan, 0);
  if (st == GL_FRAMEBUFFER_COMPLETE) st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  if (st != GL_FRAMEBUFFER_COMPLETE) {
    snprintf(err, errlen, "G7 framebuffer incomplete (0x%x)", st);
    g07_destroy(c);
    return NULL;
  }
  return c;
}

G07 *g07_create_metal(int scale, char *err, size_t errlen) {
  if (scale < 1 || scale > GPU_MAX_SCALE) {
    snprintf(err, errlen, "scale %d: 1..%d", scale, GPU_MAX_SCALE);
    return NULL;
  }
  G07 *c = calloc(1, sizeof *c);
  if (c == NULL || (c->mtl = gpu_mtl_c_create(scale, err, errlen)) == NULL) {
    free(c);
    return NULL;
  }
  c->scale = scale;
  c->w = VC_W * scale;
  c->h = VC_H * 2 * scale;
  return c;
}

void g07_destroy(G07 *c) {
  if (c == NULL) return;
  if (c->mtl != NULL) {
    gpu_mtl_c_destroy(c->mtl);
    free(c->own);
    g07_pack_destroy(c->pack);
    free(c);
    return;
  }
  gpu_gl_program_done(c->prog);
  gpu_gl_program_done(c->prog_plan);
  for (int i = 0; i < G07_UP_N; i++)
    for (int j = 0; j < 3; j++)
      if (c->up_prog[i][j]) gpu_gl_program_done(c->up_prog[i][j]);
  if (c->fbo_info) GL(DeleteFramebuffers)(1, &c->fbo_info);
  if (c->fbo_info1) GL(DeleteFramebuffers)(1, &c->fbo_info1);
  GLuint tex[] = {c->t_out, c->t_stage, c->t_src, c->t_3d, c->t_none, c->t_plan, c->t_cap, c->t_info, c->t_info1};
  for (unsigned i = 0; i < sizeof tex / sizeof tex[0]; i++)
    if (tex[i]) GL(DeleteTextures)(1, &tex[i]);
  if (c->fbo) GL(DeleteFramebuffers)(1, &c->fbo);
  if (c->fbo_plan) GL(DeleteFramebuffers)(1, &c->fbo_plan);
  if (c->fbo_cap) GL(DeleteFramebuffers)(1, &c->fbo_cap);
  if (c->vao) GL(DeleteVertexArrays)(1, &c->vao);
  free(c->own);
  g07_pack_destroy(c->pack);
  free(c->rb);
  free(c);
}

/* vc_render_line_a1 (p20_frame.c): OBJs, BGs (the 3D BG only marked; vc_bg_line also reads s->f3d.out for it, which is
 * not used), windows */
void g07_stage_build(VCoreState *s, VC_Line *stage) {
  for (int y = 0; y < VC_H; y++) {
    VC_Line *ln = &stage[y];
    memset(ln, 0, sizeof *ln);
    memset(ln->winmask, 0x3F, sizeof ln->winmask);
    vc_obj_line(s, VC_ENG_A, y, ln);
    for (int n = 0; n < 4; n++) vc_bg_line(s, VC_ENG_A, n, y, ln);
    vc_window_line(s, VC_ENG_A, y, ln);
  }
}

unsigned g07_upload_3d(G07 *c, const uint32_t *l3d, int sc) {
  size_t n = (size_t)VC_W * sc * VC_H * sc;
  uint8_t *b = malloc(n * 4);
  if (b == NULL) return 0;
  for (size_t i = 0; i < n; i++) {
    uint32_t p = l3d[i];
    b[i * 4 + 0] = (uint8_t)(p & 63);
    b[i * 4 + 1] = (uint8_t)((p >> 8) & 63);
    b[i * 4 + 2] = (uint8_t)((p >> 16) & 63);
    b[i * 4 + 3] = (uint8_t)((p >> 24) & 31);
  }
  if (c->mtl != NULL) {
    unsigned t = gpu_mtl_c_upload_3d(c->mtl, b, VC_W * sc, VC_H * sc);
    free(b);
    c->sc3d = sc;
    return t;
  }
  if (c->t_3d == 0) c->t_3d = make_tex(GL_RGBA8UI, 0, 0);
  GL(BindTexture)(GL_TEXTURE_2D, c->t_3d);
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8UI, VC_W * sc, VC_H * sc, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, b);
  free(b);
  c->sc3d = sc;
  return c->t_3d;
}

static void tex_unit(GLuint prog, int unit, const char *name, GLuint tex) {
  GL(ActiveTexture)(GL_TEXTURE0 + unit);
  GL(BindTexture)(GL_TEXTURE_2D, tex);
  GL(Uniform1i)(gpu_gl_uniform(prog, name), unit);
}

/* pass C's uniforms for the frame of s (have3d, smaa: a 3D layer is given, with SMAA's weights) */
static void uni_compose(GpuU *u, const G07 *c, VCoreState *s, int sc, int have3d, int smaa) {
  int scale = c->scale;
  u32 dispcnt = vc_eng32(s, VC_ENG_A, VC_REG_DISPCNT);
  u16 bld = vc_eng16(s, VC_ENG_A, VC_REG_BLDCNT), alpha = vc_eng16(s, VC_ENG_A, VC_REG_BLDALPHA);
  uint32_t prios = 0;
  for (int n = 0; n < 4; n++) prios |= (uint32_t)(vc_eng16(s, VC_ENG_A, VC_REG_BG0CNT + 2 * n) & 3) << (2 * n);
  int eva = alpha & 0x1F, evb = (alpha >> 8) & 0x1F, evy = vc_eng16(s, VC_ENG_A, VC_REG_BLDY) & 0x1F;
  memset(u, 0, sizeof *u);
  u->cSmaaOn = have3d && smaa;
  u->uHiPrec = c->hiprec && have3d && sc > 1 ? 2u : 0u;
  u->cScale = scale;
  u->cScale3d = sc;
  u->cHave3d = have3d != 0;
  u->cCompose = (uint32_t)composed(s);
  u->cATop = (vc_io16(s, VC_REG_POWCNT1) >> 15) & 1;
  u->cObjOn = (dispcnt >> 12) & 1;
  u->cPrios = prios;
  u->cBld = bld;
  u->cEva = eva > 16 ? 16 : eva;
  u->cEvb = evb > 16 ? 16 : evb;
  u->cEvy = evy > 16 ? 16 : evy;
  u->cBackdrop = vc_555_to_rgb6(vc_pal(s, VC_ENG_A, 0, 0));
  u->cHofs = vc_eng16(s, VC_ENG_A, VC_REG_BG0HOFS) & 0x1FF;
  u->cBright = vc_eng16(s, VC_ENG_A, VC_REG_MASTER_BRIGHT);
  if (c->cap.bg != 0) {
    u->cCapBg = c->cap.bg, u->cCapOx = c->cap.ox, u->cCapOy = c->cap.oy, u->cCapY0 = c->cap.y0, u->cCapY1 = c->cap.y1;
    u->cCapSer = c->cap.ser;
  }
}

int g07_compose(G07 *c, VCoreState *s, const VC_Line *stage, unsigned tex3d, int sc, unsigned smaa) {
  if (stage == NULL) {
    if (c->own == NULL) c->own = malloc(sizeof(VC_Line) * VC_H);
    if (c->own == NULL) return -1;
    g07_stage_build(s, c->own);
    stage = c->own;
  }
  if (c->pack == NULL && (c->pack = g07_pack_create()) == NULL) return -1;
  g07_pack(c->pack, s, stage);
  return g07_compose_packed(c, s, c->pack, tex3d, sc, smaa);
}

/* 1 when the plan is to be made (again) for engine A's stage of pack serial aserial and the uniforms it reads; engine A
 * composed only */
static int plan_stale(G07 *c, const GpuU *u, unsigned serial) {
  if (!u->cCompose) return 0;
  uint32_t key[9] = {u->cObjOn, u->cPrios, u->cBld, (uint32_t)u->cEva, (uint32_t)u->cEvb, (uint32_t)u->cEvy, u->cBackdrop,
                     u->cBright, u->cCapBg};
  if (serial == c->plan_serial && !memcmp(key, c->plan_key, sizeof key)) return 0;
  c->plan_serial = serial;
  memcpy(c->plan_key, key, sizeof key);
  return 1;
}

void g07_set_hiprec(G07 *c, int on) { c->hiprec = on != 0; }
void g07_set_capbg(G07 *c, unsigned tex, int bg, int ox, int oy, int y0, int y1, unsigned ser) {
  c->cap.bg = tex != 0 && y1 > y0 && (bg == 2 || bg == 3) ? (uint32_t)bg : 0;
  c->cap.tex = c->cap.bg ? tex : 0;
  c->cap.ox = ox, c->cap.oy = oy, c->cap.y0 = y0, c->cap.y1 = y1, c->cap.ser = ser;
}
void g07_set_up2d(G07 *c, int style) { c->up2d = style > G07_UP_OFF && style < G07_UP_N ? style : G07_UP_OFF; }

int g07_up2d_parse(const char *name) {
  static const char *const names[G07_UP_N] = {"off", "identity", "scale2x", "mmpx"};
  for (int i = 0; name != NULL && i < G07_UP_N; i++)
    if (!strcmp(name, names[i])) return i;
  return -1;
}

/* the 2D filter's style in force (0: Off: at 1x, Metal (not yet), or its programs failed), its programs built */
static int up_style(G07 *c) {
  int st = c->up2d;
  if (st == G07_UP_OFF || c->scale == 1 || c->mtl != NULL || c->up_failed[st]) return G07_UP_OFF;
  if (c->up_prog[st][1]) return st;
  static const char *const defs[G07_UP_N] = {"", "UP_IDENTITY", "UP_SCALE2X", "UP_MMPX"};
  char err[2048] = "";
  /* (static: gpu_gl_program keeps the name pointers in its cache, so a style's names must outlive this call) */
  static char fa[G07_UP_N][64], fc[G07_UP_N][64];
  snprintf(fa[st], sizeof fa[st], "g11_up.frag#1+%s", defs[st]);
  snprintf(fc[st], sizeof fc[st], "g07_compose.frag#1+%s", defs[st]);
  c->up_prog[st][0] = gpu_gl_program("g00_full.vert", fa[st], err, sizeof err);
  if (c->up_prog[st][0]) c->up_prog[st][1] = gpu_gl_program("g00_full.vert", fc[st], err, sizeof err);
  /* MMPX at N >= 3 is MMPX twice (4x): a second 1x stage reads the first's info (shaders/g11_up_mmpx.glsl) */
  int two = st == G07_UP_MMPX && c->scale >= 3;
  if (two && c->up_prog[st][1]) c->up_prog[st][2] = gpu_gl_program("g00_full.vert", "g11_up.frag#1+UP_MMPX2", err, sizeof err);
  for (int i = 0; i < 1 + two && c->up_prog[st][1] && (!two || c->up_prog[st][2]); i++) {
    GLuint *t = i ? &c->t_info1 : &c->t_info, *fb = i ? &c->fbo_info1 : &c->fbo_info;
    if (*t) continue;
    *t = make_tex(GL_RGBA32UI, VC_W, VC_H * 2);
    GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA32UI, VC_W, VC_H * 2, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
    GL(GenFramebuffers)(1, fb);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, *fb);
    GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *t, 0);
    if (GL(CheckFramebufferStatus)(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) snprintf(err, sizeof err, "info framebuffer");
    GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  }
  if (two && !c->up_prog[st][2] && !err[0]) snprintf(err, sizeof err, "second stage");
  if (!c->up_prog[st][1] || err[0]) {
    fprintf(stderr, "gpu: 2D filter %s: %s (drawn Off)\n", defs[st], err);
    c->up_failed[st] = 1;
    return G07_UP_OFF;
  }
  return st;
}

/* pass C's N x program for the frame: Off's, or the style's after its analysis (made again when the plan was, the pack
 * changed, or the style or what the key reads changed; in-betweens reuse it) */
static GLuint up_prepare(G07 *c, const GpuU *u, unsigned serial, int plan) {
  int st = up_style(c);

  if (st == G07_UP_OFF) return c->prog;
  uint32_t key[3] = {(uint32_t)st, u->cCompose, u->cHave3d};
  if (plan || serial != c->info_serial || memcmp(key, c->info_key, sizeof key)) {
    GLenum buf = GL_COLOR_ATTACHMENT0;
    const int two = c->up_prog[st][2] != 0;
    GL(BindFramebuffer)(GL_FRAMEBUFFER, two ? c->fbo_info1 : c->fbo_info);
    GL(DrawBuffers)(1, &buf);
    gpu_gl_invalidate(1);
    GL(Viewport)(0, 0, VC_W, VC_H * 2);
    GL(UseProgram)(c->up_prog[st][0]);
    tex_unit(c->up_prog[st][0], 0, "uUpPlan", c->t_plan);
    tex_unit(c->up_prog[st][0], 1, "uUpSrc", c->t_src);
    gpu_gl_uniforms(u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
    if (two) { /* the second stage, from the first's info */
      GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_info);
      GL(DrawBuffers)(1, &buf);
      gpu_gl_invalidate(1);
      GL(UseProgram)(c->up_prog[st][2]);
      tex_unit(c->up_prog[st][2], 2, "uUpInfo1", c->t_info1);
      gpu_gl_uniforms(u);
      GL(DrawArrays)(GL_TRIANGLES, 0, 3);
    }
    c->info_serial = serial;
    memcpy(c->info_key, key, sizeof key);
  }
  return c->up_prog[st][1];
}

/* the two screens' keys for a compose of pack k (ok 0: not to be reused, a 3D layer without a number) */
static void reuse_keys(const G07 *c, const G07Pack *k, const GpuU *u, unsigned tex3d, int sc, unsigned smaa, G07Key key[2], int ok[2]) {
  G07Key a, b;
  memset(&a, 0, sizeof a);
  memset(&b, 0, sizeof b);
  a.up = b.up = up_style((G07 *)c);
  a.eng = 0, a.sc = sc, a.aserial = k->aserial, a.id3d = tex3d ? c->id3d : 0, a.tex3d = tex3d, a.smaa = smaa;
  memcpy(&a.u, u, sizeof *u);
  int okA = tex3d == 0 || c->id3d != 0, okB = 1;
  /* engine B's screen: its 1x lines where they go and the filter's style (pass C reads nothing else for it; the filter's analysis
   * judges each screen on its own lines: g11_up.glsl's up_nb) */
  b.eng = 1, b.bserial = k->bserial;
  b.u.cATop = u->cATop, b.u.cScale = u->cScale, b.u.uHiPrec = u->uHiPrec;
  int at = u->cATop ? 0 : 1; /* the half engine A shows in */
  key[at] = a, ok[at] = okA;
  key[at ^ 1] = b, ok[at ^ 1] = okB;
}

void g07_set_reuse(G07 *c, int mode) {
  c->reuse = mode;
  c->key_ok[0] = c->key_ok[1] = 0;
}
void g07_set_3d_id(G07 *c, unsigned id) { c->id3d = id; }
/* (g07_out_gen: one count for every G07, so that no number is given to two contents) */
static unsigned long long sOutGen;
unsigned long long g07_out_gen(const G07 *c) { return c->out_gen; }
void g07_forget(G07 *c) {
  c->key_ok[0] = c->key_ok[1] = 0;
  c->out_gen = ++sOutGen;
}
void g07_reuse_counts(const G07 *c, unsigned long long n[5]) {
  n[0] = c->n_comp, n[1] = c->n_half, n[2] = c->n_none, n[3] = c->n_checked, n[4] = c->n_differ;
}

static int compose_draw(G07 *c, const G07Pack *k, unsigned tex3d, unsigned smaa, GpuU *u, int rows);

int g07_compose_packed(G07 *c, VCoreState *s, const G07Pack *k, unsigned tex3d, int sc, unsigned smaa) {
  GpuU u;
  uni_compose(&u, c, s, sc, tex3d != 0, smaa != 0);
  G07Key key[2];
  int ok[2], rows = 3; /* the halves to draw: bit 0 the top, bit 1 the bottom */
  if (c->reuse) {
    reuse_keys(c, k, &u, tex3d, sc, smaa, key, ok);
    rows = 0;
    for (int h = 0; h < 2; h++)
      if (!ok[h] || !c->key_ok[h] || memcmp(&key[h], &c->key[h], sizeof key[h]) != 0) rows |= 1 << h;
    c->n_comp++;
    c->n_half += rows == 1 || rows == 2;
    c->n_none += rows == 0;
  }
  c->id3d = 0;
  const int kept = 3 & ~rows;
  if (c->reuse == 2) rows = 3; /* check: drawn whole, the kept halves compared below */
  if (rows == 0) return 0;     /* the output is this picture already */
  c->out_gen = ++sOutGen;
  int rc = compose_draw(c, k, tex3d, smaa, &u, rows);
  if (rc != 0) {
    c->key_ok[0] = c->key_ok[1] = 0;
    return rc;
  }
  if (c->reuse == 2) {
    size_t half = (size_t)c->w * (c->h / 2) * 4;
    if (c->chk[0] == NULL) c->chk[0] = calloc(2, half), c->chk[1] = calloc(2, half);
    if (c->chk[0] != NULL && c->chk[1] != NULL) {
      g07_read_frame(c, c->chk[1]);
      for (int h = 0; h < 2; h++)
        if (kept & (1 << h)) {
          c->n_checked++;
          if (memcmp(c->chk[0] + h * half, c->chk[1] + h * half, half) != 0 && ++c->n_differ <= 20)
            fprintf(stderr, "gpu: pass C reuse check: the %s screen drawn again differs\n", h ? "bottom" : "top");
        }
      uint8_t *t = c->chk[0];
      c->chk[0] = c->chk[1], c->chk[1] = t;
    }
  }
  if (c->reuse)
    for (int h = 0; h < 2; h++)
      if (rows & (1 << h)) c->key[h] = key[h], c->key_ok[h] = ok[h];
  return 0;
}

/* pack k's stage and 1x lines into pass C's textures (GL), once a pack: by pass C, or by the capture's source A when pass C was
 * reused (g07_compose_packed drew nothing) */
static void pack_up(G07 *c, const G07Pack *k) {
  if (k->serial == c->up) return; /* (the textures keep their size: sub-image uploads) */
  GPU_PASS(GPU_PASS_UPLOAD, 1);
  /* through one unpack buffer (docs/gpu-cost.md P3): the driver converts it to the textures' layout on the GPU, not
   * on this thread as it does client memory */
  const size_t nstage = sizeof k->stage, nsrc = sizeof k->src;
  const void *stage = k->stage, *src = k->src;
  unsigned char *m = gpu_gl_unpack_map(nstage + nsrc);
  if (m != NULL) {
    memcpy(m, k->stage, nstage);
    memcpy(m + nstage, k->src, nsrc);
    if (gpu_gl_unpack_unmap() == 0) stage = (const void *)0, src = (const void *)nstage;
  }
  GL(PixelStorei)(GL_UNPACK_ALIGNMENT, 4);
  GL(BindTexture)(GL_TEXTURE_2D, c->t_stage);
  GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, VC_W, VC_H, GL_RGBA_INTEGER, GL_UNSIGNED_INT, stage);
  GL(BindTexture)(GL_TEXTURE_2D, c->t_src);
  GL(TexSubImage2D)(GL_TEXTURE_2D, 0, 0, 0, VC_W, VC_H * 2, GL_RED_INTEGER, GL_UNSIGNED_INT, src);
  gpu_gl_unpack_done();
  GPU_PASS(GPU_PASS_UPLOAD, 0);
  c->up = k->serial;
}

/* pass C for the halves in rows (3: the whole output) */
static int compose_draw(G07 *c, const G07Pack *k, unsigned tex3d, unsigned smaa, GpuU *u0, int rows) {
  GpuU u = *u0;
  int plan = plan_stale(c, &u, k->aserial);
  if (c->mtl != NULL) return gpu_mtl_compose(c->mtl, k->stage, k->src, k->serial, plan, tex3d, smaa, c->cap.tex, &u, rows);
  pack_up(c, k);

  GLenum buf = GL_COLOR_ATTACHMENT0;
  GPU_PASS(GPU_PASS_C, 1);
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  GL(BindVertexArray)(c->vao);
  if (plan) { /* the plan from the 2D layers, at 1x (a frame's in-betweens reuse it) */
    GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_plan);
    GL(DrawBuffers)(1, &buf);
    gpu_gl_invalidate(1);
    GL(Viewport)(0, 0, VC_W, VC_H);
    GL(UseProgram)(c->prog_plan);
    tex_unit(c->prog_plan, 0, "cStage", c->t_stage);
    gpu_gl_uniforms(&u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  GLuint prog = up_prepare(c, &u, k->serial, plan);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo);
  GL(DrawBuffers)(1, &buf);
  if (rows == 3) gpu_gl_invalidate(1); /* overwritten whole */
  GL(Viewport)(0, 0, VC_W * c->scale, VC_H * 2 * c->scale);
  if (rows != 3) { /* one screen: the other keeps its pixels */
    GL(Enable)(GL_SCISSOR_TEST);
    GL(Scissor)(0, rows == 1 ? 0 : VC_H * c->scale, VC_W * c->scale, VC_H * c->scale);
  }
  GL(UseProgram)(prog);
  tex_unit(prog, 0, "cPlan", c->t_plan);
  tex_unit(prog, 1, "cSrc1x", c->t_src);
  tex_unit(prog, 2, "c3d", tex3d ? tex3d : c->t_stage); /* a bound texture even when unused */
  tex_unit(prog, 3, "cSmaa", tex3d && smaa ? smaa : c->t_none);
  if (prog != c->prog) tex_unit(prog, 4, "cUpInfo", c->t_info);
  tex_unit(prog, 5, "cCapBank", c->cap.tex ? c->cap.tex : c->t_src); /* (bound when unread, as c3d) */
  gpu_gl_uniforms(&u);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  if (rows != 3) GL(Disable)(GL_SCISSOR_TEST);
  GPU_PASS(GPU_PASS_C, 0);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  GLenum e = GL(GetError)();
  return e == GL_NO_ERROR ? 0 : -(int)e;
}

unsigned g07_compose_capsrc(G07 *c, VCoreState *s, const G07Pack *k, unsigned tex3d, int sc, unsigned smaa) {
  const int h = VC_H * c->scale;
  if (c->mtl != NULL) { /* (gpu_mtl_capsrc uploads pack k when pass C was reused: g07_compose_packed drew nothing) */
    GpuU u;
    uni_compose(&u, c, s, sc, tex3d != 0, smaa != 0);
    u.cCompose = 1, u.cATop = 1, u.cBright = 0; /* (as below) */
    return gpu_mtl_capsrc(c->mtl, k->stage, k->src, k->serial, plan_stale(c, &u, k->aserial), tex3d, smaa, c->cap.tex, &u);
  }
  pack_up(c, k); /* (pass C reused for this frame left its textures at an older pack) */
  if (c->t_cap == 0) {
    c->t_cap = make_tex(GL_RGBA8, c->w, h);
    GL(TexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, c->w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GL(GenFramebuffers)(1, &c->fbo_cap);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_cap);
    GL(FramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, c->t_cap, 0);
    GLenum st = GL(CheckFramebufferStatus)(GL_FRAMEBUFFER);
    GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
    if (st != GL_FRAMEBUFFER_COMPLETE) return 0;
  }
  GpuU u;
  uni_compose(&u, c, s, sc, tex3d != 0, smaa != 0);
  u.cCompose = 1; /* the compositor's picture whatever engine A displays, on top (the output's first 192 N rows) */
  u.cATop = 1;
  u.cBright = 0; /* a capture is taken before master brightness */
  int plan = plan_stale(c, &u, k->aserial); /* (the next frame's pass C plans again when its master brightness is not 0) */
  GLenum buf = GL_COLOR_ATTACHMENT0;
  GL(Disable)(GL_DEPTH_TEST);
  GL(Disable)(GL_BLEND);
  GL(BindVertexArray)(c->vao);
  if (plan) {
    GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_plan);
    GL(DrawBuffers)(1, &buf);
    gpu_gl_invalidate(1);
    GL(Viewport)(0, 0, VC_W, VC_H);
    GL(UseProgram)(c->prog_plan);
    tex_unit(c->prog_plan, 0, "cStage", c->t_stage);
    gpu_gl_uniforms(&u);
    GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  }
  GLuint prog = up_prepare(c, &u, k->serial, plan); /* (the same style as the frame: a capture shows what the screen did) */
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_cap);
  GL(DrawBuffers)(1, &buf);
  gpu_gl_invalidate(1);
  GL(Viewport)(0, 0, c->w, h);
  GL(UseProgram)(prog);
  tex_unit(prog, 0, "cPlan", c->t_plan);
  tex_unit(prog, 1, "cSrc1x", c->t_src);
  tex_unit(prog, 2, "c3d", tex3d ? tex3d : c->t_stage);
  tex_unit(prog, 3, "cSmaa", tex3d && smaa ? smaa : c->t_none);
  if (prog != c->prog) tex_unit(prog, 4, "cUpInfo", c->t_info);
  tex_unit(prog, 5, "cCapBank", c->cap.tex ? c->cap.tex : c->t_src);
  gpu_gl_uniforms(&u);
  GL(DrawArrays)(GL_TRIANGLES, 0, 3);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  return GL(GetError)() == GL_NO_ERROR ? c->t_cap : 0;
}

unsigned g07_frame_tex(const G07 *c) { return c->mtl != NULL ? gpu_mtl_c_frame_tex(c->mtl) : c->t_out; }
double g07_gpu_ms(const G07 *c) { return c->mtl != NULL ? gpu_mtl_c_gpu_ms(c->mtl) : 0; }

int g07_up_inputs(G07 *c, VCoreState *s, const G07Pack *k, int sc, int have3d, G07UpIn *in, uint32_t *plan, uint32_t *src) {
  if (c->mtl != NULL) return -1;
  GpuU u;
  uni_compose(&u, c, s, sc, have3d, 0);
  in->scale = c->scale, in->compose = (int)u.cCompose, in->atop = (int)u.cATop, in->have3d = have3d != 0;
  in->hofs = u.cHofs, in->evy = u.cEvy, in->bright = u.cBright;
  memcpy(src, k->src, sizeof k->src);
  memset(plan, 0, sizeof(uint32_t) * VC_W * VC_H * 2);
  if (!u.cCompose) return 0;
  uint32_t *t = malloc(sizeof(uint32_t) * VC_W * VC_H * 4);
  if (t == NULL) return -1;
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo_plan);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(ReadPixels)(0, 0, VC_W, VC_H, GL_RGBA_INTEGER, GL_UNSIGNED_INT, t);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
  for (int i = 0; i < VC_W * VC_H; i++) plan[2 * i] = t[4 * i], plan[2 * i + 1] = t[4 * i + 1];
  free(t);
  return GL(GetError)() == GL_NO_ERROR ? 0 : -1;
}

void g07_read_frame(G07 *c, uint8_t *rgba) {
  if (c->mtl != NULL) {
    gpu_mtl_c_read(c->mtl, rgba);
    return;
  }
  GL(PixelStorei)(GL_PACK_ALIGNMENT, 4);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, c->fbo);
  GL(ReadBuffer)(GL_COLOR_ATTACHMENT0);
  GL(ReadPixels)(0, 0, c->w, c->h, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
  GL(BindFramebuffer)(GL_FRAMEBUFFER, 0);
}
