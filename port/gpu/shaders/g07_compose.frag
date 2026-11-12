// port/gpu pass C: engine A's compositing at N x, and the screens (docs/gpu.md, G7). OWNED BY G7.
// p09's compositor (vc_compose_line) and p10's display (mode 1, master brightness) on integers: the 2D layers at the DS
// pixel (x / N, y / N), the 3D layer at the N x pixel. Engine A's display modes 0/2/3 and engine B are vcore's 1x lines
// (cSrc1x), nearest. With SMAA the 3D layer goes through SMAA's last pass here (2026-11-02). Out: RGBA8, both screens,
// 256 N x 384 N, the top screen first.
// Two programs (2026-11-04, docs/gpu-cost.md): what the 2D layers decide does not change within a DS pixel, so this file
// with C_PLAN defined works it out once a DS pixel, at 1x, into the plan (cPlan): the picture there without the 3D layer,
// and what the 3D layer's pixel does where it is not transparent. The N x pass reads the plan and the 3D layer. The plan
// is made once a pack: a frame's in-between pictures reuse it.
#ifdef C_PLAN
uniform highp usampler2D cStage; // 256 x 192: x = OBJ pixel, y = BG0 | BG1 << 16, z = BG2 | BG3 << 16 (BGR555 | opaque
                                 // bit 15), w = window mask | bg_on << 8 | bg3d << 12
layout(location = 0) out uvec2 oPlan;
#else
uniform highp usampler2D cPlan;  // 256 x 192, C_PLAN's (c_plan)
uniform highp usampler2D c3d;    // the 3D layer, RGBA8UI (r6, g6, b6, a5), cScale3d x the DS size
uniform highp usampler2D cSrc1x; // 256 x 384: rows 0-191 engine A's line (when it is not composed), 192-383 engine B's
uniform sampler2D cSmaa;         // SMAA's blending weights (RGBA8, the 3D layer's size) when cSmaaOn is 1
layout(location = 0) out vec4 oColor;
#endif
// cScale, cScale3d, cHave3d, cSmaaOn, cCompose, cATop, cObjOn, cPrios, cBld, cEva, cEvb, cEvy, cBackdrop, cHofs, cBright:
// g00_head.glsl's block

const uint CP_OPAQUE = 0x8000u;
const uint CP_SEMI = 1u << 18, CP_BMP = 1u << 19;
const int CK_OBJ = 4, CK_BD = 5;

struct CCand { // a 2D layer's pixel (C_PLAN's)
  int kind;   // 0-3 BG, 4 OBJ, 5 backdrop, -1 none
  uint color; // RGB6
  uint px;    // the OBJ pixel (flags)
};

uint c_rgb6(uint c555) {
  return ((c555 & 31u) << 1) | (((c555 >> 5) & 31u) << 9) | (((c555 >> 10) & 31u) << 17);
}
uint c_mk(int r, int g, int b) { return uint(r) | (uint(g) << 8) | (uint(b) << 16); }
int c_r(uint c) { return int(c & 63u); }
int c_g(uint c) { return int((c >> 8) & 63u); }
int c_b(uint c) { return int((c >> 16) & 63u); }

uint c_blend_alpha(uint c1, uint c2, int eva, int evb) {
  return c_mk(min(63, (c_r(c1) * eva + c_r(c2) * evb + 8) >> 4), min(63, (c_g(c1) * eva + c_g(c2) * evb + 8) >> 4),
              min(63, (c_b(c1) * eva + c_b(c2) * evb + 8) >> 4));
}
uint c_blend_bright(uint c, int evy) {
  int r = c_r(c), g = c_g(c), b = c_b(c);
  return c_mk(r + (((63 - r) * evy + 8) >> 4), g + (((63 - g) * evy + 8) >> 4), b + (((63 - b) * evy + 8) >> 4));
}
uint c_blend_dark(uint c, int evy) {
  int r = c_r(c), g = c_g(c), b = c_b(c);
  return c_mk(r - ((r * evy + 7) >> 4), g - ((g * evy + 7) >> 4), b - ((b * evy + 7) >> 4));
}
uint c_blend_3d(uint c1, uint c2, int a) {
  return c_mk((c_r(c1) * (a + 1) + c_r(c2) * (31 - a) + 16) >> 5, (c_g(c1) * (a + 1) + c_g(c2) * (31 - a) + 16) >> 5,
              (c_b(c1) * (a + 1) + c_b(c2) * (31 - a) + 16) >> 5);
}

// MASTER_BRIGHT (p10): up v + (63 - v) F / 16 rounded down, down v - (v F + 15) / 16
uint c_bright(uint px) {
  uint bm = (cBright >> 14) & 3u;
  int f = min(int(cBright & 31u), 16);
  if ((bm != 1u && bm != 2u) || f == 0) return px;
  int r = c_r(px), g = c_g(px), b = c_b(px);
  if (bm == 1u) return c_mk(r + (((63 - r) * f) >> 4), g + (((63 - g) * f) >> 4), b + (((63 - b) * f) >> 4));
  return c_mk(r - ((r * f + 15) >> 4), g - ((g * f + 15) >> 4), b - ((b * f + 15) >> 4));
}

#ifndef C_PLAN
// the 3D layer at the N x pixel (X, Y): the 512 pixel wide BG0, scrolled by BG0HOFS times N; 0 = transparent. Bits 29-31:
// the pixel's empty part in eighths where antialiasing left it partly covered (4 samples' resolve, SMAA's blending)
uint c_3d(ivec2 p) {
  if (cHave3d == 0u) return 0u;
  int W = 256 * cScale;
  int pos = p.x + cHofs * cScale; // (p.x + cHofs N) mod 2W: below 3W (p.x < W, cHofs < 512); no division (2W is no power
  if (pos >= 2 * W) pos -= 2 * W;   // of two at N = 3, 5, 6, 7, and an integer division costs ~10% of the pass on Apple GPUs)
  if (pos >= W) return 0u;
  ivec2 q = cScale3d == cScale ? ivec2(pos, p.y) : ivec2(pos * cScale3d / cScale, p.y * cScale3d / cScale);
  uvec4 t = cSmaaOn != 0u ? smaa_blend(c3d, cSmaa, q) : texelFetch(c3d, q, 0);
  uint a = t.a & 31u;
  if (a == 0u) return 0u;
  return (t.r & 63u) | ((t.g & 63u) << 8) | ((t.b & 63u) << 16) | (a << 24) | (((t.a >> 5) & 7u) << 29);
}
#endif

#ifdef C_PLAN
CCand c_cand(int slot, uvec4 st, uint objpx) {
  if (slot == 0) return CCand(CK_OBJ, c_rgb6(objpx & 0x7FFFu), objpx);
  int n = slot - 1;
  uint w = n < 2 ? st.y : st.z;
  uint px = (n & 1) == 0 ? (w & 0xFFFFu) : (w >> 16);
  return CCand(n, c_rgb6(px & 0x7FFFu), px);
}

// BLDCNT's effect on the top layer over the one beneath (kind k2, -1 none) where neither is the 3D layer's: 1 alpha
// blending with the weights wa, wb; 2 brightness up, 3 down (EVY); 0 none
uint c_effect(CCand top, int k2, bool fx, out int wa, out int wb) {
  bool tgt1 = ((cBld >> uint(top.kind)) & 1u) != 0u;
  bool tgt2 = k2 >= 0 && ((cBld >> uint(8 + k2)) & 1u) != 0u;
  uint mode = (cBld >> 6) & 3u;
  bool semi = top.kind == CK_OBJ && (top.px & (CP_SEMI | CP_BMP)) != 0u;
  wa = cEva;
  wb = cEvb;
  if (semi && tgt2) {
    if ((top.px & CP_BMP) != 0u) {
      int a = int((top.px >> 24) & 15u);
      wa = a + 1;
      wb = 16 - (a + 1);
    }
    return 1u;
  }
  if (!(fx && tgt1)) return 0u;
  if (mode == 1u) return tgt2 ? 1u : 0u;
  return mode;
}
uint c_apply(uint e, uint top, uint sec, int wa, int wb) {
  if (e == 1u) return c_blend_alpha(top, sec, wa, wb);
  if (e == 2u) return c_blend_bright(top, cEvy);
  if (e == 3u) return c_blend_dark(top, cEvy);
  return top;
}

// The plan of the DS pixel d (RG32UI). x: bits 0-23 the picture without the 3D layer (RGB6, master brightness applied),
// 24-25 what the 3D layer does where it is not transparent: 0 nothing (not a candidate, below the top two, or covered by
// a 2D layer that does not blend with it), 1 it is the top layer, 2 it is the second. y for 1: bits 0-23 the layer beneath
// it (RGB6), 24-25 2 or 3 brightness up or down on the 3D, 1 the 3D blended with the layer beneath by its alpha (else it
// is drawn opaque: an antialiased edge then shows x's picture in its empty part). y for 2: alpha blending of the top layer
// (bits 0-23) with the 3D, 24-25 = 1, 26-30 the top's weight, x's 26-30 the 3D's; else bits 0-23 the picture.
void main() {
  ivec2 d = ivec2(gl_FragCoord.xy);
  uvec4 st = texelFetch(cStage, d, 0);
  uint mask = st.w & 0xFFu;
  uint bgon = (st.w >> 8) & 15u;
  bool bg3d = ((st.w >> 12) & 1u) != 0u, fx = (mask & 32u) != 0u;
  uint objpx = (cObjOn != 0u && (mask & 16u) != 0u) ? st.x : 0u;
  int k1 = 99, k2 = 99, k3d = 99; // the top two 2D layers' keys (priority, then OBJ before BG0-3), the 3D layer's
  if ((objpx & CP_OPAQUE) != 0u && !((objpx & CP_BMP) != 0u && ((objpx >> 24) & 15u) == 0u))
    k1 = int((objpx >> 16) & 3u) * 8;
  for (int n = 0; n < 4; n++) {
    if (((bgon >> uint(n)) & 1u) == 0u || (mask & (1u << uint(n))) == 0u) continue;
    int k = int((cPrios >> uint(2 * n)) & 3u) * 8 + n + 1;
    if (n == 0 && bg3d) {
      k3d = k;
      continue;
    }
    uint w = n < 2 ? st.y : st.z;
    uint px = (n & 1) == 0 ? (w & 0xFFFFu) : (w >> 16);
    if ((px & CP_OPAQUE) == 0u) continue;
    if (k < k1) { k2 = k1; k1 = k; }
    else if (k < k2) k2 = k;
  }
  CCand bd = CCand(CK_BD, cBackdrop, 0u);
  // if/else, not ?: : GLSL ES 3.00 lets a compiler refuse ?: on a struct (ANGLE and SwiftShader do; docs/gpu-compat.md)
  CCand top = bd, sec = CCand(-1, 0u, 0u);
  if (k1 < 99) top = c_cand(k1 & 7, st, objpx);
  if (k2 < 99) sec = c_cand(k2 & 7, st, objpx);
  else if (k1 < 99) sec = bd;
  int wa, wb;
  uint c0 = c_bright(c_apply(c_effect(top, sec.kind, fx, wa, wb), top.color, sec.color, wa, wb));
  uvec2 o = uvec2(c0, 0u);
  if (k3d < k1) { // the 3D on top of `top`
    bool t2 = ((cBld >> uint(8 + top.kind)) & 1u) != 0u;
    uint e = t2 ? 1u : c_effect(CCand(0, 0u, 0u), -1, fx, wa, wb);
    o = uvec2(c0 | (1u << 24), top.color | (e << 24));
  } else if (k3d < k2) { // the 3D beneath `top`
    uint e = c_effect(top, 0, fx, wa, wb);
    if (e == 1u) o = uvec2(c0 | (2u << 24) | (uint(wb) << 26), top.color | (1u << 24) | (uint(wa) << 26));
    else {
      uint c = c_bright(c_apply(e, top.color, 0u, wa, wb));
      if (c != c0) o = uvec2(c0 | (2u << 24), c);
    }
  }
  oPlan = o;
}
#else
// the N x pixel q of engine A's screen (DS pixel d) from the plan and the 3D layer
uint c_planned(ivec2 d, ivec2 q) {
  uvec2 pl = texelFetch(cPlan, d, 0).xy;
  uint c0 = pl.x & 0xFFFFFFu, rank = (pl.x >> 24) & 3u;
  if (rank == 0u) return c0;
  uint l3d = c_3d(q);
  int a = int((l3d >> 24) & 31u);
  if (a == 0) return c0;
  uint t = l3d & 0xFFFFFFu, s = pl.y & 0xFFFFFFu, e = (pl.y >> 24) & 3u;
  if (rank == 2u) return e == 1u ? c_bright(c_blend_alpha(s, t, int(pl.y >> 26), int(pl.x >> 26))) : s;
  if (e == 1u) return c_bright(c_blend_3d(t, s, a));
  uint c = c_bright(e == 2u ? c_blend_bright(t, cEvy) : e == 3u ? c_blend_dark(t, cEvy) : t);
  int k = int(l3d >> 29);
  // an antialiased 3D edge drawn opaque over the 2D: its empty part shows the picture without the 3D there, as the DS's own
  // 3D-over-2D blending would (else the silhouette grows by the partly covered pixels)
  if (k != 0)
    c = c_mk((c_r(c) * (8 - k) + c_r(c0) * k + 4) >> 3, (c_g(c) * (8 - k) + c_g(c0) * k + 4) >> 3,
             (c_b(c) * (8 - k) + c_b(c0) * k + 4) >> 3);
  return c;
}

void main() {
  ivec2 p = ivec2(gl_FragCoord.xy);
  int N = cScale, sh = 192 * N;
  bool topHalf = p.y < sh;
  ivec2 q = ivec2(p.x, topHalf ? p.y : p.y - sh); // the pixel within its screen
  // q / N without an integer division (a third of the pass on Apple GPUs): (q + 1/2) / N lies at least 1 / 2N from the
  // next integer, far beyond the float quotient's error at these sizes
  ivec2 d = ivec2((vec2(q) + 0.5) / float(N));
  bool isA = topHalf == (cATop != 0u);
  uint c = isA && cCompose != 0u ? c_planned(d, q) : texelFetch(cSrc1x, ivec2(d.x, d.y + (isA ? 0 : 192)), 0).r;
  uint r = c & 63u, g = (c >> 8) & 63u, b = (c >> 16) & 63u;
  uvec3 v8 = uvec3((r << 2) | (r >> 4), (g << 2) | (g >> 4), (b << 2) | (b >> 4));
  oColor = vec4(vec3(v8) / 255.0, 1.0);
}
#endif
