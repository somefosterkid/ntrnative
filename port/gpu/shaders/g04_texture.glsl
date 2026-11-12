// port/gpu piece G4: textures and the texture blend (docs/gpu.md, G4). OWNED BY G4.
// g_texture(k, px, col): polygon k's texel at px.st (TEXIMAGE_PARAM's format, size, repeat/flip/clamp, decoded from the
// raw texture slots and palette slots: uTex, uTexPal, uploaded by src/g04_texture.c) blended with the pixel's colour
// col (r6, g6, b6, a5) by POLYGON_ATTR's mode: modulation, decal, toon / highlight (also for untextured polygons),
// shadow; as vcore's vc_tex_sample_c + vc_tex_blend + vc_toon_blend (p17, p19), in integers.
// uTex: 128K words, the 4 texture slots (512 KB) as the texture unit sees them. uTexPal: 24K words (the 6 palette
// slots, 96 KB) followed by 16 words: TOON_TABLE (32 halfwords).

uniform highp usampler2D uTex;
uniform highp usampler2D uTexPal;
// uFilter: g00_head.glsl's block (read by the G_FILTER programs only: "g00_raster.frag#1f", "#2f")

const int G4_TOON_WORD = 24576;

uint g4_w6(uint c) { return c != 0u ? c * 2u + 1u : 0u; }
uvec4 g4_px(uint c, uint a) { return uvec4(g4_w6(c & 31u), g4_w6((c >> 5u) & 31u), g4_w6((c >> 10u) & 31u), a); }

uint g4_b(uint a) {
  a &= 0x7FFFFu;
  return (texelFetch(uTex, g_at(int(a >> 2u)), 0).x >> ((a & 3u) * 8u)) & 255u;
}
uint g4_h(uint a) {
  a &= 0x7FFFEu;
  return (texelFetch(uTex, g_at(int(a >> 2u)), 0).x >> ((a & 2u) * 8u)) & 0xFFFFu;
}
// the palette address space is 128 KB; slots 6 and 7 hold nothing (colour 0, black)
uint g4_pal(uint a) {
  a &= 0x1FFFEu;
  if (a >= 0x18000u) return 0u;
  return (texelFetch(uTexPal, g_at(int(a >> 2u)), 0).x >> ((a & 2u) * 8u)) & 0xFFFFu;
}

int g4_axis(int v, int size, bool rep, bool flip) {
  if (!rep) return v < 0 ? 0 : (v >= size ? size - 1 : v);
  int m = v & (size - 1);
  if (flip && (v & size) != 0) m = size - 1 - m;
  return m;
}

// vc_tex_sample: the texel (r6, g6, b6, a5) of texparam tp, palette base pb, at s, t (12.4)
uvec4 g4_sample(uint tp, uint pb, ivec2 st) {
  uint fmt = (tp >> 26u) & 7u;
  if (fmt == 0u) return uvec4(0u);
  int sw = 8 << ((tp >> 20u) & 7u), sh = 8 << ((tp >> 23u) & 7u);
  int x = g4_axis(st.x >> 4, sw, (tp & 0x10000u) != 0u, (tp & 0x40000u) != 0u);
  int y = g4_axis(st.y >> 4, sh, (tp & 0x20000u) != 0u, (tp & 0x80000u) != 0u);
  uint base = (tp & 0xFFFFu) * 8u;
  uint i = uint(y) * uint(sw) + uint(x);
  bool t0 = (tp & 0x20000000u) != 0u;
  uint idx, a;
  if (fmt == 1u) { // A3I5
    idx = g4_b(base + i);
    a = idx >> 5u;
    a = a * 4u + a / 2u;
    return g4_px(g4_pal(pb * 16u + (idx & 31u) * 2u), a);
  }
  if (fmt == 2u) { // 4 colours, palette base in 8-byte steps
    idx = (g4_b(base + (i >> 2u)) >> ((i & 3u) * 2u)) & 3u;
    return g4_px(g4_pal(pb * 8u + idx * 2u), (t0 && idx == 0u) ? 0u : 31u);
  }
  if (fmt == 3u) {
    idx = (g4_b(base + (i >> 1u)) >> ((i & 1u) * 4u)) & 15u;
    return g4_px(g4_pal(pb * 16u + idx * 2u), (t0 && idx == 0u) ? 0u : 31u);
  }
  if (fmt == 4u) {
    idx = g4_b(base + i);
    return g4_px(g4_pal(pb * 16u + idx * 2u), (t0 && idx == 0u) ? 0u : 31u);
  }
  if (fmt == 6u) { // A5I3
    idx = g4_b(base + i);
    return g4_px(g4_pal(pb * 16u + (idx & 7u) * 2u), idx >> 3u);
  }
  if (fmt == 7u) {
    uint c = g4_h(base + i * 2u);
    return g4_px(c, (c & 0x8000u) != 0u ? 31u : 0u);
  }
  // 4x4 compressed: the texel's 2 bits in the block's row, the block's palette word in slot 1
  uint blk = uint(y >> 2) * uint(sw >> 2) + uint(x >> 2);
  uint row = g4_b(base + blk * 4u + uint(y & 3));
  uint tx = (row >> (uint(x & 3) * 2u)) & 3u;
  uint ia = ((base & 0x1FFFFu) >> 1u) + ((base & 0x40000u) != 0u ? 0x10000u : 0u);
  uint pi = g4_h(0x20000u + ia + blk * 2u);
  uint pofs = pb * 16u + (pi & 0x3FFFu) * 4u;
  uint mode = pi >> 14u;
  if (mode == 0u || mode == 2u) {
    if (mode == 0u && tx == 3u) return uvec4(0u);
    return g4_px(g4_pal(pofs + tx * 2u), 31u);
  }
  uint c0 = g4_pal(pofs), c1 = g4_pal(pofs + 2u);
  if (tx < 2u) return g4_px(tx == 1u ? c1 : c0, 31u);
  if (mode == 1u && tx == 3u) return uvec4(0u);
  uint c = 0u;
  for (uint k = 0u; k < 3u; k++) {
    uint a0 = (c0 >> (5u * k)) & 31u, a1 = (c1 >> (5u * k)) & 31u, v;
    if (mode == 1u) v = (a0 + a1) >> 1u;
    else if (tx == 2u) v = (a0 * 5u + a1 * 3u) >> 3u;
    else v = (a0 * 3u + a1 * 5u) >> 3u;
    c |= (v & 31u) << (5u * k);
  }
  return g4_px(c, 31u);
}

// G9, Filter=bilinear (N > 1 only): the four texels around the sample point (the texel centres are at +0.5, the 12.4
// coordinate's fraction gives the weights in 1/16 steps), interpolated in the decoded 6-bit colours.
// The alpha rules stay: a format with a hard alpha (colour-0 transparent, 4x4 mode 0/1, direct bit 15) keeps its edge
// hard: a transparent nearest texel stays transparent and an opaque one stays opaque (alpha 31), and transparent
// neighbours take no part in the colour (no dark fringe from their palette entry); A3I5 / A5I3 interpolate their alpha
// and weight the colour by it.
uvec4 g4_filtered(uint tp, uint pb, ivec2 st) {
  uvec4 nr = g4_sample(tp, pb, st);
  uint fmt = (tp >> 26u) & 7u;
  bool smooth_a = fmt == 1u || fmt == 6u;
  if (!smooth_a && nr.a == 0u) return nr;
  ivec2 u = st - 8;
  ivec2 b = u >> 4;
  ivec2 f = u & 15;
  uint csum[3] = uint[3](0u, 0u, 0u);
  uint wsum = 0u, asum = 0u;
  for (int j = 0; j < 2; j++)
    for (int i = 0; i < 2; i++) {
      uint w = uint((i == 0 ? 16 - f.x : f.x) * (j == 0 ? 16 - f.y : f.y));
      uvec4 t = g4_sample(tp, pb, (b + ivec2(i, j)) << 4);
      asum += w * t.a;
      uint cw = smooth_a ? w * t.a : (t.a == 0u ? 0u : w);
      wsum += cw;
      for (int c = 0; c < 3; c++) csum[c] += cw * t[c];
    }
  if (wsum == 0u) return nr;
  uvec4 o;
  for (int c = 0; c < 3; c++) o[c] = (csum[c] + wsum / 2u) / wsum;
  o.a = smooth_a ? (asum + 128u) >> 8u : 31u;
  return o;
}

// p19's vc_toon_blend: toon (the table colour shades the texel) or highlight (texel * red + table colour)
uvec4 g4_toon(uvec4 vc, uvec4 tx) {
  bool highlight = (uDisp3d & 2u) != 0u;
  uint idx = (vc.r >> 1u) & 31u;
  uint tw = texelFetch(uTexPal, g_at(G4_TOON_WORD + int(idx >> 1u)), 0).x;
  uvec4 tc = g4_px((tw >> ((idx & 1u) * 16u)) & 0xFFFFu, 0u);
  uvec3 o;
  for (int i = 0; i < 3; i++) {
    uint v;
    if (highlight) {
      v = ((tx[i] + 1u) * (vc.r + 1u) - 1u) / 64u + tc[i];
      if (v > 63u) v = 63u;
    } else {
      v = ((tx[i] + 1u) * (tc[i] + 1u) - 1u) / 64u;
    }
    o[i] = v;
  }
  uint at = g4_w6(tx.a), av = g4_w6(vc.a);
  uint a = (((at + 1u) * (av + 1u) - 1u) / 64u) >> 1u;
  return uvec4(o, a);
}

uvec4 g_texture(int k, GPx px, uvec4 col) {
  uvec4 P0 = g_poly(k, 0);
  uint attr = P0.x, tp = P0.y;
  uint mode = (attr >> 4u) & 3u;
  if (((tp >> 26u) & 7u) == 0u) {
    if (mode == 2u) return g4_toon(col, uvec4(63u, 63u, 63u, 31u));
    return col;
  }
#ifdef G_FILTER // Filter=bilinear has pass R programs of its own (src/g00_frame.c picks them when it applies): in the plain ones
  // g4_filtered's four more samples held registers that cost pass R 17% (M2 2026-11, town at 3x, Metal 0.72 -> 0.60 ms)
  uvec4 tx = (uFilter != 0u && uScale > uSnap) ? g4_filtered(tp, P0.z & 0x1FFFu, px.st) : g4_sample(tp, P0.z & 0x1FFFu, px.st);
#else
  uvec4 tx = g4_sample(tp, P0.z & 0x1FFFu, px.st);
#endif
  if ((uDisp3d & 1u) == 0u) return col;
  if (mode == 2u) return g4_toon(col, tx);
  if (mode == 1u || mode == 3u) { // decal; shadow polygons texture like decal
    uvec3 o;
    for (int i = 0; i < 3; i++) {
      uint v = col[i], t = tx[i];
      if (tx.a == 0u) o[i] = v;
      else if (tx.a == 31u) o[i] = t;
      else o[i] = (t * tx.a + v * (31u - tx.a)) / 32u;
    }
    return uvec4(o, col.a);
  }
  uvec3 o;
  for (int i = 0; i < 3; i++) o[i] = ((tx[i] + 1u) * (col[i] + 1u) - 1u) / 64u;
  return uvec4(o, ((tx.a + 1u) * (col.a + 1u) - 1u) / 32u);
}
