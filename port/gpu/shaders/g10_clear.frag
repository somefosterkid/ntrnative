// The fast 3D renderer: the rear plane or clear image (g06_clear.glsl's g_clear) into pass R's k = 1 texel, the first draw of
// the triangles' pass (src/g10_fast_mtl.inc)
layout(location = 0) out uvec4 oS;
void main() {
  GPixel S;
  S.color = uvec4(0u);
  S.depth = 0u;
  S.opaque_id = 0u;
  S.trans_id = 0u;
  S.flags = 0u;
  S.cov = 0u;
  S.last = 0u;
  g_clear(S, ivec2(gl_FragCoord.xy));
  oS = uvec4(S.depth, S.opaque_id | (S.trans_id << 8) | (S.flags << 16) | (S.cov << 24),
             S.color.r | (S.color.g << 8) | (S.color.b << 16) | (S.color.a << 24), S.last);
}
