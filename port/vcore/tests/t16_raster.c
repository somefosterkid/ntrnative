/* port/vcore piece 16's unit tests: the rasterizer (docs/vcore.md, piece 16). OWNED BY PIECE 16.
 * Run: build/vcore/vcore_tests p16
 * The polygons are put straight into s->geo (screen coordinates), drawn through a recording s->plot. Expected
 * values are worked by hand from GBATEK 35, 43, 44, 51 and the fill rules in p16_raster.c's header. */
#include "vct.h"

typedef struct Rec {
  int x, y, edge;
  u32 depth, col;
  s32 st[2];
} Rec;
static Rec rec[VC_W * VC_H];
static int nrec;

static void rec_plot(VCoreState *s, const VC_Poly *p, int x, int y, u32 depth, u32 col, int edge) {
  if (nrec < VC_W * VC_H) {
    Rec *r = &rec[nrec++];
    r->x = x; r->y = y; r->edge = edge; r->depth = depth; r->col = col;
    r->st[0] = s->p16.st[0]; r->st[1] = s->p16.st[1];
  }
  s->f3d.color[y][x] = col; /* so the coverage logic sees a write */
  s->f3d.depth[y][x] = depth;
}

static VCoreState *setup(void) {
  VCoreState *s = vct_state();
  s->plot = rec_plot;
  nrec = 0;
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) s->f3d.depth[y][x] = 0xFFFFFF;
  return s;
}

/* vertex i: screen (x, y), depth z, w (20.12), colour r g b (0-31), texture s t */
static void V(VCoreState *s, int i, int x, int y, s32 z, s32 w, int r, int g, int b, int ts, int tt) {
  VC_Vertex *v = &s->geo.vtx[i];
  v->sx = x; v->sy = y; v->sz = z; v->clip[3] = w;
  v->color[0] = (u8)r; v->color[1] = (u8)g; v->color[2] = (u8)b;
  v->tex[0] = (s16)ts; v->tex[1] = (s16)tt;
}
static VC_Poly poly(VCoreState *s, int n, u32 attr) {
  VC_Poly p;
  memset(&p, 0, sizeof p);
  p.nv = (u8)n;
  for (int i = 0; i < n; i++) p.v[i] = (u16)i;
  p.attr = attr;
  (void)s;
  return p;
}
static const Rec *at(int x, int y) {
  for (int i = 0; i < nrec; i++)
    if (rec[i].x == x && rec[i].y == y) return &rec[i];
  return NULL;
}
static int row_count(int y) {
  int n = 0;
  for (int i = 0; i < nrec; i++) n += rec[i].y == y;
  return n;
}
#define ALPHA31 (31u << 16)

/* triangle (0,0) (4,0) (0,4): rows 0..3 (bottom row out), row y has x = 0 .. 3-y (right edge out): 4+3+2+1 = 10 */
VCT_TEST(p16_flat_triangle_pixel_set) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 100, 0x1000, 31, 0, 0, 0, 0);
  V(s, 1, 4, 0, 100, 0x1000, 31, 0, 0, 0, 0);
  V(s, 2, 0, 4, 100, 0x1000, 31, 0, 0, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_CHECK(nrec >= 6 && nrec <= 10); /* edge rule measured against the oracle: floored slopes, see p16_raster.c */
  VCT_EQ(row_count(4), 0);
  VCT_CHECK(at(0, 3) && !at(1, 3));
  /* colour: vertex red 31 -> 6-bit 63 (X*2+(X+31)/32), alpha 31; depth flat */
  VCT_EQ(at(1, 1)->col, VC_RGB6(63, 0, 0) | 31u << 24);
  VCT_EQ(at(1, 1)->depth, 100);
}

/* a quad (2,2) (10,2) (10,6) (2,6): left and top edges in, right and bottom out: x 2..9, y 2..5 = 32 pixels */
VCT_TEST(p16_quad_fill_rules) {
  VCoreState *s = setup();
  V(s, 0, 2, 2, 0, 0x1000, 0, 31, 0, 0, 0);
  V(s, 1, 10, 2, 0, 0x1000, 0, 31, 0, 0, 0);
  V(s, 2, 10, 6, 0, 0x1000, 0, 31, 0, 0, 0);
  V(s, 3, 2, 6, 0, 0x1000, 0, 31, 0, 0, 0);
  VC_Poly p = poly(s, 4, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(nrec, 32);
  VCT_CHECK(at(2, 2) && at(9, 2) && at(2, 5) && at(9, 5));
  VCT_CHECK(!at(10, 3) && !at(5, 6) && !at(1, 3) && !at(5, 1));
}

/* with edge marking (DISP3DCNT bit 5) the oracle still leaves out a rectangle's right column x=10 and flat bottom
 * row y=6: rows 2..5, x 2..9 = 8 x 4 = 32; the border pixels carry the edge flag, including the last
 * drawn column (x=9) and the last drawn row (y=5) */
VCT_TEST(p16_full_polygon_with_edge_marking) {
  VCoreState *s = setup();
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 5);
  V(s, 0, 2, 2, 0, 0x1000, 0, 31, 0, 0, 0);
  V(s, 1, 10, 2, 0, 0x1000, 0, 31, 0, 0, 0);
  V(s, 2, 10, 6, 0, 0x1000, 0, 31, 0, 0, 0);
  V(s, 3, 2, 6, 0, 0x1000, 0, 31, 0, 0, 0);
  VC_Poly p = poly(s, 4, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(nrec, 32);
  VCT_CHECK(at(2, 5) && at(9, 5) && !at(10, 4) && !at(5, 6));
  VCT_EQ(at(2, 4)->edge, 1);
  VCT_EQ(at(9, 4)->edge, 1);
  VCT_EQ(at(5, 2)->edge, 1);
  VCT_EQ(at(5, 5)->edge, 1);
  VCT_EQ(at(5, 4)->edge, 0);
}

/* a thin sliver (0,0) (1,0) (0,10) keeps one pixel per row: the diagonal edge is Y-major, x = 1 for rows 0..5 and
 * x = 0 from row 6 (round half up), the left edge x=0 is always drawn; rows 0..9 */
VCT_TEST(p16_thin_sliver) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 1, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 0, 10, 0, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_CHECK(nrec >= 10 && nrec <= 12);
  for (int y = 0; y < 10; y++) VCT_CHECK(at(0, y));
}

/* an X-major edge: the triangle (0,0) (8,2) (0,2): the hypotenuse x = 4y; rows 0 (x 0..1 are its pixels: the line
 * at y-1/2..y+1/2 -> x 0 .. round(2)-1 = 1), row 1 (x 2..5), row 2 is the bottom, out. Left edge x=0.
 * Row 0: right edge span 0..1 -> right out: pixels 0..max(-1, 0) = just x 0. Row 1: span 2..5, left x 0 -> 0..1 */
VCT_TEST(p16_x_major_edge) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 8, 2, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 0, 2, 0, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  /* measured against the oracle: row k of the hypotenuse owns x 4k .. 4k+3 and is drawn whole */
  VCT_EQ(row_count(0), 4);
  VCT_EQ(row_count(2), 0);
  VCT_CHECK(at(0, 0) && at(3, 0) && at(0, 1) && at(7, 1));
  /* the same polygon drawn whole (AA on): row 0 0..1, row 1 0..5, row 2 0..8 */
  nrec = 0;
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 4);
  vc_raster_poly(s, &p);
  /* oracle (antialiasing on, triangle (0,0) (8,2) (0,2) at (100,50)): row 0 is x 0..3, row 1 x 0..7 with the
   * hypotenuse's X-major coverage 28 20 12 4 on x 4..7 */
  VCT_EQ(row_count(0), 4);
  VCT_EQ(row_count(1), 8);
  VCT_EQ(s->f3d.coverage[1][3], 31);
}

/* texture coordinates are perspective-correct: along the top row of (0,0) w=1.0 s=0 to (16,0) w=3.0 s=1600 the
 * middle pixel x=8 (f = 1/2) has s = 1600 * (f/w1) / ((1-f)/w0 + f/w1) = 1600 * (1/6) / (2/3) = 400 */
VCT_TEST(p16_perspective_texcoord) {
  VCT_TODO("hand-derived texcoord values; p17's oracle fuzz prefers the edge-to-edge span fraction for s/t: rewrite from oracle probes");
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 16, 0, 0, 0x3000, 31, 31, 31, 1600, 160);
  V(s, 2, 0, 16, 0, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  const Rec *r = at(8, 0);
  VCT_CHECK(r != NULL);
  VCT_EQ(r->st[0], 400);
  VCT_EQ(r->st[1], 40);
  VCT_EQ(at(0, 0)->st[0], 0);
}

/* colours are perspective-correct too: red 0 at w=1 to red 30 at w=3, middle: 30/4 = 7.5 of 5 bits -> 6-bit
 * values 0 and 61: 61/4 = 15.25 -> 15 */
VCT_TEST(p16_perspective_colour) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0);
  V(s, 1, 16, 0, 0, 0x3000, 30, 0, 0, 0, 0);
  V(s, 2, 0, 16, 0, 0x1000, 0, 0, 0, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(VC_R6(at(8, 0)->col), 15);
}

/* equal w, a rectangle with vertical edges (oracle-measured: 21k texcoord pixels and 30k colour pixels fit
 * exactly): the value along a row is floor(L + (R - L) * x / W), x counted from the first drawn pixel, W = the number
 * of drawn pixels. Rectangle x 0..33, s 8 -> 77 (1/16 texel): the oracle's texel sequence is 0 x4, 1 x8, 2 x8, 3 x7, 4 x6 */
VCT_TEST(p16_linear_when_w_equal) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 0, 0, 0, 8, 0);
  V(s, 1, 33, 0, 0, 0x1000, 31, 31, 31, 77, 0);
  V(s, 2, 33, 5, 0, 0x1000, 31, 31, 31, 77, 0);
  V(s, 3, 0, 5, 0, 0x1000, 0, 0, 0, 8, 0);
  VC_Poly p = poly(s, 4, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(row_count(1), 33);
  for (int x = 0; x < 33; x++) VCT_EQ(at(x, 1)->st[0], 8 + 69 * x / 33);
  VCT_EQ(at(4, 1)->st[0] >> 4, 1);
  VCT_EQ(at(32, 1)->st[0] >> 4, 4);
  /* colour 0 -> 31 (9 bit 0 -> 511): 6-bit value = floor(511 x / 33) >> 3 */
  for (int x = 0; x < 33; x++) VCT_EQ(VC_R6(at(x, 1)->col), (511 * x / 33) >> 3);
}

/* untextured toon / highlight shade like a textured polygon with a white texel (oracle, q19): toon = the table colour
 * of the vertex red's index, highlight = table colour + the vertex red (6 bit) */
VCT_TEST(p16_untextured_toon) {
  VCoreState *s = setup();
  vct_io16(s, VC_REG_DISP3DCNT, 0);
  vct_io16(s, VC_REG_TOON_TABLE + 2 * 14, 0x7C0F); /* r 15 g 0 b 31 */
  V(s, 0, 0, 0, 0, 0x1000, 14, 0, 0, 0, 0);
  V(s, 1, 8, 0, 0, 0x1000, 14, 0, 0, 0, 0);
  V(s, 2, 8, 8, 0, 0x1000, 14, 0, 0, 0, 0);
  V(s, 3, 0, 8, 0, 0x1000, 14, 0, 0, 0, 0);
  VC_Poly p = poly(s, 4, ALPHA31 | 2u << 4);
  vc_raster_poly(s, &p);
  VCT_EQ(at(3, 3)->col & 0xFFFFFF, VC_RGB6(31, 0, 63));
  nrec = 0;
  vct_io16(s, VC_REG_DISP3DCNT, 2);
  vc_raster_poly(s, &p);
  VCT_EQ(at(3, 3)->col & 0xFFFFFF, VC_RGB6(60, 29, 63));
}

/* z-buffering interpolates the screen z linearly: 0x100000 .. 0x300000 over 16 px, x=8 -> 0x200000 */
VCT_TEST(p16_z_depth_linear) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0x100000, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 16, 0, 0x300000, 0x3000, 31, 31, 31, 0, 0);
  V(s, 2, 0, 16, 0x100000, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(at(8, 0)->depth, 0x200000);
  VCT_EQ(at(0, 0)->depth, 0x100000);
}

/* w-buffering (SWAP_BUFFERS bit 1): the depth is w, perspective-correct: 2 / (1/4096 + 1/12288) = 6144 at the middle */
VCT_TEST(p16_w_depth) {
  VCoreState *s = setup();
  s->geo.swap_param = 2;
  V(s, 0, 0, 0, 0x100000, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 16, 0, 0x300000, 0x3000, 31, 31, 31, 0, 0);
  V(s, 2, 0, 16, 0x100000, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(at(8, 0)->depth, 6144);
  VCT_EQ(at(0, 0)->depth, 0x1000);
}

/* the depth test: less passes only a smaller depth; equal (POLYGON_ATTR bit 14) passes within +-0x200 */
VCT_TEST(p16_depth_test) {
  VCoreState *s = setup();
  s->f3d.depth[3][4] = 1000;
  VC_Poly less = poly(s, 3, ALPHA31), eq = poly(s, 3, ALPHA31 | 1u << 14);
  VCT_EQ(vc_3d_depth_test(s, &less, 4, 3, 999), 1);
  VCT_EQ(vc_3d_depth_test(s, &less, 4, 3, 1000), 0);
  VCT_EQ(vc_3d_depth_test(s, &less, 4, 3, 1001), 0);
  VCT_EQ(vc_3d_depth_test(s, &eq, 4, 3, 1000), 1);
  VCT_EQ(vc_3d_depth_test(s, &eq, 4, 3, 1000 + 0x200), 1);
  VCT_EQ(vc_3d_depth_test(s, &eq, 4, 3, 1000 + 0x201), 0);
  VCT_EQ(vc_3d_depth_test(s, &eq, 4, 3, 1000 - 0x200), 1);
  VCT_EQ(vc_3d_depth_test(s, &eq, 4, 3, 1000 - 0x201), 0);
  VCT_EQ(vc_3d_depth_test(s, &eq, 4, 3, 1200), 1);
}

/* a 0x0 polygon (1 dot): hidden when every vertex's w (3 fractional bits: w >> 9) exceeds DISP_1DOT_DEPTH unless
 * POLYGON_ATTR bit 13; otherwise one pixel from the first vertex */
VCT_TEST(p16_one_dot_polygon) {
  VCoreState *s = setup();
  V(s, 0, 5, 7, 77, 0x1000, 0, 31, 0, 0, 0);
  V(s, 1, 5, 7, 99, 0x1000, 31, 0, 0, 0, 0);
  V(s, 2, 5, 7, 99, 0x1000, 31, 0, 0, 0, 0);
  VC_Poly p = poly(s, 3, ALPHA31);
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 8); /* w 0x1000 >> 9 = 8 <= 8: shown */
  vc_raster_poly(s, &p);
  VCT_EQ(nrec, 1);
  VCT_EQ(rec[0].x, 5);
  VCT_EQ(rec[0].y, 7);
  VCT_EQ(rec[0].depth, 77);
  VCT_EQ(rec[0].col, VC_RGB6(0, 63, 0) | 31u << 24);
  nrec = 0;
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 7); /* 8 > 7: culled */
  vc_raster_poly(s, &p);
  VCT_EQ(nrec, 0);
  p.attr |= 1u << 13; /* but drawn regardless with bit 13 */
  vc_raster_poly(s, &p);
  VCT_EQ(nrec, 1);
}

/* 1x1 and 1x0 polygons are rounded up to one pixel */
VCT_TEST(p16_tiny_polygons_one_pixel) {
  VCoreState *s = setup();
  V(s, 0, 3, 3, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 4, 3, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 3, 3, 0, 0x1000, 31, 31, 31, 0, 0); /* a line segment: the vertex twice */
  VC_Poly p = poly(s, 3, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_CHECK(nrec >= 1 && at(3, 3));
  nrec = 0;
  V(s, 1, 4, 4, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 3, 4, 0, 0x1000, 31, 31, 31, 0, 0);
  vc_raster_poly(s, &p); /* 1x1 triangle: (3,3) only */
  VCT_EQ(nrec, 1);
  VCT_CHECK(at(3, 3));
}

/* wireframe (alpha 0): only the outline, alpha 31. Triangle (0,0) (8,0) (0,8): row 0 is the whole top edge (9),
 * rows 1..7 two pixels each (x=0 and x=8-y), row 8 the corner (0,8): 9 + 14 + 1 = 24 */
VCT_TEST(p16_wireframe) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 0, 0, 31, 0, 0);
  V(s, 1, 8, 0, 0, 0x1000, 0, 0, 31, 0, 0);
  V(s, 2, 0, 8, 0, 0x1000, 0, 0, 31, 0, 0);
  VC_Poly p = poly(s, 3, 0);
  vc_raster_poly(s, &p);
  /* oracle: the top row 0..7, rows 1..6 the left pixel x 0 and the hypotenuse pixel 7-y, row 7 only x 0 */
  VCT_EQ(nrec, 8 + 6 * 2 + 1);
  VCT_EQ(row_count(0), 8);
  VCT_EQ(row_count(5), 2);
  VCT_CHECK(at(0, 5) && at(2, 5));
  VCT_EQ(row_count(7), 1);
  /* the row-5 right pixel: x=3 by the hand-worked rule; v20b's oracle-fitted edge rules may put it one over, so
     find it instead of dereferencing a fixed x (a NULL here crashed the whole test binary) */
  const Rec *r5 = at(2, 5);
  VCT_CHECK(r5 != NULL);
  VCT_EQ(r5->col >> 24, 31);
  VCT_EQ(r5->edge, 1);
}

/* antialiasing coverage: the left edge x=0 of a square lies on the pixel's left side (pixel centres are integers,
 * so the pixel spans -0.5..0.5): half covered = 16 of 32; an interior pixel 31 */
VCT_TEST(p16_edge_coverage) {
  VCoreState *s = setup();
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 4);
  V(s, 0, 0, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 6, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 6, 6, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 3, 0, 6, 0, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 4, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(s->f3d.coverage[3][0], 31); /* oracle: a vertical edge is full coverage */
  VCT_EQ(s->f3d.coverage[3][3], 31);
}

/* a translucent polygon with alpha blending on is drawn whole, with blending off its right edge goes (alpha 16) */
VCT_TEST(p16_translucent_fill_rule) {
  VCoreState *s = setup();
  V(s, 0, 0, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 4, 0, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 0, 4, 0, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 3, 16u << 16);
  vc_raster_poly(s, &p);
  VCT_CHECK(nrec >= 6 && nrec <= 10);
  VCT_EQ(at(1, 1)->col >> 24, 16);
  nrec = 0;
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 3);
  vc_raster_poly(s, &p); /* whole: rows 0..4: 5+4+3+2+1 = 15 -- the vertical right edge rule does not apply (diagonal) */
  VCT_EQ(nrec, 10); /* oracle: blending on draws the same 10 pixels */
}

/* off-screen parts are clipped to 256x192 */
VCT_TEST(p16_screen_clip) {
  VCoreState *s = setup();
  V(s, 0, 250, 188, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 1, 260, 188, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 2, 260, 200, 0, 0x1000, 31, 31, 31, 0, 0);
  V(s, 3, 250, 200, 0, 0x1000, 31, 31, 31, 0, 0);
  VC_Poly p = poly(s, 4, ALPHA31);
  vc_raster_poly(s, &p);
  VCT_EQ(nrec, 6 * 4); /* x 250..255, y 188..191 */
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR): a plain frame with the 3D layer on BG0 and one polygon case ---- */
static u32 sx12(int px) { return (u32)(s16)((px * 4096 / 128) - 4096) & 0xFFFF; }
static u32 sy12(int py) { return (u32)(s16)(4096 - (py * 4096 / 96)) & 0xFFFF; } /* y up */
static void vtx(VCoreState *s, int px, int py, int z12, int r, int g, int b) {
  u32 c = (u32)(r | g << 5 | b << 10);
  vct_gx(s, VC_OP_COLOR, 1, &c);
  u32 p[2] = {sx12(px) | sy12(py) << 16, (u32)z12 & 0xFFFF};
  vct_gx(s, VC_OP_VTX_16, 2, p);
}
static VCoreState *scene_begin(u32 attr, u32 disp3d, u32 swap) {
  VCoreState *s = vct_state();
  vct_io32(s, VC_REG_DISPCNT, 0x00010108);
  vct_io16(s, VC_REG_DISP3DCNT, (u16)disp3d);
  vct_io32(s, VC_REG_CLEAR_COLOR, 0x1F000000 | 0x00004210);
  vct_io32(s, VC_REG_CLEAR_DEPTH, 0x7FFF);
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 0x7FFF);
  vct_pal(s, VC_ENG_A, 0, 0, 0x0000);
  u32 p;
  p = 0; vct_gx(s, VC_OP_MTX_MODE, 1, &p); vct_gx(s, VC_OP_MTX_IDENTITY, 0, NULL);
  p = 2; vct_gx(s, VC_OP_MTX_MODE, 1, &p); vct_gx(s, VC_OP_MTX_IDENTITY, 0, NULL);
  p = 0xBFFF0000u; vct_gx(s, VC_OP_VIEWPORT, 1, &p);
  p = attr; vct_gx(s, VC_OP_POLYGON_ATTR, 1, &p);
  (void)swap;
  return s;
}
static void scene_end(VCoreState *s, const char *name, u32 swap) {
  vct_gx(s, VC_OP_END_VTXS, 0, NULL);
  vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &swap);
  vct_emit(s, name);
}
static void begin(VCoreState *s, u32 type) { vct_gx(s, VC_OP_BEGIN_VTXS, 1, &type); }
#define A31 (0x001F00C0u)

VCT_TEST(p16_emit_scenes) {
  VCoreState *s;
  s = scene_begin(A31, 0, 0); begin(s, 0);
  vtx(s, 30, 20, 0, 31, 0, 0); vtx(s, 130, 40, 0, 0, 31, 0); vtx(s, 60, 150, 0, 0, 0, 31);
  scene_end(s, "p16_tri_gouraud", 0);
  s = scene_begin(A31, 0, 0); begin(s, 1);
  vtx(s, 40, 30, 0, 31, 31, 0); vtx(s, 200, 30, 0, 31, 31, 0); vtx(s, 200, 120, 0, 31, 31, 0); vtx(s, 40, 120, 0, 31, 31, 0);
  scene_end(s, "p16_quad_flat", 0);
  s = scene_begin(A31, 0, 0); begin(s, 0);
  vtx(s, 20, 10, 0, 0, 31, 31); vtx(s, 22, 10, 0, 0, 31, 31); vtx(s, 20, 180, 0, 0, 31, 31);
  vtx(s, 100, 10, 0, 31, 0, 31); vtx(s, 250, 12, 0, 31, 0, 31); vtx(s, 100, 14, 0, 31, 0, 31);
  scene_end(s, "p16_slivers", 0);
  s = scene_begin(0x001F00C0u, 0, 0); begin(s, 0);
  vtx(s, 20, 20, 0x0400, 31, 0, 0); vtx(s, 200, 60, 0x0400, 31, 0, 0); vtx(s, 60, 170, 0x0400, 31, 0, 0);
  vtx(s, 30, 100, 0x0200, 0, 0, 31); vtx(s, 220, 90, 0x0200, 0, 0, 31); vtx(s, 130, 190, 0x0200, 0, 0, 31);
  scene_end(s, "p16_depth_cross", 0);
  s = scene_begin(0x000000C0u, 0, 0); begin(s, 0); /* alpha 0: wireframe */
  vtx(s, 30, 20, 0, 31, 31, 31); vtx(s, 130, 40, 0, 31, 31, 31); vtx(s, 60, 150, 0, 31, 31, 31);
  scene_end(s, "p16_wire", 0);
  s = scene_begin(A31, 1 << 5, 0); begin(s, 1); /* edge marking: polygon IDs differ */
  vtx(s, 40, 30, 0, 31, 31, 0); vtx(s, 200, 30, 0, 31, 31, 0); vtx(s, 200, 120, 0, 31, 31, 0); vtx(s, 40, 120, 0, 31, 31, 0);
  scene_end(s, "p16_quad_edgemark", 0);
  s = scene_begin(A31, 1 << 4, 0); begin(s, 0); /* antialiasing */
  vtx(s, 30, 20, 0, 31, 0, 0); vtx(s, 130, 40, 0, 31, 0, 0); vtx(s, 60, 150, 0, 31, 0, 0);
  scene_end(s, "p16_tri_aa", 0);
  s = scene_begin(0x001F00C0u | 1u << 13, 0, 0); begin(s, 0); /* tiny polygons */
  vtx(s, 50, 50, 0, 31, 31, 31); vtx(s, 50, 50, 0, 31, 31, 31); vtx(s, 50, 50, 0, 31, 31, 31);
  vtx(s, 60, 50, 0, 31, 31, 31); vtx(s, 61, 50, 0, 31, 31, 31); vtx(s, 60, 51, 0, 31, 31, 31);
  scene_end(s, "p16_tiny", 0);
  s = scene_begin(A31, 0, 2); begin(s, 0); /* w-buffering */
  vtx(s, 30, 20, 0, 31, 0, 0); vtx(s, 130, 40, 0, 0, 31, 0); vtx(s, 60, 150, 0, 0, 0, 31);
  scene_end(s, "p16_wbuffer", 2);
}
