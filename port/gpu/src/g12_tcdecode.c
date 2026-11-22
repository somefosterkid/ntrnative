/* port/gpu piece G12: the texture cache's decode and layer layout (g12_tcdecode.h). OWNED BY G12.
 * The rules are g04_texture.glsl's g4_sample and vcore p17's tex_sample_slow, texel for texel. */
#include "g12_tcdecode.h"

#include <string.h>

static u32 w6(u32 c) { return c ? c * 2 + 1 : 0; }

typedef struct Src {
  const u8 *tex, *pal;
} Src;

static u32 rb(const Src *s, u32 a) { return s->tex[a & 0x7FFFF]; }
static u32 rh(const Src *s, u32 a) {
  a &= 0x7FFFE;
  return s->tex[a] | (u32)s->tex[a + 1] << 8;
}
/* the palette address space is 128 KB; slots 6 and 7 hold nothing (colour 0) */
static u32 rp(const Src *s, u32 a) {
  a &= 0x1FFFE;
  if (a >= G12_PAL_BYTES) return 0;
  return s->pal[a] | (u32)s->pal[a + 1] << 8;
}

static void put(u8 *o, u32 c, u32 a) {
  o[0] = (u8)w6(c & 31);
  o[1] = (u8)w6((c >> 5) & 31);
  o[2] = (u8)w6((c >> 10) & 31);
  o[3] = (u8)a;
}

int g12_tc_width(u32 tp) { return ((tp >> 26) & 7) ? 8 << ((tp >> 20) & 7) : 0; }
int g12_tc_height(u32 tp) { return ((tp >> 26) & 7) ? 8 << ((tp >> 23) & 7) : 0; }

static void range(u32 *lo, u32 *hi, u32 a, u32 n, u32 space) {
  if (a + n > space) { /* wraps: the whole space */
    *lo = 0;
    *hi = space;
    return;
  }
  *lo = a;
  *hi = a + n;
}

int g12_tc_decode(const u8 *tex, const u8 *pal, u32 tp, u32 pltt, u8 *out, G12Ranges *rg) {
  int fmt = (tp >> 26) & 7;
  if (fmt == 0) return -1;
  Src s = {tex, pal};
  u32 w = (u32)(8 << ((tp >> 20) & 7)), h = (u32)(8 << ((tp >> 23) & 7));
  u32 base = (tp & 0xFFFF) * 8, pb = pltt & 0x1FFF, n = w * h;
  int t0 = (tp >> 29) & 1;
  G12Ranges r;
  memset(&r, 0, sizeof r);
  u32 tb = fmt == 2 ? n / 4 : fmt == 3 ? n / 2 : fmt == 5 ? n / 4 : fmt == 7 ? n * 2 : n;
  range(&r.tex_lo, &r.tex_hi, base, tb, G12_TEX_BYTES);
  switch (fmt) { /* the palette bytes of the fixed-size formats */
  case 1: range(&r.pal_lo, &r.pal_hi, pb * 16, 64, G12_PAL_SPACE); break;
  case 2: range(&r.pal_lo, &r.pal_hi, pb * 8, 8, G12_PAL_SPACE); break;
  case 3: range(&r.pal_lo, &r.pal_hi, pb * 16, 32, G12_PAL_SPACE); break;
  case 4: range(&r.pal_lo, &r.pal_hi, pb * 16, 512, G12_PAL_SPACE); break;
  case 6: range(&r.pal_lo, &r.pal_hi, pb * 16, 16, G12_PAL_SPACE); break;
  default: break;
  }

  if (fmt == 5) {
    u32 ia0 = ((base & 0x1FFFF) >> 1) + ((base & 0x40000) ? 0x10000 : 0);
    u32 nb = (w >> 2) * (h >> 2);
    range(&r.idx_lo, &r.idx_hi, 0x20000 + ia0, nb * 2, G12_TEX_BYTES);
    u32 mn = 0xFFFFFFFFu, mx = 0;
    int whole = 0;
    for (u32 by = 0; by < h >> 2; by++)
      for (u32 bx = 0; bx < w >> 2; bx++) {
        u32 blk = by * (w >> 2) + bx;
        u32 pi = rh(&s, 0x20000 + ia0 + blk * 2);
        u32 pofs = pb * 16 + (pi & 0x3FFF) * 4;
        u32 mode = pi >> 14;
        u32 c0 = rp(&s, pofs), c1 = rp(&s, pofs + 2);
        u32 po = pofs & 0x1FFFF;
        if (po + 8 > G12_PAL_SPACE) whole = 1;
        if (po < mn) mn = po;
        if (po + 8 > mx) mx = po + 8;
        u32 col[4];
        int clear3 = 0;
        if (mode == 0 || mode == 2) {
          for (int t = 0; t < 4; t++) col[t] = rp(&s, pofs + t * 2);
          clear3 = mode == 0;
        } else {
          col[0] = c0;
          col[1] = c1;
          for (int t = 2; t < 4; t++) {
            u32 c = 0;
            for (u32 k = 0; k < 3; k++) {
              u32 a0 = (c0 >> (5 * k)) & 31, a1 = (c1 >> (5 * k)) & 31, v;
              if (mode == 1) v = (a0 + a1) >> 1;
              else if (t == 2) v = (a0 * 5 + a1 * 3) >> 3;
              else v = (a0 * 3 + a1 * 5) >> 3;
              c |= (v & 31) << (5 * k);
            }
            col[t] = c;
          }
          clear3 = mode == 1;
        }
        for (u32 yy = 0; yy < 4; yy++) {
          u32 row = rb(&s, base + blk * 4 + yy);
          for (u32 xx = 0; xx < 4; xx++) {
            u32 tx = (row >> (xx * 2)) & 3;
            u8 *o = out + (((by * 4 + yy) * w) + bx * 4 + xx) * 4;
            if (clear3 && tx == 3) memset(o, 0, 4);
            else put(o, col[tx], 31);
          }
        }
      }
    if (whole) {
      mn = 0;
      mx = G12_PAL_SPACE;
    }
    if (mx > mn) {
      r.pal_lo = mn;
      r.pal_hi = mx;
    }
  } else {
    for (u32 i = 0; i < n; i++) {
      u32 idx, a, c;
      u8 *o = out + (size_t)i * 4;
      switch (fmt) {
      case 1:
        idx = rb(&s, base + i);
        a = idx >> 5;
        put(o, rp(&s, pb * 16 + (idx & 31) * 2), a * 4 + a / 2);
        break;
      case 2:
        idx = (rb(&s, base + (i >> 2)) >> ((i & 3) * 2)) & 3;
        put(o, rp(&s, pb * 8 + idx * 2), (t0 && idx == 0) ? 0 : 31);
        break;
      case 3:
        idx = (rb(&s, base + (i >> 1)) >> ((i & 1) * 4)) & 15;
        put(o, rp(&s, pb * 16 + idx * 2), (t0 && idx == 0) ? 0 : 31);
        break;
      case 4:
        idx = rb(&s, base + i);
        put(o, rp(&s, pb * 16 + idx * 2), (t0 && idx == 0) ? 0 : 31);
        break;
      case 6:
        idx = rb(&s, base + i);
        put(o, rp(&s, pb * 16 + (idx & 7) * 2), idx >> 3);
        break;
      default:
        c = rh(&s, base + i * 2);
        put(o, c, (c & 0x8000) ? 31 : 0);
        break;
      }
    }
  }
  if (rg) *rg = r;
  return 0;
}

/* ---- the layer ---- */
static int need(int n, int rep, int flip) { return rep ? (flip ? 2 * n : n) : 2 * n; }

int g12_tc_layer_size(u32 tp) {
  int w = g12_tc_width(tp), h = g12_tc_height(tp);
  if (!w) return 0;
  int S = 32;
  int a = need(w, tp >> 16 & 1, tp >> 18 & 1), b = need(h, tp >> 17 & 1, tp >> 19 & 1);
  if (a > S) S = a;
  if (b > S) S = b;
  return S > G12_MAX_BUCKET ? 0 : S;
}

/* layer coordinate X (0 <= X < S) -> the image's index on one axis */
static int src_axis(int X, int n, int rep, int flip, int S) {
  if (rep) {
    int m = X & (n - 1);
    if (flip && (X & n)) m = n - 1 - m;
    return m;
  }
  if (X < n) return X;
  return X < n + (S - n) / 2 ? n - 1 : 0;
}

int g12_tc_layout(const u8 *img, u32 tp, u8 *layer) {
  int S = g12_tc_layer_size(tp);
  if (!S) return 0;
  int w = g12_tc_width(tp), h = g12_tc_height(tp);
  int xs[G12_MAX_BUCKET], ys[G12_MAX_BUCKET];
  for (int i = 0; i < S; i++) {
    xs[i] = src_axis(i, w, tp >> 16 & 1, tp >> 18 & 1, S);
    ys[i] = src_axis(i, h, tp >> 17 & 1, tp >> 19 & 1, S);
  }
  for (int y = 0; y < S; y++)
    for (int x = 0; x < S; x++) memcpy(layer + ((size_t)y * S + x) * 4, img + ((size_t)ys[y] * w + xs[x]) * 4, 4);
  return S;
}

/* the 8 neighbours' indices of (y, x) on the wrapping S x S layer (S a power of two) */
static inline void nbrs(size_t nb[8], int y, int x, int S) {
  if (y > 0 && y < S - 1 && x > 0 && x < S - 1) {
    size_t i = (size_t)y * S + x, s = (size_t)S;
    nb[0] = i - s - 1; nb[1] = i - s; nb[2] = i - s + 1; nb[3] = i - 1;
    nb[4] = i + 1; nb[5] = i + s - 1; nb[6] = i + s; nb[7] = i + s + 1;
    return;
  }
  int M = S - 1, n = 0;
  for (int dy = -1; dy <= 1; dy++)
    for (int dx = -1; dx <= 1; dx++)
      if (dx || dy) nb[n++] = (size_t)((y + dy) & M) * S + (size_t)((x + dx) & M);
}

void g12_tc_fill(u8 *layer, int S) {
  enum { NMAX = G12_MAX_BUCKET * G12_MAX_BUCKET };
  /* fl: 0 transparent, 1 opaque in the source, 2 filled by a pass; mk: queued as a candidate.
   * (not thread-safe: one worker decodes) Only pixels near the filled region are visited: the
   * candidates of a pass are the transparent neighbours of the pixels the pass before filled. */
  static u8 fl[NMAX], mk[NMAX];
  static u32 colq[NMAX]; /* the colour of candidate q, packed */
  static u32 cand[NMAX], nxt[NMAX];
  size_t n = (size_t)S * S, cnt = 0, todo = 0, nc = 0;
  for (size_t i = 0; i < n; i++) {
    fl[i] = layer[i * 4 + 3] != 0;
    cnt += fl[i];
  }
  todo = n - cnt;
  if (cnt == 0 || todo == 0) return;
  /* pass 1 candidates: the transparent pixels with an opaque neighbour (S is a power of two: wrap by mask). Scan
   * the rarer kind: the opaque pixels mark their transparent neighbours, or each transparent pixel looks. */
  const int M = S - 1, sh = __builtin_ctz((unsigned)S);
  if (cnt <= todo) {
    for (int y = 0; y < S; y++)
      for (int x = 0; x < S; x++) {
        if (!fl[(size_t)y * S + x]) continue;
        for (int dy = -1; dy <= 1; dy++)
          for (int dx = -1; dx <= 1; dx++) {
            size_t j = (size_t)((y + dy) & M) * S + (size_t)((x + dx) & M);
            if (fl[j] || mk[j]) continue;
            mk[j] = 1;
            cand[nc++] = (u32)j;
          }
      }
    for (size_t q = 0; q < nc; q++) mk[cand[q]] = 0;
  } else {
    for (int y = 0; y < S; y++)
      for (int x = 0; x < S; x++) {
        size_t i = (size_t)y * S + x;
        if (fl[i]) continue;
        int hit = 0;
        for (int dy = -1; dy <= 1 && !hit; dy++)
          for (int dx = -1; dx <= 1; dx++)
            if ((dx || dy) && fl[(size_t)((y + dy) & M) * S + (size_t)((x + dx) & M)]) { hit = 1; break; }
        if (hit) cand[nc++] = (u32)i;
      }
  }
  for (int pass = 0; pass < 8 && nc; pass++) {
    for (size_t q = 0; q < nc; q++) {
      size_t i = cand[q];
      int y = (int)(i >> sh), x = (int)(i & (size_t)M);
      u32 c[3] = {0, 0, 0}, m = 0;
      size_t nb[8];
      nbrs(nb, y, x, S);
      for (int d = 0; d < 8; d++) {
        size_t j = nb[d];
        u32 f = fl[j] != 0; /* branchless: the filled/transparent mix is random */
        m += f;
        c[0] += f * layer[j * 4]; c[1] += f * layer[j * 4 + 1]; c[2] += f * layer[j * 4 + 2];
      }
      /* (c + m/2) / m by multiplication (m 1..8, c <= 2040: exact, the t12 test compares with the division) */
      u32 inv = (65536u + m - 1) / m, h = m / 2;
      colq[q] = ((c[0] + h) * inv >> 16) | ((c[1] + h) * inv >> 16) << 8 | ((c[2] + h) * inv >> 16) << 16;
    }
    size_t nn = 0;
    for (size_t q = 0; q < nc; q++) {
      size_t i = cand[q];
      layer[i * 4] = (u8)colq[q]; layer[i * 4 + 1] = (u8)(colq[q] >> 8); layer[i * 4 + 2] = (u8)(colq[q] >> 16);
      fl[i] = 2;
    }
    todo -= nc;
    for (size_t q = 0; q < nc; q++) {
      size_t i = cand[q];
      int y = (int)(i >> sh), x = (int)(i & (size_t)M);
      size_t nb[8];
      nbrs(nb, y, x, S);
      for (int d = 0; d < 8; d++) {
        size_t j = nb[d];
        if (fl[j] || mk[j]) continue;
        mk[j] = 1;
        nxt[nn++] = (u32)j;
      }
    }
    for (size_t q = 0; q < nn; q++) mk[nxt[q]] = 0;
    memcpy(cand, nxt, nn * sizeof(u32));
    nc = nn;
  }
  if (todo) {
    u32 sum[3] = {0, 0, 0};
    u8 mean[3];
    for (size_t i = 0; i < n; i++)
      if (fl[i] == 1)
        for (int k = 0; k < 3; k++) sum[k] += layer[i * 4 + k];
    for (int k = 0; k < 3; k++) mean[k] = (u8)((sum[k] + cnt / 2) / cnt);
    u8 pat[4] = {mean[0], mean[1], mean[2], 0}; /* a transparent texel's alpha is 0 */
    for (size_t i = 0; i < n; i++)
      if (!fl[i]) memcpy(layer + i * 4, pat, 4);
  }
}

void g12_tc_rgba8(const u8 *raw, int n, u8 *out) {
  for (int i = 0; i < n; i++) {
    for (int k = 0; k < 3; k++) out[i * 4 + k] = (u8)((raw[i * 4 + k] * 255 + 31) / 63);
    out[i * 4 + 3] = (u8)((raw[i * 4 + 3] * 255 + 15) / 31);
  }
}
