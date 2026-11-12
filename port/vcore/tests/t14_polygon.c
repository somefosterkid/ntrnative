/* port/vcore piece 14's unit tests: vertices, polygon assembly, clipping, viewport (docs/vcore.md, piece 14).
 * OWNED BY PIECE 14. Run: build/vcore/vcore_tests p14
 * Expected values are worked by hand from GBATEK 44 (vertex commands, primitive types, clipping), 43 (POLYGON_ATTR),
 * 35 (VIEWPORT, DISP_1DOT_DEPTH), 53 (POS_TEST). The clip matrix is set straight into s->p13.clip (identity unless a
 * test says), so the tests do not depend on piece 13's matrix commands. */
#include "vct.h"

#define ONE 0x1000
#define ATTR_BOTH 0x001F00C0 /* both faces, alpha 31 */

static void identity(VCoreState *s) {
  memset(&s->p13.clip, 0, sizeof s->p13.clip);
  for (int i = 0; i < 4; i++) s->p13.clip.m[i * 5] = ONE;
}

static VCoreState *fresh(void) {
  VCoreState *s = vct_state();
  identity(s);
  vc_poly_reset(s);
  return s;
}

static void cmd(VCoreState *s, u32 op, u32 a, u32 b) {
  u32 p[2] = {a, b};
  if (op >= VC_OP_VTX_16 && op <= VC_OP_VTX_DIFF) vc_cmd_vtx(s, op, p, 2);
  else vc_cmd_poly(s, op, p, 2);
}
static void begin(VCoreState *s, u32 attr, int type) {
  cmd(s, VC_OP_POLYGON_ATTR, attr, 0);
  cmd(s, VC_OP_BEGIN_VTXS, (u32)type, 0);
}
static void v16(VCoreState *s, int x, int y, int z) {
  cmd(s, VC_OP_VTX_16, (u32)(x & 0xFFFF) | (u32)(y & 0xFFFF) << 16, (u32)(z & 0xFFFF));
}

/* ---- the vertex formats ---- */

VCT_TEST(p14_vtx16_fixed_point) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  /* x = 0x0234, y = -0x0800 (0xF800), z = -1 (0xFFFF): 12-bit fraction, signed 16 */
  v16(s, 0x0234, -0x800, -1);
  VCT_EQ(s->p14.cur[0], 0x0234);
  VCT_EQ(s->p14.cur[1], -0x800);
  VCT_EQ(s->p14.cur[2], -1);
  v16(s, 0x1000, 0, 0);
  v16(s, 0, 0x1000, 0);
  VCT_EQ(s->geo.nvtx, 3);
  /* identity clip matrix: clip = (x, y, z, 1.0) */
  VCT_EQ(s->geo.vtx[0].clip[0], 0x0234);
  VCT_EQ(s->geo.vtx[0].clip[1], -0x800);
  VCT_EQ(s->geo.vtx[0].clip[2], -1);
  VCT_EQ(s->geo.vtx[0].clip[3], ONE);
}

VCT_TEST(p14_vtx10_six_bit_fraction) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  /* x = 1 (1/64), y = 0x200 (the 10-bit sign bit: -512/64 = -8.0), z = 0x1FF (511/64) */
  cmd(s, VC_OP_VTX_10, 1u | 0x200u << 10 | 0x1FFu << 20, 0);
  VCT_EQ(s->p14.cur[0], 0x40);
  VCT_EQ(s->p14.cur[1], -0x8000);
  VCT_EQ(s->p14.cur[2], 0x1FF * 0x40);
  /* bits 30-31 unused */
  cmd(s, VC_OP_VTX_10, 0xC0000000u | 0x3FFu, 0);
  VCT_EQ(s->p14.cur[0], -0x40);
  VCT_EQ(s->p14.cur[1], 0);
  VCT_EQ(s->p14.cur[2], 0);
}

VCT_TEST(p14_vtx_xy_xz_yz_keep_the_third) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  v16(s, 0x100, 0x200, 0x300);
  cmd(s, VC_OP_VTX_XY, 0xF000u << 16 | 0x0400u, 0); /* x = 0x400, y = -0x1000, z stays */
  VCT_EQ(s->p14.cur[0], 0x400);
  VCT_EQ(s->p14.cur[1], -0x1000);
  VCT_EQ(s->p14.cur[2], 0x300);
  cmd(s, VC_OP_VTX_XZ, 0x0555u << 16 | 0x0666u, 0); /* x = 0x666, z = 0x555, y stays */
  VCT_EQ(s->p14.cur[0], 0x666);
  VCT_EQ(s->p14.cur[1], -0x1000);
  VCT_EQ(s->p14.cur[2], 0x555);
  cmd(s, VC_OP_VTX_YZ, 0x8000u << 16 | 0x0777u, 0); /* y = 0x777, z = -0x8000, x stays */
  VCT_EQ(s->p14.cur[0], 0x666);
  VCT_EQ(s->p14.cur[1], 0x777);
  VCT_EQ(s->p14.cur[2], -0x8000);
  /* three vertices were made */
  VCT_EQ(s->p14.count, 1); /* four vertices: a triangle completed, one pending */
}

VCT_TEST(p14_vtx_diff_signed_10bit) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  v16(s, 0x1000, 0x1000, 0x1000);
  /* dx = +1, dy = -1 (0x3FF), dz = +511 (0x1FF): the 9-bit-fraction values / 8 are the raw numbers in 12-bit units */
  cmd(s, VC_OP_VTX_DIFF, 1u | 0x3FFu << 10 | 0x1FFu << 20, 0);
  VCT_EQ(s->p14.cur[0], 0x1001);
  VCT_EQ(s->p14.cur[1], 0x0FFF);
  VCT_EQ(s->p14.cur[2], 0x11FF);
  /* the most negative difference: 0x200 = -512 = -0.125 */
  cmd(s, VC_OP_VTX_DIFF, 0x200u | 0x200u << 10 | 0x200u << 20, 0);
  VCT_EQ(s->p14.cur[0], 0x1001 - 512);
  VCT_EQ(s->p14.cur[1], 0x0FFF - 512);
  VCT_EQ(s->p14.cur[2], 0x11FF - 512);
}

VCT_TEST(p14_vertex_outside_list_ignored) {
  VCoreState *s = fresh();
  v16(s, 1, 2, 3); /* no BEGIN_VTXS: the position is kept, no vertex made */
  VCT_EQ(s->geo.nvtx, 0);
  begin(s, ATTR_BOTH, 0);
  cmd(s, VC_OP_END_VTXS, 0, 0);
  v16(s, 1, 2, 3);
  v16(s, 1, 2, 3);
  v16(s, 1, 2, 3);
  VCT_EQ(s->geo.nvtx, 0);
}

/* ---- primitive types ---- */

/* a counter-clockwise triangle at (x, y) of size 0x400 */
static void tri_ccw(VCoreState *s, int x, int y) {
  v16(s, x, y, 0);
  v16(s, x + 0x400, y, 0);
  v16(s, x, y + 0x400, 0);
}

VCT_TEST(p14_triangle_list) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  tri_ccw(s, 0, 0);
  tri_ccw(s, 0x400, 0x400);
  v16(s, 0, 0, 0); /* a seventh vertex: an incomplete third triangle makes nothing */
  VCT_EQ(s->geo.npoly, 2);
  VCT_EQ(s->geo.nvtx, 6);
  VCT_EQ(s->geo.poly[0].nv, 3);
  VCT_EQ(s->geo.poly[0].v[0], 0);
  VCT_EQ(s->geo.poly[0].v[2], 2);
  VCT_EQ(s->geo.poly[1].v[0], 3);
  VCT_EQ(s->geo.poly[1].type, 0);
  VCT_EQ(s->geo.poly[1].index, 1);
  VCT_EQ(s->geo.poly[0].front, 1);
}

VCT_TEST(p14_quad_list_vertex_order) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 1);
  v16(s, -0x400, 0x400, 0); /* v0 top-left, v1 bottom-left, v2 bottom-right, v3 top-right: anti-clockwise */
  v16(s, -0x400, -0x400, 0);
  v16(s, 0x400, -0x400, 0);
  v16(s, 0x400, 0x400, 0);
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].nv, 4);
  VCT_EQ(s->geo.poly[0].front, 1);
  VCT_EQ(s->geo.poly[0].type, 1);
  for (int i = 0; i < 4; i++) VCT_EQ(s->geo.poly[0].v[i], i);
  /* 3D screen: x -0.25 .. 0.25 of the 256x192 viewport: 96..160 wide, y 72..120 */
  VCT_EQ(s->geo.vtx[0].sx, 96);
  VCT_EQ(s->geo.vtx[0].sy, 72);
  VCT_EQ(s->geo.vtx[2].sx, 160);
  VCT_EQ(s->geo.vtx[2].sy, 120);
  VCT_EQ(s->geo.poly[0].ymin, 72);
  VCT_EQ(s->geo.poly[0].ymax, 120);
}

VCT_TEST(p14_triangle_strip_winding_and_sharing) {
  VCoreState *s = fresh();
  begin(s, 0x001F0080, 2); /* front faces only: a strip's second triangle must be flipped to stay front */
  /* GBATEK 44's strip: v0 (0,0.5) v1 (0,0) v2 (0.25,0.5) v3 (0.25,0) v4 (0.5,0.5): v0 v1 v2 anti-clockwise */
  v16(s, 0, 0x800, 0);
  v16(s, 0, 0, 0);
  v16(s, 0x400, 0x800, 0);
  v16(s, 0x400, 0, 0);
  v16(s, 0x800, 0x800, 0);
  VCT_EQ(s->geo.npoly, 3);
  VCT_EQ(s->geo.nvtx, 5); /* unclipped strip triangles share their vertices */
  /* polygon 0: v0 v1 v2; polygon 1: v2 v1 v3 (first two swapped); polygon 2: v2 v3 v4 */
  VCT_EQ(s->geo.poly[0].v[0], 0); VCT_EQ(s->geo.poly[0].v[1], 1); VCT_EQ(s->geo.poly[0].v[2], 2);
  VCT_EQ(s->geo.poly[1].v[0], 2); VCT_EQ(s->geo.poly[1].v[1], 1); VCT_EQ(s->geo.poly[1].v[2], 3);
  VCT_EQ(s->geo.poly[2].v[0], 2); VCT_EQ(s->geo.poly[2].v[1], 3); VCT_EQ(s->geo.poly[2].v[2], 4);
  for (int i = 0; i < 3; i++) {
    VCT_EQ(s->geo.poly[i].front, 1);
    VCT_EQ(s->geo.poly[i].type, 2);
  }
}

VCT_TEST(p14_quad_strip_vertex_order) {
  VCoreState *s = fresh();
  begin(s, 0x001F0080, 3);
  /* v0 v2 v4 on top, v1 v3 v5 below (GBATEK 44): quads (v0 v1 v3 v2) and (v2 v3 v5 v4) */
  int top = 0x400, bot = -0x400;
  v16(s, 0, top, 0);
  v16(s, 0, bot, 0);
  v16(s, 0x400, top, 0);
  v16(s, 0x400, bot, 0);
  VCT_EQ(s->geo.npoly, 1); /* four vertices make the first quad, odd counts nothing */
  v16(s, 0x800, top, 0);
  VCT_EQ(s->geo.npoly, 1);
  v16(s, 0x800, bot, 0);
  VCT_EQ(s->geo.npoly, 2);
  VCT_EQ(s->geo.nvtx, 6);
  const u16 want0[4] = {0, 1, 3, 2}, want1[4] = {2, 3, 5, 4};
  for (int i = 0; i < 4; i++) {
    VCT_EQ(s->geo.poly[0].v[i], want0[i]);
    VCT_EQ(s->geo.poly[1].v[i], want1[i]);
  }
  VCT_EQ(s->geo.poly[0].front, 1);
  VCT_EQ(s->geo.poly[1].front, 1);
  VCT_EQ(s->geo.poly[1].type, 3);
}

VCT_TEST(p14_begin_restarts_the_list) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  v16(s, 0, 0, 0);
  v16(s, 0x400, 0, 0);
  begin(s, ATTR_BOTH, 0); /* two vertices are dropped */
  tri_ccw(s, 0, 0);
  VCT_EQ(s->geo.npoly, 1);
}

/* ---- culling ---- */

static void one_tri(VCoreState *s, u32 attr, int ccw) {
  begin(s, attr, 0);
  v16(s, 0, 0, 0);
  if (ccw) { v16(s, 0x400, 0, 0); v16(s, 0, 0x400, 0); }
  else { v16(s, 0, 0x400, 0); v16(s, 0x400, 0, 0); }
}

VCT_TEST(p14_cull_front_only) {
  VCoreState *s = fresh();
  one_tri(s, 0x001F0080, 1); /* bit 7: front surface rendered */
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].front, 1);
  one_tri(s, 0x001F0080, 0); /* a clockwise triangle is a back face: culled */
  VCT_EQ(s->geo.npoly, 1);
}

VCT_TEST(p14_cull_back_only) {
  VCoreState *s = fresh();
  one_tri(s, 0x001F0040, 0); /* bit 6: back surface rendered */
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].front, 0);
  one_tri(s, 0x001F0040, 1);
  VCT_EQ(s->geo.npoly, 1);
}

VCT_TEST(p14_cull_both_and_neither) {
  VCoreState *s = fresh();
  one_tri(s, ATTR_BOTH, 1);
  one_tri(s, ATTR_BOTH, 0);
  VCT_EQ(s->geo.npoly, 2);
  VCT_EQ(s->geo.poly[0].front, 1);
  VCT_EQ(s->geo.poly[1].front, 0);
  one_tri(s, 0x001F0000, 1); /* both bits clear: nothing is drawn */
  one_tri(s, 0x001F0000, 0);
  VCT_EQ(s->geo.npoly, 2);
}

VCT_TEST(p14_cull_line_segment_always_drawn) {
  VCoreState *s = fresh();
  begin(s, 0x001F0000, 0); /* GBATEK 44: a triangle with a repeated vertex is a line segment, no front or back */
  v16(s, 0, 0, 0);
  v16(s, 0x400, 0, 0);
  v16(s, 0x400, 0, 0);
  VCT_EQ(s->geo.npoly, 1);
}

VCT_TEST(p14_facing_follows_the_clip_matrix) {
  /* mirroring x (scale -1) turns an anti-clockwise triangle clockwise */
  VCoreState *s = fresh();
  s->p13.clip.m[0] = -ONE;
  one_tri(s, 0x001F0080, 1);
  VCT_EQ(s->geo.npoly, 0);
  one_tri(s, 0x001F0040, 1);
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].front, 0);
}

/* ---- clipping ---- */

static VC_Vertex mk(int x, int y, int z, int w, int r) {
  VC_Vertex v;
  memset(&v, 0, sizeof v);
  v.clip[0] = x; v.clip[1] = y; v.clip[2] = z; v.clip[3] = w;
  v.color[0] = (u8)r;
  return v;
}

VCT_TEST(p14_clip_one_plane_two_new_vertices) {
  /* A(-0.5,-0.5) B(1.5,-0.5) C(-0.5,0.5), w = 1: B is beyond x <= w. A-B meets x = 1 at t = 0.75, B-C at t = 0.25:
   * the triangle becomes a quad A, (1,-0.5), (1,-0.25), C; red 0 / 31 / 0 interpolates to 23.25 and 23.25 (truncated
   * to 23, and B->C: 31 - 7.75 = 23.25 computed as 31 + (-31 * 0.25 truncated toward zero = -7) = 24). */
  VC_Vertex in[3] = {mk(-0x800, -0x800, 0, ONE, 0), mk(0x1800, -0x800, 0, ONE, 31), mk(-0x800, 0x800, 0, ONE, 0)}, out[10];
  int n = vc_p14_clip(in, 3, 0, out);
  VCT_EQ(n, 4);
  VCT_EQ(out[0].clip[0], -0x800); VCT_EQ(out[0].clipped, 0);
  VCT_EQ(out[1].clip[0], 0x1000); VCT_EQ(out[1].clip[1], -0x800); VCT_EQ(out[1].clipped, 1);
  VCT_EQ(out[1].color[0], 23);
  VCT_EQ(out[2].clip[0], 0x1000); VCT_EQ(out[2].clip[1], -0x400); VCT_EQ(out[2].clipped, 1);
  VCT_EQ(out[2].color[0], 24);
  VCT_EQ(out[3].clip[0], -0x800); VCT_EQ(out[3].clip[1], 0x800); VCT_EQ(out[3].clipped, 0);
  VCT_EQ(out[1].clip[3], ONE);
}

VCT_TEST(p14_clip_texcoords_interpolate) {
  VC_Vertex in[3] = {mk(-0x800, -0x800, 0, ONE, 0), mk(0x1800, -0x800, 0, ONE, 0), mk(-0x800, 0x800, 0, ONE, 0)}, out[10];
  in[0].tex[0] = 0; in[1].tex[0] = 0x400; in[2].tex[0] = 0; /* s = 0, 64.0, 0 (12.4) */
  int n = vc_p14_clip(in, 3, 0, out);
  VCT_EQ(n, 4);
  VCT_EQ(out[1].tex[0], 0x300); /* 0.75 of the way A -> B */
  VCT_EQ(out[2].tex[0], 0x400 - 0x100); /* B -> C: 0.25 of the way */
}

VCT_TEST(p14_clip_corner_two_planes) {
  /* A(0,0) B(3,0) C(0,3) with w = 1 covers the whole square [0,1]^2 and its corner (1,1): the clip is that square:
   * (0,1) (0,0) (1,0) (1,1) after x <= w then y <= w, 4 vertices, the corner among them */
  VC_Vertex in[3] = {mk(0, 0, 0, ONE, 0), mk(0x3000, 0, 0, ONE, 0), mk(0, 0x3000, 0, ONE, 0)}, out[10];
  int n = vc_p14_clip(in, 3, 0, out);
  VCT_EQ(n, 4);
  VCT_EQ(out[0].clip[0], 0);      VCT_EQ(out[0].clip[1], 0x1000);
  VCT_EQ(out[1].clip[0], 0);      VCT_EQ(out[1].clip[1], 0);
  VCT_EQ(out[2].clip[0], 0x1000); VCT_EQ(out[2].clip[1], 0);
  VCT_EQ(out[3].clip[0], 0x1000); VCT_EQ(out[3].clip[1], 0x1000);
}

VCT_TEST(p14_clip_inside_unchanged_outside_dropped) {
  VC_Vertex in[3] = {mk(0, 0, 0, ONE, 1), mk(0x400, 0, 0, ONE, 2), mk(0, 0x400, 0, ONE, 3)}, out[10];
  VCT_EQ(vc_p14_clip(in, 3, 0, out), 3);
  VCT_EQ(out[1].clip[0], 0x400);
  VCT_EQ(out[2].color[0], 3);
  VCT_EQ(out[2].clipped, 0);
  /* wholly beyond x > w */
  VC_Vertex off[3] = {mk(0x2000, 0, 0, ONE, 0), mk(0x3000, 0, 0, ONE, 0), mk(0x2000, 0x400, 0, ONE, 0)};
  VCT_EQ(vc_p14_clip(off, 3, 0, out), 0);
}

VCT_TEST(p14_clip_near_plane) {
  /* z < -w is clipped away whatever the attribute: A(0,0,-2) B(0.5,0,0) C(0,0.5,0), w = 1: A out. Edges C-A and A-B
   * meet z = -1 at t = 0.5 (from the inside end): A-B (0.25, 0, -1), C-A (0, 0.25, -1) */
  VC_Vertex in[3] = {mk(0, 0, -0x2000, ONE, 0), mk(0x800, 0, 0, ONE, 0), mk(0, 0x800, 0, ONE, 0)}, out[10];
  int n = vc_p14_clip(in, 3, 0, out);
  VCT_EQ(n, 4);
  /* order: i=0 cur A out, prev C in -> intersect(C, A); i=1 cur B in, prev A out -> intersect(A, B), B; i=2 C */
  VCT_EQ(out[0].clip[0], 0);      VCT_EQ(out[0].clip[1], 0x400); VCT_EQ(out[0].clip[2], -0x1000);
  VCT_EQ(out[1].clip[0], 0x400);  VCT_EQ(out[1].clip[1], 0);     VCT_EQ(out[1].clip[2], -0x1000);
  VCT_EQ(out[2].clip[0], 0x800);
  VCT_EQ(out[3].clip[1], 0x800);
}

VCT_TEST(p14_clip_far_plane_hidden_unless_bit12) {
  VC_Vertex in[3] = {mk(0, 0, 0, ONE, 0), mk(0x400, 0, 0x2000, ONE, 0), mk(0, 0x400, 0, ONE, 0)}, out[10];
  VCT_EQ(vc_p14_clip(in, 3, 0, out), 0); /* attr bit 12 clear: the polygon is dropped */
  int n = vc_p14_clip(in, 3, 1, out);     /* set: clipped at z = w */
  VCT_EQ(n, 4);
  for (int i = 0; i < n; i++) VCT_CHECK(out[i].clip[2] <= out[i].clip[3]);
}

VCT_TEST(p14_far_plane_polygon_through_commands) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  v16(s, 0, 0, 0); v16(s, 0x400, 0, 0x2000); v16(s, 0, 0x400, 0);
  VCT_EQ(s->geo.npoly, 0);
  VCT_EQ(s->geo.nvtx, 0); /* GBATEK 44: not stored in vertex RAM either */
  begin(s, ATTR_BOTH | 1 << 12, 0);
  v16(s, 0, 0, 0); v16(s, 0x400, 0, 0x2000); v16(s, 0, 0x400, 0);
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].nv, 4);
  VCT_EQ(s->geo.nvtx, 4);
}

VCT_TEST(p14_clipped_strip_triangle_becomes_separate) {
  VCoreState *s = fresh();
  begin(s, 0x001F0080, 2);
  v16(s, 0, 0x800, 0);
  v16(s, 0, 0, 0);
  v16(s, 0x400, 0x800, 0);
  v16(s, 0x400, 0, 0);
  VCT_EQ(s->geo.nvtx, 4);
  /* v4 far to the right: triangle (v2 v3 v4) with v4 at x = 3.0 > w is clipped: its vertices are new entries */
  v16(s, 0x3000, 0x400, 0);
  VCT_EQ(s->geo.npoly, 3);
  VCT_EQ(s->geo.poly[2].nv, 4);
  VCT_EQ(s->geo.nvtx, 4 + 4);
  for (int i = 0; i < 4; i++) VCT_CHECK(s->geo.poly[2].v[i] >= 4);
}

VCT_TEST(p14_clip_through_commands_marks_clipped) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  v16(s, -0x800, -0x800, 0); v16(s, 0x1800, -0x800, 0); v16(s, -0x800, 0x800, 0);
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].nv, 4);
  VCT_EQ(s->geo.nvtx, 4);
  int nc = 0;
  for (u32 i = 0; i < s->geo.nvtx; i++) nc += s->geo.vtx[i].clipped;
  VCT_EQ(nc, 2);
  /* the new vertices sit on the right edge: x = w -> screen x = 256 (GBATEK 35: one pixel beyond X2) */
  VCT_EQ(s->geo.vtx[s->geo.poly[0].v[1]].sx, 256);
}

/* ---- the viewport ---- */

VCT_TEST(p14_viewport_default_fullscreen) {
  VC_Vertex v = mk(0, 0, 0, ONE, 0);
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sx, 128); /* (0 + 1) * 256 / 2 */
  VCT_EQ(v.sy, 96);
  v = mk(-ONE, ONE, 0, ONE, 0); /* the top-left corner of the view volume */
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sx, 0);
  VCT_EQ(v.sy, 0);
  v = mk(ONE, -ONE, 0, ONE, 0); /* the bottom-right: one past the last pixel */
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sx, 256);
  VCT_EQ(v.sy, 192);
}

VCT_TEST(p14_viewport_with_x1_and_scaled_w) {
  /* VIEWPORT x1 = 16, y1 = 8, x2 = 111, y2 = 103: 96 x 96 pixels, y counted from the bottom, so the top edge is at
   * screen y 191 - 103 = 88. (0,0,0,w=2.0) is the centre either way: x = 16 + 48, y = 88 + 48. */
  VC_Vertex v = mk(0, 0, 0, 2 * ONE, 0);
  vc_p14_viewport(&v, 16, 8, 111, 103);
  VCT_EQ(v.sx, 64);
  VCT_EQ(v.sy, 136);
  v = mk(-ONE, ONE, 0, ONE, 0);
  vc_p14_viewport(&v, 16, 8, 111, 103);
  VCT_EQ(v.sx, 16);
  VCT_EQ(v.sy, 88);
  v = mk(ONE, -ONE, 0, ONE, 0);
  vc_p14_viewport(&v, 16, 8, 111, 103);
  VCT_EQ(v.sx, 112);
  VCT_EQ(v.sy, 184);
  /* x = 0.5 with w = 1: (0.5 + 1) * 96 / 2 = 72 + 16 */
  v = mk(ONE / 2, 0, 0, ONE, 0);
  vc_p14_viewport(&v, 16, 8, 111, 103);
  VCT_EQ(v.sx, 88);
}

VCT_TEST(p14_viewport_command) {
  VCoreState *s = fresh();
  /* x1 = 16, y1 = 8, x2 = 111, y2 = 103 packed as the command sends them */
  cmd(s, VC_OP_VIEWPORT, 16u | 8u << 8 | 111u << 16 | 103u << 24, 0);
  begin(s, ATTR_BOTH, 0);
  v16(s, 0, 0, 0); v16(s, 0x1000, 0, 0); v16(s, 0, 0x1000, 0);
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.vtx[0].sx, 64);
  VCT_EQ(s->geo.vtx[0].sy, 136);
  VCT_EQ(s->geo.vtx[1].sx, 112);
  VCT_EQ(s->geo.vtx[2].sy, 88);
  VCT_EQ(s->geo.poly[0].ymin, 88);
  VCT_EQ(s->geo.poly[0].ymax, 136);
}

VCT_TEST(p14_depth_formula) {
  /* sz = (z/w * 0x4000 + 0x3FFF) * 0x200: z = 0 -> 0x3FFF * 0x200; z = w -> 0x7FFF * 0x200; z = w/2 */
  VC_Vertex v = mk(0, 0, 0, ONE, 0);
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sz, 0x7FFE00);
  v = mk(0, 0, ONE, ONE, 0);
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sz, 0xFFFE00);
  v = mk(0, 0, ONE / 2, ONE, 0); /* 0x2000 + 0x3FFF = 0x5FFF */
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sz, 0xBFFE00);
  v = mk(0, 0, ONE, 2 * ONE, 0); /* z/w = 0.5 again */
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sz, 0xBFFE00);
  v = mk(0, 0, -ONE, ONE, 0); /* z = -w would be -0x200: clamped at the nearest, 0 */
  vc_p14_viewport(&v, 0, 0, 255, 191);
  VCT_EQ(v.sz, 0);
}

/* ---- polygon state ---- */

VCT_TEST(p14_attr_latched_at_begin) {
  VCoreState *s = fresh();
  cmd(s, VC_OP_POLYGON_ATTR, 0x001F00C5, 0);
  cmd(s, VC_OP_BEGIN_VTXS, 0, 0);
  cmd(s, VC_OP_POLYGON_ATTR, 0x000000C9, 0); /* inside the list: no effect until the next BEGIN_VTXS */
  cmd(s, VC_OP_TEXIMAGE_PARAM, 0x02345678, 0);
  cmd(s, VC_OP_PLTT_BASE, 0x1ABC, 0);
  tri_ccw(s, 0, 0);
  VCT_EQ(s->geo.poly[0].attr, 0x001F00C5);
  VCT_EQ(s->geo.poly[0].texparam, 0x02345678);
  VCT_EQ(s->geo.poly[0].pltt, 0x1ABC);
  cmd(s, VC_OP_END_VTXS, 0, 0);
  cmd(s, VC_OP_BEGIN_VTXS, 0, 0);
  tri_ccw(s, 0, 0);
  VCT_EQ(s->geo.poly[1].attr, 0x000000C9);
}

VCT_TEST(p14_one_dot_polygon) {
  /* a polygon of a single screen pixel (three equal vertices; GBATEK 35): shown only with bit 13 and when some
   * vertex has w <= DISP_1DOT_DEPTH (12.3 fixed point: w = 1.0 is 8) */
  VCoreState *s = fresh();
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 0x7FFF);
  begin(s, ATTR_BOTH, 0);
  v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0);
  VCT_EQ(s->geo.npoly, 0); /* bit 13 clear: hidden */
  begin(s, ATTR_BOTH | 1 << 13, 0);
  v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0);
  VCT_EQ(s->geo.npoly, 1);
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 7); /* boundary 0.875 < w = 1.0: too far */
  v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0);
  VCT_EQ(s->geo.npoly, 1);
  vct_io16(s, VC_REG_DISP_1DOT_DEPTH, 8);
  v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0); v16(s, 0x100, 0x100, 0);
  VCT_EQ(s->geo.npoly, 2);
}

VCT_TEST(p14_polygon_limit_2048) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 0);
  for (int i = 0; i < 2049; i++) tri_ccw(s, 0, 0);
  VCT_EQ(s->geo.npoly, 2048);
  VCT_EQ(s->geo.nvtx, 6144);
  VCT_EQ(s->geo.overflow, 1);
}

VCT_TEST(p14_vertex_limit_6144) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH, 1);
  for (int i = 0; i < 1537; i++) {
    v16(s, -0x400, 0x400, 0); v16(s, -0x400, -0x400, 0); v16(s, 0x400, -0x400, 0); v16(s, 0x400, 0x400, 0);
  }
  VCT_EQ(s->geo.npoly, 1536);
  VCT_EQ(s->geo.nvtx, 6144);
  VCT_EQ(s->geo.overflow, 1);
}

VCT_TEST(p14_pos_test) {
  VCoreState *s = fresh();
  /* translation by (1, 2, 3): row 3 of the matrix (row vectors, GBATEK 42) */
  s->p13.clip.m[12] = 1 * ONE; s->p13.clip.m[13] = 2 * ONE; s->p13.clip.m[14] = 3 * ONE;
  cmd(s, VC_OP_POS_TEST, 0x0800u << 16 | 0x0400u, 0x0200); /* (0.25, 0.5, 0.125) */
  VCT_EQ(s->p14.pos_result[0], 0x400 + 0x1000);
  VCT_EQ(s->p14.pos_result[1], 0x800 + 0x2000);
  VCT_EQ(s->p14.pos_result[2], 0x200 + 0x3000);
  VCT_EQ(s->p14.pos_result[3], ONE);
  VCT_EQ(s->geo.npoly, 0); /* no picture */
  VCT_EQ(s->p14.cur[0], 0x400); /* it overwrote the current vertex */
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR): one case each in an otherwise plain 3D frame ---- */

static void gx(VCoreState *s, u32 op, u32 a, u32 b, int n) {
  u32 p[2] = {a, b};
  vct_gx(s, op, (u32)n, p);
}

static VCoreState *scene_state(void) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  vct_io32(s, VC_REG_DISPCNT, 0x00010000 | 1 << 3 | 1 << 8);
  vct_pal(s, VC_ENG_A, 0, 0, 0x2800);
  vct_io16(s, VC_REG_DISP3DCNT, 0);
  vct_io32(s, VC_REG_CLEAR_COLOR, 0);
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0x7FFF);
  u32 id[16] = {ONE, 0, 0, 0, 0, ONE, 0, 0, 0, 0, ONE, 0, 0, 0, 0, ONE};
  gx(s, VC_OP_MTX_MODE, 0, 0, 1);
  vct_gx(s, VC_OP_MTX_LOAD_4x4, 16, id);
  gx(s, VC_OP_MTX_MODE, 2, 0, 1);
  gx(s, VC_OP_MTX_IDENTITY, 0, 0, 0);
  gx(s, VC_OP_VIEWPORT, 0xBFFF0000u, 0, 1);
  gx(s, VC_OP_TEXIMAGE_PARAM, 0, 0, 1);
  return s;
}
static void s_v16(VCoreState *s, int x, int y, int z) {
  gx(s, VC_OP_VTX_16, (u32)(x & 0xFFFF) | (u32)(y & 0xFFFF) << 16, (u32)(z & 0xFFFF), 2);
}
/* the same with a quarter of the size (the scenes that need to stay inside the view volume) */
static void s_v16q(VCoreState *s, int x, int y, int z) { s_v16(s, x / 4, y / 4, z / 4); }
static void s_poly(VCoreState *s, u32 attr, u32 color, int type) {
  gx(s, VC_OP_POLYGON_ATTR, attr, 0, 1);
  gx(s, VC_OP_COLOR, color, 0, 1);
  gx(s, VC_OP_BEGIN_VTXS, (u32)type, 0, 1);
}
static void s_end(VCoreState *s) {
  gx(s, VC_OP_END_VTXS, 0, 0, 0);
  gx(s, VC_OP_SWAP_BUFFERS, 0, 0, 1);
}

VCT_TEST(p14_scene_cull) {
  VCoreState *s = scene_state();
  /* front-only: the ccw triangle at the left is drawn, the cw one at the right is not; back-only: the opposite */
  s_poly(s, 0x001F0080, 0x001F, 0);
  s_v16q(s, -0x3000, -0x1000, 0); s_v16q(s, -0x1800, -0x1000, 0); s_v16q(s, -0x2400, 0x1000, 0);
  s_v16q(s, 0x1800, -0x1000, 0); s_v16q(s, 0x2400, 0x1000, 0); s_v16q(s, 0x3000, -0x1000, 0);
  s_poly(s, 0x001F0040, 0x03E0, 0);
  s_v16q(s, -0x3000, 0x1800, 0); s_v16q(s, -0x1800, 0x1800, 0); s_v16q(s, -0x2400, 0x2800, 0); /* ccw: culled */
  s_v16q(s, 0x1800, 0x1800, 0); s_v16q(s, 0x2400, 0x2800, 0); s_v16q(s, 0x3000, 0x1800, 0);
  s_end(s);
  vct_emit(s, "p14_scene_cull");
}

VCT_TEST(p14_scene_quad_and_strips) {
  VCoreState *s = scene_state();
  s_poly(s, ATTR_BOTH, 0x7C00, 1); /* a quad */
  s_v16q(s, -0x3000, 0x3000, 0); s_v16q(s, -0x3000, 0x1800, 0); s_v16q(s, -0x1800, 0x1800, 0); s_v16q(s, -0x1800, 0x3000, 0);
  s_poly(s, ATTR_BOTH, 0x03FF, 2); /* a triangle strip of 4 */
  s_v16q(s, 0, 0x3000, 0); s_v16q(s, 0, 0x1800, 0); s_v16q(s, 0x1000, 0x3000, 0); s_v16q(s, 0x1000, 0x1800, 0);
  s_poly(s, ATTR_BOTH, 0x7FE0, 3); /* a quad strip of 6 */
  s_v16q(s, -0x3000, 0, 0); s_v16q(s, -0x3000, -0x1000, 0); s_v16q(s, -0x1800, 0x400, 0); s_v16q(s, -0x1800, -0x1400, 0);
  s_v16q(s, 0, 0, 0); s_v16q(s, 0, -0x1000, 0);
  s_end(s);
  vct_emit(s, "p14_scene_quad_and_strips");
}

VCT_TEST(p14_scene_clip_planes) {
  VCoreState *s = scene_state();
  /* a triangle past the right edge, one past the top, one in the bottom-left corner, one through the near plane */
  s_poly(s, ATTR_BOTH, 0x001F, 0);
  s_v16(s, 0x0800, -0x0800, 0); s_v16(s, 0x2800, -0x0400, 0); s_v16(s, 0x0C00, 0x0800, 0);
  s_poly(s, ATTR_BOTH, 0x03E0, 0);
  s_v16(s, -0x0C00, 0x0800, 0); s_v16(s, -0x0400, 0x2000, 0); s_v16(s, -0x0800, 0x0C00, 0);
  s_poly(s, ATTR_BOTH, 0x7C00, 0);
  s_v16(s, -0x3000, -0x3000, 0); s_v16(s, -0x0400, -0x0C00, 0); s_v16(s, -0x0C00, -0x0400, 0);
  s_poly(s, ATTR_BOTH, 0x7FFF, 0);
  s_v16(s, -0x0400, -0x0400, 0); s_v16(s, 0x0400, -0x0400, 0); s_v16(s, 0x0000, 0x0400, -0x2000);
  s_end(s);
  vct_emit(s, "p14_scene_clip_planes");
}

VCT_TEST(p14_scene_far_plane) {
  VCoreState *s = scene_state();
  s_poly(s, ATTR_BOTH, 0x001F, 0); /* crosses the far plane, bit 12 clear: not drawn */
  s_v16(s, -0x1000, -0x1000, 0); s_v16(s, 0x0000, -0x1000, 0x2000); s_v16(s, -0x0800, 0x1000, 0);
  s_poly(s, ATTR_BOTH | 1 << 12, 0x03E0, 0); /* bit 12 set: drawn, clipped */
  s_v16(s, 0x0400, -0x0800, 0); s_v16(s, 0x0800, -0x0800, 0x2000); s_v16(s, 0x0400, 0x0800, 0);
  s_end(s);
  vct_emit(s, "p14_scene_far_plane");
}

VCT_TEST(p14_scene_viewport) {
  VCoreState *s = scene_state();
  gx(s, VC_OP_VIEWPORT, 32u | 16u << 8 | 191u << 16 | 143u << 24, 0, 1); /* x 32..191, y 16..143 from the bottom */
  s_poly(s, ATTR_BOTH, 0x7FFF, 0);
  s_v16(s, -0x1000, -0x1000, 0); s_v16(s, 0x0800, -0x1000, 0); s_v16(s, 0, 0x0C00, 0);
  s_end(s);
  vct_emit(s, "p14_scene_viewport");
}

VCT_TEST(p14_scene_vtx_formats) {
  VCoreState *s = scene_state();
  s_poly(s, ATTR_BOTH, 0x001F, 0);
  gx(s, VC_OP_VTX_10, 0x20u | 0x3F0u << 10, 0, 1);          /* (0.5, -0.25, 0): 6-bit fractions */
  gx(s, VC_OP_VTX_XY, (0x800u << 16) | 0x1000u, 0, 1);      /* (1.0, 0.5, z kept) */
  gx(s, VC_OP_VTX_XZ, 0x0800u, 0, 1);                       /* x = 0.5, z = 0, y kept (0.5) */
  s_poly(s, ATTR_BOTH, 0x03E0, 0);
  s_v16(s, -0x1000, -0x1000, 0);
  gx(s, VC_OP_VTX_DIFF, 0x1FFu, 0, 1);
  gx(s, VC_OP_VTX_YZ, 0x0C00u, 0, 1);
  s_end(s);
  vct_emit(s, "p14_scene_vtx_formats");
}

/* the vertices a clip makes lie exactly on their plane (oracle: a quad clipped to the screen edge must reach x = w) */
VCT_TEST(p14_clip_vertices_exactly_on_plane) {
  VC_Vertex in[3], out[10];
  memset(in, 0, sizeof in);
  s32 xs[3] = {0, 8186, 3};
  s32 ws[3] = {6711, 8186, 7453};
  for (int i = 0; i < 3; i++) {
    in[i].clip[0] = i == 1 ? 17284 : xs[i];
    in[i].clip[1] = i * 100;
    in[i].clip[3] = ws[i];
  }
  int n = vc_p14_clip(in, 3, 1, out);
  VCT_CHECK(n >= 3);
  for (int i = 0; i < n; i++) {
    VCT_CHECK(out[i].clip[0] <= out[i].clip[3] && out[i].clip[0] >= -out[i].clip[3]);
    if (out[i].clipped) VCT_EQ(out[i].clip[0], out[i].clip[3]);
  }
}

/* A quad that is not planar in clip space is clipped to the near and far planes first: the corner it ends with at
 * (x, y) = (1, 1) has z -0.698 (the other order, x and y first, gives -0.758; oracle). */
VCT_TEST(p14_clip_order_depth_planes_first) {
  VCoreState *s = fresh();
  begin(s, ATTR_BOTH | 1 << 12, 1);
  v16(s, -3 * ONE, -2 * ONE, -819);
  v16(s, 3 * ONE, -2 * ONE, 3277);
  v16(s, 3 * ONE, 2 * ONE, -9011);
  v16(s, -3 * ONE, 2 * ONE, 3686);
  VCT_EQ(s->geo.npoly, 1);
  VCT_EQ(s->geo.poly[0].nv, 4);
  int found = 0;
  for (int i = 0; i < 4; i++) {
    const VC_Vertex *v = &s->geo.vtx[s->geo.poly[0].v[i]];
    if (v->clip[0] == v->clip[3] && v->clip[1] == v->clip[3]) {
      found = 1;
      VCT_CHECK(v->clip[2] < -2859 + 12 && v->clip[2] > -2859 - 12);
    }
  }
  VCT_CHECK(found);
}
