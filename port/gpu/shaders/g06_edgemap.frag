// port/gpu pass E (G11): where pass P's Edge=ds thickening can find an edge-flagged pixel at all. Output is at DS
// resolution (256 x 192, R8UI). Stage 0: 1 where any of the DS pixel's N k x N k samples has the edge flag (pass R's, uSamp). Stage 1:
// the OR over the 3 x 3 neighbours of stage 0. The thickening looks at most N - 1 pixels along an axis from a pixel, which
// lies in the same or a neighbouring DS pixel, so a pixel whose stage-1 value is 0 has no marked pixel to find.
// Multisampling (k = 2): stage 0 runs per output pixel (at N, R16UI: 4 bits, the samples' p_fine_mark, and 12 with antialiasing
// on, the samples' antialiasing neighbours), stage 2 makes its DS pixels, stage 1 as above, and stage 3 (at N, 4 bits) the
// samples' whole p_edge_mark: the fine mark, or the Edge=ds (or fine) thickening walk, so that pass P reads one bit where it
// walked and its slow draw the neighbour where it searched (2026-11).
uniform highp usampler2D uEm;  // stage 1: stage 0 (k = 2: stage 2); stages 2 and 3: stage 0
#if G_K == 2
uniform highp usampler2D uEdgeMap; // stage 3: stage 1
#endif
// uStage: g00_head.glsl's block
layout(location = 0) out uvec4 oOut;

const ivec2 E_DIR[4] = ivec2[4](ivec2(0, -1), ivec2(0, 1), ivec2(-1, 0), ivec2(1, 0));
// pass P's p_fine_mark: an edge-flagged pixel with a 4-neighbour of another ID and a greater depth
#if G_K == 1
bool e_fine(ivec2 q) {
  uvec4 s = g_samp(q);
  if ((((s.y >> 16) & 255u) & F_EDGE) == 0u) return false;
  ivec2 sz = g_samp_size();
  for (int k = 0; k < 4; k++) {
    ivec2 n = q + E_DIR[k];
    if (n.x < 0 || n.y < 0 || n.x >= sz.x || n.y >= sz.y) {
      if (((uClearColor >> 24) & 63u) != (s.y & 63u)) return true;
    } else {
      uvec4 a = g_samp(n);
      if ((a.y & 63u) != (s.y & 63u) && s.x < a.x) return true;
    }
  }
  return false;
}
#else
// k = 2: sample s against its four neighbours n0-n3 (E_DIR order; on[k] false: off the grid), fetched by the caller, as 1 or
// 0; and best, pass P's antialiasing neighbour of an opaque edge sample (with antialiasing on, a pixel that is not alike: its
// slow draw reads it only there): the farthest 4-neighbour behind it on the grid (g06_post.frag's p_sample; its E_DIR index,
// 4 none)
uint e_fine4(uvec4 s, uvec4 n0, uvec4 n1, uvec4 n2, uvec4 n3, bvec4 on, out uint best) {
  best = 4u;
  if ((((s.y >> 16) & 255u) & F_EDGE) == 0u) return 0u;
  uint id = s.y & 63u, udep = s.x;
  bool mark = false;
  uvec4 n[4] = uvec4[4](n0, n1, n2, n3);
  for (int k = 0; k < 4; k++) {
    if (!on[k]) {
      mark = mark || ((uClearColor >> 24) & 63u) != id;
    } else {
      mark = mark || ((n[k].y & 63u) != id && s.x < n[k].x);
      if (n[k].x > udep) {
        udep = n[k].x;
        best = uint(k);
      }
    }
  }
  return mark ? 1u : 0u;
}
uvec4 e_samp(ivec2 b, int l) { return g_unpack(texelFetch(uSamp, ivec3(b, l), 0).xy); } // pixel b's sample l (layer)

// pass P's thickening walk (g06_post.frag's p_edge_mark, the fine marks from stage 0) for the four samples of the pixel at q,
// of opaque IDs id and F_WRITTEN wr. A walk from a sample along a direction marks it when the first sample on the line that
// stage 0 marked fine lies within T - 1 steps and every sample up to it has the walker's ID: the fine bits of the T / 2
// pixels ahead (one fetch each, for both lines of the direction) give that sample's step, then the IDs up to it are
// fetched, none depending on another (the walk's own form, a fetch deciding the next, cost twice this; 2026-11). Along each
// direction a pair of samples walks the same line, the trailing one a step behind: its first step is the leading sample (the
// walk stops there when the IDs differ and ends there when that sample is fine), its later steps the leading sample's walk
// one step short. So one walk from the leading sample (by its ID, written or not) serves both. Bit j k + i: sample (i, j).
uint e_thick(ivec2 q, uvec4 id, bvec4 wr, uint fine, int T) {
  uint mk = 0u;
  ivec2 b = q >> 1, nb = g_samp_size() >> 1;
  int np = T >> 1; // the pixels the walks reach along a direction
  uint steps = (1u << uint(T - 1)) - 1u;
  for (int k = 0; k < 4; k++) {
    ivec2 d = E_DIR[k];
    bool fwd = d.x + d.y > 0;
    // line l's fine samples by step: bit i - 1 is the sample i steps ahead of the pair's leading sample
    uint m0 = 0u, m1 = 0u;
    for (int p = 1; p <= np; p++) {
      ivec2 c = b + d * p;
      if (c.x < 0 || c.y < 0 || c.x >= nb.x || c.y >= nb.y) break;
      uint f = texelFetch(uEm, c, 0).x;
      // the pixel's samples along the axis: sn the near one (step 2p - 1), sf the far one (step 2p)
      uint sn = fwd ? 0u : 1u, sf = 1u - sn;
      uint sh = uint(2 * p - 2);
      if (d.x != 0) {
        m0 |= (((f >> sn) & 1u) | ((f >> sf) & 1u) << 1) << sh;
        m1 |= (((f >> (2u + sn)) & 1u) | ((f >> (2u + sf)) & 1u) << 1) << sh;
      } else {
        m0 |= (((f >> (2u * sn)) & 1u) | ((f >> (2u * sf)) & 1u) << 1) << sh;
        m1 |= (((f >> (2u * sn + 1u)) & 1u) | ((f >> (2u * sf + 1u)) & 1u) << 1) << sh;
      }
    }
    for (int l = 0; l < 2; l++) {
      ivec2 a = d.x != 0 ? ivec2(0, l) : ivec2(l, 0), e = a + abs(d); // the pair, e after a along x or y
      ivec2 lead = fwd ? e : a, trail = fwd ? a : e;
      int li = lead.y * 2 + lead.x, ti = trail.y * 2 + trail.x;
      uint lb = 1u << uint(li), tb = 1u << uint(ti), m = (l == 0 ? m0 : m1) & steps;
      bool needl = wr[li] && (mk & lb) == 0u, needt = wr[ti] && (mk & tb) == 0u && id[ti] == id[li];
      if (needt && (fine & lb) != 0u) {
        mk |= tb;
        needt = false;
      }
      if (m == 0u || (!needl && !needt)) continue;
      int s = 1; // the first fine sample's step (GLSL ES 3.00 has no findLSB)
      for (; (m & 1u) == 0u; m >>= 1) s++;
      uint bad = 0u;
      for (int i = 1; i <= s; i++) bad |= (g_samp(q + lead + d * i).y & 63u) ^ id[li];
      if (bad != 0u) continue;
      if (needl) mk |= lb;
      if (needt && s <= T - 2) mk |= tb;
    }
  }
  return mk;
}
#endif

void main() {
  ivec2 b = ivec2(gl_FragCoord.xy);
  uint v = 0u;
  if (uStage == 0) {
#if G_K == 2
    // multisampling: this stage runs per output pixel (b), into a map at N; stage 2 makes its DS pixels. Only the samples of a
    // pixel pass R marked as having an edge-flagged one (sample 0's bit 30 of .x) are looked at. (A fragment per DS pixel looking
    // at its 4 N^2 samples cost 1.7 ms at 3x, M2 2026-11.)
    if ((texelFetch(uSamp, ivec3(b, 0), 0).x & (1u << 30)) != 0u) {
      ivec2 q = b * 2;
      // bit (j k + i): sample (i, j) is a marked edge (pass P's p_fine_mark: stage 3's thickening reads it here); bits 4 + 3 (j k
      // + i) up: its antialiasing neighbour (pass P's slow draw reads it here, R16UI). The four samples (s0-s3) and their eight
      // 4-neighbours outside the pixel are fetched once each (on4: which of those lie on the grid; e_fine per sample fetched
      // 20 and cost 0.08 ms against 0.05, M2 3x, 2026-11-04)
      ivec2 sz = g_samp_size() >> 1;
      bvec4 on4 = bvec4(b.y > 0, b.y + 1 < sz.y, b.x > 0, b.x + 1 < sz.x); // up, down, left, right
      ivec2 bu = ivec2(b.x, max(b.y - 1, 0)), bd = ivec2(b.x, min(b.y + 1, sz.y - 1)), bl = ivec2(max(b.x - 1, 0), b.y),
            br = ivec2(min(b.x + 1, sz.x - 1), b.y);
      uvec4 s0 = e_samp(b, 0), s1 = e_samp(b, 1), s2 = e_samp(b, 2), s3 = e_samp(b, 3);
      uvec4 u0 = e_samp(bu, 2), u1 = e_samp(bu, 3), d0 = e_samp(bd, 0), d1 = e_samp(bd, 1);
      uvec4 l0 = e_samp(bl, 1), l2 = e_samp(bl, 3), r1 = e_samp(br, 0), r3 = e_samp(br, 2);
      uint n0, n1, n2, n3;
      v = e_fine4(s0, u0, s2, l0, s1, bvec4(on4.x, true, on4.z, true), n0) |
          e_fine4(s1, u1, s3, s0, r1, bvec4(on4.x, true, true, on4.w), n1) << 1 |
          e_fine4(s2, s0, d0, l2, s3, bvec4(true, on4.y, on4.z, true), n2) << 2 |
          e_fine4(s3, s1, d1, s2, r3, bvec4(true, on4.y, true, on4.w), n3) << 3;
      v |= (n0 | n1 << 3 | n2 << 6 | n3 << 9) << 4;
    }
  } else if (uStage == 3) {
    // the samples' p_edge_mark (bit j k + i: sample (i, j)); where the DS pixel's map (stage 1) is 0 there is nothing to find
    ivec2 q = b * 2;
    int T = uEdgeFine != 0u ? uSnap : uScale; // Edge=ds: N k samples (one DS pixel); fine: k
    if (texelFetch(uEdgeMap, q / uScale, 0).x != 0u) {
      uint fine = texelFetch(uEm, b, 0).x & 15u;
      uvec4 s0 = g_samp(q), id, fl;
      if ((texelFetch(uSamp, ivec3(b, 0), 0).x & (1u << 31)) != 0u) { // alike: the four have sample 0's ID and flags
        id = uvec4(s0.y & 63u);
        fl = uvec4((s0.y >> 16) & 255u);
      } else {
        uvec4 s1 = g_samp(q + ivec2(1, 0)), s2 = g_samp(q + ivec2(0, 1)), s3 = g_samp(q + ivec2(1, 1));
        id = uvec4(s0.y, s1.y, s2.y, s3.y) & 63u;
        fl = (uvec4(s0.y, s1.y, s2.y, s3.y) >> 16) & 255u;
      }
      bvec4 wr = notEqual(fl & F_WRITTEN, uvec4(0u));
      v = fine | (T > 1 && any(wr) ? e_thick(q, id, wr, fine, T) : 0u);
    }
  } else if (uStage == 2) {
    int n = uScale / 2;
    for (int j = 0; j < n; j++)
      for (int i = 0; i < n; i++) v |= texelFetch(uEm, b * n + ivec2(i, j), 0).x & 15u;
#else
    for (int j = 0; j < uScale; j++)
      for (int i = 0; i < uScale; i++)
        if (e_fine(b * uScale + ivec2(i, j))) v = 1u;
#endif
  } else {
    for (int dy = -1; dy <= 1; dy++)
      for (int dx = -1; dx <= 1; dx++) {
        ivec2 c = b + ivec2(dx, dy);
        if (c.x >= 0 && c.y >= 0 && c.x < 256 && c.y < 192) v |= texelFetch(uEm, c, 0).x;
      }
  }
  oOut = uvec4(v, 0u, 0u, 0u);
}
