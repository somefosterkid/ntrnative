/* port/vcore piece 6's unit tests: affine and double-size OBJs (docs/vcore.md, piece 6). OWNED BY PIECE 6.
 * Run: build/vcore/vcore_tests p06. Expected values are worked by hand from GBATEK 15, 16 and 06:
 *   box position bx, by (0 .. box size-1), centre cx = boxw/2, cy = boxh/2, dx = bx - cx, dy = by - cy,
 *   u = ((PA*dx + PB*dy) >> 8) + w/2, v = ((PC*dx + PD*dy) >> 8) + h/2, outside 0..w-1 / 0..h-1 -> transparent.
 * The scenes at the end (VCT_EMIT=DIR) are one case each for the oracle. */
#include "vct.h"

#include <math.h>

/* write parameter group g (PA..PD, 8.8) into engine e's OAM: the fourth halfword of entries 4g..4g+3 */
static void put_group(VCoreState *s, int e, int g, int pa, int pb, int pc, int pd) {
  int p[4] = {pa, pb, pc, pd};
  for (int k = 0; k < 4; k++) {
    int ofs = e * 0x400 + (g * 4 + k) * 8 + 6;
    s->oam[ofs] = p[k] & 0xFF;
    s->oam[ofs + 1] = (p[k] >> 8) & 0xFF;
  }
}

/* an affine OBJ in OAM slot i with the raw attributes, and the decoded struct a caller of piece 5 would hold */
static VC_ObjAttr put_obj(VCoreState *s, int e, int i, int x, int y, int w, int h, int dbl, int group, int mode,
                          int tile, int bpp8, int pal) {
  VC_ObjAttr o;
  memset(&o, 0, sizeof o);
  int shape = w == h ? 0 : (w > h ? 1 : 2);
  int a = 0;
  static const int sq[4] = {8, 16, 32, 64};
  static const int hz[4][2] = {{16, 8}, {32, 8}, {32, 16}, {64, 32}};
  for (int z = 0; z < 4; z++) {
    if (shape == 0 && sq[z] == w) a = z;
    if (shape == 1 && hz[z][0] == w && hz[z][1] == h) a = z;
    if (shape == 2 && hz[z][1] == w && hz[z][0] == h) a = z;
  }
  o.attr[0] = (y & 0xFF) | 0x100 | (dbl ? 0x200 : 0) | (mode << 10) | (bpp8 ? 0x2000 : 0) | (shape << 14);
  o.attr[1] = (x & 0x1FF) | (group << 9) | (a << 14);
  o.attr[2] = tile | (pal << 12);
  u8 *oam = s->oam + e * 0x400 + i * 8;
  for (int k = 0; k < 3; k++) {
    oam[k * 2] = o.attr[k] & 0xFF;
    oam[k * 2 + 1] = o.attr[k] >> 8;
  }
  o.index = i;
  o.x = x;
  o.y = y;
  o.w = w;
  o.h = h;
  o.affine = 1;
  o.dbl = dbl;
  o.mode = mode;
  o.bpp8 = bpp8;
  o.tile = tile;
  o.palette = pal;
  o.affine_param = group;
  return o;
}

#define ID 0x100
#define PUT(s, e, i, x, y, w, h, dbl, g) put_obj(s, e, i, x, y, w, h, dbl, g, 0, 0, 0, 0)

VCT_TEST(p06_identity_centre) {
  /* 16x16, identity: (bx, by) -> (bx, by) */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0, 0, ID);
  VC_ObjAttr o = PUT(s, 0, 0, 10, 20, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 13, 25, &u, &v), 1);
  VCT_EQ(u, 3);
  VCT_EQ(v, 5);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 10, 20, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 25, 35, &u, &v), 1);
  VCT_EQ(u, 15);
  VCT_EQ(v, 15);
  /* outside the box */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 26, 30, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 9, 30, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 15, 36, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 15, 19, &u, &v), 0);
}

VCT_TEST(p06_scale_2x_magnify) {
  /* PA = PD = 0x80 (0.5 texel per pixel): bx=0 -> dx=-8 -> -4 -> u 4; bx=3 -> dx=-5 -> -640>>8 = -3 -> u 5;
   * bx=8 -> u 8; by=15 -> dy=7 -> 896>>8 = 3 -> v 11 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, 0x80, 0, 0, 0x80);
  VC_ObjAttr o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 0, 0, &u, &v), 1);
  VCT_EQ(u, 4);
  VCT_EQ(v, 4);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 3, 15, &u, &v), 1);
  VCT_EQ(u, 5);
  VCT_EQ(v, 11);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 8, 8, &u, &v), 1);
  VCT_EQ(u, 8);
  VCT_EQ(v, 8);
}

VCT_TEST(p06_scale_half_minify) {
  /* PA = PD = 0x200: bx=4 -> dx=-4 -> -8 -> u 0; bx=11 -> dx=3 -> 6 -> u 14; bx=3 -> -10+8 = -2 outside;
   * bx=12 -> 8+8 = 16 outside */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, 0x200, 0, 0, 0x200);
  VC_ObjAttr o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 4, 4, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 11, 11, &u, &v), 1);
  VCT_EQ(u, 14);
  VCT_EQ(v, 14);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 3, 8, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 12, 8, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 8, 12, &u, &v), 0);
}

VCT_TEST(p06_rotate_90) {
  /* PA=0, PB=0x100, PC=-0x100, PD=0: u = dy + 8, v = -dx + 8. box (2,5): dx=-6, dy=-3 -> u 5, v 14 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, 0, 0x100, -0x100, 0);
  VC_ObjAttr o = PUT(s, 0, 0, 100, 50, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 102, 55, &u, &v), 1);
  VCT_EQ(u, 5);
  VCT_EQ(v, 14);
  /* box (0,0): dx=dy=-8 -> u = 0, v = 8+8 = 16 -> outside */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 100, 50, &u, &v), 0);
  /* box (1,0): dx=-7, dy=-8 -> u 0, v 15 */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 101, 50, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 15);
}

VCT_TEST(p06_rotate_negative_params) {
  /* a 180 degree turn: PA = PD = -0x100: u = -dx + 8, v = -dy + 8. box (3,4): dx=-5, dy=-4 -> u 13, v 12 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, -0x100, 0, 0, -0x100);
  VC_ObjAttr o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 3, 4, &u, &v), 1);
  VCT_EQ(u, 13);
  VCT_EQ(v, 12);
  /* box (0,0): u = 8+8 = 16 outside */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 0, 0, &u, &v), 0);
}

VCT_TEST(p06_shear_fraction) {
  /* PB = 0x80: v = dy + 8, u = dx + dy/2 + 8 (floor). box (4,7): dx=-4, dy=-1: -1152>>8 = -5 -> u 3, v 7;
   * box (4,9): dy=1: -896>>8 = -4 -> u 4, v 9 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0x80, 0, ID);
  VC_ObjAttr o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 4, 7, &u, &v), 1);
  VCT_EQ(u, 3);
  VCT_EQ(v, 7);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 4, 9, &u, &v), 1);
  VCT_EQ(u, 4);
  VCT_EQ(v, 9);
}

VCT_TEST(p06_double_size) {
  /* 16x16 double size, identity: the box is 32x32 with the centre at 16; the texture is still 16x16, in the middle:
   * box x 8..23 shows u 0..15; box 7 and 24 are outside; box (8,8) -> (0,0) */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0, 0, ID);
  VC_ObjAttr o = PUT(s, 0, 0, 40, 30, 16, 16, 1, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 48, 38, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 63, 53, &u, &v), 1);
  VCT_EQ(u, 15);
  VCT_EQ(v, 15);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 47, 40, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 40, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 50, 29, &u, &v), 0); /* above the box */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 50, 62, &u, &v), 0); /* below the 32-line box (30..61) */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 50, 61, &u, &v), 0); /* box row 31 -> dy=15 -> v 23, outside the texture */
  /* with PA=PD=0x200 (2 texels per pixel) the double box shows the whole texture, minified: box (12,12): dx=-4 ->
   * -8 -> u 0; box (19,19): dx=3 -> 6 -> u 14 */
  put_group(s, 0, 0, 0x200, 0, 0, 0x200);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 52, 42, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 59, 49, &u, &v), 1);
  VCT_EQ(u, 14);
  VCT_EQ(v, 14);
}

VCT_TEST(p06_double_size_rotation_keeps_corners) {
  /* at 45 degrees (PA=PB=PD=0xB5, PC=-0xB5, 0.707) a texture corner lands outside the normal box but inside the
   * double one. Box (16,6) of the double box: dx=0, dy=-10: u = (181*-10 >> 8 = -1810>>8 = -8) + 8 = 0,
   * v = (181*-10 >> 8 = -8) + 8 = 0 -> texel (0,0). Box (16,1): dy=-15: -2715>>8 = -11 -> u -3: outside. In a
   * normal 16x16 box the first pixel would lie at box row -9: not part of the OBJ */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, 0xB5, 0xB5, -0xB5, 0xB5);
  VC_ObjAttr o = PUT(s, 0, 0, 0, 0, 16, 16, 1, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 16, 6, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 16, 1, &u, &v), 0);
  VC_ObjAttr n = PUT(s, 0, 1, 0, 0, 16, 16, 0, 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &n, 8, -2, &u, &v), 0);
}

VCT_TEST(p06_non_square) {
  /* 8x32, PA=0, PB=0x100, PC=-0x100, PD=0, box 8x32, centre (4, 16): u = dy + 4, v = -dx + 16.
   * box (0,12): dx=-4, dy=-4 -> u 0, v 20. box (7,19): dx=3, dy=3 -> u 7, v 13. box (0,11): u -1 outside */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, 0, 0x100, -0x100, 0);
  VC_ObjAttr o = PUT(s, 0, 0, 20, 10, 8, 32, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 20, 22, &u, &v), 1);
  VCT_EQ(u, 0);
  VCT_EQ(v, 20);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 27, 29, &u, &v), 1);
  VCT_EQ(u, 7);
  VCT_EQ(v, 13);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 20, 21, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 28, 29, &u, &v), 0); /* x beyond the 8-wide box */
}

VCT_TEST(p06_parameter_group_select) {
  /* attr 1 bits 9-13 pick the group; group g's PA..PD sit at OAM offsets (4g+k)*8+6 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0, 0, ID);
  put_group(s, 0, 3, 0x200, 0, 0, 0x200);
  put_group(s, 0, 31, 0x80, 0, 0, 0x80);
  /* group 3: PA at byte 3*32+6 = 102, PD at 3*32+30 = 126 */
  VCT_EQ(s->oam[102], 0x00);
  VCT_EQ(s->oam[103], 0x02);
  VCT_EQ(s->oam[126], 0x00);
  VCT_EQ(s->oam[127], 0x02);
  int u, v;
  VC_ObjAttr o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 3);
  VCT_EQ(o.attr[1] >> 9 & 31, 3);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 11, 11, &u, &v), 1); /* 2 texels per pixel: dx=3 -> 6 -> u 14 */
  VCT_EQ(u, 14);
  o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 31);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 15, 15, &u, &v), 1); /* dx=7 -> 896>>8 = 3 -> u 11 */
  VCT_EQ(u, 11);
  /* group 24 is all zero: every pixel shows texel (8,8); bits 12-13 of attr 1 are parameter bits, not flips, and
   * the decoded flip fields are ignored */
  o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 24);
  o.hflip = o.vflip = 1;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 5, 5, &u, &v), 1);
  VCT_EQ(u, 8);
  VCT_EQ(v, 8);
  o = PUT(s, 0, 0, 0, 0, 16, 16, 0, 0);
  o.hflip = o.vflip = 1;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 5, 6, &u, &v), 1);
  VCT_EQ(u, 5);
  VCT_EQ(v, 6);
}

VCT_TEST(p06_engine_b_oam) {
  /* engine B's parameters are in its own OAM half (+0x400) */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0, 0, ID);
  put_group(s, 1, 0, 0x200, 0, 0, 0x200);
  VC_ObjAttr o = PUT(s, 1, 0, 0, 0, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 1, &o, 11, 4, &u, &v), 1); /* dx=3 -> u 14; dy=-4 -> -8 -> v 0 */
  VCT_EQ(u, 14);
  VCT_EQ(v, 0);
}

VCT_TEST(p06_position_wrap) {
  /* x is 9 bits signed: field 0x1F8 = -8 puts box x 8 at screen 0; y 8 bits: y 250 + box row 8 = line 2 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0, 0, ID);
  VC_ObjAttr o = PUT(s, 0, 0, 0x1F8, 250, 16, 16, 0, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 0, 2, &u, &v), 1);
  VCT_EQ(u, 8);
  VCT_EQ(v, 8);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 7, 0, &u, &v), 1);
  VCT_EQ(u, 15);
  VCT_EQ(v, 6);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 8, 0, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 0, 10, &u, &v), 0);
}

VCT_TEST(p06_double_y_wrap) {
  /* a 64x64 OBJ in a 128-line double box at y 200 covers lines 200..327: 200..255 are off screen, lines 0..71 show
   * the rest of it (GBATEK 15: displayed at the top, not the bottom). Identity: box row = (line - 200) & 255;
   * line 10 -> row 66 -> dy = 66-64 = 2 -> v = 34; box column 64 -> dx 0 -> u 32 */
  VCoreState *s = vct_state();
  put_group(s, 0, 0, ID, 0, 0, ID);
  VC_ObjAttr o = PUT(s, 0, 0, 0, 200, 64, 64, 1, 0);
  int u, v;
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 10, &u, &v), 1);
  VCT_EQ(u, 32);
  VCT_EQ(v, 34);
  /* line 0 -> row 56: dy=-8 -> v 24; line 32 -> row 88 -> dy 24 -> v 56; line 40 -> row 96 -> dy 32 -> v 64
   * outside; line 71 -> row 127 -> v 95 outside; lines 72.. are past the box (row 128) */
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 0, &u, &v), 1);
  VCT_EQ(v, 24);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 32, &u, &v), 1);
  VCT_EQ(v, 56);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 40, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 71, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 72, &u, &v), 0);
  VCT_EQ(vc_obj_affine_uv(s, 0, &o, 64, 100, &u, &v), 0);
}

/* ---- the line function, through piece 5's and 7's texel functions and vc_obj_plot ---- */

/* the line function needs pieces 5 and 7; a stub texel function (returns 0 for an opaque texel) means they aren't
 * merged yet: the test then reports TODO instead of passing on nothing */
static int texel_functions_present(VCoreState *s) {
  VC_ObjAttr probe;
  memset(&probe, 0, sizeof probe);
  probe.w = probe.h = 8;
  return vc_obj_tile_texel(s, 0, &probe, 0, 1) != 0;
}

/* 8x8 4bpp tile 0: all texels colour 1 except texel (7,0) = 0 (row 0's byte 3 = 0x01); palette 0 colour 1 = red */
static void setup_tile(VCoreState *s, u8 *vram) {
  vct_io32(s, VC_REG_DISPCNT, 0x00011010); /* mode 1, OBJ on, 1D */
  memset(vram, 0x11, 32);
  vram[3] = 0x01;
  vct_map_flat(s, VC_RGN_AOBJ, vram, 0x4000);
  vct_pal(s, 0, 1, 1, 0x001F);
}

VCT_TEST(p06_line_draws_through_texel_functions) {
  static u8 vram[0x4000];
  VCoreState *s = vct_state();
  memset(vram, 0, sizeof vram);
  setup_tile(s, vram);
  if (!texel_functions_present(s)) VCT_TODO("needs piece 5's vc_obj_tile_texel and vc_obj_plot merged");
  put_group(s, 0, 0, ID, 0, 0, ID);
  VC_ObjAttr o = put_obj(s, 0, 0, 20, 30, 8, 8, 0, 0, 0, 0, 0, 0);
  VC_Line *ln = &s->line;
  memset(ln, 0, sizeof *ln);
  vc_obj_affine_line(s, 0, &o, 30, ln); /* row 0: texels 0-6 drawn, 7 transparent */
  VCT_CHECK(ln->obj[19] == 0);
  for (int x = 20; x <= 26; x++) VCT_CHECK(ln->obj[x] != 0);
  VCT_EQ(ln->obj[27], 0);
  VCT_EQ(ln->obj[28], 0);
  memset(ln, 0, sizeof *ln);
  vc_obj_affine_line(s, 0, &o, 29, ln); /* the line above the OBJ: nothing */
  for (int x = 0; x < VC_W; x++) VCT_EQ(ln->obj[x], 0);
  /* 2x magnified (PA = 0x80): 8 box pixels show the middle texels 2..5 (dx -4..3 -> -2..1 +4): all drawn */
  put_group(s, 0, 0, 0x80, 0, 0, 0x80);
  memset(ln, 0, sizeof *ln);
  vc_obj_affine_line(s, 0, &o, 30, ln); /* box row 0: dy=-4 -> v = (-512>>8 = -2) + 4 = 2: a full row */
  for (int x = 20; x <= 27; x++) VCT_CHECK(ln->obj[x] != 0);
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR): one case each, a patterned 32x32 OBJ (tiles 0-15) ---- */

/* 4bpp 32x32 in 1D mapping: tile t = row*4 + col; texel colour index 1 + ((x/2 + y/2*3 + tile) % 15) */
static void fill_pattern4(u8 *vram) {
  for (int t = 0; t < 16; t++)
    for (int py = 0; py < 8; py++)
      for (int px = 0; px < 8; px++) {
        int X = (t % 4) * 8 + px, Y = (t / 4) * 8 + py;
        int c = 1 + ((X / 2) + (Y / 2) * 3 + X / 8 + Y / 8) % 15;
        u8 *b = vram + t * 32 + py * 4 + px / 2;
        *b |= (px & 1) ? c << 4 : c;
      }
}

/* 8bpp: tile t uses 64 bytes, in 1D mapping tile numbers step by 2 (32-byte units) */
static void fill_pattern8(u8 *vram) {
  for (int t = 0; t < 16; t++)
    for (int py = 0; py < 8; py++)
      for (int px = 0; px < 8; px++) {
        int X = (t % 4) * 8 + px, Y = (t / 4) * 8 + py;
        vram[t * 64 + py * 8 + px] = 1 + ((X * 5 + Y * 7 + X / 8) % 200);
      }
}

static VCoreState *scene_base(void) {
  VCoreState *s = vct_state();
  vct_io32(s, VC_REG_DISPCNT, 0x00011010); /* mode 1, OBJ on, 1D mapping */
  vct_io8(s, VC_REG_VRAMCNT_A, 0x82);      /* bank A: engine A OBJ */
  fill_pattern4(vc_bank(s, VC_BANK_A));
  fill_pattern8(vc_bank(s, VC_BANK_A) + 0x4000);
  for (int p = 0; p < 16; p++)
    for (int c = 1; c < 16; c++)
      vct_pal(s, 0, 1, p * 16 + c, (u16)(((c * 2 + p) & 31) | ((c * 7 + p * 3) & 31) << 5 | ((c * 13 + 4) & 31) << 10));
  for (int c = 1; c < 256; c++)
    vct_pal(s, 0, 1, c, (u16)(((c * 3) & 31) | ((c * 5) & 31) << 5 | ((c * 11) & 31) << 10));
  s->pltt[0] = 0x10; /* backdrop */
  s->pltt[1] = 0x20;
  return s;
}

static int fx(double v) { return (int)lround(v * 256.0); }

static void rot_group(VCoreState *s, int g, double deg, double sx, double sy) {
  double a = deg * 3.14159265358979 / 180.0;
  put_group(s, 0, g, fx(cos(a) / sx), fx(sin(a) / sx), fx(-sin(a) / sy), fx(cos(a) / sy));
}

VCT_TEST(p06_scene_identity_double_and_rotations) {
  VCoreState *s = scene_base();
  rot_group(s, 0, 0, 1, 1);
  rot_group(s, 1, 30, 1, 1);
  rot_group(s, 2, 90, 1, 1);
  rot_group(s, 3, 45, 1, 1);
  put_obj(s, 0, 0, 8, 8, 32, 32, 0, 0, 0, 0, 0, 0);
  put_obj(s, 0, 1, 56, 8, 32, 32, 0, 1, 0, 0, 0, 0);   /* rotated, clipped by the box */
  put_obj(s, 0, 2, 120, 4, 32, 32, 1, 1, 0, 0, 0, 0);  /* rotated, double: corners kept */
  put_obj(s, 0, 3, 8, 80, 32, 32, 0, 2, 0, 0, 0, 3);
  put_obj(s, 0, 4, 60, 76, 32, 32, 1, 3, 0, 0, 0, 5);
  put_obj(s, 0, 5, 140, 80, 32, 32, 1, 0, 0, 0, 0, 7); /* double with identity: the texture in the middle */
  vct_emit(s, "p06_rot_double");
}

VCT_TEST(p06_scene_scaling_and_shear) {
  VCoreState *s = scene_base();
  rot_group(s, 0, 0, 2, 2);     /* magnify */
  rot_group(s, 1, 0, 0.5, 0.5); /* minify */
  rot_group(s, 2, 0, 1, 3);     /* tall stretch */
  put_group(s, 0, 3, ID, 0x80, 0, ID); /* shear */
  rot_group(s, 4, 20, 1.5, 0.75);
  put_group(s, 0, 5, -ID, 0, 0, ID);   /* mirrored in x */
  put_obj(s, 0, 0, 8, 8, 32, 32, 0, 0, 0, 0, 0, 1);
  put_obj(s, 0, 1, 56, 8, 32, 32, 0, 1, 0, 0, 0, 2);
  put_obj(s, 0, 2, 104, 8, 32, 32, 0, 2, 0, 0, 0, 3);
  put_obj(s, 0, 3, 152, 8, 32, 32, 0, 3, 0, 0, 0, 4);
  put_obj(s, 0, 4, 8, 80, 32, 32, 1, 4, 0, 0, 0, 6);
  put_obj(s, 0, 5, 80, 80, 32, 32, 0, 5, 0, 0, 0, 7);
  put_obj(s, 0, 6, 150, 90, 32, 32, 1, 1, 0, 0, 0, 9);
  vct_emit(s, "p06_scale_shear");
}

VCT_TEST(p06_scene_shapes_8bpp_edges) {
  VCoreState *s = scene_base();
  rot_group(s, 0, 25, 1, 1);
  rot_group(s, 1, 90, 1, 1);
  rot_group(s, 2, 0, 1, 1);
  put_obj(s, 0, 0, 10, 10, 16, 8, 1, 0, 0, 0, 0, 2);   /* shapes: 16x8 double */
  put_obj(s, 0, 1, 60, 10, 8, 16, 0, 1, 0, 0, 0, 3);   /* 8x16 rotated 90 */
  put_obj(s, 0, 2, 100, 10, 32, 16, 1, 0, 0, 0, 0, 4);
  put_obj(s, 0, 3, 150, 10, 16, 32, 1, 0, 0, 0, 0, 5);
  put_obj(s, 0, 4, 20, 70, 32, 32, 1, 0, 0, 512, 1, 0); /* 8bpp: tile 512 -> byte 0x4000 (1D, 32-byte units) */
  put_obj(s, 0, 5, 0x1F0, 100, 32, 32, 1, 0, 0, 0, 0, 8);  /* x = -16: partly off the left edge */
  put_obj(s, 0, 6, 220, 140, 32, 32, 1, 0, 0, 0, 0, 9);    /* partly off the right edge */
  put_obj(s, 0, 7, 100, 170, 32, 32, 1, 0, 0, 0, 0, 10);   /* partly below the screen */
  put_obj(s, 0, 8, 190, 230, 32, 32, 1, 2, 0, 0, 0, 11);   /* y wraps: box lines 230..293 -> top */
  put_obj(s, 0, 9, 180, 60, 16, 16, 0, 0, 0, 0, 0, 12);
  vct_emit(s, "p06_shapes_edges");
}

VCT_TEST(p06_scene_priority_and_semi) {
  VCoreState *s = scene_base();
  rot_group(s, 0, 15, 1, 1);
  rot_group(s, 1, 0, 1, 1);
  put_obj(s, 0, 0, 40, 40, 32, 32, 1, 0, 0, 0, 0, 1);
  put_obj(s, 0, 1, 60, 50, 32, 32, 1, 0, 0, 0, 0, 2); /* behind OBJ 0 where both draw */
  put_obj(s, 0, 2, 120, 40, 32, 32, 0, 1, 1, 0, 0, 3); /* semi-transparent */
  vct_io16(s, VC_REG_BLDCNT, 0x2000);
  vct_io16(s, VC_REG_BLDALPHA, 0x0808);
  vct_emit(s, "p06_prio_semi");
}
