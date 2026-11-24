// The fast 3D renderer with 4 samples: one fragment per output pixel (g10_fast4.vert) for its four samples, the 2N grid's points
// (2x + i, 2y + j), sample i + 2j in attachment i + 2j (pass R's k = 1 texel each, as g10_fast.frag's). The samples inside the
// triangle (the top-left rule, as the GPU's on the 2N grid), the colour and texture coordinates once at the first sample (pass R
// shades once per output pixel), g_plot4 on the four samples' state. "#1f": Filter=bilinear (G_FILTER). On Metal (G10_DIRECT, below)
// the attachments are pass R's own k = 2 layers (RG32UI, g00_raster.frag's r_out) plus one of the state they drop, so there is no
// pack pass; on GL they are the four samples' k = 1 texels of a grid of their own (g10_pack.frag packs them).
// GL (src/g10_fast_gl.c) as g10_fast.frag's: "+G10_FETCH" the outputs' old values; "+G10_ILOCK" / "+G10_IORDER" the four samples'
// layers of an image, this main as g10_main inside the ordered part (the end of the file), a discard as a return.
#if defined(G10_ILOCK) || defined(G10_IORDER)
#define G10_IMAGE 1
#endif
#ifdef GPU_SPIRV // (GLSL ES 3.00 and 3.30 have no locations on the varyings: they go by name)
#define G10_VARY(n) layout(location = n)
#else
#define G10_VARY(n)
#endif
#if defined(G10_FETCH)
layout(location = 0) inout highp uvec4 oS0;
layout(location = 1) inout highp uvec4 oS1;
layout(location = 2) inout highp uvec4 oS2;
layout(location = 3) inout highp uvec4 oS3;
#define G10_LOAD4 uvec4 s0 = oS0, s1 = oS1, s2 = oS2, s3 = oS3
#define G10_DISCARD discard
#elif defined(G10_IMAGE)
layout(rgba32ui, binding = 0) coherent uniform highp uimage2DArray uS;
uvec4 oS0, oS1, oS2, oS3, gS0, gS1, gS2, gS3;
#define G10_LOAD4 uvec4 s0 = gS0, s1 = gS1, s2 = gS2, s3 = gS3
#define G10_DISCARD return
#define main g10_main
#else // Metal (G10_DIRECT): pass R's own RG32UI layers as the attachments (no pack pass; src/g10_fast_mtl.inc), the state they
// do not hold (the translucent ID, F_STENCIL) in a fifth, never stored (a byte a sample: ID | stencil << 6)
#define G10_DIRECT 1
layout(input_attachment_index = 0, set = 0, binding = 9) uniform highp usubpassInput iS0;
layout(input_attachment_index = 1, set = 0, binding = 10) uniform highp usubpassInput iS1;
layout(input_attachment_index = 2, set = 0, binding = 11) uniform highp usubpassInput iS2;
layout(input_attachment_index = 3, set = 0, binding = 12) uniform highp usubpassInput iS3;
layout(input_attachment_index = 4, set = 0, binding = 13) uniform highp usubpassInput iS4;
layout(location = 0) out uvec4 oS0; // (uvec4 as the inputs: Metal wants one type on an attachment; the formats take .xy and .x)
layout(location = 1) out uvec4 oS1;
layout(location = 2) out uvec4 oS2;
layout(location = 3) out uvec4 oS3;
layout(location = 4) out uvec4 oA;
#define G10_LOAD4 uvec4 s0 = subpassLoad(iS0), s1 = subpassLoad(iS1), s2 = subpassLoad(iS2), s3 = subpassLoad(iS3); uint sa = subpassLoad(iS4).x
#ifdef G10_KEEP // "+G10_KEEP" (BWPORT_FAST4=keep): no discard, the old values written back (the same pictures)
#define G10_DISCARD                                                                                                     \
  {                                                                                                                     \
    oS0 = subpassLoad(iS0), oS1 = subpassLoad(iS1), oS2 = subpassLoad(iS2), oS3 = subpassLoad(iS3), oA = subpassLoad(iS4); \
    return;                                                                                                             \
  }
#else
#define G10_DISCARD discard
#endif
#endif
G10_VARY(0) flat in ivec3 vA;
G10_VARY(1) flat in ivec3 vB;
G10_VARY(2) flat in ivec3 vC;
G10_VARY(3) flat in ivec2 vO;
G10_VARY(4) flat in vec3 vZ;
G10_VARY(5) flat in vec3 vIW;
G10_VARY(6) flat in vec3 vR;
G10_VARY(7) flat in vec3 vG;
G10_VARY(8) flat in vec3 vBl;
G10_VARY(9) flat in vec3 vS;
G10_VARY(10) flat in vec3 vT;
G10_VARY(11) flat in int vK;
G10_VARY(12) flat in int vF;
G10_VARY(13) flat in vec3 vE0;
G10_VARY(14) flat in vec3 vE1;
G10_VARY(15) flat in vec3 vE2;
float pl(vec3 p, vec2 d) { return p.x + p.y * d.x + p.z * d.y; }
void main() {
  int k = vK;
  ivec2 P = ivec2(gl_FragCoord.xy);
  ivec2 g0 = P * 4 + 1 - vO; // sample 0 in G relative to vertex 0
  ivec3 E0 = vA * g0.x + vB * g0.y + vC;
  bvec3 tl = bvec3(uvec3(greaterThan(vA, ivec3(0))) | (uvec3(equal(vA, ivec3(0))) & uvec3(greaterThan(vB, ivec3(0)))));
  // the polygon's edges at the samples (2N pixels along the minor axis, positive inside), for the grown samples and the edge flags
  vec3 q0 = vec3(vec2(P * 2) + 0.5, 1.0);
  vec3 e0 = vec3(dot(vE0, q0), dot(vE1, q0), dot(vE2, q0)), ex = vec3(vE0.x, vE1.x, vE2.x), ey = vec3(vE0.y, vE1.y, vE2.y);
  // a filled polygon's top and left edges grown out on the 2N grid as AA off's on the N grid (src/g10_fast.c grow_poly: 7/8 of a
  // pixel X-major, 63/64 Y-major): the DS takes a left edge's floored x and an X-major edge's whole run, so where two polygons'
  // edges are not quite one (a vertex of one mid-way along the other's edge, each clipped on its own) it leaves no sample out, and
  // the GPU's rule left single samples empty between them: with the game's edge marking on, each drew a dark mark on its
  // neighbours, dotted black lines along tile edges (Platinum's town, 2026-11-19 fk3). The fan's inner edges (tri edge 0 from
  // vertex 0 unless the triangle is the first, tri edge 2 back to it unless the last) do not grow
  int nv = (vF >> 4) & 15, ti = vF >> 8;
  vec3 gr = vec3(0.0);
  if (nv > 0) {
    bvec3 ptl = bvec3(uvec3(greaterThan(ex, vec3(0.0))) | (uvec3(equal(ex, vec3(0.0))) & uvec3(greaterThan(ey, vec3(0.0)))));
    vec3 amt = mix(vec3(0.984375), vec3(0.875), greaterThan(abs(ey), abs(ex)));
    gr = mix(vec3(0.0), amt, ptl);
  }
  bool dg0 = ti != 1, dg2 = ti + 2 != nv;
  uvec4 hit, grown = uvec4(0u);
  for (int i = 0; i < 4; i++) {
    ivec3 E = E0 + vA * (2 * (i & 1)) + vB * (2 * (i >> 1));
    bvec3 in3 = bvec3(uvec3(greaterThan(E, ivec3(0))) | (uvec3(equal(E, ivec3(0))) & uvec3(tl)));
    hit[i] = all(in3) ? ~0u : 0u;
    vec3 e3 = e0 + ex * float(i & 1) + ey * float(i >> 1);
    if (!all(in3) && nv > 0 && (in3.x || !dg0) && (in3.z || !dg2) && all(greaterThanEqual(e3, -gr))) grown[i] = ~0u;
  }
  hit |= grown;
  if (all(equal(hit, uvec4(0u)))) G10_DISCARD; // (the attachments keep their values)
  uvec4 dep;
  bool wbuf = (uSwap & 2u) != 0u;
  for (int i = 0; i < 4; i++) {
    vec2 d = vec2(g0 + ivec2(2 * (i & 1), 2 * (i >> 1)));
    dep[i] = wbuf ? uint(min(1.0 / pl(vIW, d) + 0.5, 16777215.0)) : uint(clamp(pl(vZ, d) + 0.5, 0.0, 16777215.0));
    if (grown[i] != 0u) { // a grown sample loses to a polygon at its depth (+ 256, AA off's rule); never nearer than at the edge
      vec3 e3 = e0 + ex * float(i & 1) + ey * float(i >> 1);
      float e = min(min(e3.x, e3.y), e3.z);
      vec2 en = e == e3.x ? vE0.xy : e == e3.y ? vE1.xy : vE2.xy;
      vec2 de = d + 2.0 * (abs(en.x) >= abs(en.y) ? vec2(-e / en.x, 0.0) : vec2(0.0, -e / en.y)); // (G: twice the 2N grid)
      uint ze = wbuf ? uint(min(1.0 / pl(vIW, de) + 0.5, 16777215.0)) : uint(clamp(pl(vZ, de) + 0.5, 0.0, 16777215.0));
      dep[i] = min(max(dep[i], ze) + 256u, 16777215u);
    }
  }
  G10_LOAD4;
#ifdef G10_DIRECT
  uvec4 X = uvec4(s0.x, s1.x, s2.x, s3.x), D = X & 0xFFFFFFu;
#else
  uvec4 D = uvec4(s0.x, s1.x, s2.x, s3.x);
#endif
  uint attr = g_poly(k, 0).x;
  if (((attr >> 4) & 3u) != 3u) { // g_early on the four: none passes, nothing changes
    bvec4 pass = (attr & (1u << 14)) != 0u ? lessThanEqual(max(dep, D) - min(dep, D), uvec4(0x200u)) : lessThan(dep, D);
    if (!any(bvec4(uvec4(pass) & hit))) G10_DISCARD;
  }
  // the colour and texture coordinates at the first sample
  vec2 d0 = vec2(g0);
  float w0 = 1.0 / pl(vIW, d0);
  GPx px;
  px.x = P.x * 2;
  px.row = P.y * 2;
  px.w = w0;
  px.rgb = uvec3(clamp(ivec3(floor((vec3(pl(vR, d0), pl(vG, d0), pl(vBl, d0)) * w0 + 1e-3) / 8.0)), ivec3(0), ivec3(63)));
  px.st = ivec2(floor(vec2(pl(vS, d0), pl(vT, d0)) * w0 + 1e-3));
  px.depth = dep.x;
  px.edge = false;
  px.cov = 31u;
#ifdef G_TEXCACHE // the cache's programs ("#1c", shaders/g12_texcache.glsl G12_HW): s and t's steps an output pixel (4 G), in texels
  float gs = pl(vS, d0) * w0, gt = pl(vT, d0) * w0;
  g12_hdx = vec2(vS.y - gs * vIW.y, vT.y - gt * vIW.y) * (w0 * 4.0 / 16.0);
  g12_hdy = vec2(vS.z - gs * vIW.z, vT.z - gt * vIW.z) * (w0 * 4.0 / 16.0);
#endif
  uvec4 col = g_texture(k, px, uvec4(px.rgb, g_poly(k, 1).w));
  // the samples' edge flag and coverage
  uvec4 edge, cov;
  for (int i = 0; i < 4; i++) {
    vec3 e3 = e0 + ex * float(i & 1) + ey * float(i >> 1);
    float e = min(min(e3.x, e3.y), e3.z);
    // (a flat edge's samples are edge samples, as the DS's flat edge's run, with whole coverage: AA leaves flat edges alone)
    vec3 ec = mix(e3, vec3(1e9), equal(ex, vec3(0.0)));
    float c = min(min(ec.x, ec.y), ec.z);
    edge[i] = (vF & 1) != 0 || e < 1.0 ? ~0u : 0u;
    cov[i] = c < 1.0 ? uint(clamp(c + 0.5, 0.0, 1.0) * 31.0 + 0.5) : (vF & 2) != 0 ? 0u : 31u;
  }
  // the state as g_plot4's lanes
#ifdef G10_DIRECT
  uvec4 Y = uvec4(s0.y, s1.y, s2.y, s3.y), A = (uvec4(sa) >> uvec4(0u, 8u, 16u, 24u)) & 255u;
  uvec4 C = Y & 0x7FFFFFu, I = ((X >> 24) & 63u) | ((A & 63u) << 8);
  uvec4 F = ((Y >> 23) & 7u) | (((A >> 6) & 1u) << 3) | (((Y >> 26) & 1u) << 4) | ((Y >> 27) << 8);
#else
  uvec4 Y = uvec4(s0.y, s1.y, s2.y, s3.y), Z = uvec4(s0.z, s1.z, s2.z, s3.z);
  uvec4 C = (Z & 63u) | (((Z >> 8) & 63u) << 6) | (((Z >> 16) & 63u) << 12) | ((Z >> 24) << 18);
  uvec4 F = Y >> 16, I = Y & 0xFFFFu;
#endif

  // (a grown sample keeps the DS's coverage, also over one nothing has drawn yet: 31 there until 2026-11-19 fk4, a little farther
  // from Exact in the town and Black's battle captures; the 4 samples cover the gaps, so no black dots either way)
  uvec4 C0 = C, D0 = D, F0 = F, I0 = I;
  g_plot4(C, D, F, I, hit, dep, edge, k, col);
  bvec4 ch = bvec4(uvec4(notEqual(C, C0)) | uvec4(notEqual(D, D0)) | uvec4(notEqual(F & 255u, F0 & 255u)) | uvec4(notEqual(I, I0)));
#ifdef G10_DIRECT
  // the four packed as g00_raster.frag's r_out (and sample 0's bits 30, edge, and 31, alike, as it and g10_pack.frag set them)
  if (!any(ch)) G10_DISCARD;
  uvec4 cv = g_sel(F >> 8, cov, g_m(ch)), fl = F & 255u;
  uvec4 Xn = D | ((I & 63u) << 24), Yn = C | ((F & 7u) << 23) | (((F >> 4) & 1u) << 26) | (cv << 27);
  if (any(notEqual(fl & F_EDGE, uvec4(0u)))) Xn.x |= 1u << 30;
  if (all(equal(C, C.xxxx)) && all(equal(fl, fl.xxxx)) && all(equal(I & 255u, (I & 255u).xxxx)) &&
      ((fl.x & F_EDGE) == 0u || (uDisp3d & 0x30u) == 0u) &&
      ((uDisp3d & 0x80u) == 0u || (fl.x & F_FOG) == 0u || all(equal(D >> 9, uvec4(D.x >> 9)))))
    Xn.x |= 1u << 31;
  uvec4 An = ((I >> 8) & 63u) | (((F >> 3) & 1u) << 6);
  oS0 = uvec4(Xn.x, Yn.x, 0u, 0u);
  oS1 = uvec4(Xn.y, Yn.y, 0u, 0u);
  oS2 = uvec4(Xn.z, Yn.z, 0u, 0u);
  oS3 = uvec4(Xn.w, Yn.w, 0u, 0u);
  oA = uvec4(An.x | (An.y << 8) | (An.z << 16) | (An.w << 24), 0u, 0u, 0u);
#else
  uvec4 Yn = I | ((F & 255u) << 16) | (cov << 24);
  uvec4 Zn = (C & 63u) | (((C >> 6) & 63u) << 8) | (((C >> 12) & 63u) << 16) | ((C >> 18) << 24);
  oS0 = ch.x ? uvec4(D.x, Yn.x, Zn.x, uint(k)) : s0;
  oS1 = ch.y ? uvec4(D.y, Yn.y, Zn.y, uint(k)) : s1;
  oS2 = ch.z ? uvec4(D.z, Yn.z, Zn.z, uint(k)) : s2;
  oS3 = ch.w ? uvec4(D.w, Yn.w, Zn.w, uint(k)) : s3;
#endif
}
#ifdef G10_IMAGE
#undef main
#ifdef G10_ILOCK
layout(pixel_interlock_ordered) in;
#endif
void main() {
  ivec2 p = ivec2(gl_FragCoord.xy);
#ifdef G10_ILOCK
  beginInvocationInterlockARB();
#else
  beginFragmentShaderOrderingINTEL();
#endif
  gS0 = imageLoad(uS, ivec3(p, 0));
  gS1 = imageLoad(uS, ivec3(p, 1));
  gS2 = imageLoad(uS, ivec3(p, 2));
  gS3 = imageLoad(uS, ivec3(p, 3));
  oS0 = gS0, oS1 = gS1, oS2 = gS2, oS3 = gS3;
  g10_main();
  if (oS0 != gS0) imageStore(uS, ivec3(p, 0), oS0);
  if (oS1 != gS1) imageStore(uS, ivec3(p, 1), oS1);
  if (oS2 != gS2) imageStore(uS, ivec3(p, 2), oS2);
  if (oS3 != gS3) imageStore(uS, ivec3(p, 3), oS3);
#ifdef G10_ILOCK
  endInvocationInterlockARB();
#endif
}
#endif
