// The fast 3D renderer: one fragment of polygon k through pass R's per-pixel rules (G4 texture, G5 plot) on the pixel's state read
// from the colour attachment (programmable blending: SPIRV-Cross's --msl-framebuffer-fetch makes the subpass input the
// attachment). The state is pass R's k = 1 texel (g00_raster.frag's r_out): depth, IDs | flags << 16 | coverage << 24, colour,
// the last polygon that changed it. "#1f": Filter=bilinear (G_FILTER, as pass R's). AA off; 4 samples are g10_fast4.frag's
// (and g10_pack.frag on GL).
// GL (src/g10_fast_gl.c): "+G10_FETCH" reads the state as the output's old value (EXT_shader_framebuffer_fetch's inout);
// "+G10_ILOCK" / "+G10_IORDER" from an image, the fragments of a pixel in primitive order (ARB_fragment_shader_interlock /
// INTEL_fragment_shader_ordering): this main runs as g10_main inside the ordered part (the end of the file).
#if defined(G10_ILOCK) || defined(G10_IORDER)
#define G10_IMAGE 1
#endif
#ifdef GPU_SPIRV // (GLSL ES 3.00 and 3.30 have no locations on the varyings: they go by name)
#define G10_VARY(n) layout(location = n)
#else
#define G10_VARY(n)
#endif
#if defined(G10_FETCH)
layout(location = 0) inout highp uvec4 oS;
#define G10_LOAD oS
#elif defined(G10_IMAGE)
layout(rgba32ui, binding = 0) coherent uniform highp uimage2D uS;
uvec4 oS, gA;
#define G10_LOAD gA
#define main g10_main
#else
layout(input_attachment_index = 0, set = 0, binding = 9) uniform highp usubpassInput iS;
layout(location = 0) out uvec4 oS;
#define G10_LOAD subpassLoad(iS)
#endif
G10_VARY(0) in vec3 vCol;
G10_VARY(1) in vec2 vSt;
G10_VARY(2) in float vW;
G10_VARY(3) flat in int vK;
G10_VARY(4) flat in int vF;
G10_VARY(5) flat in vec3 vE0; // the fan triangle's polygon edges (g10_fast.vert)
G10_VARY(6) flat in vec3 vE1;
G10_VARY(7) flat in vec3 vE2;
G10_VARY(8) flat in vec3 vE3; // polygon edges 0 and nv - 1: grow_poly moves the fan's shared vertex 0 out of both, so every fan
G10_VARY(9) flat in vec3 vE4; // triangle reaches past them (a quad's second triangle missed edge 0: no grown-pixel rule there)
void main() {
  // the planes' steps a pixel (before any branch: the 2 x 2 quad's derivatives), for a grown pixel's depth at its edge (below)
  vec2 dz = vec2(dFdx(gl_FragCoord.z), dFdy(gl_FragCoord.z)), dw = vec2(dFdx(vW), dFdy(vW));
  vec2 dsx = dFdx(vSt), dsy = dFdy(vSt);
  vec3 dcx = dFdx(vCol), dcy = dFdy(vCol);
  uvec4 a = G10_LOAD;
  int k = vK;
  GPixel S;
  S.depth = a.x;
  S.opaque_id = a.y & 255u;
  S.trans_id = (a.y >> 8) & 255u;
  S.flags = (a.y >> 16) & 255u;
  S.cov = a.y >> 24;
  S.color = uvec4(a.z & 255u, (a.z >> 8) & 255u, (a.z >> 16) & 255u, a.z >> 24);
  S.last = a.w;
  GPx px;
  px.x = int(gl_FragCoord.x);
  px.row = int(gl_FragCoord.y);
  px.w = vW;
  px.rgb = uvec3(clamp(ivec3(floor((vCol + 1e-3) / 8.0)), ivec3(0), ivec3(63)));
  px.st = ivec2(floor(vSt + 1e-3));
  px.depth = (uSwap & 2u) != 0u ? uint(min(vW + 0.5, 16777215.0)) : uint(gl_FragCoord.z * 16777215.0 + 0.5);
  px.edge = (vF & 1) != 0; // lines, dots (src/g10_fast.h GPU_FAST_ALLEDGE)
  px.cov = (vF & 2) != 0 ? 0u : 31u;
  // a filled polygon's edge pixel: a 4-neighbour outside (an edge less than a pixel away along its minor axis), its coverage the
  // part of the pixel inside along that axis (the DS's coverage is the covered fraction too; its formula is p16's)
  vec3 q = vec3(gl_FragCoord.xy, 1.0);
  float d0 = dot(vE0, q), d1 = dot(vE1, q), d2 = dot(vE2, q), d3 = dot(vE3, q), d4 = dot(vE4, q);
  float d = min(min(min(d0, d1), min(d2, d3)), d4);
  // the nearest edge's normal: 1 a pixel along its minor axis
  vec2 en = d == d0 ? vE0.xy : d == d1 ? vE1.xy : d == d2 ? vE2.xy : d == d3 ? vE3.xy : vE4.xy;
  // the coverage: the DS's pixel x covers [x, x + 1) of its own grid, and the triangles sit half a pixel down and right of it (the
  // GPU samples the DS's grid at the pixel's top-left corner), so its centre is half a pixel on in both axes; the DS's coverage is
  // 1/2 + that centre's distance inside the edge (p16's aa_edge: Y-major 1 - the row centre's x fraction on the floored column,
  // X-major 15 + 32 times the distance in N 16ths; a vertical or flat edge 31). d + 1/2 here was half a pixel short: a vertical
  // left edge's column had 15 and showed the ground below it half through (slivers on Platinum's paths, 2026-11-19 fk2)
  float c0 = d0 + 0.5 * (vE0.x + vE0.y), c1 = d1 + 0.5 * (vE1.x + vE1.y), c2 = d2 + 0.5 * (vE2.x + vE2.y), c3 = d3 + 0.5 * (vE3.x + vE3.y),
        c4 = d4 + 0.5 * (vE4.x + vE4.y);
  // the coverage from every edge, inner (vF bits 16-20, src/g10_fast.c fast_seams) or silhouette: the DS draws a shared edge's pixel
  // with both polygons, each with its coverage, the later one taking it where its depth passes, and pass P blends the winner with a
  // neighbour behind it; here the GPU gives the pixel to one side and the other side's grown pixel (below) competes for it at the
  // edge's depth, as the DS's does. Two rules keep the seams sealed: only a silhouette's coverage 0 leaves a pixel out (cs; on an
  // inner edge the neighbour may not reach it: holes, 2026-11-24), and an inner edge's coverage-0 pixel, which the DS leaves to the
  // neighbour (p16 ends this side's span before it) to be drawn whole, takes the silhouettes' coverage (31 away from them): with 0
  // it showed what lay behind it (specks along the seams). Inner edges had whole coverage from 2026-11-24 to 2026-11-25: hard steps
  // where a roof meets its eave, which the DS's antialiasing smooths (Cherrygrove's houses on the phone; specks 8223 -> 8293, differ
  // -3.6 % at 1x over the 90 captures)
  int im = vF >> 16;
  float ds = min(min(min((im & 1) != 0 ? 1e9 : c0, (im & 2) != 0 ? 1e9 : c1), min((im & 4) != 0 ? 1e9 : c2, (im & 8) != 0 ? 1e9 : c3)),
                 (im & 16) != 0 ? 1e9 : c4);
  float ca = min(min(min(c0, c1), min(c2, c3)), c4);
  bool sil0 = false;
  if (d < 1.0) {
    px.edge = true;
    uint ci = uint(clamp(floor((ca + 0.5) * 32.0), 0.0, 31.0)), cs = uint(clamp(floor((ds + 0.5) * 32.0), 0.0, 31.0));
    px.cov = ci > 0u || d < -1.0 / 512.0 ? ci : cs;
    sil0 = cs == 0u;
  }
  // outside the polygon: a grown top or left edge's pixel (src/g10_fast.c grow_poly) filling a gap between two neighbours; it loses
  // to a polygon at its depth drawn before or after it (as an edge pixel it would take the pixel at an equal depth). Its coverage is
  // the DS's for that column (above), and it is marked F_GROWN: where nothing lies behind it in the end, g06_post's antialiasing
  // keeps its whole colour (coverage 0 drew black dots where the neighbour drawn later does not cover it: Black's battle intro);
  // with something behind it blends to that as the DS's edge pixel does (it kept coverage 31 over a pixel nothing had drawn yet
  // until 2026-11-19, fk4: a dark polygon's grown column stayed opaque over the farther one drawn later, dark specks in Black's
  // battle field). Its depth is never nearer than the polygon's at the
  // edge (the point on the edge along its minor axis, as the DS takes an edge pixel's values from the edge): the plane of a
  // polygon seen almost edge-on (1 pixel wide, its depth steep across) pulled the grown column thousands of steps nearer, and
  // it covered the ground behind it as dark slivers (Platinum's town, 2026-11-19). Across an inner edge (fast_seams) it competes at
  // that depth without the 256, as the DS's span draws the shared pixel with both neighbours: the later one takes it where its depth
  // passes (it always lost until 2026-11-25: roof pixels poked into a house's eave at its peak). Its texture coordinates and colour are the
  // polygon's just inside that edge too: extrapolated past it they sampled the texel beyond the polygon's own (a sand tile's grown
  // column showed the grass column at the other side of its texture: green slivers on the paths, 2026-11-19 fk2)
  bool grown = d < -1.0 / 512.0;
  if (grown) {
    vec2 to = abs(en.x) >= abs(en.y) ? vec2(-d / en.x, 0.0) : vec2(0.0, -d / en.y);
    uint ze = (uSwap & 2u) != 0u ? uint(clamp(vW + dot(dw, to) + 0.5, 0.0, 16777215.0))
                                 : uint(clamp(gl_FragCoord.z + dot(dz, to), 0.0, 1.0) * 16777215.0 + 0.5);
    int ib = d == d0 ? 1 : d == d1 ? 2 : d == d2 ? 4 : d == d3 ? 8 : 16;
    px.depth = min(max(px.depth, ze) + ((im & ib) != 0 ? 0u : 256u), 16777215u);
    vec2 ti = abs(en.x) >= abs(en.y) ? vec2((1.0 / 64.0 - d) / en.x, 0.0) : vec2(0.0, (1.0 / 64.0 - d) / en.y);
    px.st = ivec2(floor(vSt + dsx * ti.x + dsy * ti.y + 1e-3));
    px.rgb = uvec3(clamp(ivec3(floor((vCol + dcx * ti.x + dcy * ti.y + 1e-3) / 8.0)), ivec3(0), ivec3(63)));
  }
  oS = a;
  // an edge pixel of coverage 0 under the DS's antialiasing (DISP3DCNT bit 4): the DS does not draw it (p16's aa_edge: a right
  // edge's column whose x fraction is near 0 ends the span one pixel before it), Fast blended it to the colour below (specks of
  // the colour under the polygon along right edges, Platinum's town 2026-11-19 fk4). Filled polygons only (dots have coverage 0
  // and are drawn); a grown pixel keeps its rule (above)
  // a silhouette's edge pixel of coverage 0 is the DS's neighbour's (p16 ends a right edge's span before it): nothing drawn. Only a
  // silhouette's: on an inner edge the GPU gave the pixel to this polygon alone, and leaving it out left it to nobody (holes)
  if ((uDisp3d & 0x10u) != 0u && (vF & 3) == 0 && sil0 && d >= -1.0 / 512.0) return;
  if (!g_early(S, k, px)) return;
#ifdef G_TEXCACHE // the texture cache's programs ("#1c": shaders/g12_texcache.glsl's G12_HW): s and t's steps a pixel, in texels
  g12_hdx = dsx * (1.0 / 16.0);
  g12_hdy = dsy * (1.0 / 16.0);
#endif
  uvec4 col = g_texture(k, px, uvec4(px.rgb, g_poly(k, 1).w));
  g_plot(S, k, px, col);
  uvec4 o = uvec4(S.depth, S.opaque_id | (S.trans_id << 8) | (S.flags << 16) | (S.cov << 24),
                  S.color.r | (S.color.g << 8) | (S.color.b << 16) | (S.color.a << 24), S.last);
  if (o != a) o = uvec4(o.x, (o.y & 0xFFFFFFu) | (px.cov << 24), o.z, uint(k)); // changed: this polygon's coverage and index
  if (grown && o != a && ((o.y >> 16) & F_TRANS) == 0u) o.y |= F_GROWN << 16;
  oS = o;
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
  gA = imageLoad(uS, p);
  g10_main();
  if (oS != gA) imageStore(uS, p, oS);
#ifdef G10_ILOCK
  endInvocationInterlockARB();
#endif
}
#endif
