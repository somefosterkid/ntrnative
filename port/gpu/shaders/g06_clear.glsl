// port/gpu piece G6: the clear (docs/gpu.md, G6). OWNED BY G6.
// g_clear(S, p): CLEAR_COLOR (colour, alpha, polygon ID, fog flag) and CLEAR_DEPTH as vcore's vc_3d_clear (p19), or the
// rear-plane bitmap (DISP3DCNT bit 14: colour from texture slot 2, depth and fog from slot 3, 256 x 256, scrolled by
// CLRIMAGE_OFFSET and wrapped, sampled at the DS pixel p / N: nearest at N x). GBATEK 54.
// Also g_coverage: the antialiasing coverage of an edge pixel from the span record's edge parameters (vcore p16's
// edge_cov, at the N x pixel: the edges' ax, ay, dx, dy are in N x pixels, so the part of the N x pixel covered).

uniform highp usampler2D uClrImg; // 65536 texels: slot 2's colour | slot 3's depth/fog << 16, by * 256 + bx (src/g06_post.c)
// uClrOff: g00_head.glsl's block

uint g_w6(uint c) { return c > 0u ? c * 2u + 1u : 0u; } // GBATEK 50: 5 -> 6 bits, X > 0 -> X * 2 + 1
uvec3 g_rgb6(uint c) { return uvec3(g_w6(c & 31u), g_w6((c >> 5) & 31u), g_w6((c >> 10) & 31u)); }
uint g_depth_expand(uint x) { return x * 0x200u + ((x + 1u) / 0x8000u) * 0x1FFu; } // GBATEK 54

void g_clear(inout GPixel S, ivec2 p) {
  uint cc = uClearColor;
  S.color = uvec4(g_rgb6(cc & 0x7FFFu), (cc >> 16) & 31u);
  S.depth = g_depth_expand(uClearDepth & 0x7FFFu);
  S.opaque_id = (cc >> 24) & 63u;
  S.trans_id = 255u;
  S.flags = (cc & 0x8000u) != 0u ? F_FOG : 0u;
  S.cov = 0u;
  S.last = ~0u;
  if ((uDisp3d & 0x4000u) != 0u) {
    uint bx = (uint(p.x / uScale) + (uClrOff & 0xFFu)) & 0xFFu;
    uint by = (uint(p.y / uScale) + ((uClrOff >> 8) & 0xFFu)) & 0xFFu;
    uint t = texelFetch(uClrImg, g_at(int(by * 256u + bx)), 0).x;
    uint pc = t & 0xFFFFu, pd = t >> 16;
    S.color = uvec4(g_rgb6(pc & 0x7FFFu), (pc & 0x8000u) != 0u ? 31u : 0u);
    S.depth = g_depth_expand(pd & 0x7FFFu);
    S.flags = (pd & 0x8000u) != 0u ? F_FOG : 0u;
  }
}

// edge e = (ax, ay, dx, dy) at pixel (x, y); left: the polygon lies to its right
uint g_edge_cov(vec4 e, int x, int y, bool left) {
  if (e.w == 0.0) return 31u;
  float c, fx = float(x), fy = float(y);
  if (e.z == 0.0) {
    c = left ? (fx + 0.5) - e.x : e.x - (fx - 0.5);
  } else if (abs(e.z) <= e.w) {
    float xc = e.x + (fy - e.y) * e.z / e.w;
    c = left ? (fx + 0.5) - xc : xc - (fx - 0.5);
  } else {
    float yl = e.y + (fx - e.x) * e.w / e.z;
    float d = yl - fy;
    c = ((e.z > 0.0) == left) ? 0.5 + d : 0.5 - d;
  }
  c = clamp(c, 0.0, 1.0);
  return min(uint(c * 32.0), 31u);
}

// t5: the edges' pixel runs (left lo, hi, right lo, hi); t6, t7: the left and right edges' (ax, ay, dx, dy)
uint g_coverage(int x, int y, vec4 t5, vec4 t6, vec4 t7) {
  float fx = float(x);
  bool inl = fx >= t5.x && fx <= t5.y, inr = fx >= t5.z && fx <= t5.w;
  uint cov = 31u;
  if (inl) cov = g_edge_cov(t6, x, y, true);
  if (inr) {
    uint c2 = g_edge_cov(t7, x, y, false);
    if (!inl || c2 < cov) cov = c2;
  }
  return cov;
}
