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
void main() {
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
  float d = min(min(dot(vE0, q), dot(vE1, q)), dot(vE2, q));
  if (d < 1.0) {
    px.edge = true;
    px.cov = uint(clamp(d + 0.5, 0.0, 1.0) * 31.0 + 0.5);
  }
  // outside the polygon: a grown top or left edge's pixel (src/g10_fast.c grow_poly) filling a gap between two neighbours; it loses
  // to a polygon at its depth drawn before or after it (as an edge pixel it would take the pixel at an equal depth) and keeps its
  // whole colour (coverage 0 drew black dots under the DS's antialiasing)
  if (d < -1.0 / 512.0) {
    px.depth = min(px.depth + 256u, 16777215u);
    px.cov = 31u;
  }
  oS = a;
  if (!g_early(S, k, px)) return;
  uvec4 col = g_texture(k, px, uvec4(px.rgb, g_poly(k, 1).w));
  g_plot(S, k, px, col);
  uvec4 o = uvec4(S.depth, S.opaque_id | (S.trans_id << 8) | (S.flags << 16) | (S.cov << 24),
                  S.color.r | (S.color.g << 8) | (S.color.b << 16) | (S.color.a << 24), S.last);
  if (o != a) o = uvec4(o.x, (o.y & 0xFFFFFFu) | (px.cov << 24), o.z, uint(k)); // changed: this polygon's coverage and index
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
