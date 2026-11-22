// port/gpu 2D upscaling, the MMPX style (g11_up.glsl is the framework). OWNED BY THE 2D FILTER.
// MMPX: Morgan McGuire and Mara Gagiu, "MMPX Style-Preserving Pixel-Art Magnification", Journal of Computer Graphics
// Techniques 10(2), 2021 (https://casual-effects.com/research/McGuire2021PixelArt/). The rules below follow the authors'
// reference code: Copyright 2020 Morgan McGuire & Mara Gagiu, available under the MIT license (licenses/MMPX-MIT.txt).
// Rewritten for this framework: the key image instead of RGBA colours, positions (which neighbour) instead of colours.
//
// Each source pixel E becomes four quarters J (top-left), K, L, M (bottom-right), each one of E, B (up), D (left), F
// (right), H (down), chosen by exact-equality rules over E's 5 x 5 cross (out to 3 along the axes) and a luma order (the
// darker side wins a tie: thin dark lines stay whole). Keys are compared exactly; the luma of a colour key is r + g + b, of
// the "3D on top" sentinel 256 (brighter than any colour: the 3D acts as the background, the 2D's shapes are kept against it).
// Stages: N = 2: one stage (UP_MMPX, g11_up.frag at 1x: the four quarters). N >= 3: MMPX twice (4x): UP_MMPX writes the
// first stage into a 256 x 384 texture, UP_MMPX2 (also at 1x) runs MMPX on the 2x picture that first stage describes, for
// the DS pixel's four 2x pixels, and stores the 16 4x cells' DS pixels; an N x pixel takes the 4x cell its centre falls in
// (N = 3: cells 0, 2, 3 of each row and column: a nearest resize of the 4x picture).
// Info: picks are 6 bits, (dx + 2) | (dy + 2) << 3, relative to the DS pixel, in -2..2. N = 2: x bits 0-23 the four
// quarters (top-left, top-right, bottom-left, bottom-right); N >= 3: x, y, z the 16 cells (row-major, pick k at bits 6k of
// the 96); w the key (gpu_render --up-info).
#if defined(UP_MMPX) || defined(UP_MMPX2)
uint mm_luma(uint k) { return k == UP_K3D ? 256u : (k & 63u) + ((k >> 8) & 63u) + ((k >> 16) & 63u); }
bool mm_any3(uint e, uint a, uint b, uint c) { return e == a || e == b || e == c; }
bool mm_all2(uint e, uint a, uint b) { return e == a && e == b; }
bool mm_all3(uint e, uint a, uint b, uint c) { return e == a && e == b && e == c; }
bool mm_all4(uint e, uint a, uint b, uint c, uint d) { return e == a && e == b && e == c && e == d; }
bool mm_none2(uint e, uint a, uint b) { return e != a && e != b; }
bool mm_none4(uint e, uint a, uint b, uint c, uint d) { return e != a && e != b && e != c && e != d; }

// a quarter's choice (0 E, 1 B, 2 D, 3 F, 4 H) as an offset; a pick's 6-bit code and back
ivec2 mm_dir(int c) { return c == 1 ? ivec2(0, -1) : c == 2 ? ivec2(-1, 0) : c == 3 ? ivec2(1, 0) : c == 4 ? ivec2(0, 1) : ivec2(0); }
uint mm_code(ivec2 o) { return uint(o.x + 2) | (uint(o.y + 2) << 3); }
ivec2 mm_off(uint v) { return ivec2(int(v & 7u) - 2, int((v >> 3) & 7u) - 2); }

#ifdef UP_MMPX2
uniform highp usampler2D uUpInfo1; // the first stage (UP_MMPX's info), 256 x 384
int mm_y0;                         // the screen's first row in the info textures
// the DS pixel (within the screen) that 2x pixel P (within the screen, clamped to it) shows after the first stage
ivec2 mm_pos(ivec2 P) {
  P = clamp(P, ivec2(0), ivec2(511, 383));
  ivec2 d = P >> 1;
  uint v = texelFetch(uUpInfo1, ivec2(d.x, d.y + mm_y0), 0).x >> uint(6 * ((P.x & 1) + 2 * (P.y & 1)));
  return clamp(d + mm_off(v & 63u), ivec2(0), ivec2(255, 191));
}
uint mm_src(ivec2 p, int dx, int dy) {
  ivec2 s = mm_pos(p + ivec2(dx, dy));
  return texelFetch(uUpInfo1, ivec2(s.x, s.y + mm_y0), 0).w;
}
#else
uint mm_src(ivec2 p, int dx, int dy) { return up_keyat(p, dx, dy); }
#endif

// MMPX at p (a DS pixel in engine order for the first stage, a 2x pixel within its screen for the second): J, K, L, M
ivec4 mm_run(ivec2 p) {
  uint A = mm_src(p, -1, -1), B = mm_src(p, 0, -1), C = mm_src(p, 1, -1);
  uint D = mm_src(p, -1, 0), E = mm_src(p, 0, 0), F = mm_src(p, 1, 0);
  uint G = mm_src(p, -1, 1), H = mm_src(p, 0, 1), I = mm_src(p, 1, 1);
  if (((A ^ E) | (B ^ E) | (C ^ E) | (D ^ E) | (F ^ E) | (G ^ E) | (H ^ E) | (I ^ E)) == 0u) return ivec4(0);
  int J = 0, K = 0, L = 0, M = 0;
  uint P = mm_src(p, 0, -2), S = mm_src(p, 0, 2), Q = mm_src(p, -2, 0), R = mm_src(p, 2, 0);
  uint Bl = mm_luma(B), Dl = mm_luma(D), El = mm_luma(E), Fl = mm_luma(F), Hl = mm_luma(H);
  // 1:1 slopes
  if ((D == B && D != H && D != F) && (El >= Dl || E == A) && mm_any3(E, A, C, G) && (El < Dl || A != D || E != P || E != Q)) J = 2;
  if ((B == F && B != D && B != H) && (El >= Bl || E == C) && mm_any3(E, A, C, I) && (El < Bl || C != B || E != P || E != R)) K = 1;
  if ((H == D && H != F && H != B) && (El >= Hl || E == G) && mm_any3(E, A, G, I) && (El < Hl || G != H || E != S || E != Q)) L = 4;
  if ((F == H && F != B && F != D) && (El >= Fl || E == I) && mm_any3(E, C, G, I) && (El < Fl || I != H || E != R || E != S)) M = 3;
  // intersections
  if ((E != F && mm_all4(E, C, I, D, Q) && mm_all2(F, B, H)) && F != mm_src(p, 3, 0)) K = M = 3;
  if ((E != D && mm_all4(E, A, G, F, R) && mm_all2(D, B, H)) && D != mm_src(p, -3, 0)) J = L = 2;
  if ((E != H && mm_all4(E, G, I, B, P) && mm_all2(H, D, F)) && H != mm_src(p, 0, 3)) L = M = 4;
  if ((E != B && mm_all4(E, A, C, H, S) && mm_all2(B, D, F)) && B != mm_src(p, 0, -3)) J = K = 1;
  if (Bl < El && mm_all4(E, G, H, I, S) && mm_none4(E, A, D, C, F)) J = K = 1;
  if (Hl < El && mm_all4(E, A, B, C, P) && mm_none4(E, D, G, I, F)) L = M = 4;
  if (Fl < El && mm_all4(E, A, D, G, Q) && mm_none4(E, B, C, I, H)) K = M = 3;
  if (Dl < El && mm_all4(E, C, F, I, R) && mm_none4(E, B, A, G, H)) J = L = 2;
  // 2:1 slopes
  if (H != B) {
    if (H != A && H != E && H != C) {
      if (mm_all3(H, G, F, R) && mm_none2(H, D, mm_src(p, 2, -1))) L = M;
      if (mm_all3(H, I, D, Q) && mm_none2(H, F, mm_src(p, -2, -1))) M = L;
    }
    if (B != I && B != G && B != E) {
      if (mm_all3(B, A, F, R) && mm_none2(B, D, mm_src(p, 2, 1))) J = K;
      if (mm_all3(B, C, D, Q) && mm_none2(B, F, mm_src(p, -2, 1))) K = J;
    }
  }
  if (F != D) {
    if (D != I && D != E && D != C) {
      if (mm_all3(D, A, H, S) && mm_none2(D, B, mm_src(p, 1, 2))) J = L;
      if (mm_all3(D, G, B, P) && mm_none2(D, H, mm_src(p, 1, -2))) L = J;
    }
    if (F != E && F != A && F != G) {
      if (mm_all3(F, C, H, S) && mm_none2(F, B, mm_src(p, -1, 2))) K = M;
      if (mm_all3(F, I, B, P) && mm_none2(F, H, mm_src(p, -1, -2))) M = K;
    }
  }
  return ivec4(J, K, L, M);
}

#ifdef UP_MMPX2
// pick k (0-15) of the 96 bits x, y, z
void mm_put(inout uvec3 r, int k, uint v) {
  int b = 6 * k, sh = b & 31;
  if ((b >> 5) == 0) r.x |= v << uint(sh);
  else if ((b >> 5) == 1) r.y |= v << uint(sh);
  else r.z |= v << uint(sh);
  if (sh > 26) {
    if ((b >> 5) == 0) r.y |= v >> uint(32 - sh);
    else r.z |= v >> uint(32 - sh);
  }
}
// the second stage at DS pixel p: its four 2x pixels, each MMPX over the first stage's 2x picture
uvec4 up_analyse(ivec2 p) {
  mm_y0 = p.y < 192 ? 0 : 192;
  ivec2 d = ivec2(p.x, p.y - mm_y0);
  uvec3 r = uvec3(0u);
  for (int j = 0; j < 2; j++)
    for (int i = 0; i < 2; i++) {
      ivec2 P = 2 * d + ivec2(i, j);
      ivec4 c = mm_run(P);
      mm_put(r, (2 * j) * 4 + 2 * i, mm_code(mm_pos(P + mm_dir(c.x)) - d));
      mm_put(r, (2 * j) * 4 + 2 * i + 1, mm_code(mm_pos(P + mm_dir(c.y)) - d));
      mm_put(r, (2 * j + 1) * 4 + 2 * i, mm_code(mm_pos(P + mm_dir(c.z)) - d));
      mm_put(r, (2 * j + 1) * 4 + 2 * i + 1, mm_code(mm_pos(P + mm_dir(c.w)) - d));
    }
  return uvec4(r, texelFetch(uUpInfo1, p, 0).w);
}
#else
// the first stage (N = 2: the info itself)
uvec4 up_analyse(ivec2 p) {
  ivec4 c = mm_run(p);
  uint x = mm_code(mm_dir(c.x)) | (mm_code(mm_dir(c.y)) << 6) | (mm_code(mm_dir(c.z)) << 12) | (mm_code(mm_dir(c.w)) << 18);
  return uvec4(x, 0u, 0u, up_key(p));
}
// pass C: the quarter (N = 2) or the 4x cell (N >= 3: cell = floor(4 f / 2N), without a division) of the N x pixel
UpPick up_pick(uvec4 info, ivec2 f, int N) {
  int k;
  if (N == 2) k = (f.x >= N ? 1 : 0) + (f.y >= N ? 2 : 0);
  else {
    ivec2 f2 = 2 * f;
    int cx = (f2.x >= N ? 1 : 0) + (f2.x >= 2 * N ? 1 : 0) + (f2.x >= 3 * N ? 1 : 0);
    int cy = (f2.y >= N ? 1 : 0) + (f2.y >= 2 * N ? 1 : 0) + (f2.y >= 3 * N ? 1 : 0);
    k = cy * 4 + cx;
  }
  int b = 6 * k, sh = b & 31, wd = b >> 5;
  uint v = (wd == 0 ? info.x : wd == 1 ? info.y : info.z) >> uint(sh);
  if (sh > 26) v |= (wd == 0 ? info.y : info.z) << uint(32 - sh);
  return UpPick(mm_off(v & 63u), ivec2(0), 0);
}
#endif
#endif
