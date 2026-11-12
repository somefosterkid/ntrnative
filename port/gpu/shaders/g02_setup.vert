// port/gpu pass S: the span records built on the GPU (docs/gpu-cost.md, P1). OWNED BY G2.
// One vertex per record (polygon x row), GL_POINTS with the rasterizer off: transform feedback writes the vertex's twelve
// outputs s0-s11 (texels 0-11 of g02_spans.h's GpuSpan, float bits) interleaved into a buffer, record after record, which
// src/g02_setup.c copies into the span texture pass R reads (record r's texel t at texel 12 r + t, g00_head.glsl g_span). The
// vertex finds its polygon's setup (src/g02_spans.c setup_poly) by a binary search of the find entries, runs vcore p16's
// vc_span_row for its row in 32-bit integers (the forms of g02_spans.c's gpu_s32_*, tested against p16's s64 code by
// tests/t02_spans.c) and packs the texels as gpu_span_pack does. (No array element is passed as an out or inout argument:
// Apple's GL compiler crashes on that.)
// S_FRAG (src/gpu_gl.c defines it when this file is a fragment shader): the same body for pass S's fragment form
// (src/g02_setup.c pass_s_frag, for drivers whose transform feedback is wrong), at the end. S_COMP: the same body as a compute
// shader, one invocation per record writing a buffer (the Metal core's pass S, src/gpu_mtl.m; tools/gpu_msl.py translates it).
#if defined(S_FRAG) || defined(S_COMP)
uvec4 s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
#else
flat out uvec4 s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
#endif
void s_zero() {
  s0 = uvec4(0u); s1 = s0; s2 = s0; s3 = s0; s4 = s0; s5 = s0; s6 = s0; s7 = s0; s8 = s0; s9 = s0; s10 = s0; s11 = s0;
}

uniform highp usampler2D uSetup; // the setup texels
uniform highp usampler2D uFind;  // per polygon with records: its first record, its first setup texel
// uNFind, uZero, uFirst, uPart, uN, uR: g00_head.glsl's block

const int S_MAXE = 10, S_MAXRUN = 12, S_BIG = 1073741824;
const uint SU_DOT = 1u, SU_WIRE = 2u, SU_FULL = 4u, SU_AA = 8u, SU_FLATBOT = 16u;

ivec4 s_tx(int i) { return ivec4(texelFetch(uSetup, g_at(i), 0)); }

int s_fdiv(int n, int d) { // floor(n / d), d > 0
  return n >= 0 ? int(uint(n) / uint(d)) : -int((uint(-n) + uint(d) - 1u) / uint(d));
}

// p16's edge_span (g02_spans.c gpu_s32_edge_span)
bool s_edge_span(int ax, int ay, int bx, int by, int y, out int lo, out int hi, out int rd) {
  lo = 0; hi = 0; rd = 0;
  if (y < ay || y > by) return false;
  int dx = bx - ax, dy = by - ay;
  if (dy == 0) {
    lo = ax; hi = bx; rd = bx - 1;
    return true;
  }
  uint adx = uint(abs(dx)), udy = uint(dy), k = uint(y - ay);
  uint S = adx == udy ? 262144u : adx * (262144u / udy);
  if (adx <= udy) {
    int f = int((S * k) >> 18);
    if (dx >= 0) {
      lo = ax + f; rd = lo - 1;
    } else {
      lo = ax - 1 - f; rd = k == 0u ? lo : lo - 1;
    }
    hi = lo;
    return true;
  }
  if (y == by) return false;
  int q = int((S * (k + 1u) + 131072u) >> 18);
  lo = dx > 0 ? ax + q : ax - q;
  hi = lo; rd = lo - 1;
  return true;
}

int s_xmajor_q(int adx, int dy, int k) {
  uint S = uint(adx) * (262144u / uint(dy));
  return int((S * uint(k + 1) + 131072u) >> 18);
}

// p16's edge_run: an X-major edge's run on row y of a polygon drawn whole
bool s_edge_run(int ax, int ay, int bx, int by, int y, out int rlo, out int rhi) {
  rlo = 0; rhi = 0;
  int dx = bx - ax, dy = by - ay, adx = abs(dx);
  if (dy == 0 || adx <= dy || y < ay || y >= by) return false;
  int k = y - ay, a = k > 0 ? s_xmajor_q(adx, dy, k - 1) : 0, b = s_xmajor_q(adx, dy, k);
  if (dx > 0) { rlo = ax + a; rhi = ax + b - 1; }
  else { rlo = ax - b; rhi = ax - a - 1; }
  return true;
}

// p16's aa_edge (g02_spans.c gpu_s32_aa_edge): x = first (left) or last (right), lo, hi, cov0, sh
// the record's inner run (t10.zw, g02_spans.h): lo .. hi with the antialiasing window w (DS pixels lo, hi when mm > 1) cut away;
// a window inside the run keeps its left part
vec2 s_inner_cut(vec2 r, vec2 w, float mm) {
  if (w.x > w.y) return r;
  float a = w.x * mm, b = w.y * mm + mm - 1.0;
  if (b < r.x || a > r.y) return r;
  return a <= r.x ? vec2(b + 1.0, r.y) : vec2(r.x, a - 1.0);
}
bool s_aa_edge(int ax, int ay, int dx, int dy, int by, int y, bool left, out ivec4 w, out int sh) {
  w = ivec4(0, 1, 0, -1);
  sh = 0;
  if (dy == 0 || dx == 0) return false;
  int adx = abs(dx), sg = dx > 0 ? 1 : -1, k = y - ay;
  if (adx <= dy) {
    uint S = adx == dy ? 262144u : uint(adx) * (262144u / uint(dy));
    uint A = S * uint(2 * k + 1), B = S * uint(k);
    int ui = int(A >> 19), uf = int(((A & 0x7FFFFu) << 1) | 1u);
    int ti = int(B >> 18), tf = int((B & 0x3FFFFu) << 2);
    int fl = sg > 0 ? ui : -ui - 1, fr = sg > 0 ? uf : 1048576 - uf;
    int xl = sg > 0 || tf == 0 ? sg * ti : -ti - 1, xr = sg > 0 || tf == 0 ? tf : 1048576 - tf;
    int xc = xl + (xr != 0 ? 1 : 0);
    int rnd = ax + fl + (fr >= 524288 ? 1 : 0);
    if (left) {
      int cp = fl + 1;
      if (cp - 1 <= xl && xc <= cp) {
        int cov = (1048576 - fr) >> 15;
        if (cov > 0) w = ivec4(ax + cp - 1, ax + cp - 1, ax + cp - 1, cov);
        else w.x = ax + cp;
      } else w.x = rnd;
    } else {
      int pp = fl;
      if (pp <= xl && xc <= pp + 1) {
        int cov = fr >> 15;
        if (cov > 0) w = ivec4(ax + pp, ax + pp, ax + pp, cov);
        else w.x = ax + pp - 1;
      } else w.x = rnd - 1;
    }
    return true;
  }
  if (!left && sg < 0 && y == by - 1) sh = 1;
  int xc = s_fdiv(ax * dy + sg * adx * k + sg * (adx / 2), dy) + sh;
  int span = adx / dy + 3, f, l;
  int M = 32 * ax * dy - 16 * dy + 16 * sg * adx * (2 * k + 1);
  if (left) {
    int jmin = s_fdiv(M - 15 * adx, 32 * dy) + 1;
    if (jmin <= xc + span) { f = max(jmin, xc - span); l = xc + span; }
    else { f = xc; l = xc; }
  } else {
    int jmax = s_fdiv(M + 15 * adx, 32 * dy) + sh;
    if (jmax >= xc - span) { f = xc - span; l = min(jmax, xc + span); }
    else { f = xc; l = xc; }
  }
  if (sh != 0 && l > f) l--;
  w = ivec4(left ? f : l, xc - span, xc + span, -1);
  return true;
}

// the sign of n1 / d1 - n2 / d2 (gpu_s32_mid_cmp)
int s_mid_cmp(int n1, int d1, int n2, int d2) {
  if (d1 < 8192 && d2 < 8192 && abs(n1) < 1073741824 && abs(n2) < 1073741824) {
    // n1 d2 against n2 d1 (d > 0) exactly: n = 4096 h + l (0 <= l < 4096), n d = 4096 (h d + (l d >> 12)) + (l d & 4095), each
    // part below 2^31 at these sizes; no integer division (the two below cost pass S ~5% on the M2)
    int la = (n1 & 4095) * d2, lb = (n2 & 4095) * d1;
    int ha = (n1 >> 12) * d2 + (la >> 12), hb = (n2 >> 12) * d1 + (lb >> 12);
    if (ha != hb) return ha < hb ? -1 : 1;
    la &= 4095; lb &= 4095;
    return la < lb ? -1 : la > lb ? 1 : 0;
  }
  int q1 = s_fdiv(n1, d1), q2 = s_fdiv(n2, d2);
  if (q1 != q2) return q1 < q2 ? -1 : 1;
  int a = (n1 - q1 * d1) * d2, b = (n2 - q2 * d2) * d1;
  return a < b ? -1 : a > b ? 1 : 0;
}

uvec4 s_f(vec4 v) { return floatBitsToUint(v); }

// ---- the attribute ends (p16's edge_attr, in double there) in float-float: a value is hi + lo, |lo| <= ulp(hi) / 2, and the
// record takes hi and lo as gpu_span_pack takes (float)v and v - (float)v. The error-free steps (Dekker, Knuth) need every
// rounding as written: s_k passes a value through an opaque XOR with uZero (0), so the compiler can neither reassociate nor
// fuse them (GLSL ES 3.00 has no `precise`). The Metal kernel (S_COMP) is compiled with safe math (src/gpu_mtl.m), which
// does neither, and the MSL keeps each step a statement of its own: s_k is the identity there (the XOR cost pass S ~12% on
// the M2; the records bit for bit the same, 2026-11-04).
#ifdef S_COMP
float s_k(float x) { return x; }
#else
float s_k(float x) { return uintBitsToFloat(floatBitsToUint(x) ^ uZero); }
#endif
vec2 ff_qsum(float a, float b) { float s = s_k(a + b); return vec2(s, b - s_k(s - a)); } // |a| >= |b|
vec2 ff_tsum(float a, float b) {
  float s = s_k(a + b), bb = s_k(s - a);
  return vec2(s, s_k(a - s_k(s - bb)) + s_k(b - bb));
}
vec2 ff_split(float a) {
  float t = s_k(4097.0 * a), h = s_k(t - s_k(t - a));
  return vec2(h, s_k(a - h));
}
vec2 ff_tprod(float a, float b) {
  float p = s_k(a * b);
  vec2 A = ff_split(a), B = ff_split(b);
  float e = s_k(s_k(s_k(A.x * B.x) - p) + s_k(A.x * B.y));
  e = s_k(e + s_k(A.y * B.x));
  e = s_k(e + s_k(A.y * B.y));
  return vec2(p, e);
}
vec2 ff_add(vec2 a, vec2 b) {
  vec2 s = ff_tsum(a.x, b.x), t = ff_tsum(a.y, b.y);
  s = ff_qsum(s.x, s_k(s.y + t.x));
  return ff_qsum(s.x, s_k(s.y + t.y));
}
vec2 ff_mul(vec2 a, vec2 b) {
  vec2 p = ff_tprod(a.x, b.x);
  return ff_qsum(p.x, s_k(p.y + s_k(s_k(a.x * b.y) + s_k(a.y * b.x))));
}
vec2 ff_div(vec2 a, vec2 b) { // three quotient digits, each from the exact remainder
  float q1 = s_k(a.x / b.x);
  vec2 r = ff_add(a, -ff_mul(b, vec2(q1, 0.0)));
  float q2 = s_k(r.x / b.x);
  r = ff_add(r, -ff_mul(b, vec2(q2, 0.0)));
  float q3 = s_k(r.x / b.x);
  return ff_add(ff_qsum(q1, q2), vec2(q3, 0.0));
}
vec2 ff_int(int i) { // exact: two halves of 16 bits
  return ff_tsum(float(i >> 16) * 65536.0, float(i & 0xFFFF));
}
float ff_floor(vec2 a) { float f = floor(a.x); return f == a.x ? f + floor(a.y) : f; }

struct SAttr {
  vec2 w, z, s, t;
  vec3 c;
};
SAttr s_vertex(int at) {
  ivec4 v0 = s_tx(at), v1 = s_tx(at + 1);
  SAttr r;
  r.w = ff_int(v0.x); r.z = ff_int(v0.y); r.s = ff_int(v0.z); r.t = ff_int(v0.w);
  r.c = vec3(v1.xyz);
  return r;
}
// edge (ax, ay, dx, dy) from vertex a to b at pixel column xe of row y: f = n / d as p16 takes it (clamped to 0..1), then the
// perspective form q = f wa / ((1 - f) wb + f wa), w = wa wb / ((1 - f) wb + f wa) with f's fraction kept whole
SAttr s_edge_attr(int va, int vb, int ax, int ay, int dx, int dy, int xe, int y, bool right) {
  SAttr a = s_vertex(va), b = s_vertex(vb), r;
  int n, d;
  if (dy == 0 || abs(dx) > dy) { n = xe - ax + (right ? 1 : 0); d = dx; }
  else { n = y - ay; d = dy; }
  if (d == 0) { n = 0; d = 1; }
  if (d < 0) { n = -n; d = -d; }
  n = clamp(n, 0, d);
  vec2 q, f = ff_div(vec2(float(n), 0.0), vec2(float(d), 0.0));
  if (a.w != b.w) {
    vec2 na = ff_mul(a.w, vec2(float(n), 0.0)), D = ff_add(ff_mul(b.w, vec2(float(d - n), 0.0)), na);
    q = ff_div(na, D);
    r.w = ff_div(ff_mul(ff_mul(a.w, vec2(float(d), 0.0)), b.w), D);
  } else {
    q = f;
    r.w = a.w;
  }
  for (int i = 0; i < 3; i++) {
    vec2 c = ff_add(vec2(a.c[i], 0.0), ff_mul(q, vec2(b.c[i] - a.c[i], 0.0)));
    r.c[i] = ff_floor(ff_add(c, vec2(1e-6, 0.0)));
  }
  r.s = ff_add(a.s, ff_mul(q, ff_add(b.s, -a.s)));
  r.t = ff_add(a.t, ff_mul(q, ff_add(b.t, -a.t)));
  r.z = ff_add(a.z, ff_mul(f, ff_add(b.z, -a.z)));
  return r;
}

// record rec's twelve texels into s0-s11
void s_build(int rec) {
  if (uNFind < 1) {
    s_zero();
    return;
  }
  int lo = 0, hi = uNFind - 1;
  while (lo < hi) { // the last polygon whose first record <= rec
    int mid = (lo + hi + 1) >> 1;
    if (int(texelFetch(uFind, g_at(mid), 0).x) <= rec) lo = mid;
    else hi = mid - 1;
  }
  int base = int(texelFetch(uFind, g_at(lo), 0).y);
  ivec4 h0 = s_tx(base), h1 = s_tx(base + 1), h2 = s_tx(base + 2);
  uint fl = uint(h0.x);
  int m = int((fl >> 8) & 255u), ne = int((fl >> 16) & 15u), W = h0.w & 0xFFFF, H = h0.w >> 16;
  int ri = rec - h0.y;
  int ymin = h1.x, yend = h1.y;
  int y = (fl & SU_DOT) != 0u ? h1.z : h1.z + ri / m;

  // the row (vc_span_row): what the record needs of it
  bool ok = false;
  int xs = 0, xe = 0, dxs = 0, dxe = 0, den_f = 1, den_z = 1, rflags = 0, nrun = 0, Llo = 0, Rhi = 0, le = 0, re = 0;
  int runlo[S_MAXRUN], runhi[S_MAXRUN];
  ivec4 al = ivec4(0, 1, 0, -1), ar = ivec4(0, 1, 0, -1);
  int alsh = 0, arsh = 0;
  bool alv = false, arv = false;
  // edge k (ax, ay, bx, by) is setup texel base + 3 + k, fetched where it is used (cached) rather than kept in four arrays
  // indexed by le and re (private memory on Apple GPUs: pass S -15% on the M2 without them)
  if ((fl & SU_DOT) != 0u) {
    ok = true;
    xs = xe = dxs = dxe = h1.w;
    nrun = 1;
    runlo[0] = runhi[0] = h1.w;
    rflags = int(ROW_ALLEDGE | ROW_COV0);
  } else if (y >= 0 && y < H) {
    int rlo[S_MAXRUN], rhi[S_MAXRUN], nr = 0;
    bool flatrow = false, nonflat = false;
    for (int k = 0; k < S_MAXE; k++) {
      if (k >= ne) break;
      int a, b, c;
      ivec4 e = s_tx(base + 3 + k);
      if (e.w == e.y && e.y == y) flatrow = true;
      if (e.w != e.y && s_edge_span(e.x, e.y, e.z, e.w, y, a, b, c)) nonflat = true;
    }
    int Lhi = 0, Rrd = 0, lmn = 0, lmd = 1, rmn = 0, rmd = 1;
    le = -1; re = -1; Llo = S_BIG; Rhi = -S_BIG;
    for (int k = 0; k < S_MAXE; k++) {
      if (k >= ne) break;
      int lo, hi, rd;
      ivec4 e = s_tx(base + 3 + k);
      if (!s_edge_span(e.x, e.y, e.z, e.w, y, lo, hi, rd)) continue;
      int dy = e.w - e.y, dx = e.z - e.x;
      if (dy != 0 && y == e.w) {
        bool cont = false;
        for (int j = 0; j < S_MAXE; j++) {
          if (j >= ne) break;
          ivec4 f = s_tx(base + 3 + j);
          if (f.w != f.y && f.y == y) cont = true;
        }
        if (cont) continue;
      }
      if ((flatrow || y != ymin) && dy != 0 && e.y == y && rd == lo) rd--;
      int cl = max(lo, 0), ch = min(hi, W - 1);
      if (dy == 0 && nonflat) {
        if (cl <= ch && nr < S_MAXRUN) { rlo[nr] = cl; rhi[nr] = ch; nr++; }
        continue;
      }
      int mn, md;
      if (dy == 0 || y == e.w) { mn = dy == 0 ? e.x : e.z; md = 1; }
      else { mn = 2 * e.x * dy + dx * (2 * (y - e.y) + 1); md = 2 * dy; }
      int c1 = le < 0 ? -1 : s_mid_cmp(mn, md, lmn, lmd), c2 = re < 0 ? 1 : s_mid_cmp(mn, md, rmn, rmd);
      if (le < 0 || c1 < 0 || (c1 == 0 && lo < Llo)) { lmn = mn; lmd = md; Llo = lo; Lhi = hi; le = k; }
      if (re < 0 || c2 > 0 || (c2 == 0 && hi > Rhi)) { rmn = mn; rmd = md; Rhi = hi; re = k; Rrd = rd; }
      if (cl <= ch && nr < S_MAXRUN) { rlo[nr] = cl; rhi[nr] = ch; nr++; }
    }
    if (le >= 0) {
      ivec4 eL = s_tx(base + 3 + le), eR = s_tx(base + 3 + re);
      int Lrun = Llo, Rrun = S_BIG;
      if ((fl & SU_FULL) != 0u) {
        int a, b;
        if (s_edge_run(eL.x, eL.y, eL.z, eL.w, y, a, b)) {
          Lrun = min(Lrun, a);
          int cl = max(a, 0), ch = min(b, W - 1);
          if (cl <= ch && nr < S_MAXRUN) { rlo[nr] = cl; rhi[nr] = ch; nr++; }
        }
        if (s_edge_run(eR.x, eR.y, eR.z, eR.w, y, a, b)) {
          Rrun = b;
          int cl = max(a, 0), ch = min(b, W - 1);
          if (cl <= ch && nr < S_MAXRUN) { rlo[nr] = cl; rhi[nr] = ch; nr++; }
        }
      }
      int ldx = eL.z - eL.x, ldy = eL.w - eL.y, rdx = eR.z - eR.x, rdy = eR.w - eR.y;
      bool Lsteep = !(ldx > ldy);
      for (int a = 1; a < S_MAXRUN; a++) { // the runs sorted by their start (an insertion sort) and merged
        if (a >= nr) break;
        int l = rlo[a], h = rhi[a], j = a - 1;
        while (j >= 0 && rlo[j] > l) { rlo[j + 1] = rlo[j]; rhi[j + 1] = rhi[j]; j--; }
        rlo[j + 1] = l; rhi[j + 1] = h;
      }
      for (int a = 0; a < S_MAXRUN; a++) {
        if (a >= nr) break;
        if (nrun > 0 && rlo[a] <= runhi[nrun - 1] + 1) runhi[nrun - 1] = max(runhi[nrun - 1], rhi[a]);
        else { runlo[nrun] = rlo[a]; runhi[nrun] = rhi[a]; nrun++; }
      }
      if ((fl & SU_WIRE) != 0u) {
        ok = true;
        rflags = int(ROW_WIRE);
        xs = dxs = Llo;
        xe = dxe = Rhi;
        den_f = den_z = Rhi > Llo ? Rhi - Llo : 1;
      } else {
        xs = Lrun;
        bool rvert = rdx == 0 && rdy != 0;
        xe = (fl & SU_FULL) != 0u && !rvert ? Rhi : Rrd;
        if (Rrun != S_BIG) xe = Rrun;
        if (xe < Lhi && Lsteep) xe = Lhi;
        dxs = xs; dxe = xe;
        ok = true;
        if ((fl & SU_AA) != 0u) {
          rflags |= int(ROW_AA);
          alv = s_aa_edge(eL.x, eL.y, ldx, ldy, eL.w, y, true, al, alsh);
          arv = s_aa_edge(eR.x, eR.y, rdx, rdy, eR.w, y, false, ar, arsh);
          if (alv) dxs = al.x;
          if (arv) dxe = ar.x;
          if (dxe < dxs) ok = false;
        } else if (xe < xs) ok = false;
        if (ldx == 0 && ldy != 0) rflags |= int(ROW_VERTL);
        if ((fl & SU_FLATBOT) != 0u && y == yend) rflags |= int(ROW_ALLEDGE);
        den_f = Rrd - xs + 1 + (rvert ? 0 : 1);
        den_z = xe - xs + 2;
      }
    }
  }

  // the record (gpu_span_pack at widening m)
  int xmax = W * m - 1;
  int px0 = max(dxs * m, 0), px1 = min(dxe * m + m - 1, xmax);
  if (!ok || px1 < px0) {
    s_zero();
    s0 = s_f(vec4(1.0, 0.0, 0.0, 0.0));
    return;
  }
  bool aa = (rflags & int(ROW_AA)) != 0;
  // the edge-flag mask: p16's runs and the row's last pixel, scaled, sorted, merged (cap: 6 runs, 2 with antialiasing). The
  // runs are sorted and apart already, so one pass puts the last pixel before the first run that starts after it (where a
  // stable sort puts it) and merges as it goes, with no arrays (a private array costs on Apple GPUs: memory, not registers)
  int nm = 0, ml = 0, mh = 0; // the mask's runs (the first six in r01, r23, r45), the last one
  vec4 r01 = vec4(1.0, 0.0, 1.0, 0.0), r23 = r01, r45 = r01; // (an empty run is lo > hi)
  {
    bool ev = xe >= 0 && xe <= W - 1; // the row's last pixel: after the runs that start at or before it
    int el = xe * m;
    int ia = 0;
    for (int it = 0; it < S_MAXRUN + 1; it++) {
      int l, h;
      if (ev && (ia >= nrun || el < runlo[ia] * m)) { l = el; h = el + m - 1; ev = false; }
      else if (ia < nrun) { l = runlo[ia] * m; h = runhi[ia] * m + m - 1; ia++; }
      else break;
      h = min(h, xmax);
      if (l > h) continue;
      if (nm > 0 && l <= mh + 1) mh = max(mh, h);
      else { ml = l; mh = h; nm++; }
      int i = nm - 1;
      vec2 v = vec2(float(ml), float(mh));
      if (i == 0) r01.xy = v;
      else if (i == 1) r01.zw = v;
      else if (i == 2) r23.xy = v;
      else if (i == 3) r23.zw = v;
      else if (i == 4) r45.xy = v;
      else if (i == 5) r45.zw = v;
    }
  }
  int flags = rflags;
  if (aa && m > 1) flags |= int(ROW_DSGRID);
  if (m > 1) flags |= m << 8;
  if (!aa && nm > 2) flags |= int(ROW_MASK2);
  int lax = 0, lay = 0, ldx = 0, ldy = 0, rax = 0, ray = 0, rdx = 0, rdy = 0;
  if (le < ne) { ivec4 e = s_tx(base + 3 + le); lax = e.x; lay = e.y; ldx = e.z - e.x; ldy = e.w - e.y; } // (a dot has none: 0)
  if (re < ne) { ivec4 e = s_tx(base + 3 + re); rax = e.x; ray = e.y; rdx = e.z - e.x; rdy = e.w - e.y; }
  // t6-t9: the mask's further runs, or the antialiasing windows of both edges
  if (!aa) {
    s6 = s_f(r23); s7 = s_f(r45); s8 = uvec4(0u); s9 = uvec4(0u);
  } else {
    s6 = s_f(vec4(alv ? float(al.y) : 1.0, alv ? float(al.z) : 0.0, alv ? float(al.w + 1 + 64 * alsh) : 0.0, float(lax)));
    s7 = s_f(vec4(float(y - lay), float(ldx), float(ldy), 0.0));
    s8 = s_f(vec4(arv ? float(ar.y) : 1.0, arv ? float(ar.z) : 0.0, arv ? float(ar.w + 1 + 64 * arsh) : 0.0, float(rax)));
    s9 = s_f(vec4(float(y - ray), float(rdx), float(rdy), 0.0));
  }
  // t0-t5, t10, t11: the run, the attribute ends (p16's edge_attr: the left edge at Llo, the right at Rhi), the flags
  SAttr L, R;
  int vat = base + 3 + ne;
  if ((fl & SU_DOT) != 0u) {
    L = s_vertex(vat);
    R = L;
  } else {
    int bl = (h2[le >> 2] >> (8 * (le & 3))) & 255, br = (h2[re >> 2] >> (8 * (re & 3))) & 255;
    L = s_edge_attr(vat + 2 * (bl & 15), vat + 2 * (bl >> 4), lax, lay, ldx, ldy, Llo, y, false);
    R = s_edge_attr(vat + 2 * (br & 15), vat + 2 * (br >> 4), rax, ray, rdx, rdy, Rhi, y, true);
  }
  s0 = s_f(vec4(float(px0), float(px1), float(den_f * m), float(den_z * m)));
  s1 = s_f(vec4(L.w.x, R.w.x, L.z.x, R.z.x));
  s2 = s_f(vec4(L.c, L.s.x));
  s3 = s_f(vec4(R.c, R.s.x));
  vec2 inner = vec2(r01.y + 1.0, r01.z - 1.0); // between the mask's first two runs (g02_spans.c gpu_span_inner)
  if (aa) {
    float mm = m > 1 ? float(m) : 1.0;
    inner = s_inner_cut(s_inner_cut(inner, uintBitsToFloat(s6.xy), mm), uintBitsToFloat(s8.xy), mm);
  }
  s4 = s_f(vec4(L.t.x, R.t.x, float(flags), float(xs * m)));
  s5 = s_f(r01);
  s10 = s_f(vec4(L.z.y, R.z.y, inner));
  s11 = s_f(vec4(L.t.y, R.t.y, L.s.y, R.s.y));
}

#if !defined(S_FRAG) && !defined(S_COMP)
void main() {
  gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
  s_build(gl_VertexID + uFirst);
}
#elif defined(S_FRAG)
// uPart 0-2: record (y - uPart uR) 1024 + x's texels 4 uPart..4 uPart + 3 into the four targets (pixel row y of three bands of
// uR rows); uPart 3: the span texture's texel y 1024 + x (record r's texel t at 12 r + t) from the bands, one target
uniform highp usampler2D uF0, uF1, uF2, uF3;
layout(location = 0) out uvec4 o0;
layout(location = 1) out uvec4 o1;
layout(location = 2) out uvec4 o2;
layout(location = 3) out uvec4 o3;
void main() {
  ivec2 p = ivec2(gl_FragCoord.xy);
  o1 = uvec4(0u); o2 = o1; o3 = o1;
  if (uPart == 3) {
    int i = p.y * 1024 + p.x, r = i / 12, t = i - 12 * r, k = t & 3;
    ivec2 a = ivec2(r & 1023, (t >> 2) * uR + (r >> 10));
    o0 = k == 0 ? texelFetch(uF0, a, 0) : k == 1 ? texelFetch(uF1, a, 0) : k == 2 ? texelFetch(uF2, a, 0) : texelFetch(uF3, a, 0);
    return;
  }
  int rec = (p.y - uPart * uR) * 1024 + p.x;
  if (rec >= uN) {
    o0 = o1;
    return;
  }
  s_build(rec);
  if (uPart == 0) {
    o0 = s0; o1 = s1; o2 = s2; o3 = s3;
  } else if (uPart == 1) {
    o0 = s4; o1 = s5; o2 = s6; o3 = s7;
  } else {
    o0 = s8; o1 = s9; o2 = s10; o3 = s11;
  }
}
#else
// record r's twelve texels at 12 r .. 12 r + 11 of the buffer (as the span texture holds them); uN records
layout(local_size_x = 64) in;
layout(std430, binding = 1) writeonly buffer GpuSpanOut { uvec4 oSpan[]; };
void main() {
  int rec = int(gl_GlobalInvocationID.x);
  if (rec >= uN) return;
  s_build(rec);
  int o = 12 * rec;
  oSpan[o] = s0; oSpan[o + 1] = s1; oSpan[o + 2] = s2; oSpan[o + 3] = s3;
  oSpan[o + 4] = s4; oSpan[o + 5] = s5; oSpan[o + 6] = s6; oSpan[o + 7] = s7;
  oSpan[o + 8] = s8; oSpan[o + 9] = s9; oSpan[o + 10] = s10; oSpan[o + 11] = s11;
}
#endif
