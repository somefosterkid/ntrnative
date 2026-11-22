// port/gpu piece G12: the texture cache's sampling (docs/gpu.md, G12). OWNED BY G12. Only in the G_TEXCACHE programs
// ("g00_raster.frag#1c", "#2c": src/gpu_gl.c's assemble and tools/gpu_msl.py add this file to those alone).
// The decoded textures are layers of five 2D arrays by side (uTc0..uTc4: 32..512 texels, RGBA8, the wrap modes baked in:
// src/g12_tcdecode.c's layout, texel (x, y) of the texture at texel (x, y) of the layer; with TexMips their mip chains,
// src/g12_tcmips.c). uTcPoly: 4 texels per polygon k (drawing order, as uPoly; src/g12_tcgl.c):
//   t0: x = ready(1) | bucket << 1 | filtered << 4 | magnify-nearest << 5 | layer << 16, y, z = the texture's width and height,
//   w = 1 / S (float bits)
//   t1, t2, t3: the planes A x + B y + C of s/w, t/w (texels) and 1/w over the N k grid's pixel positions (float bits; xyz =
//   A, B, C; src/g02_spans.c's gpu_spans_planes), all 0 when the polygon has no area (then: level 0)
// g12_sample(k, tp, st, pos, tx): polygon k's texel at st (12.4) for the pixel whose first sample is pos (the N k grid), as
// g4_sample returns it; false when its texture has no layer (the caller samples the texture slots).
// Exact (filtered 0: BWPORT_GPU_TEXCACHE=exact): the DS's texel index (g4_axis's wrap) + 1/2, the arrays NEAREST with one level.
// Filtered (TEXCACHE=on): the DS's coordinate (the middle of its 1/16 step; clamp axes clamped into the texture, the padding
// does the rest) and the screen gradients of s and t from the planes (ds/dx = (A_s - s A_w) / W at the pixel's centre, per
// output pixel), through textureGrad: the arrays' filters (TexMag, TexMips: trilinear, Aniso) do the rest. Back to the DS's
// (r6, g6, b6, a5) by round(v * 63) and round(a * 31) (the inverse of g12_tc_rgba8); the hard-alpha formats (all but A3I5 and
// A5I3) threshold the filtered alpha at 1/2 (31 or 0: the coverage-preserving mips keep cut-outs solid).
// The filtered path also clamps the coordinate on the axes that repeat or mirror (not clamp) to the polygon's own s/t range (its
// vertices', 12.4 in the 4th words of t2 (s: low, high 16 bits) and t3 (t)) minus half its footprint (at least half a texel; a
// narrower range: its middle), where the sampler blends texels at all (bit 6 of t0.x): the taps no longer read the neighbouring
// tile's texels across the polygon's edge.
// Magnify-nearest (TexMag=nearest): where the footprint's longer axis covers at most one texel (GL's lambda <= 0 without
// anisotropy) the pixel takes the DS's own texel by texelFetch, as the exact mode does. The sampler's MAG_FILTER alone does not
// do it once anisotropy is on: D3D12 (and so Mesa's d3d12 on Windows' GPUs) has no anisotropic filter with point magnification,
// and llvmpipe's anisotropic path filters magnified footprints too (both drew the magnified ground bilinear: a third of the
// pixels of a 3x town frame changed and the grass tiles' edges showed lines, the bilinear tap reaching the wrapped texel).

uniform highp sampler2DArray uTc0;
uniform highp sampler2DArray uTc1;
uniform highp sampler2DArray uTc2;
uniform highp sampler2DArray uTc3;
uniform highp sampler2DArray uTc4;
uniform highp usampler2D uTcPoly;

vec3 g12_plane(int k, int i) { return uintBitsToFloat(texelFetch(uTcPoly, g_at(k * 4 + i), 0).xyz); }

bool g12_sample(int k, uint tp, ivec2 st, ivec2 pos, out uvec4 tx) {
  uvec4 m = texelFetch(uTcPoly, g_at(k * 4), 0);
  tx = uvec4(0u);
  if ((m.x & 1u) == 0u) return false;
  float inv = uintBitsToFloat(m.w);
  vec2 f, dx = vec2(0.0), dy = vec2(0.0);
  ivec2 e = ivec2(g4_axis(st.x >> 4, int(m.y), (tp & 0x10000u) != 0u, (tp & 0x40000u) != 0u),
                  g4_axis(st.y >> 4, int(m.z), (tp & 0x20000u) != 0u, (tp & 0x80000u) != 0u));
  bool near = (m.x & 16u) == 0u;
  if (near) {
    f = vec2(e) + 0.5;
  } else {
    f = (vec2(st) + 0.5) * (1.0 / 16.0);
    if ((tp & 0x10000u) == 0u) f.x = clamp(f.x, 0.0, float(m.y) - 1.0 / 64.0);
    if ((tp & 0x20000u) == 0u) f.y = clamp(f.y, 0.0, float(m.z) - 1.0 / 64.0);
    vec3 ps = g12_plane(k, 1), pt = g12_plane(k, 2), pw = g12_plane(k, 3);
    uint rs = texelFetch(uTcPoly, g_at(k * 4 + 2), 0).w, rt = texelFetch(uTcPoly, g_at(k * 4 + 3), 0).w;
    vec3 q = vec3(vec2(pos) + 0.5 * float(G_K), 1.0);
    float W = dot(pw, q);
    if (W > 0.0) {
      float s = dot(ps, q) / W, t = dot(pt, q) / W, g = float(G_K) * inv / W;
      dx = vec2(ps.x - s * pw.x, pt.x - t * pw.x) * g;
      dy = vec2(ps.y - s * pw.y, pt.y - t * pw.y) * g;
    }
    near = (m.x & 32u) != 0u && max(dot(dx, dx), dot(dy, dy)) <= inv * inv;
    // the polygon's own s/t range (its vertices'): the filter's taps stay inside it on the axes that don't clamp, else the
    // taps read the neighbouring tile's texels across the polygon's edge (the 1x lines at sand and grass tile edges)
    if (!near && (m.x & 64u) != 0u && (rs | rt) != 0u && W > 0.0) {
      vec2 h = max(vec2(0.5), 0.5 * (abs(dx) + abs(dy)) / inv);
      vec2 lo = vec2(float(int(rs << 16u) >> 16), float(int(rt << 16u) >> 16)) * (1.0 / 16.0);
      vec2 hi = vec2(float(int(rs) >> 16), float(int(rt) >> 16)) * (1.0 / 16.0);
      if ((tp & 0x10000u) != 0u) f.x = (hi.x - lo.x < 2.0 * h.x) ? 0.5 * (lo.x + hi.x) : clamp(f.x, lo.x + h.x, hi.x - h.x);
      if ((tp & 0x20000u) != 0u) f.y = (hi.y - lo.y < 2.0 * h.y) ? 0.5 * (lo.y + hi.y) : clamp(f.y, lo.y + h.y, hi.y - h.y);
    }
  }
  vec3 c = vec3(f * inv, float(m.x >> 16u));
  ivec3 ic = ivec3(e, int(m.x >> 16u));
  uint b = (m.x >> 1u) & 7u;
  vec4 v;
  if (b == 0u) v = near ? texelFetch(uTc0, ic, 0) : textureGrad(uTc0, c, dx, dy);
  else if (b == 1u) v = near ? texelFetch(uTc1, ic, 0) : textureGrad(uTc1, c, dx, dy);
  else if (b == 2u) v = near ? texelFetch(uTc2, ic, 0) : textureGrad(uTc2, c, dx, dy);
  else if (b == 3u) v = near ? texelFetch(uTc3, ic, 0) : textureGrad(uTc3, c, dx, dy);
  else v = near ? texelFetch(uTc4, ic, 0) : textureGrad(uTc4, c, dx, dy);
  uint fmt = (tp >> 26u) & 7u;
  uint a = (fmt == 1u || fmt == 6u) ? uint(round(v.a * 31.0)) : (v.a >= 0.5 ? 31u : 0u);
  tx = uvec4(uvec3(round(v.rgb * 63.0)), a);
  return true;
}
