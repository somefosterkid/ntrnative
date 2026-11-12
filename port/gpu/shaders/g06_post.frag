// port/gpu pass P: edge marking, fog, antialiasing (docs/gpu.md, G6). OWNED BY G6.
// In: pass R's samples (uSamp, g00_head.glsl: depth, opaque ID | translucent ID << 8 | flags << 16 | coverage << 24, colour) of
// this sample and its neighbours on the N k grid. Out: the 3D layer at N (vcore's s->f3d.out), r6, g6, b6, a5. vcore p19's
// vc_3d_post per sample (multisampling, G_K > 1: a fragment per pixel does its samples and the resolve, main below). At N x:
// the neighbours are 1 sample away; Edge=ds
// (default) thickens the outline inward to N k samples (one DS pixel), Edge=fine (uEdgeFine) keeps it one pixel of the N grid
// (uSnap = k samples).
// uEdgeCol, uFogTab, uFogColor, uFogOffset, uEdgeFine: g00_head.glsl's block
#if G_K == 2
uniform highp usampler2D uMark;    // pass E's stage 3 at N (g06_edgemap.frag), bit j k + i: sample (i, j)'s p_edge_mark
uniform highp usampler2D uAaNb;    // pass E's stage 0 at N, bits 4 + 3 (j k + i) up: sample (i, j)'s antialiasing neighbour
#else
uniform highp usampler2D uEdgeMap; // pass E (g06_edgemap.frag): 0 where no edge-flagged pixel is near, DS resolution
#endif
layout(location = 0) out uvec4 oOut;

const ivec2 P_DIR[4] = ivec2[4](ivec2(0, -1), ivec2(0, 1), ivec2(-1, 0), ivec2(1, 0));

struct PPx {
  uvec4 col;
  uint depth, id, flags, cov;
};

PPx p_load(ivec2 q) {
  PPx s;
  uvec4 a = g_samp(q);
  s.col = uvec4(a.z & 255u, (a.z >> 8) & 255u, (a.z >> 16) & 255u, a.z >> 24);
  s.depth = a.x;
  s.id = a.y & 63u; // the opaque ID
  s.flags = (a.y >> 16) & 255u;
  s.cov = (a.y >> 24) & 31u;
  return s;
}

bool p_inside(ivec2 q) {
  ivec2 sz = g_samp_size();
  return q.x >= 0 && q.y >= 0 && q.x < sz.x && q.y < sz.y;
}

// vcore: a flagged edge pixel with a 4-neighbour of another ID and a greater depth (off screen: the rear plane's ID)
bool p_fine_mark(ivec2 q, PPx s) {
  if ((s.flags & F_EDGE) == 0u) return false;
  for (int k = 0; k < 4; k++) {
    ivec2 n = q + P_DIR[k];
    bool hit;
    if (!p_inside(n)) {
      hit = ((uClearColor >> 24) & 63u) != s.id;
    } else {
      uvec4 a = g_samp(n);
      hit = (a.y & 63u) != s.id && s.depth < a.x;
    }
    if (hit) return true;
  }
  return false;
}

#if G_K == 2
// pass E's stage 3 did p_edge_mark below for every sample (read only when pass E ran: edge marking on)
bool p_edge_mark(ivec2 q, PPx s) { return ((texelFetch(uMark, q >> 1, 0).x >> uint(((q.y & 1) << 1) + (q.x & 1))) & 1u) != 0u; }
#else
bool p_edge_mark(ivec2 q, PPx s) {
  if (p_fine_mark(q, s)) return true;
  int T = uEdgeFine != 0u ? uSnap : uScale; // Edge=ds: one DS pixel wide; fine: one pixel of the N grid (k samples)
  if (T <= 1 || (s.flags & F_WRITTEN) == 0u) return false;
  if (texelFetch(uEdgeMap, q / uScale, 0).x == 0u) return false;
  // Edge=ds: also the pixels of the same polygon ID up to N - 1 N x pixels from a marked edge pixel
  for (int k = 0; k < 4; k++)
    for (int i = 1; i < T; i++) {
      ivec2 n = q + P_DIR[k] * i;
      if (!p_inside(n)) break;
      // p_fine_mark reads the neighbour's depth, ID and flags only: one fetch (G11), and the four more only for an edge
      uvec4 a = g_samp(n);
      if ((a.y & 63u) != s.id) break;
      if (((a.y >> 16) & F_EDGE) == 0u) continue;
      PPx t;
      t.depth = a.x;
      t.id = a.y & 63u;
      t.flags = (a.y >> 16) & 255u;
      if (p_fine_mark(n, t)) return true;
    }
  return false;
}
#endif

// GBATEK 51 FOG_TABLE: the density 0-128 at the 15-bit depth (the interpolation's rounding fitted to the oracle)
int p_fog_density(uint d15) {
  int step = max(0x400 >> int((uDisp3d >> 8) & 15u), 1);
  int off = int(uFogOffset), d = int(d15), r;
  if (d < off + step) {
    r = int(uFogTab[0]);
  } else if (d >= off + step * 32) {
    r = int(uFogTab[31]);
  } else {
    int n = (d - off) / step;
    int a = int(uFogTab[n - 1]), b = int(uFogTab[n]);
    int x = (b - a) * (d - (off + step * n) + 1);
    int num = b > a ? x - 1 : x; // rising segments lose one unit of the numerator (fitted)
    r = a + g_fdiv(num, step);
  }
  return r == 127 ? 128 : r;
}

// fog on the pixel's colour o
uvec4 p_fog(uvec4 o, PPx s) {
  if ((uDisp3d & 0x80u) != 0u && (s.flags & F_FOG) != 0u) {
    uint D = uint(p_fog_density(s.depth >> 9));
    uvec3 fc = g_rgb6(uFogColor & 0x7FFFu);
    uint fa = (uFogColor >> 16) & 31u;
    uint a = (fa * D + o.a * (128u - D)) >> 7;
    uvec3 c = o.rgb;
    if ((uDisp3d & 0x40u) == 0u) c = (fc * D + c * (128u - D)) >> 7;
    o = uvec4(c, a);
  }
  return o;
}

// the pixel after edge marking and fog (the antialiasing's source)
uvec4 p_base(ivec2 q, PPx s, out bool marked) {
  uvec4 o = s.col;
  marked = false;
  if ((uDisp3d & 0x20u) != 0u && p_edge_mark(q, s)) {
    o = uvec4(g_rgb6(uEdgeCol[s.id >> 3]), 31u);
    marked = true;
  }
  return p_fog(o, s);
}

// one sample's 3D layer value
uvec4 p_sample(ivec2 q) {
  PPx s = p_load(q);
  bool marked;
  uvec4 o = p_base(q, s, marked);
  // antialiasing: an opaque polygon's edge pixel gets alpha = its coverage (16 when edge-marked), blended with the
  // farthest 4-neighbour that lies behind it (as a translucent pixel is, skipped when that pixel has alpha 0)
  if ((uDisp3d & 0x10u) != 0u && (s.flags & F_EDGE) != 0u && (s.flags & F_TRANS) == 0u) {
    uint a = min(marked ? 16u : s.cov, 31u);
    uint udep = s.depth;
    ivec2 best = ivec2(-1);
    for (int k = 0; k < 4; k++) {
      ivec2 n = q + P_DIR[k];
      if (!p_inside(n)) continue;
      uint nd = g_samp(n).x;
      if (nd > udep) {
        udep = nd;
        best = n;
      }
    }
    uvec4 under = uvec4(0u);
    if (best.x >= 0) {
      bool m2;
      under = p_base(best, p_load(best), m2);
    }
    if (under.a == 0u) {
      o = uvec4(o.rgb, a);
    } else {
      uvec3 c = ((a + 1u) * o.rgb + (31u - a) * under.rgb) / 32u;
      o = uvec4(c, max(a, under.a));
    }
  }
  return o;
}

#if G_K == 1
void main() {
  oOut = p_sample(ivec2(gl_FragCoord.xy));
#else
// Multisampling: one fragment makes an output pixel from its four samples. Edge marking is one bit a sample (pass E's
// stage 3, uMark), then the fog; a sample whose value needs its neighbours (an opaque edge-flagged one with antialiasing on)
// is slow (p_sample's work, done below in its parts); the rest, most of the picture, need nothing more.
// Two draws (2026-11): this file with P_FAST draws the pixels that have no slow sample and discards the others, marking the
// stencil where it wrote; the program without P_FAST then runs on the unmarked pixels alone. The GPU packs those few into
// full groups of fragments, where one draw ran a group at the pace of its slowest pixel, and the first draw's program
// holds none of the slow code.
// the slow samples' helpers: an opaque edge sample (the antialiasing's); a base packed in a word (r6, g6, b6, a5 a byte each);
// the lowest set bit's index (m != 0); the base sample i blends with (bk as in main: in the pixel, that sample's; outside
// it, u; none, 0); p_sample's blend of base o with it at alpha a
bool p_aa_edge(PPx s) { return (s.flags & F_EDGE) != 0u && (s.flags & F_TRANS) == 0u; }
uint p_pack(uvec4 c) { return c.r | c.g << 8 | c.b << 16 | c.a << 24; }
uvec4 p_unpack(uint c) { return uvec4(c & 255u, (c >> 8) & 255u, (c >> 16) & 255u, c >> 24); }
int p_lsb(uint m) {
  int i = 0;
  while ((m & 1u) == 0u) {
    m >>= 1;
    i++;
  }
  return i;
}
uint p_under(int i, uint bk, uint b0, uint b1, uint b2, uint b3, uint u) {
  uint k = (bk >> (3 * i)) & 7u;
  if (k == 4u) return 0u;
  ivec2 n = ivec2(i & 1, i >> 1) + P_DIR[k];
  if (n.x < 0 || n.y < 0 || n.x > 1 || n.y > 1) return u;
  int j = n.y * 2 + n.x;
  return j == 0 ? b0 : (j == 1 ? b1 : (j == 2 ? b2 : b3));
}
uvec4 p_aa(uvec4 o, uint u, uint cov) {
  uint a = min(cov, 31u);
  uvec4 under = p_unpack(u);
  if (under.a == 0u) return uvec4(o.rgb, a);
  return uvec4(((a + 1u) * o.rgb + (31u - a) * under.rgb) / 32u, max(a, under.a));
}

// the resolve (vcore's resolve_row arithmetic): all alike, that sample, exactly (an interior: pass R shaded the polygon
// once per pixel, so a texture or an alpha-tested outline is as sharp as without antialiasing); otherwise the
// premultiplied average, the alpha averaged (the pixel's coverage). Bits 5-7 of the alpha: the empty part of a pixel
// that is partly empty (samples of alpha 0), in eighths (pass C blends that part with the 2D beneath where the 3D would
// otherwise be drawn opaque; readers of the alpha take bits 0-4)
uvec4 p_resolve(uvec4 o0, uvec4 o1, uvec4 o2, uvec4 o3) {
  if (o1 == o0 && o2 == o0 && o3 == o0) return o0;
  uint sa = o0.a + o1.a + o2.a + o3.a;
  if (sa == 0u) return uvec4(0u);
  uvec3 s = o0.a * o0.rgb + o1.a * o1.rgb + o2.a * o2.rgb + o3.a * o3.rgb;
  uint ne = uint(o0.a == 0u) + uint(o1.a == 0u) + uint(o2.a == 0u) + uint(o3.a == 0u);
  return uvec4((s + sa / 2u) / sa, (sa + 2u) / 4u | (ne * 2u) << 5);
}

// a sample's base (p_base) given its edge mark m
uvec4 p_base_m(PPx s, bool m) { return p_fog(m ? uvec4(g_rgb6(uEdgeCol[s.id >> 3]), 31u) : s.col, s); }

void main() {
  ivec2 q = ivec2(gl_FragCoord.xy) * 2;
  uint mk = (uDisp3d & 0x20u) != 0u ? texelFetch(uMark, ivec2(gl_FragCoord.xy), 0).x : 0u;
  // alike (g00_raster.frag, sample 0's bit 31 of .x): sample 0 is the pixel, but for the edge marks
  if ((texelFetch(uSamp, ivec3(ivec2(gl_FragCoord.xy), 0), 0).x & (1u << 31)) != 0u) {
    PPx s = p_load(q);
    uvec4 c = p_fog(s.col, s);
    if (mk == 0u) {
      oOut = c;
      return;
    }
    uvec4 e = p_base_m(s, true);
    oOut = p_resolve((mk & 1u) != 0u ? e : c, (mk & 2u) != 0u ? e : c, (mk & 4u) != 0u ? e : c, (mk & 8u) != 0u ? e : c);
    return;
  }
  PPx s0 = p_load(q), s1 = p_load(q + ivec2(1, 0)), s2 = p_load(q + ivec2(0, 1)), s3 = p_load(q + ivec2(1, 1));
  uvec4 o0 = p_base_m(s0, (mk & 1u) != 0u), o1 = p_base_m(s1, (mk & 2u) != 0u), o2 = p_base_m(s2, (mk & 4u) != 0u),
        o3 = p_base_m(s3, (mk & 8u) != 0u);
  // p_sample's antialiasing: the samples that blend
  uint aam = 0u;
  if ((uDisp3d & 0x10u) != 0u)
    aam = (p_aa_edge(s0) ? 1u : 0u) | (p_aa_edge(s1) ? 2u : 0u) | (p_aa_edge(s2) ? 4u : 0u) | (p_aa_edge(s3) ? 8u : 0u);
  if (aam == 0u) {
    oOut = p_resolve(o0, o1, o2, o3);
    return;
  }
#ifdef P_FAST
  discard; // the first draw's program: the slow pixels are the second draw's
#endif
  // the 4-neighbour behind each (3 bits a sample: its P_DIR index, 4 none): pass E's stage 0 found it when pass E ran (edge
  // marking on); one outside the pixel gets its base here
  bool em = (uDisp3d & 0x20u) != 0u;
  uint bk = em ? texelFetch(uAaNb, ivec2(gl_FragCoord.xy), 0).x >> 4 : 0x924u, b0 = p_pack(o0), b1 = p_pack(o1),
       b2 = p_pack(o2), b3 = p_pack(o3), u0 = 0u, u1 = 0u, u2 = 0u, u3 = 0u;
  for (uint m = aam; m != 0u; m &= m - 1u) {
    int i = p_lsb(m);
    ivec2 d = ivec2(i & 1, i >> 1);
    int best = int((bk >> (3 * i)) & 7u);
    if (!em) {
      uint udep = g_samp(q + d).x;
      best = 4;
      for (int k = 0; k < 4; k++) {
        ivec2 n = q + d + P_DIR[k];
        if (!p_inside(n)) continue;
        uint nd = g_samp(n).x;
        if (nd > udep) {
          udep = nd;
          best = k;
        }
      }
      bk = (bk & ~(7u << (3 * i))) | uint(best) << (3 * i);
    }
    ivec2 nd = d + (best < 4 ? P_DIR[best] : ivec2(0));
    if (best < 4 && (nd.x < 0 || nd.y < 0 || nd.x > 1 || nd.y > 1)) {
      ivec2 n = q + nd;
      PPx t = p_load(n);
      uint r = p_pack(p_base_m(t, em && p_edge_mark(n, t)));
      if (i == 0) u0 = r;
      else if (i == 1) u1 = r;
      else if (i == 2) u2 = r;
      else u3 = r;
    }
  }
  if ((aam & 1u) != 0u) o0 = p_aa(o0, p_under(0, bk, b0, b1, b2, b3, u0), (mk & 1u) != 0u ? 16u : s0.cov);
  if ((aam & 2u) != 0u) o1 = p_aa(o1, p_under(1, bk, b0, b1, b2, b3, u1), (mk & 2u) != 0u ? 16u : s1.cov);
  if ((aam & 4u) != 0u) o2 = p_aa(o2, p_under(2, bk, b0, b1, b2, b3, u2), (mk & 4u) != 0u ? 16u : s2.cov);
  if ((aam & 8u) != 0u) o3 = p_aa(o3, p_under(3, bk, b0, b1, b2, b3, u3), (mk & 8u) != 0u ? 16u : s3.cov);
  oOut = p_resolve(o0, o1, o2, o3);
#endif
}
