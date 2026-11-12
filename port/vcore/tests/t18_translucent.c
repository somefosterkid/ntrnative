/* port/vcore piece 18's unit tests: translucency, sorting, polygon IDs, shadows (docs/vcore.md, piece 18). OWNED BY PIECE 18.
 * Run: build/vcore/vcore_tests p18
 * The depth test is piece 16's (a stub until merged), so these tests call vc_p18_plot with the verdict of GBATEK 43's
 * rule (less, or equal within +-0x200) worked out by dt() below. Expected values are worked by hand from GBATEK 51. */
#include "vct.h"

static VC_Poly mkpoly(u32 alpha, u32 id, u32 mode, u32 extra) {
  VC_Poly p;
  memset(&p, 0, sizeof p);
  p.attr = (alpha << 16) | (id << 24) | (mode << 4) | extra | 0xC0;
  return p;
}

static int dt(VCoreState *s, const VC_Poly *p, int x, int y, u32 d) {
  u32 old = s->f3d.depth[y][x];
  if (p->attr & 0x4000) return d + 0x200 >= old && d <= old + 0x200;
  return d < old;
}

static void put(VCoreState *s, const VC_Poly *p, int x, int y, u32 d, u32 c) {
  vc_p18_plot(s, p, x, y, d, c, 0, dt(s, p, x, y, d));
}

static VCoreState *fb(void) {
  VCoreState *s = vct_state();
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) {
      s->f3d.depth[y][x] = 0xFFFFFF;
      s->f3d.trans_id[y][x] = 0xFF;
    }
  vct_io16(s, VC_REG_DISP3DCNT, 8); /* alpha blending on */
  return s;
}

#define PX(r, g, b, a) (VC_RGB6(r, g, b) | ((u32)(a) << 24))

VCT_TEST(p18_translucent_flags) {
  VCoreState *s = vct_state();
  VC_Poly p = mkpoly(31, 0, 0, 0);
  VCT_EQ(vc_p18_is_translucent(s, &p), 0);
  p = mkpoly(0, 0, 0, 0);
  VCT_EQ(vc_p18_is_translucent(s, &p), 0); /* wire-frame */
  p = mkpoly(1, 0, 0, 0);
  VCT_EQ(vc_p18_is_translucent(s, &p), 1);
  p = mkpoly(30, 0, 0, 0);
  VCT_EQ(vc_p18_is_translucent(s, &p), 1);
  p = mkpoly(31, 0, 0, 0);
  p.texparam = 1u << 26; /* A3I5 */
  VCT_EQ(vc_p18_is_translucent(s, &p), 0); /* textures disabled */
  vct_io16(s, VC_REG_DISP3DCNT, 1);
  VCT_EQ(vc_p18_is_translucent(s, &p), 1);
  p.texparam = 6u << 26; /* A5I3 */
  VCT_EQ(vc_p18_is_translucent(s, &p), 1);
  p.texparam = 2u << 26; /* 4 colours */
  VCT_EQ(vc_p18_is_translucent(s, &p), 0);
  p.texparam = 7u << 26; /* direct */
  VCT_EQ(vc_p18_is_translucent(s, &p), 0);
}

static void setpoly(VCoreState *s, int i, u32 alpha, int ymin) {
  s->geo.poly[i] = mkpoly(alpha, 0, 0, 0);
  s->geo.poly[i].ymin = ymin;
  s->geo.poly[i].ymax = ymin + 5;
  s->geo.poly[i].index = (u16)i;
}

VCT_TEST(p18_sort_auto) {
  VCoreState *s = vct_state();
  /* sent: 0 T y30, 1 O y50, 2 T y10, 3 O y20, 4 T y30 (equal Y with 0), 5 T y10 */
  setpoly(s, 0, 15, 30); setpoly(s, 1, 31, 50); setpoly(s, 2, 15, 10);
  setpoly(s, 3, 31, 20); setpoly(s, 4, 15, 30); setpoly(s, 5, 15, 10);
  s->geo.npoly = 6;
  u32 prm = 0;
  vc_cmd_swap(s, &prm, 1);
  VCT_EQ(s->geo.swapped, 1);
  /* opaque by Y: 3, 1; translucent by Y, equal Y keeps the order sent: 2, 5, 0, 4 */
  static const u16 want[6] = {3, 1, 2, 5, 0, 4};
  for (int i = 0; i < 6; i++) VCT_EQ(s->geo.order[i], want[i]);
  VCT_EQ(s->geo.poly[0].translucent, 1);
  VCT_EQ(s->geo.poly[1].translucent, 0);
}

VCT_TEST(p18_sort_key_ymax_then_ymin) {
  /* MEASURED against the oracle: the key is the lowest vertex's Y (ymax), ties by ymin, then the order sent; opaque
   * polygons are sorted by it too; with the manual bit the translucent ones keep the order sent */
  VCoreState *s = vct_state();
  setpoly(s, 0, 15, 10); s->geo.poly[0].ymax = 100;  /* A: top 10, bottom 100 */
  setpoly(s, 1, 15, 30); s->geo.poly[1].ymax = 60;   /* B: top 30, bottom 60 */
  setpoly(s, 2, 15, 20); s->geo.poly[2].ymax = 60;   /* C: bottom 60 like B, top 20 */
  setpoly(s, 3, 31, 5);  s->geo.poly[3].ymax = 90;   /* opaque D */
  setpoly(s, 4, 31, 50); s->geo.poly[4].ymax = 70;   /* opaque E */
  s->geo.npoly = 5;
  u32 prm = 0;
  vc_cmd_swap(s, &prm, 1);
  static const u16 want[5] = {4, 3, 2, 1, 0};
  for (int i = 0; i < 5; i++) VCT_EQ(s->geo.order[i], want[i]);
}

VCT_TEST(p18_sort_manual) {
  VCoreState *s = vct_state();
  setpoly(s, 0, 15, 30); setpoly(s, 1, 31, 50); setpoly(s, 2, 15, 10);
  setpoly(s, 3, 31, 20); setpoly(s, 4, 15, 5);
  s->geo.npoly = 5;
  u32 prm = 1;
  vc_cmd_swap(s, &prm, 1);
  VCT_EQ(s->geo.swap_param, 1);
  /* translucent as sent after the opaque ones */
  static const u16 want[5] = {3, 1, 0, 2, 4};
  for (int i = 0; i < 5; i++) VCT_EQ(s->geo.order[i], want[i]);
  prm = 3; /* w-buffering bit kept */
  vc_cmd_swap(s, &prm, 1);
  VCT_EQ(s->geo.swap_param, 3);
}

VCT_TEST(p18_opaque_write) {
  VCoreState *s = fb();
  VC_Poly p = mkpoly(31, 5, 0, 0x8000);
  put(s, &p, 3, 4, 0x1000, PX(10, 20, 30, 31));
  VCT_EQ(s->f3d.color[4][3], PX(10, 20, 30, 31));
  VCT_EQ(s->f3d.depth[4][3], 0x1000);
  VCT_EQ(s->f3d.opaque_id[4][3], 5);
  VCT_CHECK(s->f3d.flags[4][3] & VC_F3_FOG);
  VCT_CHECK(s->f3d.flags[4][3] & VC_F3_WRITTEN);
  /* farther opaque pixel: depth test fails, nothing changes */
  VC_Poly q = mkpoly(31, 6, 0, 0);
  put(s, &q, 3, 4, 0x2000, PX(1, 1, 1, 31));
  VCT_EQ(s->f3d.color[4][3], PX(10, 20, 30, 31));
  VCT_EQ(s->f3d.opaque_id[4][3], 5);
  /* closer opaque one replaces it, and the fog flag with it */
  put(s, &q, 3, 4, 0x0800, PX(1, 2, 3, 31));
  VCT_EQ(s->f3d.color[4][3], PX(1, 2, 3, 31));
  VCT_EQ(s->f3d.opaque_id[4][3], 6);
  VCT_CHECK(!(s->f3d.flags[4][3] & VC_F3_FOG));
}

VCT_TEST(p18_translucent_over_opaque) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0);
  put(s, &o, 0, 0, 0x4000, PX(40, 0, 20, 31));
  /* alpha 15: (0*16 + 40*16)/32 = 20; (62*16 + 0)/32 = 31; (0*16 + 20*16)/32 = 10; alpha max(15, 31) = 31 */
  VC_Poly t = mkpoly(15, 2, 0, 0);
  put(s, &t, 0, 0, 0x2000, PX(0, 62, 0, 15));
  VCT_EQ(s->f3d.color[0][0], PX(20, 31, 10, 31));
  VCT_EQ(s->f3d.opaque_id[0][0], 1);
  VCT_EQ(s->f3d.trans_id[0][0], 2);
  VCT_CHECK(s->f3d.flags[0][0] & VC_F3_TRANS);
  /* depth not updated (attr bit 11 clear) */
  VCT_EQ(s->f3d.depth[0][0], 0x4000);
  /* behind the opaque pixel: no blend */
  put(s, &t, 1, 0, 0x2000, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.color[1][0], 0); /* untouched clear */
  VC_Poly t2 = mkpoly(15, 3, 0, 0);
  put(s, &o, 2, 0, 0x1000, PX(40, 0, 20, 31));
  put(s, &t2, 2, 0, 0x3000, PX(0, 62, 0, 15));
  VCT_EQ(s->f3d.color[0][2], PX(40, 0, 20, 31));
}

VCT_TEST(p18_blend_alpha_values) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0);
  /* alpha 1: (63*2 + 0*30)/32 = 3 ; alpha 30: (63*31 + 0*1)/32 = 61 */
  put(s, &o, 0, 0, 0x4000, PX(0, 0, 0, 31));
  put(s, &o, 1, 0, 0x4000, PX(0, 0, 0, 31));
  VC_Poly t1 = mkpoly(1, 2, 0, 0), t30 = mkpoly(30, 2, 0, 0);
  put(s, &t1, 0, 0, 0x100, PX(63, 63, 63, 1));
  put(s, &t30, 1, 0, 0x100, PX(63, 63, 63, 30));
  VCT_EQ(s->f3d.color[0][0], PX(3, 3, 3, 31));
  VCT_EQ(s->f3d.color[0][1], PX(61, 61, 61, 31));
}

VCT_TEST(p18_blend_disabled_and_clear_alpha0) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0), t = mkpoly(15, 2, 0, 0);
  vct_io16(s, VC_REG_DISP3DCNT, 0); /* blending off: overwritten with the polygon's colour and alpha */
  put(s, &o, 0, 0, 0x4000, PX(40, 0, 20, 31));
  put(s, &t, 0, 0, 0x2000, PX(0, 62, 0, 15));
  VCT_EQ(s->f3d.color[0][0], PX(0, 62, 0, 15));
  /* blending on but the old pixel has alpha 0 (the rear plane): overwritten */
  vct_io16(s, VC_REG_DISP3DCNT, 8);
  s->f3d.color[1][0] = PX(9, 9, 9, 0);
  put(s, &t, 0, 1, 0x2000, PX(0, 62, 0, 15));
  VCT_EQ(s->f3d.color[1][0], PX(0, 62, 0, 15));
}

VCT_TEST(p18_same_id_translucent) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0);
  put(s, &o, 0, 0, 0x4000, PX(40, 0, 0, 31));
  VC_Poly a = mkpoly(15, 2, 0, 0), b = mkpoly(15, 2, 0, 0), c = mkpoly(15, 3, 0, 0);
  put(s, &a, 0, 0, 0x3000, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.color[0][0], PX(20, 0, 0, 31));
  put(s, &b, 0, 0, 0x2000, PX(0, 0, 0, 15)); /* same ID: not drawn again */
  VCT_EQ(s->f3d.color[0][0], PX(20, 0, 0, 31));
  put(s, &c, 0, 0, 0x2000, PX(0, 0, 0, 15)); /* other ID: blended again: 20*16/32 = 10 */
  VCT_EQ(s->f3d.color[0][0], PX(10, 0, 0, 31));
  VCT_EQ(s->f3d.trans_id[0][0], 3);
}

VCT_TEST(p18_depth_update_flag) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0);
  put(s, &o, 0, 0, 0x4000, PX(40, 0, 0, 31));
  put(s, &o, 1, 0, 0x4000, PX(40, 0, 0, 31));
  VC_Poly keep = mkpoly(15, 2, 0, 0), set = mkpoly(15, 2, 0, 0x800);
  put(s, &keep, 0, 0, 0x3000, PX(0, 0, 0, 15));
  put(s, &set, 1, 0, 0x3000, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.depth[0][0], 0x4000);
  VCT_EQ(s->f3d.depth[0][1], 0x3000);
  /* a later polygon at 0x3800 passes the first pixel, fails the second */
  VC_Poly n = mkpoly(15, 9, 0, 0);
  put(s, &n, 0, 0, 0x3800, PX(0, 0, 0, 15));
  put(s, &n, 1, 0, 0x3800, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.trans_id[0][0], 9);
  VCT_EQ(s->f3d.trans_id[0][1], 2);
}

VCT_TEST(p18_alpha_zero_and_alpha_test) {
  VCoreState *s = fb();
  VC_Poly p = mkpoly(31, 1, 0, 0);
  /* a pixel of alpha 0 (a transparent texel) is not rendered: no colour, no depth */
  put(s, &p, 0, 0, 0x100, PX(5, 5, 5, 0));
  VCT_EQ(s->f3d.color[0][0], 0);
  VCT_EQ(s->f3d.depth[0][0], 0xFFFFFF);
  VCT_EQ(s->f3d.flags[0][0], 0);
  /* alpha test on, ref 10: drawn only if alpha > 10 */
  vct_io16(s, VC_REG_DISP3DCNT, 8 | 4);
  vct_io32(s, VC_REG_ALPHA_TEST_REF, 10);
  VC_Poly t = mkpoly(10, 1, 0, 0);
  put(s, &t, 1, 0, 0x100, PX(5, 5, 5, 10));
  VCT_EQ(s->f3d.depth[0][1], 0xFFFFFF);
  VCT_EQ(s->f3d.color[0][1], 0);
  VC_Poly t2 = mkpoly(11, 1, 0, 0);
  put(s, &t2, 2, 0, 0x100, PX(5, 5, 5, 11));
  VCT_EQ(s->f3d.color[0][2], PX(5, 5, 5, 11)); /* clear alpha 0: overwritten, not blended */
  /* alpha test off: alpha 1 passes */
  vct_io16(s, VC_REG_DISP3DCNT, 8);
  VC_Poly t3 = mkpoly(1, 1, 0, 0);
  put(s, &t3, 3, 0, 0x100, PX(5, 5, 5, 1));
  VCT_EQ(s->f3d.color[0][3], PX(5, 5, 5, 1));
}

VCT_TEST(p18_wireframe_alpha) {
  VCoreState *s = fb();
  VC_Poly w = mkpoly(0, 4, 0, 0); /* wire-frame: lines rendered with a fixed alpha of 31 (opaque) */
  put(s, &w, 0, 0, 0x100, PX(5, 6, 7, 0));
  VCT_EQ(s->f3d.color[0][0], PX(5, 6, 7, 31));
  VCT_EQ(s->f3d.opaque_id[0][0], 4);
}

VCT_TEST(p18_depth_equal) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0);
  put(s, &o, 0, 0, 0x4000, PX(40, 0, 0, 31));
  VC_Poly e = mkpoly(31, 2, 0, 0x4000);
  put(s, &e, 0, 0, 0x4100, PX(1, 1, 1, 31)); /* within +-0x200: passes */
  VCT_EQ(s->f3d.opaque_id[0][0], 2);
  put(s, &e, 0, 0, 0x4400, PX(2, 2, 2, 31)); /* 0x300 away from 0x4100: fails */
  VCT_EQ(s->f3d.opaque_id[0][0], 2);
  VCT_EQ(s->f3d.color[0][0], PX(1, 1, 1, 31));
}

VCT_TEST(p18_fog_flag_and) {
  VCoreState *s = fb();
  VC_Poly o = mkpoly(31, 1, 0, 0x8000), t = mkpoly(15, 2, 0, 0), t2 = mkpoly(15, 3, 0, 0x8000);
  put(s, &o, 0, 0, 0x4000, PX(40, 0, 0, 31));
  put(s, &o, 1, 0, 0x4000, PX(40, 0, 0, 31));
  put(s, &t, 0, 0, 0x3000, PX(0, 0, 0, 15));  /* fog flag 1 AND 0 = 0 */
  put(s, &t2, 1, 0, 0x3000, PX(0, 0, 0, 15)); /* 1 AND 1 = 1 */
  VCT_CHECK(!(s->f3d.flags[0][0] & VC_F3_FOG));
  VCT_CHECK(s->f3d.flags[0][1] & VC_F3_FOG);
}

VCT_TEST(p18_shadow_mask_and_shadow) {
  VCoreState *s = fb();
  VC_Poly floor = mkpoly(31, 7, 0, 0);
  for (int x = 0; x < 4; x++) put(s, &floor, x, 0, 0x4000, PX(40, 40, 40, 31));
  /* mask volume (ID 0): back faces at x=0..3; at x=0 the back face is in front of the floor (passes: no stencil),
   * at x=1,2 behind the floor (fails: stencil set), x=3 in front */
  VC_Poly mask = mkpoly(15, 0, 3, 0);
  put(s, &mask, 0, 0, 0x3000, PX(0, 0, 0, 15));
  put(s, &mask, 1, 0, 0x5000, PX(0, 0, 0, 15));
  put(s, &mask, 2, 0, 0x5000, PX(0, 0, 0, 15));
  put(s, &mask, 3, 0, 0x3000, PX(0, 0, 0, 15));
  VCT_CHECK(!(s->f3d.flags[0][0] & VC_F3_STENCIL));
  VCT_CHECK(s->f3d.flags[0][1] & VC_F3_STENCIL);
  VCT_EQ(s->f3d.color[0][1], PX(40, 40, 40, 31)); /* the mask draws nothing */
  /* shadow polygon (ID 9, alpha 15, black): front faces in front of the floor at all four pixels. MEASURED against
   * the oracle (GBATEK 46 says the opposite): it is drawn only where the stencil is set (and the stencil is reset),
   * blended: 40*16/32 = 20 */
  VC_Poly sh = mkpoly(15, 9, 3, 0);
  for (int x = 0; x < 4; x++) put(s, &sh, x, 0, 0x3000, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.color[0][0], PX(40, 40, 40, 31));
  VCT_EQ(s->f3d.color[0][1], PX(20, 20, 20, 31));
  VCT_EQ(s->f3d.color[0][2], PX(20, 20, 20, 31));
  VCT_EQ(s->f3d.color[0][3], PX(40, 40, 40, 31));
  VCT_CHECK(!(s->f3d.flags[0][1] & VC_F3_STENCIL));
  VCT_EQ(s->f3d.trans_id[0][1], 9);
}

VCT_TEST(p18_shadow_same_id_not_cast) {
  VCoreState *s = fb();
  VC_Poly obj = mkpoly(31, 9, 0, 0), sh = mkpoly(15, 9, 3, 0), sh2 = mkpoly(15, 10, 3, 0);
  put(s, &obj, 0, 0, 0x4000, PX(40, 40, 40, 31));
  put(s, &obj, 1, 0, 0x4000, PX(40, 40, 40, 31));
  s->f3d.flags[0][0] |= VC_F3_STENCIL; /* the mask volume set the stencil at both pixels */
  s->f3d.flags[0][1] |= VC_F3_STENCIL;
  put(s, &sh, 0, 0, 0x3000, PX(0, 0, 0, 15)); /* same ID as the pixel: not shaded */
  VCT_EQ(s->f3d.color[0][0], PX(40, 40, 40, 31));
  put(s, &sh2, 1, 0, 0x3000, PX(0, 0, 0, 15)); /* different ID: shaded */
  VCT_EQ(s->f3d.color[0][1], PX(20, 20, 20, 31));
  /* a second shadow with the same ID 10: the pixel's ID is now 10, so not shaded twice */
  s->f3d.flags[0][1] |= VC_F3_STENCIL;
  put(s, &sh2, 1, 0, 0x2000, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.color[0][1], PX(20, 20, 20, 31));
  /* a shadow of another ID is shaded again: 20*16/32 = 10 */
  VC_Poly sh3 = mkpoly(15, 11, 3, 0);
  s->f3d.flags[0][1] |= VC_F3_STENCIL;
  put(s, &sh3, 1, 0, 0x2000, PX(0, 0, 0, 15));
  VCT_EQ(s->f3d.color[0][1], PX(10, 10, 10, 31));
}

VCT_TEST(p18_plot_clips) {
  VCoreState *s = fb();
  VC_Poly p = mkpoly(31, 1, 0, 0);
  vc_p18_plot(s, &p, -1, 0, 0, PX(1, 1, 1, 31), 0, 1);
  vc_p18_plot(s, &p, 0, VC_H, 0, PX(1, 1, 1, 31), 0, 1);
  vc_3d_plot(s, &p, 256, 0, 0, PX(1, 1, 1, 31), 0);
  VCT_CHECK(1);
}

/* synthetic scenes for the oracle: triangles through the whole core (need pieces 13, 14, 16) */
static void tri(VCoreState *s, int x0, int y0, int x1, int y1, int x2, int y2, int z, u32 attr, u32 color) {
  u32 a = attr;
  vct_gx(s, VC_OP_POLYGON_ATTR, 1, &a);
  vct_gx(s, VC_OP_COLOR, 1, &color);
  u32 t = 0;
  vct_gx(s, VC_OP_BEGIN_VTXS, 1, &t);
  int xs[3] = {x0, x1, x2}, ys[3] = {y0, y1, y2};
  for (int i = 0; i < 3; i++) {
    u32 v[2] = {(u32)(xs[i] & 0xFFFF) | ((u32)(ys[i] & 0xFFFF) << 16), (u32)z & 0xFFFF};
    vct_gx(s, VC_OP_VTX_16, 2, v);
  }
  vct_gx(s, VC_OP_END_VTXS, 0, 0);
}

static VCoreState *scene(u32 swapparam) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_DISP3DCNT, 8);
  vct_eng32(s, VC_ENG_A, VC_REG_DISPCNT, 0x00010108); /* graphics display mode 1, BG0 on, 3D */
  (void)swapparam;
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  return s;
}

VCT_TEST(p18_emit_scenes) {
  const u32 C0 = 0x001F, C1 = 0x03E0, C2 = 0x7C00;
  u32 sw = 0;
  VCoreState *s = scene(0);
  /* opaque red under translucent green and blue (different IDs), a same-ID pair overlapping */
  tri(s, -0x400, 0x300, 0x400, 0x300, 0, -0x300, -0x800, 0x1F0000 | 0xC0, C0);
  tri(s, -0x300, 0x200, 0x300, 0x200, 0, -0x400, -0x1000, (15u << 16) | (2u << 24) | 0xC0, C1);
  tri(s, -0x200, 0x100, 0x200, 0x100, 0, -0x200, -0x1000, (15u << 16) | (3u << 24) | 0xC0, C2);
  vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &sw);
  vct_emit(s, "p18_translucent");
}
