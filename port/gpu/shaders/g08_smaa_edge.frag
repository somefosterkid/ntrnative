// port/gpu SMAA, pass 1: the edges of the 3D layer at N (docs/gpu.md "Antialiasing: SMAA"). The SMAA passes are adapted
// from the SMAA reference implementation (SMAA.hlsl, github.com/iryoku/smaa; Jimenez, Echevarria, Sousa, Gutierrez, "SMAA:
// Enhanced Subpixel Morphological Antialiasing", 2012) under its MIT licence:
//   Copyright (C) 2013 Jorge Jimenez, Jose I. Echevarria, Belen Masia, Fernando Navarro, Diego Gutierrez
//   (the full notice is in port/gpu/src/g08_smaa.c).
// The detector is ours: an edge is where pass R's aux (uSamp, g00_head.glsl: depth, opaque ID | translucent ID << 8 |
// flags << 16, colour, the last polygon) says two polygons meet at an opaque polygon's edge, AND the reference's luma step
// (SMAALumaEdgeDetectionPS's weights) is at least aLuma. The aux keeps the pixel art inside a polygon (textures,
// alpha-tested sprites, the translucent shadows) as it is; the luma step drops the meeting lines of one surface's polygons
// (a flat mesh's triangles: the DS's vertex depth is 15 bits times 0x200, so their slopes differ and a depth test alone
// finds them; the battle platforms' fans). Out (RG8): r = an edge on the left of the pixel, g = above it (row y - 1).
uniform highp usampler2D aLayer; // pass P's 3D layer, r6 g6 b6 a5
// aLuma: g00_head.glsl's block
layout(location = 0) out vec4 oEdge;

ivec2 a_size;

uvec4 a_aux(ivec2 q) { return g_samp(clamp(q, ivec2(0), a_size - 1)); }

// two polygons meet between pixel P and its neighbour Q (left or above); PP is the pixel past P, QQ the one past Q
bool a_meet(uvec4 P, uvec4 Q, uvec4 PP, uvec4 QQ) {
  // the same polygon last changed both: inside it, unless it is a translucent one on both (a fade, weather, water over the
  // scene): then the opaque IDs and depth below decide, as the translucent polygon keeps them (and the edge flag)
  if (P.w == Q.w && (((P.y & Q.y) >> 16) & F_TRANS) == 0u) return false;
  // neither is on an opaque polygon's edge: an alpha-test cutout, or a translucent polygon's side (the DS flags no
  // translucent edges; the shadows under the battle's Pokemon are translucent pixel art)
  if ((((P.y | Q.y) >> 16) & F_EDGE) == 0u) return false;
  if (((P.y ^ Q.y) & 63u) != 0u) return true; // the opaque IDs differ
  // a silhouette or a crease, unless the slope across the step is the slope on both sides (one plane, within the
  // vertex depth's 0x200)
  float dp = float(P.x), dq = float(Q.x), d = dp - dq, s1 = dq - float(QQ.x), s2 = float(PP.x) - dp;
  float tol = 512.0 + 0.25 * max(abs(s1), abs(s2));
  return abs(d - s1) > tol || abs(d - s2) > tol;
}

float a_luma(ivec2 q) {
  uvec4 t = texelFetch(aLayer, clamp(q, ivec2(0), a_size - 1), 0);
  return dot(vec3(t.rgb & 63u), vec3(0.2126, 0.7152, 0.0722)) / 63.0 * float(t.a & 31u) / 31.0;
}

void main() {
  ivec2 p = ivec2(gl_FragCoord.xy);
  a_size = g_samp_size();
  const ivec2 X = ivec2(1, 0), Y = ivec2(0, 1);
  float l = a_luma(p);
  vec2 e = step(vec2(aLuma), abs(l - vec2(a_luma(p - X), a_luma(p - Y))));
  if (p.x == 0) e.x = 0.0;
  if (p.y == 0) e.y = 0.0;
  if (e.x + e.y > 0.0) {
    uvec4 P = g_samp(p);
    if (e.x > 0.0 && !a_meet(P, a_aux(p - X), a_aux(p + X), a_aux(p - 2 * X))) e.x = 0.0;
    if (e.y > 0.0 && !a_meet(P, a_aux(p - Y), a_aux(p + Y), a_aux(p - 2 * Y))) e.y = 0.0;
  }
  oEdge = vec4(e, 0.0, 0.0);
}
