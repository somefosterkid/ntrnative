// port/gpu piece G5: the per-pixel rules (docs/gpu.md, G5). OWNED BY G5.
// vcore's vc_3d_depth_test (p16) and vc_p18_plot (p18) on the pixel state S: wire-frame alpha, alpha 0, the alpha
// test, shadow polygons (the mask, then the shadow), opaque writes, translucent blending with the same-ID rule, the
// fog flag, the depth update. GBATEK 43, 46, 51, 35.
// G5: checked at 1x against vcore on random pixel-aligned rectangle scenes (all rules, edges excluded): 1000 scenes, 0 px differ.

// GBATEK 43: "less", or "equal" within +-0x200 with POLYGON_ATTR bit 14
bool g_depth_test(uint attr, uint old, uint depth) {
  if ((attr & (1u << 14)) != 0u) {
    uint d = depth > old ? depth - old : old - depth;
    return d <= 0x200u;
  }
  return depth < old;
}

// GBATEK 51: FrameBuf = (Poly * (A + 1) + FrameBuf * (31 - A)) / 32 per channel, alpha = max
uvec4 g_blend(uvec4 fb, uvec3 rgb, uint a) {
  uvec3 o = (rgb * (a + 1u) + fb.rgb * (31u - a)) >> 5u;
  return uvec4(o, max(a, fb.a));
}

// vcore p16's early depth test: a plain (non-shadow) polygon pixel that fails the depth test changes nothing, so
// pass R may skip g_texture for it: `if (!g_early(S, k, px)) continue;` between g_pixel and g_texture.
bool g_early(GPixel S, int k, GPx px) {
  uint attr = g_poly(k, 0).x;
  if (((attr >> 4) & 3u) == 3u) return true; // shadow polygons: the mask needs the failing pixels
  return g_depth_test(attr, S.depth, px.depth);
}

void g_plot(inout GPixel S, int k, GPx px, uvec4 col) {
  uint attr = g_poly(k, 0).x;
  bool pass = g_depth_test(attr, S.depth, px.depth);
  uint a = col.a;
  if (((attr >> 16) & 31u) == 0u) a = 31u; // wire-frames: alpha 31
  if (a == 0u) return;
  if ((uDisp3d & 4u) != 0u && a <= uAlphaRef) return;
  uint id = (attr >> 24) & 63u;
  bool fogbit = (attr & 0x8000u) != 0u;
  uvec3 rgb = col.rgb;
  bool blending = (uDisp3d & 8u) != 0u;

  if (((attr >> 4) & 3u) == 3u) { // shadow polygons (GBATEK 46)
    if (id == 0u) {               // the mask: the stencil where the depth test fails
      if (!pass) S.flags |= F_STENCIL;
      return;
    }
    bool had = (S.flags & F_STENCIL) != 0u; // the shadow: where the mask is set (vcore: measured), then cleared
    S.flags &= ~F_STENCIL;
    if (!had) return;
    uint cur = (S.flags & F_TRANS) != 0u ? S.trans_id : S.opaque_id;
    if (cur == id || !pass) return;
    if ((S.flags & F_TRANS) != 0u && S.trans_id == id) return;
    if (blending && S.color.a != 0u && a < 31u) S.color = g_blend(S.color, rgb, a);
    else S.color = uvec4(rgb, a);
    if (!fogbit) S.flags &= ~F_FOG;
    S.flags |= F_TRANS | F_WRITTEN;
    S.trans_id = id;
    if ((attr & 0x800u) != 0u) S.depth = px.depth;
    return;
  }

  if (!pass) return;
  if (a >= 31u) { // opaque: colour, depth, ID, fog and edge flags replaced (the stencil kept)
    S.color = uvec4(rgb, 31u);
    S.depth = px.depth;
    S.opaque_id = id;
    S.trans_id = 255u;
    S.flags = (S.flags & F_STENCIL) | F_WRITTEN | (fogbit ? F_FOG : 0u) | (px.edge ? F_EDGE : 0u);
    return;
  }
  // translucent: not over a pixel the same polygon ID wrote translucently
  if ((S.flags & F_TRANS) != 0u && S.trans_id == id) return;
  if (blending && S.color.a != 0u) S.color = g_blend(S.color, rgb, a);
  else S.color = uvec4(rgb, a);
  if (!fogbit) S.flags &= ~F_FOG; // GBATEK 51: the fog flag ANDed
  S.flags |= F_TRANS | F_WRITTEN;
  S.trans_id = id;
  if ((attr & 0x800u) != 0u) S.depth = px.depth;
}

// Multisampling (docs/gpu.md "Antialiasing"): g_plot on up to four samples at once, one vector lane per sample, so that the
// samples take no branches of their own (a branch per sample made a fragment of four samples cost about four of one). The
// state: C = r6 | g6 << 6 | b6 << 12 | a5 << 18, D = depth, F = flags | coverage << 8, I = opaque ID | translucent ID << 8.
// hit: the lanes the polygon covers (all ones), dep and edge their depth and edge flag. Returns the lanes it changed (all ones)
// and the same rules as g_plot lane by lane.
uvec4 g_m(bvec4 b) { return uvec4(0u) - uvec4(b); }
uvec4 g_sel(uvec4 a, uvec4 b, uvec4 m) { return (a & ~m) | (b & m); }
// the translucent write on the lanes w: blended over a pixel with alpha (when blending is on and `under` allows), else replaced
void g_trans4(inout uvec4 C, inout uvec4 D, inout uvec4 F, inout uvec4 I, uvec4 w, uvec4 dep, uint attr, uvec3 rgb, uint a, bool under31) {
  uvec4 fa = (C >> 18) & 31u;
  uvec4 o = uvec4(rgb.r | (rgb.g << 6) | (rgb.b << 12) | (a << 18));
  if ((uDisp3d & 8u) != 0u && under31) {
    uvec4 bl = g_m(notEqual(fa, uvec4(0u)));
    uvec4 r = ((rgb.r * (a + 1u)) + (C & 63u) * (31u - a)) >> 5u;
    uvec4 g = ((rgb.g * (a + 1u)) + ((C >> 6) & 63u) * (31u - a)) >> 5u;
    uvec4 b = ((rgb.b * (a + 1u)) + ((C >> 12) & 63u) * (31u - a)) >> 5u;
    uvec4 x = r | (g << 6) | (b << 12) | (max(fa, uvec4(a)) << 18);
    o = g_sel(o, x, bl);
  }
  C = g_sel(C, o, w);
  uvec4 f = F;
  if ((attr & 0x8000u) == 0u) f &= ~uvec4(F_FOG);
  f |= uvec4(F_TRANS | F_WRITTEN);
  F = g_sel(F, f, w);
  I = g_sel(I, (I & 255u) | uvec4(((attr >> 24) & 63u) << 8), w);
  if ((attr & 0x800u) != 0u) D = g_sel(D, dep, w);
}

uvec4 g_plot4(inout uvec4 C, inout uvec4 D, inout uvec4 F, inout uvec4 I, uvec4 hit, uvec4 dep, uvec4 edge, int k, uvec4 col) {
  uint attr = g_poly(k, 0).x;
  uvec4 P;
  if ((attr & (1u << 14)) != 0u) P = g_m(lessThanEqual(max(dep, D) - min(dep, D), uvec4(0x200u)));
  else P = g_m(lessThan(dep, D));
  P &= hit;
  uint a = col.a;
  if (((attr >> 16) & 31u) == 0u) a = 31u; // wire-frames: alpha 31
  if (a == 0u) return uvec4(0u);
  if ((uDisp3d & 4u) != 0u && a <= uAlphaRef) return uvec4(0u);
  uint id = (attr >> 24) & 63u;
  uvec4 C0 = C, D0 = D, F0 = F;
  uvec4 tr = g_m(notEqual(F & F_TRANS, uvec4(0u)));
  uvec4 same = tr & g_m(equal(I >> 8, uvec4(id))); // a translucent pixel of this polygon ID
  if (((attr >> 4) & 3u) == 3u) { // shadow polygons (GBATEK 46)
    if (id == 0u) {               // the mask: the stencil where the depth test fails
      F |= uvec4(F_STENCIL) & hit & ~P;
    } else {                      // the shadow: where the mask is set, then cleared
      uvec4 had = hit & g_m(notEqual(F & F_STENCIL, uvec4(0u)));
      F = g_sel(F, F & ~uvec4(F_STENCIL), hit);
      uvec4 cur = g_sel(I & 255u, I >> 8, tr);
      uvec4 w = had & P & ~g_m(equal(cur, uvec4(id))) & ~same;
      g_trans4(C, D, F, I, w, dep, attr, col.rgb, a, a < 31u);
    }
  } else if (a >= 31u) { // opaque: colour, depth, ID, fog and edge flags replaced (the stencil kept)
    uvec4 f = (F & ~uvec4(255u)) | (F & F_STENCIL) | uvec4(F_WRITTEN | ((attr & 0x8000u) != 0u ? F_FOG : 0u)) | (edge & F_EDGE);
    C = g_sel(C, uvec4(col.r | (col.g << 6) | (col.b << 12) | (31u << 18)), P);
    D = g_sel(D, dep, P);
    I = g_sel(I, uvec4(id | (255u << 8)), P);
    F = g_sel(F, f, P);
  } else { // translucent: not over a pixel the same polygon ID wrote translucently
    g_trans4(C, D, F, I, P & ~same, dep, attr, col.rgb, a, true);
  }
  return g_m(notEqual(C, C0)) | g_m(notEqual(D, D0)) | g_m(notEqual(F & 255u, F0 & 255u));
}
