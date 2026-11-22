// port/gpu 2D upscaling (the "2D filter"): the shared part (design: docs/gpu.md "2D upscaling"). OWNED BY THE 2D FILTER.
// A style turns the DS's 2D picture of each screen, at 1x, into an info texture (g11_up.frag: one RGBA32UI texel per DS
// pixel of both screens, rows 0-191 engine A, 192-383 engine B, the cSrc1x order); pass C's N x program for that style
// (g07_compose.frag with UP_<STYLE> defined) asks the style's up_pick, for each N x pixel, which DS pixel(s) it shows and
// finishes it at that DS pixel exactly as Off does (the plan, the 3D layer read once at the N x pixel). So every colour on
// screen is still one the compositor made (selection styles) or an integer mix of two (blending styles), and the 3D layer
// is never filtered. Integers only: GL, GLES and Metal agree bit for bit.
// Only programs that define UP_<STYLE> see the g11_*.glsl files (src/gpu_gl.c's assemble, tools/gpu_msl.py): every other
// program's text is what it was without them.
#if defined(UP_IDENTITY) || defined(UP_SCALE2X) || defined(UP_MMPX) || defined(UP_MMPX2)
#define UP 1
#endif

#ifdef UP
uniform highp usampler2D uUpPlan; // the analysis' inputs: pass C's plan (256 x 192, RG32UI) and 1x lines (256 x 384, R32UI),
uniform highp usampler2D uUpSrc;  // the same textures as g07_compose.frag's cPlan and cSrc1x

// up_key's sentinel for "the 3D layer is on top here": equal only to itself (a colour is at most 0x3F3F3F; a translucent
// 2D layer over the 3D has bit 30 and a colour)
const uint UP_K3D = 0x7F000000u;

// what the filter sees at DS pixel p (engine order, as the info texture): engine A composed: the plan's picture without
// the 3D layer where the 3D does not matter, UP_K3D where the 3D is on top, the top 2D layer's colour | bit 30 where a 2D
// layer is drawn over the 3D (never equal to the same colour drawn opaque); else the 1x line's colour (engine B, engine A
// in display modes 0/2/3). Without a 3D layer the plan's picture everywhere (pass C then shows it).
uint up_key(ivec2 p) {
  if (p.y < 192 && cCompose != 0u) {
    uvec2 pl = texelFetch(uUpPlan, p, 0).xy;
    uint rank = (pl.x >> 24) & 3u;
    if (rank == 0u || cHave3d == 0u) return pl.x & 0x3F3F3Fu;
    if (rank == 1u) return UP_K3D;
    return (pl.y & 0x3F3F3Fu) | (1u << 30);
  }
  return texelFetch(uUpSrc, p, 0).r & 0x3F3F3Fu;
}

// p + o clamped within p's own screen (nothing is judged across the two screens)
ivec2 up_nb(ivec2 p, ivec2 o) {
  int y0 = p.y < 192 ? 0 : 192;
  return clamp(p + o, ivec2(0, y0), ivec2(255, y0 + 191));
}
uint up_keyat(ivec2 p, int dx, int dy) { return up_key(up_nb(p, ivec2(dx, dy))); }

// the metrics: exact equality (selection styles); a YCbCr distance in integers for the blending ones (sentinels at the
// largest distance from everything but themselves)
bool up_eq(uint a, uint b) { return a == b; }
int up_dist(uint a, uint b) {
  if (a == b) return 0;
  if (((a | b) & 0xFF000000u) != 0u) return 0x7FFFFFFF;
  int dr = int(a & 63u) - int(b & 63u), dg = int((a >> 8) & 63u) - int((b >> 8) & 63u), db = int((a >> 16) & 63u) - int((b >> 16) & 63u);
  // Y, Cb, Cr of the difference (BT.601, 1/256 steps), weighted 48 : 7 : 6 as xBRZ does (squared)
  int y = 77 * dr + 150 * dg + 29 * db, cb = 128 * db - 43 * dr - 85 * dg, cr = 128 * dr - 107 * dg - 21 * db;
  return ((y >> 4) * (y >> 4)) * 48 + ((cb >> 4) * (cb >> 4)) * 7 + ((cr >> 4) * (cr >> 4)) * 6;
}

// a style's answer for one N x pixel: show DS pixel d + dA, mixed with d + dB by w / 64 (w = 0: dA alone). The offsets are
// relative to the N x pixel's own DS pixel, in -2..2; pass C clamps them within the screen
struct UpPick {
  ivec2 dA, dB;
  int w;
};

// f: the N x pixel's centre within its DS pixel in 1 / 2N steps (1, 3, .., 2N - 1)
#ifdef UP_IDENTITY
// the identity (the framework's check: equals Off at every N): info x unused, y the key (gpu_render --up-info)
uvec4 up_analyse(ivec2 p) { return uvec4(0u, up_key(p), 0u, 0u); }
UpPick up_pick(uvec4 info, ivec2 f, int N) { return UpPick(ivec2(0), ivec2(0), 0); }
#endif
#endif
