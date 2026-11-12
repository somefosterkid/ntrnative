// The fast 3D renderer with 4 samples on GL (src/g10_fast_gl.c): the four samples' k = 1 texels (uHw's layers) into pass R's k = 2
// layers (g00_raster.frag's r_out and its bits 30, edge, and 31, alike). Metal draws into pass R's layers directly (g10_fast4.frag's
// G10_DIRECT form) and has no pack pass.
uniform highp usampler2DArray uHw; // the four samples (g10_fast4.frag)
layout(location = 0) out uvec2 oS[4];
uvec2 hp_pk(uvec4 a) {
  uint C = (a.z & 63u) | (((a.z >> 8) & 63u) << 6) | (((a.z >> 16) & 63u) << 12) | ((a.z >> 24) << 18);
  uint F = ((a.y >> 16) & 255u) | ((a.y >> 24) << 8);
  uint I = a.y & 255u;
  return uvec2(a.x | ((I & 63u) << 24), C | ((F & 7u) << 23) | (((F >> 4) & 1u) << 26) | ((F >> 8) << 27));
}
void main() {
  ivec2 p = ivec2(gl_FragCoord.xy);
  uvec4 s0 = texelFetch(uHw, ivec3(p, 0), 0), s1 = texelFetch(uHw, ivec3(p, 1), 0), s2 = texelFetch(uHw, ivec3(p, 2), 0),
        s3 = texelFetch(uHw, ivec3(p, 3), 0);
  oS[0] = hp_pk(s0);
  oS[1] = hp_pk(s1);
  oS[2] = hp_pk(s2);
  oS[3] = hp_pk(s3);
  uvec4 fl = uvec4((s0.y >> 16) & 255u, (s1.y >> 16) & 255u, (s2.y >> 16) & 255u, (s3.y >> 16) & 255u);
  if (any(notEqual(fl & F_EDGE, uvec4(0u)))) oS[0].x |= 1u << 30;
  uvec4 C = uvec4(s0.z, s1.z, s2.z, s3.z), I = uvec4(s0.y & 63u, s1.y & 63u, s2.y & 63u, s3.y & 63u);
  uvec4 D = uvec4(s0.x, s1.x, s2.x, s3.x);
  if (all(equal(C, C.xxxx)) && all(equal(fl, fl.xxxx)) && all(equal(I, I.xxxx)) &&
      ((fl.x & F_EDGE) == 0u || (uDisp3d & 0x30u) == 0u) &&
      ((uDisp3d & 0x80u) == 0u || (fl.x & F_FOG) == 0u || all(equal(D >> 9, uvec4(D.x >> 9)))))
    oS[0].x |= 1u << 31;
}
