// The fast 3D renderer with 4 samples: the rear plane or clear image (g06_clear.glsl's g_clear) into the four samples' texels, the
// first draw of the triangles' pass (src/g10_fast_mtl.inc; the four samples of an output pixel are on one DS pixel)
#ifdef GPU_SPIRV // Metal: pass R's layers and the fifth attachment (g10_fast4.frag's G10_DIRECT form)
layout(location = 0) out uvec4 oS0; // (.xy and .x are taken: RG32UI and R32UI)
layout(location = 1) out uvec4 oS1;
layout(location = 2) out uvec4 oS2;
layout(location = 3) out uvec4 oS3;
layout(location = 4) out uvec4 oA;
#else
layout(location = 0) out uvec4 oS0;
layout(location = 1) out uvec4 oS1;
layout(location = 2) out uvec4 oS2;
layout(location = 3) out uvec4 oS3;
#endif
void main() {
  GPixel S;
  S.color = uvec4(0u);
  S.depth = 0u;
  S.opaque_id = 0u;
  S.trans_id = 0u;
  S.flags = 0u;
  S.cov = 0u;
  S.last = 0u;
  g_clear(S, ivec2(gl_FragCoord.xy) * 2);
#ifdef GPU_SPIRV
  uint C = S.color.r | (S.color.g << 6) | (S.color.b << 12) | (S.color.a << 18);
  oS0 = uvec4(S.depth | ((S.opaque_id & 63u) << 24), C | ((S.flags & 7u) << 23) | (((S.flags >> 4) & 1u) << 26) | (S.cov << 27), 0u, 0u);
  if ((S.flags & F_EDGE) != 0u) oS0.x |= 1u << 30;                                   // (the four are the same: alike unless
  if ((S.flags & F_EDGE) == 0u || (uDisp3d & 0x30u) == 0u) oS0.x |= 1u << 31;         // an edge flag pass P acts on)
  oS1 = oS2 = oS3 = oS0;
  uint a = (S.trans_id & 63u) | (((S.flags >> 3) & 1u) << 6);
  oA = uvec4(a | (a << 8) | (a << 16) | (a << 24), 0u, 0u, 0u);
#else
  oS0 = uvec4(S.depth, S.opaque_id | (S.trans_id << 8) | (S.flags << 16) | (S.cov << 24),
              S.color.r | (S.color.g << 8) | (S.color.b << 16) | (S.color.a << 24), S.last);
  oS1 = oS0;
  oS2 = oS0;
  oS3 = oS0;
#endif
}
