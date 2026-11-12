// port/gpu piece G3: a polygon's attributes and depth at one pixel (docs/gpu.md, G3). OWNED BY G3.
// From the row's span record (src/g02_spans.h): the fraction (x - x0) / den_f across the row, perspective-correct by
// w as vcore p16's lerp_px (k0 = (1 - f) / L.w, k1 = f / R.w, q = k1 / (k0 + k1)), depth linear by (x - x0) / den_z;
// the colour's 9-bit value floored to 6 bits, texcoords floored, depth rounded and clamped (p16's clamp_depth).
//
// vcore does this in doubles; a float has 24 bits, short for depth (24 bits) and texcoords (15 bits + a fraction).
// So every LINEAR interpolation whose fraction is n / d with whole n and d (always at 1x) is done exactly: the whole
// parts in integers (a + dq * n + (dr * n) / d, dq and dr the floor-division of the span's difference by d), only
// the fractional parts of the two ends and the remainder in floats (their sum is < 4: error 1e-7). The perspective
// fraction q is a float (error ~1e-7 of the span, so the colour carries a 1e-4 epsilon instead of vcore's 1e-6).

int g_fdiv(int a, int b) { // floor(a / b), b > 0 (GLSL leaves / and % of negative operands open: positive operands only)
  return a >= 0 ? a / b : -((-a + b - 1) / b);
}

// floor(a / b) for b > 0 and whole a, given an estimate e of a / b that is off by less than 1 (a float quotient, or a
// product of a small integer and a float fraction): the remainder shows which way, no integer division (llvmpipe
// scalarizes them: ~12 cycles a lane)
int g_fixq(int a, int b, float e) {
  int q = int(floor(e));
  int r = a - q * b;
  return q + (r < 0 ? -1 : 0) + (r >= b ? 1 : 0);
}

// floor(a + (b - a) * n / d + off) for floats a, b (each a float hi part plus a float lo part: the record holds vcore's
// doubles as hi + lo) with n, d whole, d >= 1, |n| and d below 2^11 or so; fd = float(d), fq = float(n) / fd
int g_lin(float a, float alo, float b, float blo, int n, int d, float off, float fd, float fq) {
  int ai = int(floor(a)), bi = int(floor(b));
  float fa = (a - float(ai)) + alo, fb = (b - float(bi)) + blo;
  int dz = bi - ai;
  int dq = g_fixq(dz, d, float(dz) / fd);
  int dr = dz - dq * d;
  int m = dr * n;
  int qq = g_fixq(m, d, float(dr) * fq);
  int rem = m - qq * d;
  float small = float(rem) / fd + fa + (fb - fa) * fq + off;
  return ai + dq * n + qq + int(floor(small));
}

// g_lin for whole-number ends (the colours): floor(a + (b - a) * n / d)
int g_lini(float a, float b, int n, int d, float fq) {
  int ai = int(floor(a)), dz = int(floor(b)) - ai;
  return ai + g_fixq(dz * n, d, float(dz) * fq);
}

// vcore p16's antialiasing coverage of one edge at pixel x (a = lo, hi, cov0 + 1 + 64 sh, ax; b = k, dx, dy)
// floor(n / d), d > 0, exact when it lies in lo .. hi; otherwise some value below lo or above hi. A float quotient and the
// remainder's correction instead of integer division, which the M2 runs as a long sequence (2026-11: the coverage's two
// divisions per sample and g_cover2 together: pass R 3.2 -> 2.8 ms at 3x with 4 samples)
int g_fdiv_near(int n, int d, float lo, float hi) {
  int q = int(floor(clamp(float(n) / float(d), lo - 1.0, hi + 1.0)));
  int r = n - q * d;
  return q + (r < 0 ? -1 : 0) + (r >= d ? 1 : 0);
}
uint g_aa_side(int x, vec4 a, vec4 b, bool left) {
  float fx = float(x);
  if (fx < a.x || fx > a.y) return 31u;
  int code = int(a.z), sh = code >> 6, c0 = (code & 63) - 1;
  if (c0 >= 0) return uint(c0);
  int ax = int(a.w), k = int(b.x), dx = int(b.y), dy = int(b.z);
  int adx = abs(dx), sg = dx > 0 ? 1 : -1;
  int N = (2 * (x - sh) + 1 - 2 * ax) * dy - sg * adx * (2 * k + 1);
  // left: ceil((16 adx + 16 N) / adx) - 1 = -floor(-(16 adx + 16 N) / adx) - 1; right: floor((16 adx - 16 N) / adx); 0 .. 31 kept
  int c = left ? -g_fdiv_near(-(16 * adx + 16 * N), adx, -33.0, 0.0) - 1 : g_fdiv_near(16 * adx - 16 * N, adx, -1.0, 32.0);
  return uint(clamp(c, 0, 31));
}

// Multisampling (2026-11, docs/gpu.md "Antialiasing"): pass R's fragment is one output pixel holding G_K x G_K samples
// (g00_head.glsl); each sample is a pixel of the N k grid the spans are built on (sample (i, j) of pixel (x, y) is
// (k x + i, k y + j)). Coverage, depth, the early depth reject and the edge flags are per sample (g_cover, from the sample
// row's record, g_row); the colour and texture coordinates once per pixel (g_shade).

// polygon `ent` on one row of the N k grid: the parts of its span record every sample of the row reads
struct GRow {
  int rec;
  vec4 t0, t1, t4;
};

// record rec's parts g_row reads
GRow g_row_rec(int rec) {
  GRow r;
  r.rec = rec;
  r.t0 = g_span(rec, 0);
  r.t1 = g_span(rec, 1);
  r.t4 = g_span(rec, 4);
  return r;
}

// false when the row is outside the polygon's rows (band-clipped) or its run misses the pixel's samples x0 .. x0 + k - 1
bool g_row(uvec4 ent, int row, int x0, out GRow r) {
  r.rec = int(ent.z) - GPU_LIST_BIAS + row;
  r.t0 = vec4(1.0, 0.0, 1.0, 1.0);
  r.t1 = vec4(1.0);
  r.t4 = vec4(0.0);
  // the three fetches without a test between them (record 0 for a row outside): with multisampling the rows' fetches then
  // overlap (M2 2026-11: town at 3x, 4 samples, pass R 7.1 -> 6.3 ms; AA off 2.36 -> 2.26)
  bool inr = row >= int(ent.w & 0xFFFFu) && row <= int(ent.w >> 16);
  int rc = inr ? r.rec : 0;
  r.rec = rc; // a valid record for g_cover2's fetches either way
  r.t0 = g_span(rc, 0);
  r.t1 = g_span(rc, 1);
  r.t4 = g_span(rc, 4);
  return inr && !(float(x0 + G_K - 1) < r.t0.x || float(x0) > r.t0.y);
}

// the row's samples x0 .. x0 + k - 1, lanes s0 .. s0 + k - 1 of the vectors: a lane's sample is covered (hit) when the run
// holds it and (early: a plain polygon, not a shadow) it passes the depth test against old (G11: a sample that fails changes
// nothing, so neither the exact depth nor the colour is needed for it); then its depth, edge flag and coverage. The samples
// share the record's further fetches (the depth's low parts, the edge windows), made only when a sample still needs them.
// attr: the polygon's POLYGON_ATTR.
void g_cover_row(GRow r, int x0, int s0, bool early, uvec4 old, uint attr, inout bvec4 hit, inout uvec4 depth, inout bvec4 edge,
                 inout uvec4 cov) {
  vec4 t0 = r.t0, t1 = r.t1, t4 = r.t4;
  bool wbuf = (uSwap & 2u) != 0u;
  bool c[G_K];
  bool any_c = false;
  for (int i = 0; i < G_K; i++) {
    float fx = float(x0 + i);
    bool ok = fx >= t0.x && fx <= t0.y;
    // a cheap float bound on the depth first (the exact value is within 4 of it: floats of 24 bits at 2^24, the lo parts)
    if (ok && early && !wbuf) {
      float da = t1.z + (t1.w - t1.z) * ((fx - t4.w) / t0.w), sd = float(old[s0 + i]);
      if ((attr & 0x4000u) != 0u) ok = abs(da - sd) <= 520.0;
      else ok = da - sd <= 8.0;
    }
    c[i] = ok;
    any_c = any_c || ok;
  }
  if (!any_c) return;
  vec4 dlo = g_span(r.rec, 10);
  uint d[G_K];
  any_c = false;
  for (int i = 0; i < G_K; i++) {
    float fx = float(x0 + i), nf = fx - t4.w;
    bool whole_z = nf == floor(nf) && t0.w == floor(t0.w) && t0.w >= 1.0 && t0.w < 65536.0 && abs(nf) < 65536.0;
    float fz = nf / t0.w, dd;
    uint z;
    if (wbuf) { // the w-buffer: depth is w (only here is w needed), the sample's own fraction
      float f = nf;
      if ((uint(t4.z) & ROW_AA) != 0u) f = clamp(nf, 0.0, t0.z);
      f /= t0.z;
      dd = t1.x;
      if (t1.x != t1.y) dd = 1.0 / ((1.0 - f) / t1.x + f / t1.y);
      z = dd < 0.0 ? 0u : (dd > 16777215.0 ? 0xFFFFFFu : uint(dd + 0.5));
    } else if (whole_z) {
      z = uint(clamp(g_lin(t1.z, dlo.x, t1.w, dlo.y, int(nf), int(t0.w), 0.5, t0.w, fz), 0, 0xFFFFFF));
    } else {
      dd = t1.z + (t1.w - t1.z) * fz;
      z = dd < 0.0 ? 0u : (dd > 16777215.0 ? 0xFFFFFFu : uint(dd + 0.5));
    }
    uint o = old[s0 + i];
    if (c[i] && early) {
      if ((attr & 0x4000u) != 0u) c[i] = (z > o ? z - o : o - z) <= 0x200u;
      else c[i] = z < o;
    }
    d[i] = z;
    any_c = any_c || c[i];
  }
  if (!any_c) return;
  uint rf = uint(t4.z);
  if (float(x0) >= dlo.z && float(x0 + G_K - 1) <= dlo.w) { // all in the inner run (g02_spans.h t10): no edge flag, full coverage
    bool m = (rf & ROW_ALLEDGE) != 0u;
    if ((rf & ROW_WIRE) != 0u && !m) return;
    for (int i = 0; i < G_K; i++) {
      if (!c[i]) continue;
      hit[s0 + i] = true;
      depth[s0 + i] = d[i];
      edge[s0 + i] = m;
      cov[s0 + i] = (rf & ROW_COV0) != 0u ? 0u : 31u;
    }
    return;
  }
  vec4 t5 = g_span(r.rec, 5), t6 = vec4(0.0), t7 = vec4(0.0), t8 = vec4(0.0), t9 = vec4(0.0);
  if ((rf & (ROW_MASK2 | ROW_AA)) != 0u) {
    t6 = g_span(r.rec, 6);
    t7 = g_span(r.rec, 7);
  }
  if ((rf & ROW_AA) != 0u) {
    t8 = g_span(r.rec, 8);
    t9 = g_span(r.rec, 9);
  }
  for (int i = 0; i < G_K; i++) {
    if (!c[i]) continue;
    int x = x0 + i;
    float fx = float(x);
    bool m = (rf & ROW_ALLEDGE) != 0u || (fx >= t5.x && fx <= t5.y) || (fx >= t5.z && fx <= t5.w);
    if ((rf & ROW_MASK2) != 0u)
      m = m || (fx >= t6.x && fx <= t6.y) || (fx >= t6.z && fx <= t6.w) || (fx >= t7.x && fx <= t7.y) || (fx >= t7.z && fx <= t7.w);
    if ((rf & ROW_WIRE) != 0u && !m) continue;
    uint cv = (rf & ROW_COV0) != 0u ? 0u : 31u;
    if ((rf & ROW_AA) != 0u) {
      uint a = 31u;
      int xa = (rf & ROW_DSGRID) != 0u ? x / int(rf >> 8) : x; // G9: a DS-grid row's windows are in DS pixels
      if ((rf & ROW_WIRE) == 0u) {
        a = g_aa_side(xa, t6, t7, true);
        if (!((rf & ROW_VERTL) != 0u && float(xa) == t6.w)) a = min(a, g_aa_side(xa, t8, t9, false));
      }
      if (a < 31u) m = true;
      cv = (rf & ROW_COV0) != 0u ? 0u : a;
    }
    hit[s0 + i] = true;
    depth[s0 + i] = d[i];
    edge[s0 + i] = m;
    cov[s0 + i] = cv;
  }
}

// the vertex colour (6 bits) and texture coordinates at x of row r. With multisampling (G_K > 1) x is the pixel's first sample
// and r the first of its rows the polygon covers, and the fraction is clamped to the span (a pixel whose first sample lies
// before the polygon's start takes its start), as vcore p21 shades a supersampled pixel; without it the clamp is vcore's
// antialiasing one (a span longer than den_f, a pixel before xs).
void g_shade(GRow r, int x, out uvec3 rgb, out ivec2 st) {
  vec4 c0 = r.t0, c1 = r.t1, c4 = r.t4;
  float cnf = float(x) - c4.w;
  bool whole_f = cnf == floor(cnf) && c0.z == floor(c0.z) && c0.z >= 1.0 && c0.z < 65536.0 && abs(cnf) < 65536.0;
  int n = int(cnf), df = int(c0.z);
  float nfc = cnf;
  if (G_K > 1 || (uint(c4.z) & ROW_AA) != 0u) {
    nfc = clamp(cnf, 0.0, c0.z);
    n = clamp(n, 0, df);
  }
  float f = nfc / c0.z;
  bool persp = c1.x != c1.y;
  float q = f;
  if (persp) {
    float k0 = (1.0 - f) / c1.x, k1 = f / c1.y;
    q = k1 / (k0 + k1);
  }
  vec4 lo1 = g_span(r.rec, 11); // the low parts: tL, tR, sL, sR
  vec4 t2 = g_span(r.rec, 2), t3 = g_span(r.rec, 3);
  vec2 sa = vec2(t2.w, c4.x), sb = vec2(t3.w, c4.y);
  ivec3 ci;
  ivec2 si;
  if (!persp && whole_f) {
    float fd = c0.z, fq = float(n) / fd;
    ci = ivec3(g_lini(t2.x, t3.x, n, df, fq), g_lini(t2.y, t3.y, n, df, fq), g_lini(t2.z, t3.z, n, df, fq));
    si = ivec2(g_lin(sa.x, lo1.z, sb.x, lo1.w, n, df, 1e-6, fd, fq), g_lin(sa.y, lo1.x, sb.y, lo1.y, n, df, 1e-6, fd, fq));
  } else {
    vec3 c = t2.xyz + (t3.xyz - t2.xyz) * q;
    vec2 ia = floor(sa);
    vec2 s = (sa - ia) + vec2(lo1.z, lo1.x) + ((sb - sa) + vec2(lo1.w - lo1.z, lo1.y - lo1.x)) * q;
    ci = ivec3(floor(c + 1e-4));
    si = ivec2(ia) + ivec2(floor(s + 1e-5));
  }
  rgb = uvec3(clamp(ci >> 3, ivec3(0), ivec3(63)));
  st = si;
}

#if G_K == 2
bvec2 g_or2(bvec2 a, bvec2 b) { return bvec2(uvec2(a) | uvec2(b)); }
bvec2 g_in2(vec2 f, vec2 r) { return bvec2(uvec2(greaterThanEqual(f, r.xx)) & uvec2(lessThanEqual(f, r.yy))); }

// g_aa_side of xa and xb (xb = xa or xa + 1) of one window: the record's parts and N's constant once, nothing when neither
// lies in the window
uvec2 g_aa_side2(int xa, int xb, vec4 a, vec4 b, bool left) {
  bvec2 in2 = g_in2(vec2(float(xa), float(xb)), a.xy);
  if (!any(in2)) return uvec2(31u);
  int code = int(a.z), sh = code >> 6, c0 = (code & 63) - 1;
  uvec2 r;
  if (c0 >= 0) {
    r = uvec2(c0);
  } else {
    int ax = int(a.w), k = int(b.x), dx = int(b.y), dy = int(b.z);
    int adx = abs(dx), sg = dx > 0 ? 1 : -1;
    int Na = (2 * (xa - sh) + 1 - 2 * ax) * dy - sg * adx * (2 * k + 1), Nb = Na + 2 * (xb - xa) * dy;
    ivec2 c;
    if (left) c = ivec2(-g_fdiv_near(-(16 * adx + 16 * Na), adx, -33.0, 0.0) - 1, -g_fdiv_near(-(16 * adx + 16 * Nb), adx, -33.0, 0.0) - 1);
    else c = ivec2(g_fdiv_near(16 * adx - 16 * Na, adx, -1.0, 32.0), g_fdiv_near(16 * adx - 16 * Nb, adx, -1.0, 32.0));
    r = uvec2(clamp(c, 0, 31));
  }
  return uvec2(in2.x ? r.x : 31u, in2.y ? r.y : 31u);
}

// g_cover_row's edge step for samples x and x + 1 of a row: bit 0 the edge flag, bits 1-5 the coverage, bit 31 not drawn (a
// wire-frame's pixel off the mask). A window's parts and N's constant once for the two (g_aa_side2): M2 2026-11, town at 3x
// with 4 samples, Metal R+S 1.65 -> 1.57 ms
uvec2 g_edge2(int x, uint rf, vec4 t5, vec4 t6, vec4 t7, vec4 t8, vec4 t9) {
  vec2 fx = vec2(float(x), float(x + 1));
  bvec2 m = bvec2((rf & ROW_ALLEDGE) != 0u);
  m = g_or2(m, g_in2(fx, t5.xy));
  m = g_or2(m, g_in2(fx, t5.zw));
  if ((rf & ROW_MASK2) != 0u) {
    m = g_or2(m, g_in2(fx, t6.xy));
    m = g_or2(m, g_in2(fx, t6.zw));
    m = g_or2(m, g_in2(fx, t7.xy));
    m = g_or2(m, g_in2(fx, t7.zw));
  }
  uvec2 cv = uvec2((rf & ROW_COV0) != 0u ? 0u : 31u);
  if ((rf & ROW_AA) != 0u) {
    uvec2 a = uvec2(31u);
    bool ds = (rf & ROW_DSGRID) != 0u; // G9: a DS-grid row's windows are in DS pixels
    int xa = ds ? x / int(rf >> 8) : x, xb = ds ? (x + 1) / int(rf >> 8) : x + 1;
    if ((rf & ROW_WIRE) == 0u) {
      a = g_aa_side2(xa, xb, t6, t7, true);
      uvec2 r = g_aa_side2(xa, xb, t8, t9, false);
      bool vl = (rf & ROW_VERTL) != 0u;
      a = uvec2(vl && float(xa) == t6.w ? a.x : min(a.x, r.x), vl && float(xb) == t6.w ? a.y : min(a.y, r.y));
    }
    m = g_or2(m, lessThan(a, uvec2(31u)));
    cv = (rf & ROW_COV0) != 0u ? uvec2(0u) : a;
  }
  uvec2 e = uvec2(m) | (cv << 1);
  if ((rf & ROW_WIRE) != 0u) e = uvec2(m.x ? e.x : 0x80000000u, m.y ? e.y : 0x80000000u);
  return e;
}

// g_cover_row's exact depth of sample x on a row
uint g_depth_px(int x, vec4 t0, vec4 t1, vec4 t4, vec4 dlo) {
  float fx = float(x), nf = fx - t4.w;
  bool whole_z = nf == floor(nf) && t0.w == floor(t0.w) && t0.w >= 1.0 && t0.w < 65536.0 && abs(nf) < 65536.0;
  float fz = nf / t0.w, dd;
  if ((uSwap & 2u) != 0u) { // the w-buffer: depth is w, the sample's own fraction
    float f = nf;
    if ((uint(t4.z) & ROW_AA) != 0u) f = clamp(nf, 0.0, t0.z);
    f /= t0.z;
    dd = t1.x;
    if (t1.x != t1.y) dd = 1.0 / ((1.0 - f) / t1.x + f / t1.y);
  } else if (whole_z) {
    return uint(clamp(g_lin(t1.z, dlo.x, t1.w, dlo.y, int(nf), int(t0.w), 0.5, t0.w, fz), 0, 0xFFFFFF));
  } else {
    dd = t1.z + (t1.w - t1.z) * fz;
  }
  return dd < 0.0 ? 0u : (dd > 16777215.0 ? 0xFFFFFFu : uint(dd + 0.5));
}

// g_depth_px of samples x and x + 1 of a row: g_lin's row constants (the ends' whole and fractional parts, dz's quotient and
// remainder by den_z) once, and the second sample's quotient from the first one's remainder (m + dr = qq d + rem + dr, the two
// remainders below d); the same values as two g_depth_px (the float sum is the same expression)
uvec2 g_depth2(int x, vec4 t0, vec4 t1, vec4 t4, vec4 dlo) {
  float nf = float(x) - t4.w, nf1 = float(x + 1) - t4.w;
  bool whole_z = nf == floor(nf) && t0.w == floor(t0.w) && t0.w >= 1.0 && t0.w < 65536.0 && abs(nf) < 65536.0 && abs(nf1) < 65536.0;
  if ((uSwap & 2u) != 0u || !whole_z) return uvec2(g_depth_px(x, t0, t1, t4, dlo), g_depth_px(x + 1, t0, t1, t4, dlo));
  float a = t1.z, b = t1.w, fd = t0.w;
  int d = int(t0.w), n = int(nf);
  int ai = int(floor(a)), bi = int(floor(b));
  float fa = (a - float(ai)) + dlo.x, fb = (b - float(bi)) + dlo.y;
  int dz = bi - ai;
  int dq = g_fixq(dz, d, float(dz) / fd);
  int dr = dz - dq * d;
  float fq = nf / t0.w, fq1 = nf1 / t0.w;
  int m = dr * n;
  int qq = g_fixq(m, d, float(dr) * fq);
  int rem = m - qq * d;
  int rem1 = rem + dr, up = rem1 >= d ? 1 : 0;
  int qq1 = qq + up;
  rem1 -= up * d;
  float small = float(rem) / fd + fa + (fb - fa) * fq + 0.5;
  float small1 = float(rem1) / fd + fa + (fb - fa) * fq1 + 0.5;
  int z0 = ai + dq * n + qq + int(floor(small)), z1 = ai + dq * (n + 1) + qq1 + int(floor(small1));
  return uvec2(clamp(ivec2(z0, z1), 0, 0xFFFFFF));
}

vec4 g_ab(float a, float b) { return vec4(a, a, b, b); }
bvec4 g_and(bvec4 a, bvec4 b) { return bvec4(uvec4(a) & uvec4(b)); }

// g_cover_row for both sample rows at once (lanes x, y: row a; z, w: row b), the depth step's fetches for both rows together, so
// that the second row adds no wait of its own. oka / okb: the row's record applies (g_row).
void g_cover2(GRow ra, bool oka, GRow rb, bool okb, int x0, bool early, uvec4 old, uint attr, out bvec4 hit,
              out uvec4 depth, out bvec4 edge, out uvec4 cov) {
  hit = bvec4(false);
  edge = bvec4(false);
  depth = uvec4(0u);
  cov = uvec4(31u);
  vec4 fx = vec4(float(x0), float(x0 + 1), float(x0), float(x0 + 1));
  bvec4 c = g_and(bvec4(oka, oka, okb, okb), g_and(greaterThanEqual(fx, g_ab(ra.t0.x, rb.t0.x)), lessThanEqual(fx, g_ab(ra.t0.y, rb.t0.y))));
  if (early && (uSwap & 2u) == 0u) { // the cheap float bound on the depth first (g_cover_row)
    vec4 z0 = g_ab(ra.t1.z, rb.t1.z), z1 = g_ab(ra.t1.w, rb.t1.w);
    vec4 da = z0 + (z1 - z0) * ((fx - g_ab(ra.t4.w, rb.t4.w)) / g_ab(ra.t0.w, rb.t0.w)), sd = vec4(old);
    c = g_and(c, (attr & 0x4000u) != 0u ? lessThanEqual(abs(da - sd), vec4(520.0)) : lessThanEqual(da - sd, vec4(8.0)));
  }
  if (!any(c)) return;
  vec4 dla = g_span(ra.rec, 10), dlb = g_span(rb.rec, 10);
  uvec4 d = uvec4(g_depth2(x0, ra.t0, ra.t1, ra.t4, dla), g_depth2(x0, rb.t0, rb.t1, rb.t4, dlb));
  if (early) c = g_and(c, (attr & 0x4000u) != 0u ? lessThanEqual(max(d, old) - min(d, old), uvec4(0x200u)) : lessThan(d, old));
  if (!any(c)) return;
  // the edge steps row by row: a row's windows are live for its own two samples only (with both rows' ten texels at once, this
  // was pass R's register peak: M2 2026-11, town at 3x with 4 samples, Metal 2.19 -> 1.89 ms, GL 2.63 -> 2.34)
  uvec4 e;
  for (int j = 0; j < 2; j++) {
    int rec = j == 0 ? ra.rec : rb.rec;
    uint rf = uint(j == 0 ? ra.t4.z : rb.t4.z);
    vec2 in2 = j == 0 ? dla.zw : dlb.zw;
    uvec2 ej;
    if (float(x0) >= in2.x && float(x0 + 1) <= in2.y) { // both in the inner run (g02_spans.h t10): no edge flag, full coverage
      bool m = (rf & ROW_ALLEDGE) != 0u;
      ej = uvec2((rf & ROW_WIRE) != 0u && !m ? 0x80000000u : (m ? 1u : 0u) | ((rf & ROW_COV0) != 0u ? 0u : 62u));
    } else {
      vec4 t5 = g_span(rec, 5), t6 = vec4(0.0), t7 = vec4(0.0), t8 = vec4(0.0), t9 = vec4(0.0);
      if ((rf & (ROW_MASK2 | ROW_AA)) != 0u) {
        t6 = g_span(rec, 6);
        t7 = g_span(rec, 7);
      }
      if ((rf & ROW_AA) != 0u) {
        t8 = g_span(rec, 8);
        t9 = g_span(rec, 9);
      }
      ej = g_edge2(x0, rf, t5, t6, t7, t8, t9);
    }
    if (j == 0) e.xy = ej;
    else e.zw = ej;
  }
  hit = g_and(c, lessThan(e, uvec4(0x80000000u)));
  depth = d;
  edge = g_and(hit, notEqual(e & 1u, uvec4(0u)));
  cov = (e >> 1) & 31u;
}
#endif
