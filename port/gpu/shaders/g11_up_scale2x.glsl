// port/gpu 2D upscaling, the Scale2x style (EPX / AdvMAME2x: Andrea Mazzoleni's public rule, written from its description;
// g11_up.glsl is the framework). OWNED BY THE 2D FILTER.
// Each DS pixel E becomes four quarters from its neighbours B (up), D (left), F (right), H (down): where B != H and D != F,
// the top-left quarter is D when D == B, the top-right F when B == F, the bottom-left D when D == H, the bottom-right F
// when H == F; else E. At N x the N x pixel takes the quarter its centre falls in (Scale2x at 2x resized nearest: at an odd
// N the middle row and column take the right / bottom quarter, as a nearest-neighbour resize of the 2x picture does).
#ifdef UP_SCALE2X
// info x: the four quarters' picks, 3 bits each (top-left, top-right, bottom-left, bottom-right): 0 E, 2 D, 3 F; y the key
uvec4 up_analyse(ivec2 p) {
  uint e = up_key(p), b = up_keyat(p, 0, -1), d = up_keyat(p, -1, 0), f = up_keyat(p, 1, 0), h = up_keyat(p, 0, 1);
  uint code = 0u;
  if (!up_eq(b, h) && !up_eq(d, f))
    code = (up_eq(d, b) ? 2u : 0u) | ((up_eq(b, f) ? 3u : 0u) << 3) | ((up_eq(d, h) ? 2u : 0u) << 6) |
           ((up_eq(h, f) ? 3u : 0u) << 9);
  return uvec4(code, e, 0u, 0u);
}
UpPick up_pick(uvec4 info, ivec2 f, int N) {
  int quarter = (f.x < N ? 0 : 1) + (f.y < N ? 0 : 2);
  uint c = (info.x >> uint(3 * quarter)) & 7u;
  return UpPick(ivec2(c == 2u ? -1 : c == 3u ? 1 : 0, 0), ivec2(0), 0);
}
#endif
