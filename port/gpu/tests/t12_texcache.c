/* port/gpu G12's unit tests, test 1 of the texture cache design: the decode and the layer layout against vcore p17's texel
 * (vc_tex_sample_c, both its fast path and its slow one), and test 3: the cache's invalidation (g12_texcache.c) on synthetic
 * frame sequences. A plain main (exit 1 on a failure): build/gpu/gpu_t12.
 *   gpu_t12                run the synthetic cases (every format, size, wrap, flip, clamp, bit 29, palette bases)
 *   gpu_t12 FILE.vcs...    also the textures of every polygon of these captures (the flattened GpuTexData as the GPU gets it)
 * OWNED BY G12. */
#include "g04_texture.h"
#include "g12_tcdecode.h"
#include "g12_texcache.h"
#include "gpu.h"
#include "vc_shared.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the J1 fill (8 full passes), kept as the reference the frontier fill must equal byte for byte */
static void tc_fill_ref(u8 *layer, int S) {
  enum { NMAX = G12_MAX_BUCKET * G12_MAX_BUCKET };
  static u8 fl[NMAX], col[NMAX][3]; /* (not thread-safe: one worker decodes) */
  static size_t nx[NMAX];
  size_t n = (size_t)S * S, cnt = 0, todo = 0;
  u32 sum[3] = {0, 0, 0};
  for (size_t i = 0; i < n; i++) {
    fl[i] = layer[i * 4 + 3] != 0;
    if (fl[i]) {
      cnt++;
      for (int k = 0; k < 3; k++) sum[k] += layer[i * 4 + k];
    } else todo++;
  }
  if (cnt == 0 || todo == 0) return;
  for (int pass = 0; pass < 8 && todo; pass++) {
    size_t nf = 0;
    for (int y = 0; y < S; y++)
      for (int x = 0; x < S; x++) {
        size_t i = (size_t)y * S + x;
        if (fl[i]) continue;
        u32 c[3] = {0, 0, 0}, m = 0;
        for (int dy = -1; dy <= 1; dy++)
          for (int dx = -1; dx <= 1; dx++) {
            if (!dx && !dy) continue;
            size_t j = (size_t)((y + dy + S) % S) * S + (size_t)((x + dx + S) % S);
            if (!fl[j]) continue;
            m++;
            for (int k = 0; k < 3; k++) c[k] += layer[j * 4 + k];
          }
        if (m) {
          for (int k = 0; k < 3; k++) col[i][k] = (u8)((c[k] + m / 2) / m);
          nx[nf++] = i;
        }
      }
    for (size_t q = 0; q < nf; q++) {
      memcpy(layer + nx[q] * 4, col[nx[q]], 3);
      fl[nx[q]] = 1;
    }
    todo -= nf;
  }
  if (todo) {
    u8 mean[3];
    for (int k = 0; k < 3; k++) mean[k] = (u8)((sum[k] + cnt / 2) / cnt);
    for (size_t i = 0; i < n; i++)
      if (!fl[i]) memcpy(layer + i * 4, mean, 3);
  }
}


static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); fails++; } } while (0)

static u64 rs = 0x9E3779B97F4A7C15ull;
static u32 rnd(void) {
  rs ^= rs << 13;
  rs ^= rs >> 7;
  rs ^= rs << 17;
  return (u32)(rs >> 16);
}

static u8 g_tex[G12_TEX_BYTES], g_pal[G12_PAL_BYTES];
static u8 img[1024 * 1024 * 4], img2[1024 * 1024 * 4], lay[512 * 512 * 4], lay2[512 * 512 * 4], lay3[512 * 512 * 4];
static long n_cmp, n_differ, n_cfg, n_fill_diff, n_fill_rnd;

static void bind(VCoreState *s, const u8 *tex, const u8 *pal) {
  VC_Region *t = &s->rgn[VC_RGN_TEX], *p = &s->rgn[VC_RGN_TEXPAL];
  memset(t, 0, sizeof *t);
  memset(p, 0, sizeof *p);
  t->npages = 32;
  for (int i = 0; i < 32; i++) {
    t->page[i].n = 1;
    t->page[i].src[0] = tex + (size_t)i * VC_PAGE_SIZE;
  }
  p->npages = 6;
  for (int i = 0; i < 6; i++) {
    p->page[i].n = 1;
    p->page[i].src[0] = pal + (size_t)i * VC_PAGE_SIZE;
  }
}

static void texel_ref(u32 v, u8 *o) { /* vcore's texel (RGB6 | a << 24) as r6 g6 b6 a5 */
  o[0] = (u8)(v & 255);
  o[1] = (u8)(v >> 8 & 255);
  o[2] = (u8)(v >> 16 & 255);
  o[3] = (u8)(v >> 24);
}

static int axis_rep(u32 tp, int ax) { return tp >> (16 + ax) & 1; }
static int axis_flip(u32 tp, int ax) { return tp >> (18 + ax) & 1; }
static int pmod(int v, int m) { return ((v % m) + m) % m; }

/* the decode (w x h, img) against vcore, texel for texel, over the whole image (every position, plus out-of-range ones) */
static void check_decode(VCoreState *s, const u8 *tex, const u8 *pal, u32 tp, u32 pltt, u8 *im) {
  VC_Poly p;
  memset(&p, 0, sizeof p);
  p.texparam = tp;
  p.pltt = pltt;
  VC_TexCtx c, c2;
  vc_tex_ctx_init(&c, s, &p);
  vc_tex_ctx_init(&c2, s, &p);
  c2.fast = 0; /* the slow path (tex_sample_slow) too */
  int w = g12_tc_width(tp), h = g12_tc_height(tp);
  CHECK(g12_tc_decode(tex, pal, tp, pltt, im, NULL) == 0);
  /* a texture's texels may be walked entirely when small, else every Kth */
  int step = (w * h > 65536) ? 7 : 1;
  for (int y = 0; y < h; y += (step > 1 ? 1 : 1))
    for (int x = (y * 3) % step; x < w; x += step) {
      u8 e[4], e2[4];
      u32 ts = (u32)x << 4 | (rnd() & 15), tt = (u32)y << 4 | (rnd() & 15);
      texel_ref(vc_tex_sample_c(&c, (s32)ts, (s32)tt), e);
      texel_ref(vc_tex_sample_c(&c2, (s32)ts, (s32)tt), e2);
      n_cmp++;
      if (memcmp(e, e2, 4) || memcmp(e, im + ((size_t)y * w + x) * 4, 4)) {
        if (n_differ++ < 5) printf("decode differs: tp %08x pltt %x at %d,%d\n", tp, pltt, x, y);
      }
    }
}

/* the layer's texel at the position the shader's rule maps st (a texel coordinate, may be outside) == vcore's texel; and the
 * clamp axes' padding: every layer position from -(S-n)/2 (wrapping) to n + (S-n)/2 reads the edge texel */
static void check_layer(VCoreState *s, u32 tp, u32 pltt, const u8 *layer, int S, int nsamp) {
  VC_Poly p;
  memset(&p, 0, sizeof p);
  p.texparam = tp;
  p.pltt = pltt;
  VC_TexCtx c;
  vc_tex_ctx_init(&c, s, &p);
  int n[2] = {g12_tc_width(tp), g12_tc_height(tp)};
  for (int k = 0; k < nsamp; k++) {
    int v[2], L[2];
    for (int a = 0; a < 2; a++) {
      v[a] = (int)(rnd() % (u32)(5 * n[a])) - 2 * n[a];
      if (k < 40) v[a] = (k & (1 << a)) ? 3 * n[a] - 1 - (k >> 2) % 3 : -2 * n[a] + (k >> 2) % 3; /* the corners */
      /* the shader: a clamp axis's coordinate is clamped to [0, n] (a footprint's centre stays on the texture); the others
       * wrap on the layer (the sampler's REPEAT) */
      L[a] = axis_rep(tp, a) ? pmod(v[a], S) : (v[a] < 0 ? 0 : v[a] > n[a] ? n[a] : v[a]);
    }
    u8 e[4];
    texel_ref(vc_tex_sample_c(&c, v[0] << 4 | (int)(rnd() & 15), v[1] << 4 | (int)(rnd() & 15)), e);
    n_cmp++;
    if (memcmp(e, layer + ((size_t)L[1] * S + L[0]) * 4, 4)) {
      if (n_differ++ < 5) printf("layer differs: tp %08x at st %d,%d (layer %d,%d S %d)\n", tp, v[0], v[1], L[0], L[1], S);
    }
  }
  for (int a = 0; a < 2; a++) {
    if (axis_rep(tp, a)) continue;
    int pad = (S - n[a]) / 2;
    for (int u = -pad; u < n[a] + pad; u++) {
      int o = (int)(rnd() % (u32)n[1 - a]); /* the other axis: an in-range texel */
      int cu = u < 0 ? 0 : u >= n[a] ? n[a] - 1 : u;
      int pos[2], ref[2];
      pos[a] = pmod(u, S);
      pos[1 - a] = o;
      ref[a] = cu;
      ref[1 - a] = o;
      u8 e[4];
      texel_ref(vc_tex_sample_c(&c, ref[0] << 4, ref[1] << 4), e);
      n_cmp++;
      if (memcmp(e, layer + ((size_t)pos[1] * S + pos[0]) * 4, 4)) {
        if (n_differ++ < 5) printf("padding differs: tp %08x axis %d u %d S %d\n", tp, a, u, S);
      }
    }
  }
}

static u32 mk_tp(int fmt, int sf, int tf, u32 wrap, int t0, u32 base8) {
  return (base8 & 0xFFFF) | (wrap << 16) | (u32)sf << 20 | (u32)tf << 23 | (u32)fmt << 26 | (u32)t0 << 29;
}

static void fill_random(void) {
  for (size_t i = 0; i < sizeof g_tex; i++) g_tex[i] = (u8)rnd();
  for (size_t i = 0; i < sizeof g_pal; i++) g_pal[i] = (u8)rnd();
  /* a palette with plenty of colour-0 texels / index-0 and alpha-0 entries: make some data zero */
  for (size_t i = 0; i < sizeof g_tex; i += 5) g_tex[i] = 0;
}

static void t_synthetic(void) {
  VCoreState *s = vcore_create();
  fill_random();
  bind(s, g_tex, g_pal);
  for (int fmt = 1; fmt <= 7; fmt++)
    for (int sf = 0; sf < 7; sf++)
      for (int tf = 0; tf < 7; tf++)
        for (int t0 = 0; t0 < 2; t0++) {
          u32 base8 = (rnd() % 0x10000);
          u32 pltt = rnd() & 0x1FFF;
          if ((fmt == 4 || fmt == 3) && (rnd() & 3) == 0) pltt = 0x1800 + (rnd() & 0x7FF); /* palette slots 6-7 and the wrap */
          u32 tp0 = mk_tp(fmt, sf, tf, 0, t0, base8);
          if (fmt == 5 && g12_tc_width(tp0) * g12_tc_height(tp0) < 16) continue;
          check_decode(s, g_tex, g_pal, tp0, pltt, img);
          n_cfg++;
          for (u32 wrap = 0; wrap < 16; wrap++) {
            u32 tp = tp0 | wrap << 16;
            int S = g12_tc_layer_size(tp);
            if (!S) continue;
            CHECK(g12_tc_layout(img, tp, lay) == S);
            check_layer(s, tp, pltt, lay, S, 1500);
            /* the fill leaves every texel with alpha > 0 as it was, and alpha everywhere */
            memcpy(lay2, lay, (size_t)S * S * 4);
            memcpy(lay3, lay, (size_t)S * S * 4);
            tc_fill_ref(lay3, S);
            g12_tc_fill(lay2, S);
            if (memcmp(lay2, lay3, (size_t)S * S * 4)) { CHECK(0); n_fill_diff++; }
            for (int i = 0; i < S * S; i++) {
              if (lay[i * 4 + 3] != lay2[i * 4 + 3]) { CHECK(0); break; }
              if (lay[i * 4 + 3] && memcmp(lay + i * 4, lay2 + i * 4, 4)) { CHECK(0); break; }
            }
            n_cfg++;
          }
        }
  /* the big sizes (1024: not cached, but the decode is exact) */
  for (int fmt = 1; fmt <= 7; fmt++) {
    u32 tp = mk_tp(fmt, 7, 7, 3, 1, rnd() % 0x10000);
    check_decode(s, g_tex, g_pal, tp, rnd() & 0x1FFF, img);
    CHECK(g12_tc_layer_size(tp) == 0);
    tp = mk_tp(fmt, 6, 3, 0, 1, rnd() % 0x10000);
    CHECK(g12_tc_layer_size(tp) == 1024 / 2 || g12_tc_layer_size(tp) == 0);
  }
  vcore_destroy(s);
}

/* the decode's ranges cover every byte it read: with everything else rubbed out the image is the same */
static void t_ranges(void) {
  VCoreState *s = vcore_create();
  static u8 t2[G12_TEX_BYTES], p2[G12_PAL_BYTES];
  fill_random();
  bind(s, g_tex, g_pal);
  for (int k = 0; k < 4000; k++) {
    int fmt = 1 + (int)(rnd() % 7);
    u32 tp = mk_tp(fmt, (int)(rnd() % 5), (int)(rnd() % 5), 0, (int)(rnd() & 1), rnd() % 0x10000);
    u32 pltt = rnd() & 0x1FFF;
    G12Ranges r;
    int w = g12_tc_width(tp), h = g12_tc_height(tp);
    if (fmt == 5 && w * h < 16) continue;
    CHECK(g12_tc_decode(g_tex, g_pal, tp, pltt, img, &r) == 0);
    memset(t2, 0x5A, sizeof t2);
    memset(p2, 0xA5, sizeof p2);
    if (r.tex_hi > r.tex_lo) memcpy(t2 + r.tex_lo, g_tex + r.tex_lo, r.tex_hi - r.tex_lo);
    if (r.idx_hi > r.idx_lo) memcpy(t2 + r.idx_lo, g_tex + r.idx_lo, r.idx_hi - r.idx_lo);
    if (r.pal_hi > r.pal_lo) {
      u32 hi = r.pal_hi > G12_PAL_BYTES ? G12_PAL_BYTES : r.pal_hi;
      if (hi > r.pal_lo) memcpy(p2 + r.pal_lo, g_pal + r.pal_lo, hi - r.pal_lo);
    }
    CHECK(g12_tc_decode(t2, p2, tp, pltt, img2, NULL) == 0);
    n_cmp++;
    if (memcmp(img, img2, (size_t)w * h * 4)) {
      n_differ++;
      printf("ranges short: fmt %d tp %08x pltt %x tex %x-%x idx %x-%x pal %x-%x\n", fmt, tp, pltt, r.tex_lo, r.tex_hi, r.idx_lo,
             r.idx_hi, r.pal_lo, r.pal_hi);
    }
    if (fmt != 5) CHECK(r.idx_hi == r.idx_lo);
  }
  vcore_destroy(s);
}

static void t_fill(void) {
  /* a white opaque texel among transparent black ones: the whole layer's colour is white, alpha stays */
  static u8 L[32 * 32 * 4];
  memset(L, 0, sizeof L);
  L[(5 * 32 + 7) * 4 + 3] = 31;
  L[(5 * 32 + 7) * 4 + 0] = L[(5 * 32 + 7) * 4 + 1] = L[(5 * 32 + 7) * 4 + 2] = 63;
  g12_tc_fill(L, 32);
  int bad = 0;
  for (int i = 0; i < 32 * 32; i++) {
    if (L[i * 4] != 63 || L[i * 4 + 1] != 63 || L[i * 4 + 2] != 63) bad++;
    if (i != 5 * 32 + 7 && L[i * 4 + 3] != 0) bad++;
  }
  CHECK(bad == 0);
  /* two opaque neighbours (red 40,0,0 and blue 0,0,20): the texel between them is their mean, outward ones too */
  memset(L, 0, sizeof L);
  L[(10 * 32 + 10) * 4 + 0] = 40; L[(10 * 32 + 10) * 4 + 3] = 31;
  L[(10 * 32 + 12) * 4 + 2] = 20; L[(10 * 32 + 12) * 4 + 3] = 31;
  g12_tc_fill(L, 32);
  CHECK(L[(10 * 32 + 11) * 4 + 0] == 20 && L[(10 * 32 + 11) * 4 + 2] == 10);
  CHECK(L[(10 * 32 + 10) * 4 + 0] == 40 && L[(10 * 32 + 12) * 4 + 2] == 20);
  /* no opaque texel: untouched */
  memset(L, 0, sizeof L);
  L[3] = 0; L[0] = 9;
  g12_tc_fill(L, 32);
  CHECK(L[0] == 9);
  /* the RGBA8 form is lossless for level 0 */
  for (int v = 0; v < 64; v++) {
    u8 raw[4] = {(u8)v, (u8)v, (u8)v, (u8)(v & 31)}, o[4];
    g12_tc_rgba8(raw, 1, o);
    CHECK((int)roundf(o[0] / 255.0f * 63.0f) == v && (int)roundf(o[3] / 255.0f * 31.0f) == (v & 31));
  }
}

/* ---- test 3: invalidation (g12_texcache.c), synthetic frame sequences ---- */
static VC_GeomOut tc_geo;
static u8 tc_tex[G12_TEX_BYTES], tc_pal[G12_PAL_BYTES];

static u32 tparam(int fmt, int ss, int st, u32 addr, u32 flags) {
  return (addr / 8) | (u32)ss << 20 | (u32)st << 23 | (u32)fmt << 26 | flags;
}
static void tc_polys(int n, const u32 *tp, const u32 *pl) {
  tc_geo.npoly = (u32)n;
  for (int i = 0; i < n; i++) {
    tc_geo.order[i] = (u16)(n - 1 - i); /* the use rows follow the drawing order */
    tc_geo.poly[n - 1 - i].texparam = tp[i];
    tc_geo.poly[n - 1 - i].pltt = pl[i];
  }
}
static G12Frame tc_run(G12Cache *c) {
  G12Frame f;
  g12_frame(c, tc_tex, tc_pal, &tc_geo, 0, &f);
  return f;
}

/* an upload's staging bytes == the layer of a fresh decode */
static int tc_upload_ok(const G12Frame *f, u32 tp, u32 pltt) {
  int S = g12_tc_layer_size(tp);
  g12_tc_decode(tc_tex, tc_pal, tp, pltt, img, NULL);
  g12_tc_layout(img, tp, lay);
  g12_tc_rgba8(lay, S * S, lay2);
  for (u32 k = 0; k < f->nup; k++)
    if (f->up[k].size == S && !memcmp(f->staging + f->up[k].offset, lay2, (size_t)S * S * 4)) return 1;
  return 0;
}

static void t_invalidation(void) {
  long fails0 = fails;
  /* xxHash64's published values */
  CHECK(g12_xxh64("", 0, 0) == 0xEF46DB3751D8E999ull);
  CHECK(g12_xxh64("abc", 3, 0) == 0x44BC2CF5AD770999ull);
  /* keys */
  u32 t16 = tparam(3, 2, 2, 0x1100, 0x30000);
  CHECK(g12_key(t16 | 0xC0000000u, 5) == g12_key(t16, 5));
  CHECK(g12_key(t16 | 1u << 29, 5) != g12_key(t16, 5));
  CHECK(g12_key(tparam(7, 2, 2, 0, 1u << 29), 5) == g12_key(tparam(7, 2, 2, 0, 0), 9));
  CHECK(g12_key(tparam(1, 2, 2, 0, 1u << 29), 5) == g12_key(tparam(1, 2, 2, 0, 0), 5));
  CHECK(g12_key(t16, 5) != g12_key(t16, 6) && g12_key(0, 3) == 0);

  for (int check = 0; check < 2; check++) {
    for (size_t i = 0; i < sizeof tc_tex; i++) tc_tex[i] = (u8)rnd();
    for (size_t i = 0; i < sizeof tc_pal; i++) tc_pal[i] = (u8)rnd();
    G12Options o = {0};
    o.check = check;
    for (int k = 0; k < 64; k++) { /* B's block words: modes 0-3, palette offsets 0-3 words (its palette range stays in chunk 1) */
      tc_tex[0x21000 + 2 * k] = (u8)(k & 3);
      tc_tex[0x21000 + 2 * k + 1] = (u8)((k & 3) << 6);
    }
    G12Cache *c = g12_create(&o);
    /* A: 16 colours 32 x 32 (512 bytes at 0x1100: chunks 8-9), palette 5 (bytes 0x50-0x6F); B: 4x4 compressed 32 x 32 at 0x2000
     * (its block words at 0x20000 + 0x1000); C: direct 1024 x 8 (never cached); untextured */
    u32 tA = t16, tB = tparam(5, 2, 2, 0x2000, 0x30000), tC = tparam(7, 7, 0, 0x40000, 0);
    u32 tp[4] = {tA, tB, tC, 0}, pl[4] = {5, 40, 0, 0};
    tc_polys(4, tp, pl);
    G12Frame f = tc_run(c);
    CHECK(f.st.news == 3 && f.st.decodes == 2 && f.st.big == 1 && f.nup == 2 && f.st.textures == 3);
    CHECK(f.use[0].bucket == 0 && f.use[0].size == 32 && f.use[0].why == G12_USE_READY && f.use[1].why == G12_USE_READY);
    CHECK(f.use[2].why == G12_USE_BIG && f.use[2].bucket == -1 && f.use[3].why == G12_USE_NOTEX);
    CHECK(f.use[0].layer != f.use[1].layer && f.grow_cap[0] == 64);
    CHECK(tc_upload_ok(&f, tA, 5) && tc_upload_ok(&f, tB, 40));
    /* the same bytes: hits, nothing dirty */
    f = tc_run(c);
    CHECK(f.st.hits == 2 && f.st.decodes == 0 && f.nup == 0 && f.st.dirty_tex == 0 && f.st.dirty_pal == 0);
    if (check) CHECK(f.st.checked == 2 && f.st.stale == 0);
    /* a palette write in A's range: A re-decodes into its layer */
    int layA = f.use[0].layer;
    tc_pal[0x50 + 6] ^= 0x15;
    f = tc_run(c);
    CHECK(f.st.changed == 1 && f.st.decodes == 1 && f.up[0].layer == layA && f.use[0].layer == layA);
    CHECK(tc_upload_ok(&f, tA, 5));
    /* a neighbouring palette in the same chunk (palette 6): A compares its 32 bytes and stays */
    tc_pal[0x70 + 3] ^= 0x40;
    tc_pal[0x7F] ^= 0x40;
    f = tc_run(c);
    CHECK(f.st.changed == 0 && f.st.decodes == 0 && f.st.kept_pal >= 1 && f.st.dirty_pal == 1);
    /* a texel byte outside A's range in a chunk A touches (0x1300): the hash keeps it */
    tc_tex[0x1300] ^= 0xFF;
    f = tc_run(c);
    CHECK(f.st.changed == 0 && f.st.decodes == 0 && f.st.kept_hash == 1);
    /* a texel byte inside: re-decode; restoring it: re-decode again */
    tc_tex[0x12FF] ^= 0x11;
    f = tc_run(c);
    CHECK(f.st.changed == 1 && f.st.decodes == 1 && tc_upload_ok(&f, tA, 5));
    tc_tex[0x12FF] ^= 0x11;
    f = tc_run(c);
    CHECK(f.st.changed == 1 && f.st.decodes == 1 && tc_upload_ok(&f, tA, 5));
    /* B's block words (slot 1): re-decode */
    tc_tex[0x20000 + 0x1000 + 9] ^= 0x80;
    f = tc_run(c);
    CHECK(f.st.changed == 1 && f.st.decodes == 1 && tc_upload_ok(&f, tB, 40));
    /* a bank remap (page 0, A's and B's texels, now shows other bytes): both re-decode; the old bank back: again; then nothing */
    memcpy(img2, tc_tex, 0x4000);
    for (int i = 0; i < 0x4000; i++) tc_tex[i] = (u8)rnd();
    f = tc_run(c);
    CHECK(f.st.changed == 2 && tc_upload_ok(&f, tA, 5) && tc_upload_ok(&f, tB, 40));
    memcpy(tc_tex, img2, 0x4000);
    f = tc_run(c);
    CHECK(f.st.changed == 2 && tc_upload_ok(&f, tA, 5));
    f = tc_run(c);
    CHECK(f.st.changed == 0 && f.st.dirty_tex == 0 && f.st.hits == 2);
    for (int k = 0; k < 16; k++) tc_run(c); /* the changes above leave A's 16-frame history */
    /* 8 changes in 16 frames: volatile (exact path, no decode) for 120 frames, then back */
    int nvol = 0;
    for (int k = 0; k < 16; k++) {
      if (k % 2 == 0) tc_tex[0x1105] ^= (u8)(1 + k);
      f = tc_run(c);
      if (f.use[0].why == G12_USE_VOLATILE) nvol++;
      if (k == 13) CHECK(f.st.vol == 0 && f.use[0].why == G12_USE_READY); /* 7 changes */
      if (k == 14) CHECK(f.st.vol == 1 && f.st.decodes == 0 && f.use[0].bucket == -1); /* the 8th */
    }
    CHECK(nvol == 2);
    for (int k = 0; k < 118; k++) {
      if (k < 30 && k % 3 == 0) tc_tex[0x1105] ^= 0x22; /* still changing: followed, never decoded */
      f = tc_run(c);
      CHECK(f.use[0].why == G12_USE_VOLATILE && f.st.decodes == 0);
    }
    f = tc_run(c); /* 120 frames on, no change in the last 16: decoded again */
    CHECK(f.use[0].why == G12_USE_READY && f.st.decodes == 1 && tc_upload_ok(&f, tA, 5));
    f = tc_run(c);
    CHECK(f.st.hits == 2 && f.st.decodes == 0);
    if (check) CHECK(f.st.stale == 0 && f.st.checked == 2);
    g12_destroy(c);
  }

  /* buckets: 65 textures of 32 grow bucket 0 from 64 to 128 layers; the decode budget spreads a scene over frames */
  {
    G12Options o = {0};
    o.check = 1;
    G12Cache *c = g12_create(&o);
    static u32 tp[80], pl[80];
    for (int i = 0; i < 65; i++) {
      tp[i] = tparam(4, 2, 2, (u32)i * 1024, 0x30000); /* 256 colours 32 x 32: 1 KB each */
      pl[i] = (u32)i;
    }
    tc_polys(64, tp, pl);
    G12Frame f = tc_run(c);
    CHECK(f.st.decodes == 64 && f.st.grows == 1 && f.grow_cap[0] == 64); /* 64 x 1K texels: under 16 x 128 x 128 */
    tc_polys(65, tp, pl); /* the 65th grows the array: the 64 layers of the old one are lost */
    f = tc_run(c);
    CHECK(f.st.hits == 64 && f.st.decodes == 1 && f.st.grows == 1 && f.grow_cap[0] == 128);
    int ready = 0;
    for (int i = 0; i < 65; i++) ready += f.use[i].why == G12_USE_READY;
    CHECK(ready == 1 && f.use[64].why == G12_USE_READY && f.use[0].why == G12_USE_PENDING);
    f = tc_run(c);
    CHECK(f.st.decodes == 64 && f.st.grows == 0 && f.st.stale == 0);
    for (int i = 0; i < 65; i++) CHECK(f.use[i].why == G12_USE_READY);
    g12_destroy(c);
    c = g12_create(&o); /* grown twice in the frame that decodes the others: every entry decoded this frame stays (uploaded after) */
    f = tc_run(c);
    CHECK(f.st.decodes == 65 && f.st.grows == 2 && f.grow_cap[0] == 128 && f.nup == 65);
    for (int i = 0; i < 65; i++) CHECK(f.use[i].why == G12_USE_READY);
    g12_destroy(c);
    /* a budget of 1 MB: four layers of 256; the fifth texture waits until one has been idle for 120 frames */
    o.budget_mb = 1;
    o.decode_texels = 4 * 256 * 256;
    c = g12_create(&o);
    for (int i = 0; i < 5; i++) {
      tp[i] = tparam(7, 5, 5, i == 4 ? 0x10000 : (u32)i * 0x20000, 0x30000);
      pl[i] = 0;
    }
    tc_polys(4, tp, pl);
    f = tc_run(c);
    CHECK(f.st.decodes == 4 && f.grow_cap[3] == 4);
    tc_polys(5, tp, pl);
    f = tc_run(c);
    CHECK(f.st.noroom == 1 && f.use[4].why == G12_USE_NOROOM && f.st.hits == 4);
    u32 tp2[4] = {tp[0], tp[1], tp[2], tp[4]}, pl2[4] = {0};
    tc_polys(4, tp2, pl2);
    int got = -1;
    for (int k = 0; k < 125 && got < 0; k++) {
      f = tc_run(c);
      if (f.use[3].why == G12_USE_READY) got = k;
    }
    CHECK(got == 119 && f.st.evicted == 1 && f.st.bytes == 1 << 20); /* texture 3 last used at frame 2: idle 120 at frame 122 (k 119) */
    tc_polys(4, tp, pl); /* texture 3 again: a layer only by evicting a texture idle for 120 frames: none */
    f = tc_run(c);
    CHECK(f.use[3].why == G12_USE_NOROOM && f.st.stale == 0);
    g12_destroy(c);
  }
  printf("invalidation: %s\n", fails == fails0 ? "ok" : "FAILED");
}

/* every textured polygon of a capture: the GPU's flattened data against vcore's own regions */
static void t_capture(const char *path) {
  char err[512];
  VCoreState *s = vcore_create();
  if (vcore_load_vcs(s, path, err, sizeof err) != 0) {
    printf("FAIL load %s: %s\n", path, err);
    fails++;
    return;
  }
  vcore_render(s);
  GpuTexData *td = gpu_texdata_create();
  gpu_texdata_build(td, s);
  const void *tex, *pal;
  int ntex, npal;
  unsigned ser;
  gpu_texdata_bytes(td, &tex, &ntex, &pal, &npal, &ser);
  long before = n_cmp, bd = n_differ;
  int ntx = 0;
  static u32 seen[VC_MAX_POLYS][2];
  int nseen = 0;
  for (u32 i = 0; i < s->geo.npoly; i++) {
    const VC_Poly *p = &s->geo.poly[s->geo.order[i]];
    u32 tp = p->texparam;
    if (((tp >> 26) & 7) == 0) continue;
    int dup = 0;
    for (int k = 0; k < nseen; k++) dup |= seen[k][0] == tp && seen[k][1] == p->pltt;
    if (dup) continue;
    seen[nseen][0] = tp;
    seen[nseen++][1] = p->pltt;
    ntx++;
    u32 pltt = p->pltt & 0x1FFF;
    check_decode(s, tex, pal, tp, pltt, img);
    int S = g12_tc_layout(img, tp, lay);
    if (S) check_layer(s, tp, pltt, lay, S, 800);
  }
  printf("%s: %d textures, %ld compared, %ld differ\n", path, ntx, n_cmp - before, n_differ - bd);
  gpu_texdata_destroy(td);
  vcore_destroy(s);
}

/* the frontier fill against the 8-pass reference on random layers: density of opaque texels from 1/4096 to 1 */
static void t_fill_ref(void) {
  static const int sizes[] = {8, 16, 32, 64, 128, 256, 512};
  for (int si = 0; si < 7; si++)
    for (int sh = 0; sh < 14; sh++)
      for (int rep = 0; rep < 6; rep++) {
        int S = sizes[si];
        for (int i = 0; i < S * S; i++) {
          u32 r = rnd();
          lay2[i * 4] = (u8)r; lay2[i * 4 + 1] = (u8)(r >> 8); lay2[i * 4 + 2] = (u8)(r >> 16);
          lay2[i * 4 + 3] = (rnd() & ((1u << sh) - 1)) == 0 ? (u8)(1 + rnd() % 31) : 0;
          if (rep == 5 && (i % S) > S / 3 && (i % S) < S / 2) lay2[i * 4 + 3] = 0; /* a transparent band */
        }
        memcpy(lay3, lay2, (size_t)S * S * 4);
        tc_fill_ref(lay3, S);
        g12_tc_fill(lay2, S);
        n_fill_rnd++;
        if (memcmp(lay2, lay3, (size_t)S * S * 4)) { CHECK(0); n_fill_diff++; }
      }
}

int main(int argc, char **argv) {
  t_fill();
  t_fill_ref();
  t_synthetic();
  printf("synthetic: %ld configs, %ld texels compared, %ld differ\n", n_cfg, n_cmp, n_differ);
  printf("fill vs the 8-pass reference: %ld random layers + synthetic, %ld differ\n", n_fill_rnd, n_fill_diff);
  long cmp0 = n_cmp, d0 = n_differ;
  t_ranges();
  printf("ranges: %ld decodes, %ld differ\n", n_cmp - cmp0, n_differ - d0);
  t_invalidation();
  for (int i = 1; i < argc; i++) t_capture(argv[i]);
  if (n_differ) fails++;
  if (fails) printf("t12: %d FAILED (%ld differ)\n", fails, n_differ);
  else printf("t12: ok (%ld differ)\n", n_differ);
  return fails != 0;
}
