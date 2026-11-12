/* port/vcore piece 19: clear image, edge marking, fog, toon / highlight, antialiasing (docs/vcore.md, piece 19).
 * OWNED BY PIECE 19. Spec: GBATEK 51 (toon, edge, fog, antialiasing), 54 (rear plane), 35 (DISP3DCNT), 50 (toon,
 * highlight). Where GBATEK leaves a detail open the oracle was probed as a black box (noted below). */
#include "vc_shared.h"

/* 5 -> 6 bit colour widening of the 3D engine: X > 0 -> X * 2 + 1 (GBATEK 50) */
static inline u32 w6(u32 c5) { return c5 ? c5 * 2 + 1 : 0; }
static inline u32 rgb555_to_rgb6(u32 c) { return VC_RGB6(w6(c & 31), w6((c >> 5) & 31), w6((c >> 10) & 31)); }
/* the 15-bit depth expanded to 24 bit: X = X * 0x200 + ((X + 1) / 0x8000) * 0x1FF (GBATEK 54) */
static inline u32 depth_expand(u32 x) { return x * 0x200 + ((x + 1) / 0x8000) * 0x1FF; }

/* ---- the rear plane (GBATEK 54) ---- */
void vc_3d_clear(VCoreState *s) {
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  u32 cc = vc_io32(s, VC_REG_CLEAR_COLOR);
  u32 cd = vc_io16(s, VC_REG_CLEAR_DEPTH) & 0x7FFF;
  u32 off = vc_io16(s, VC_REG_CLRIMAGE_OFFSET);
  u32 id = (cc >> 24) & 0x3F;
  u32 color = rgb555_to_rgb6(cc & 0x7FFF) | (((cc >> 16) & 31) << 24);
  u32 depth = depth_expand(cd);
  u8 fog = (cc >> 15) & 1 ? VC_F3_FOG : 0;
  int bitmap = (d3 >> 14) & 1;
  const VC_Region *tex = &s->rgn[VC_RGN_TEX];
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) {
      u32 c = color, d = depth;
      u8 f = fog;
      if (bitmap) {
        /* colour bitmap in texture slot 2, depth/fog bitmap in slot 3; 256x256, scrolled and wrapped */
        u32 bx = (x + (off & 0xFF)) & 0xFF, by = (y + ((off >> 8) & 0xFF)) & 0xFF;
        u32 a = (by * 256 + bx) * 2;
        u32 pc = vc_rd16(tex, 2 * 0x20000 + a), pd = vc_rd16(tex, 3 * 0x20000 + a);
        c = rgb555_to_rgb6(pc & 0x7FFF) | ((pc & 0x8000) ? 31u << 24 : 0);
        d = depth_expand(pd & 0x7FFF);
        f = (pd & 0x8000) ? VC_F3_FOG : 0;
      }
      s->f3d.color[y][x] = c;
      s->f3d.depth[y][x] = d;
      s->f3d.opaque_id[y][x] = (u8)id;
      s->f3d.trans_id[y][x] = 0xFF;
      s->f3d.flags[y][x] = f;
      s->f3d.coverage[y][x] = 0;
      s->f3d.out[y][x] = c;
    }
}

/* ---- toon / highlight (GBATEK 51 TOON_TABLE, 50) ---- */
u32 vc_toon_blend(const VCoreState *s, const VC_Poly *p, u32 vcolor, u32 texel) {
  (void)p;
  int highlight = (vc_io16(s, VC_REG_DISP3DCNT) >> 1) & 1;
  u32 idx = (VC_R6(vcolor) >> 1) & 31; /* the vertex colour's red (6 bit) is the table index */
  u32 tc = rgb555_to_rgb6(vc_io16(s, VC_REG_TOON_TABLE + idx * 2));
  u32 sc[3] = {VC_R6(tc), VC_G6(tc), VC_B6(tc)};
  u32 tx[3] = {VC_R6(texel), VC_G6(texel), VC_B6(texel)};
  u32 o[3];
  for (int i = 0; i < 3; i++) {
    u32 v;
    if (highlight) { /* oracle (GBATEK differs): texel * vertex red (6 bit, all channels) + table colour */
      v = ((tx[i] + 1) * (VC_R6(vcolor) + 1) - 1) / 64 + sc[i];
      if (v > 63) v = 63;
    } else {
      v = ((tx[i] + 1) * (sc[i] + 1) - 1) / 64;
    }
    o[i] = v;
  }
  /* alpha: ((At+1)*(Av+1)-1)/64 on 6-bit alphas (5 bit widened X*2+1), back to 5 bit */
  u32 at = w6(VC_A5(texel)), av = w6(VC_A5(vcolor));
  u32 a = (((at + 1) * (av + 1) - 1) / 64) >> 1;
  return VC_RGB6(o[0], o[1], o[2]) | (a << 24);
}

/* ---- fog density (GBATEK 51: FOG_OFFSET, FOG_TABLE; the interpolation's rounding was fitted to the oracle) ---- */
static u32 fog_density(const VCoreState *s, u32 d15) {
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  s32 step = 0x400 >> ((d3 >> 8) & 15);
  s32 off = vc_io16(s, VC_REG_FOG_OFFSET) & 0x7FFF;
  s32 t[32];
  for (int i = 0; i < 32; i++) t[i] = vc_io8(s, VC_REG_FOG_TABLE + i) & 0x7F;
  s32 d = (s32)d15, r;
  if (d < off + step) r = t[0];
  else if (d >= off + step * 32) r = t[31];
  else {
    int n = 1;
    while (d >= off + step * (n + 1)) n++;
    s32 a = t[n - 1], b = t[n];
    s32 x = (b - a) * (d - (off + step * n) + 1);
    /* floor division; rising segments lose one unit of the numerator (fitted) */
    s32 num = b > a ? x - 1 : x;
    s32 q = num / step;
    if (num % step < 0) q--;
    r = a + q;
  }
  return r == 127 ? 128 : (u32)r;
}

/* the density 0-128 at a 15-bit depth (p21's N x post builds a table of it) */
u32 vc_p19_fog_density(const VCoreState *s, u32 d15) { return fog_density(s, d15); }

/* ---- edge marking, fog, antialiasing (GBATEK 51) ---- */
void vc_3d_post(VCoreState *s) {
  VC_Frame3D *f = &s->f3d;
  u32 d3 = vc_io16(s, VC_REG_DISP3DCNT);
  int aa = (d3 >> 4) & 1, edge = (d3 >> 5) & 1, fog = (d3 >> 7) & 1, alpha_only = (d3 >> 6) & 1;
  u32 cc = vc_io32(s, VC_REG_CLEAR_COLOR);
  u32 clear_id = (cc >> 24) & 0x3F;
  static u8 marked[VC_H][VC_W];
  static const int dx[4] = {0, 0, -1, 1}, dy[4] = {-1, 1, 0, 0};
  memset(marked, 0, sizeof marked);
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) f->out[y][x] = f->color[y][x];

  /* edge marking: a flagged edge pixel whose up/down/left/right neighbour has another polygon ID and a greater
   * depth takes EDGE_COLOR[id >> 3]; outside the screen the neighbour is the rear plane's ID */
  if (edge) {
    for (int y = 0; y < VC_H; y++)
      for (int x = 0; x < VC_W; x++) {
        if (!(f->flags[y][x] & VC_F3_EDGE)) continue;
        u32 id = f->opaque_id[y][x], dep = f->depth[y][x];
        int hit = 0;
        for (int k = 0; k < 4 && !hit; k++) {
          int nx = x + dx[k], ny = y + dy[k];
          if (nx < 0 || ny < 0 || nx >= VC_W || ny >= VC_H) {
            hit = clear_id != id; /* depth of the rear plane taken as the farthest */
          } else {
            hit = f->opaque_id[ny][nx] != id && dep < f->depth[ny][nx];
          }
        }
        if (!hit) continue;
        u32 ec = rgb555_to_rgb6(vc_io16(s, VC_REG_EDGE_COLOR + (id >> 3) * 2));
        f->out[y][x] = (ec & 0xFFFFFF) | (31u << 24);
        marked[y][x] = 1;
      }
  }

  /* fog: pixels with the fog flag mix with FOG_COLOR by the density at their depth (15 bits of the 24) */
  if (fog) {
    u32 fc = vc_io32(s, VC_REG_FOG_COLOR);
    u32 fcol = rgb555_to_rgb6(fc & 0x7FFF), fa = (fc >> 16) & 31;
    for (int y = 0; y < VC_H; y++)
      for (int x = 0; x < VC_W; x++) {
        if (!(f->flags[y][x] & VC_F3_FOG)) continue;
        u32 D = fog_density(s, f->depth[y][x] >> 9), p = f->out[y][x];
        u32 a = (fa * D + VC_A5(p) * (128 - D)) >> 7;
        u32 r = VC_R6(p), g = VC_G6(p), b = VC_B6(p);
        if (!alpha_only) {
          r = (VC_R6(fcol) * D + r * (128 - D)) >> 7;
          g = (VC_G6(fcol) * D + g * (128 - D)) >> 7;
          b = (VC_B6(fcol) * D + b * (128 - D)) >> 7;
        }
        f->out[y][x] = VC_RGB6(r, g, b) | (a << 24);
      }
  }

  /* antialiasing: an opaque polygon's edge pixel gets alpha = its coverage (16 when edge-marked) and is blended
   * with what lies behind it as a translucent pixel is (skipped when the pixel behind is alpha 0). What lies
   * behind is not stored by the rendering engine: taken as the farthest 4-neighbour that is behind this pixel. */
  if (aa) {
    static u32 src[VC_H][VC_W];
    memcpy(src, f->out, sizeof src);
    for (int y = 0; y < VC_H; y++)
      for (int x = 0; x < VC_W; x++) {
        if (!(f->flags[y][x] & VC_F3_EDGE) || (f->flags[y][x] & VC_F3_TRANS)) continue;
        u32 a = marked[y][x] ? 16 : f->coverage[y][x];
        if (a > 31) a = 31;
        u32 p = src[y][x], under = 0, udep = f->depth[y][x];
        for (int k = 0; k < 4; k++) {
          int nx = x + dx[k], ny = y + dy[k];
          if (nx < 0 || ny < 0 || nx >= VC_W || ny >= VC_H) continue;
          if (f->depth[ny][nx] > udep) {
            udep = f->depth[ny][nx];
            under = src[ny][nx];
          }
        }
        u32 ua = VC_A5(under);
        if (ua == 0) {
          f->out[y][x] = (p & 0xFFFFFF) | (a << 24);
        } else {
          u32 r = ((a + 1) * VC_R6(p) + (31 - a) * VC_R6(under)) / 32;
          u32 g = ((a + 1) * VC_G6(p) + (31 - a) * VC_G6(under)) / 32;
          u32 b = ((a + 1) * VC_B6(p) + (31 - a) * VC_B6(under)) / 32;
          f->out[y][x] = VC_RGB6(r, g, b) | ((a > ua ? a : ua) << 24);
        }
      }
  }
}
