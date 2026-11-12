// port/gpu SMAA, the neighbourhood blending (docs/gpu.md "Antialiasing: SMAA"), a library: pass C runs it where it reads the 3D
// layer (g07_compose.frag's c_3d), and g08_smaa_blend.frag for the readbacks. Ported from the SMAA reference implementation
// (SMAA.hlsl, github.com/iryoku/smaa: SMAANeighborhoodBlendingVS/PS; Jimenez, Echevarria, Sousa, Gutierrez, "SMAA: Enhanced
// Subpixel Morphological Antialiasing", 2012) under its MIT licence:
//   Copyright (C) 2013 Jorge Jimenez, Jose I. Echevarria, Belen Masia, Fernando Navarro, Diego Gutierrez
//   (the full notice is in port/gpu/src/g08_smaa.c).
// The 3D layer is integer (r6 g6 b6 a5, what pass C reads), so the reference's bilinear colour fetch is done here: the
// blending offset lies along one axis, a lerp of two texels. The colour is blended premultiplied by its alpha (the layer
// goes over the 2D with its own alpha) and returned as r6 g6 b6 a5, the alpha's bits 5-7 the empty part (blended from
// texels of alpha 0) in eighths, as the 4 samples' resolve gives it; a pixel with no weight is the layer's texel as it is.

vec4 smaa_col(highp usampler2D layer, ivec2 size, ivec2 q) {
  uvec4 t = texelFetch(layer, clamp(q, ivec2(0), size - 1), 0);
  float a = float(t.a & 31u) / 31.0;
  return vec4(vec3(t.rgb & 63u) / 63.0 * a, a);
}

// the layer at pixel p's centre moved by o (one axis, |o| < 1), bilinear; *cov: the drawn part (alpha > 0) there
vec4 smaa_at(highp usampler2D layer, ivec2 size, ivec2 p, vec2 o, out float cov) {
  vec2 x = vec2(p) + o;
  vec2 f = x - floor(x);
  ivec2 i = ivec2(floor(x));
  bool h = o.y == 0.0;
  vec4 c0 = smaa_col(layer, size, i), c1 = smaa_col(layer, size, i + (h ? ivec2(1, 0) : ivec2(0, 1)));
  float t = h ? f.x : f.y;
  cov = mix(c0.a > 0.0 ? 1.0 : 0.0, c1.a > 0.0 ? 1.0 : 0.0, t);
  return mix(c0, c1, t);
}

// pixel p of the layer blended with its neighbours by the weights (SMAA's second pass, RGBA8)
uvec4 smaa_blend(highp usampler2D layer, sampler2D weights, ivec2 p) {
  ivec2 size = textureSize(layer, 0);
  ivec2 hi = size - 1;
  vec4 a;
  a.x = texelFetch(weights, min(p + ivec2(1, 0), hi), 0).a; // right
  a.y = texelFetch(weights, min(p + ivec2(0, 1), hi), 0).g; // below (its top)
  a.wz = texelFetch(weights, p, 0).xz;                      // this pixel's bottom / left
  if (dot(a, vec4(1.0)) < 1e-5) return texelFetch(layer, p, 0);
  bool h = max(a.x, a.z) > max(a.y, a.w); // max(horizontal) > max(vertical)
  vec4 off = h ? vec4(a.x, 0.0, a.z, 0.0) : vec4(0.0, a.y, 0.0, a.w);
  vec2 w = h ? a.xz : a.yw;
  w /= dot(w, vec2(1.0));
  float c0, c1;
  vec4 c = w.x * smaa_at(layer, size, p, off.xy, c0) + w.y * smaa_at(layer, size, p, -off.zw, c1);
  uint al = uint(round(c.a * 31.0));
  if (al == 0u) return uvec4(0u);
  vec3 rgb = clamp(round(c.rgb / c.a * 63.0), 0.0, 63.0);
  uint e = uint(min(round((1.0 - (w.x * c0 + w.y * c1)) * 8.0), 7.0)); // the empty part in eighths (g06_post.frag's p_resolve)
  return uvec4(uvec3(rgb), al | e << 5);
}
