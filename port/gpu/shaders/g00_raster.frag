// port/gpu pass R: the polygon loop (docs/gpu.md, "The frame" 4). OWNED BY THE SKELETON (G0).
// Every output pixel visits the polygons of its bin in the DS's drawing order and runs vcore's per-pixel steps on G_S samples'
// states in registers (multisampling, docs/gpu.md "Antialiasing"; G_S = 1 without it): G6 g_clear, then per polygon G3 g_row and
// g_cover_row for each sample row (coverage, depth, the early depth reject, the edge flags), and when a sample is left, G3 g_shade and
// G4 g_texture once for the pixel, then G5 g_plot4 on the covered samples together (vcore p16's emit_pixel: the coverage is kept
// when the plot changed the sample).
#if G_K == 2
layout(location = 0) out uvec2 oS[G_S]; // per sample: depth | opaque ID << 24, colour | flags | coverage (g00_head.glsl)
#else
layout(location = 0) out uvec4 oS[G_S]; // depth, IDs | flags << 16 | coverage << 24, colour, last (g00_head.glsl)
#endif

// uTilesX, uTileW, uFlat: g00_head.glsl's block

// the samples' states, one vector lane per sample (G_S <= 4): C, D, F, I as g_plot4 keeps them, L the last polygon. (No array
// element or vector component is passed as an out or inout parameter: Apple's GL compiler, M2 macOS 2026-11, crashes on that.)
#if G_K == 2
uvec2 r_out(uint C, uint D, uint F, uint I) { // C is r6 | g6 << 6 | b6 << 12 | a5 << 18 already
  return uvec2(D | ((I & 63u) << 24), C | ((F & 7u) << 23) | (((F >> 4) & 1u) << 26) | ((F >> 8) << 27));
}
#else
uvec4 r_out(uint C, uint D, uint F, uint I, uint L) {
  return uvec4(D, I | ((F & 255u) << 16) | ((F >> 8) << 24),
               (C & 63u) | (((C >> 6) & 63u) << 8) | (((C >> 12) & 63u) << 16) | ((C >> 18) << 24), L);
}
#endif

void main() {
  // the pixel's first sample on the N k grid; GL's row 0 = the DS's row 0 (the readback keeps it)
  int x0 = int(gl_FragCoord.x) * G_K, r0 = int(gl_FragCoord.y) * G_K;
  GPixel c0;
  g_clear(c0, ivec2(x0, r0)); // the rear plane is per DS pixel: the same for every sample
  uvec4 sC = uvec4(c0.color.r | (c0.color.g << 6) | (c0.color.b << 12) | (c0.color.a << 18));
  uvec4 sD = uvec4(c0.depth), sF = uvec4(c0.flags | (c0.cov << 8)), sI = uvec4(c0.opaque_id | (c0.trans_id << 8));
  uvec4 sL = uvec4(c0.last);
  // the bin: 16 DS columns x 8 DS rows (src/g00_frame.c), bin = band * 16 + column
  uvec2 bl = texelFetch(uBand, g_at((r0 / uBandRows) * uTilesX + x0 / uTileW), 0).xy;
  for (uint i = 0u; i < bl.y; i++) {
    uvec4 ent = texelFetch(uList, g_at(int(bl.x + i)), 0); // k, attr, first record - first row + bias, rows (band-clipped)
    int k = int(ent.x);
    uint attr = ent.y;
    bool early = uFlat == 0u && ((attr >> 4) & 3u) != 3u; // shadow polygons: the mask needs the failing samples
    // per sample: covered, depth, edge flag, coverage; crec: the record of the first of the pixel's rows the polygon covers (the
    // shading row), its parts fetched again where it shades: kept through the coverage steps, they held registers at pass R's
    // peak (M2 2026-11, town at 3x with 4 samples: Metal 1.79 -> 1.70 ms, GL 2.23 -> 2.09)
    bvec4 hit = bvec4(false), edge = bvec4(false);
    uvec4 dep = uvec4(0u), cov = uvec4(31u);
    int crec = 0;
    bool have = false;
    GRow rr[G_K]; // every row's record first (g_row), then the tests
    bool okr[G_K];
    for (int j = 0; j < G_K; j++) {
      GRow r;
      okr[j] = g_row(ent, r0 + j, x0, r);
      rr[j] = r;
    }
#if G_K == 2
    GRow ra = rr[0], rb = rr[1];
    bool oka = okr[0], okb = okr[1];
    if (!oka && !okb) continue;
    crec = oka ? ra.rec : rb.rec;
    g_cover2(ra, oka, rb, okb, x0, early, sD, attr, hit, dep, edge, cov);
#else
    for (int j = 0; j < G_K; j++) {
      GRow r = rr[j];
      bool ok = okr[j];
      if (!ok) continue;
      if (!have) crec = r.rec;
      have = true;
      g_cover_row(r, x0, j * G_K, early, sD, attr, hit, dep, edge, cov);
    }
#endif
    if (!any(hit)) continue;
    uvec4 H = g_m(hit), E = g_m(edge);
    uvec4 ncov = g_sel(uvec4(31u), cov, E); // the coverage a changed sample keeps
    if (uFlat != 0u) {
      sC = g_sel(sC, uvec4(63u | (63u << 6) | (63u << 12) | (31u << 18)), H);
      sF = g_sel(sF, (E & F_EDGE) | (ncov << 8), H);
      sL = g_sel(sL, uvec4(k), H);
      continue;
    }
    // the colour once for the pixel: at its first sample, on the shading row (vcore p21's shading point)
    uvec3 rgb;
    ivec2 st;
    g_shade(g_row_rec(crec), x0, rgb, st);
    GPx px;
    px.x = x0;
    px.row = r0;
    px.w = 1.0;
    px.rgb = rgb;
    px.st = st;
    px.depth = 0u;
    px.edge = false;
    px.cov = 31u;
    uvec4 col = g_texture(k, px, uvec4(px.rgb, g_poly(k, 1).w));
    uvec4 ch = g_plot4(sC, sD, sF, sI, H, dep, E, k, col);
    sF = g_sel(sF, (sF & 255u) | (ncov << 8), ch);
    sL = g_sel(sL, uvec4(k), ch);
  }
  // GLSL ES 3.00 indexes an output array by constant expressions only
#if G_K == 2
  oS[0] = r_out(sC.x, sD.x, sF.x, sI.x);
  // sample 0's bit 30 of .x: some sample of the pixel has the edge flag (pass E looks at those pixels only)
  if (any(notEqual(sF & F_EDGE, uvec4(0u)))) oS[0].x |= 1u << 30;
  // bit 31: "alike", the samples have the same colour, flags and opaque ID, no edge flag that pass P acts on, and the same fog
  // (fog off, the fog flag clear, or the same 15-bit depth): pass P takes sample 0 for the pixel (and walks the Edge=ds
  // thickening once for the four). Town at 3x: 84% of the pixels, battle 92%.
  uvec4 fl = sF & 255u;
  if (all(equal(sC, sC.xxxx)) && all(equal(fl, fl.xxxx)) && all(equal(sI & 255u, (sI & 255u).xxxx)) &&
      ((fl.x & F_EDGE) == 0u || (uDisp3d & 0x30u) == 0u) &&
      ((uDisp3d & 0x80u) == 0u || (fl.x & F_FOG) == 0u || all(equal(sD >> 9, uvec4(sD.x >> 9)))))
    oS[0].x |= 1u << 31;
  oS[1] = r_out(sC.y, sD.y, sF.y, sI.y);
  oS[2] = r_out(sC.z, sD.z, sF.z, sI.z);
  oS[3] = r_out(sC.w, sD.w, sF.w, sI.w);
#else
  oS[0] = r_out(sC.x, sD.x, sF.x, sI.x, sL.x);
#endif
}
