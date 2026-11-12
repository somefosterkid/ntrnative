/* port/vcore piece 19's unit tests: clear image, edge marking, fog, toon / highlight, antialiasing (docs/vcore.md,
 * piece 19). OWNED BY PIECE 19. Expected values are worked by hand from GBATEK 51 (toon, edge, fog, AA), 54 (rear
 * plane) and 50 (toon / highlight blending); the 3D colour widening is X > 0 -> 2X + 1 (GBATEK 50).
 * Run: build/vcore/vcore_tests p19 */
#include "vct.h"

#define PX(r, g, b, a) (VC_RGB6(r, g, b) | ((u32)(a) << 24))

static void fill_bg(VCoreState *s, u32 color, u32 depth, u8 id) {
  for (int y = 0; y < VC_H; y++)
    for (int x = 0; x < VC_W; x++) {
      s->f3d.color[y][x] = color;
      s->f3d.depth[y][x] = depth;
      s->f3d.opaque_id[y][x] = id;
      s->f3d.trans_id[y][x] = 0xFF;
      s->f3d.flags[y][x] = 0;
    }
}

/* CLEAR_COLOR: r 31, g 10, b 0, fog bit, alpha 20, ID 5: 5 -> 6 bit: 31 -> 63, 10 -> 21, 0 -> 0 */
VCT_TEST(p19_clear_color_fields) {
  VCoreState *s = vct_state();
  vct_io32(s, VC_REG_CLEAR_COLOR, 0x1F | (10 << 5) | 0x8000 | (20 << 16) | (5u << 24));
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0x1000);
  vc_3d_clear(s);
  VCT_EQ(s->f3d.color[0][0], PX(63, 21, 0, 20));
  VCT_EQ(s->f3d.color[191][255], PX(63, 21, 0, 20));
  VCT_EQ(s->f3d.opaque_id[100][100], 5);
  VCT_EQ(s->f3d.flags[100][100], VC_F3_FOG);
  VCT_EQ(s->f3d.trans_id[3][3], 0xFF);
  VCT_EQ(s->f3d.out[0][0], PX(63, 21, 0, 20));
}

/* CLEAR_DEPTH X -> X * 0x200 + ((X + 1) / 0x8000) * 0x1FF: 0x1000 -> 0x200000, 0x7FFF -> 0xFFFE00 + 0x1FF, 0 -> 0 */
VCT_TEST(p19_clear_depth_expand) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0x1000);
  vc_3d_clear(s);
  VCT_EQ(s->f3d.depth[7][9], 0x200000);
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0x7FFF);
  vc_3d_clear(s);
  VCT_EQ(s->f3d.depth[7][9], 0xFFFFFF);
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0);
  vc_3d_clear(s);
  VCT_EQ(s->f3d.depth[7][9], 0);
  VCT_EQ(s->f3d.flags[7][9], 0); /* CLEAR_COLOR bit 15 clear: no fog */
}

/* the rear-plane bitmap: colour in texture slot 2 (alpha = bit 15), depth + fog in slot 3, scrolled by
 * CLRIMAGE_OFFSET (X bits 0-7, Y bits 8-15) and wrapping at 256 */
VCT_TEST(p19_clear_bitmap_offset_wrap) {
  VCoreState *s = vct_state();
  static u8 vram[0x80000];
  memset(vram, 0, sizeof vram);
  vct_map_flat(s, VC_RGN_TEX, vram, sizeof vram);
  /* bitmap pixel (2, 1): colour 0x8000 | b 31 (BGR555 0x7C00), depth 0x1000 with fog */
  u32 a = (1 * 256 + 2) * 2;
  vram[0x40000 + a] = 0x00; vram[0x40000 + a + 1] = 0xFC; /* 0xFC00 */
  vram[0x60000 + a] = 0x00; vram[0x60000 + a + 1] = 0x90; /* 0x9000: fog, depth 0x1000 */
  /* bitmap pixel (0, 0): colour 0x001F without the alpha bit: transparent red */
  vram[0x40000] = 0x1F; vram[0x40001] = 0x00;
  vct_io32(s, VC_REG_CLEAR_COLOR, 7u << 24);
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 14);
  vct_io16(s, VC_REG_CLRIMAGE_OFFSET, (1 << 8) | 2); /* x 2, y 1: screen (0, 0) shows bitmap (2, 1) */
  vc_3d_clear(s);
  VCT_EQ(s->f3d.color[0][0], PX(0, 0, 63, 31));
  VCT_EQ(s->f3d.depth[0][0], 0x200000);
  VCT_EQ(s->f3d.flags[0][0], VC_F3_FOG);
  VCT_EQ(s->f3d.opaque_id[0][0], 7); /* the ID stays in CLEAR_COLOR */

  VCT_EQ(s->f3d.color[0][254] >> 24, 0); /* bitmap (0, 1) is empty: alpha 0 */
  vct_io16(s, VC_REG_CLRIMAGE_OFFSET, (255 << 8) | 255); /* screen (1, 1) -> bitmap (0, 0), wrapped */
  vc_3d_clear(s);
  VCT_EQ(s->f3d.color[1][1], PX(63, 0, 0, 0)); /* colour kept, alpha bit 15 clear -> alpha 0 */
}

/* toon: red 17 (6 bit, = 5 bit 8) indexes TOON_TABLE[8] = b 31, g 5, r 3 -> (Rs, Gs, Bs) = (7, 11, 63).
 * With a white texel R = (64 * 8 - 1) / 64 = 7, G = (64 * 12 - 1) / 64 = 11, B = (64 * 64 - 1) / 64 = 63. */
VCT_TEST(p19_toon_white) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_TOON_TABLE + 8 * 2, 0x7C00 | (5 << 5) | 3);
  vct_io16(s, VC_REG_DISP3DCNT, 0); /* bit 1 clear: toon */
  VC_Poly p;
  memset(&p, 0, sizeof p);
  u32 v = vc_toon_blend(s, &p, PX(17, 40, 50, 31), PX(63, 63, 63, 31));
  VCT_EQ(v, PX(7, 11, 63, 31));
  /* a texel of 31, 31, 31: R = (32 * 8 - 1) / 64 = 3, G = (32 * 12 - 1) / 64 = 5, B = (32 * 64 - 1) / 64 = 31;
   * alpha texel 15 (6 bit 31) with vertex alpha 31 (63): (32 * 64 - 1) / 64 = 31 -> 5 bit 15 */
  v = vc_toon_blend(s, &p, PX(17, 0, 0, 31), PX(31, 31, 31, 15));
  VCT_EQ(v, PX(3, 5, 31, 15));
}

/* highlight (oracle; GBATEK differs): R = ((Rt + 1) * (Rv + 1) - 1) / 64 + Rs clamped to 63 for all three channels,
 * Rv = the vertex red (6 bit) and Rs the table colour (X * 2 + 1). Vertex red 17 (5 bit 8) -> TOON_TABLE[8] =
 * (7, 11, 63); a white texel: 17 + 7 = 24, 17 + 11 = 28, 17 + 63 clamped to 63 */
VCT_TEST(p19_highlight) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_TOON_TABLE + 8 * 2, 0x7C00 | (5 << 5) | 3);
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 1);
  VC_Poly p;
  memset(&p, 0, sizeof p);
  u32 v = vc_toon_blend(s, &p, PX(17, 0, 0, 31), PX(63, 63, 63, 31));
  VCT_EQ(v, PX(24, 28, 63, 31));
  /* a half texel (31): (32 * 18 - 1) / 64 = 8, + 7 = 15, + 11 = 19 */
  v = vc_toon_blend(s, &p, PX(17, 0, 0, 31), PX(31, 31, 31, 31));
  VCT_EQ(v, PX(15, 19, 63, 31));
  /* table index 0 (red 0): colour 0 -> Rs = Gs = Bs = 0, Rv = 0: (63 + 1 - 1) / 64 = 0 */
  v = vc_toon_blend(s, &p, PX(0, 0, 0, 31), PX(63, 63, 63, 31));
  VCT_EQ(v, PX(0, 0, 0, 31));
}

/* edge marking: ID 1 at depth 100 beside ID 2 at depth 200 is marked with EDGE_COLOR[0]; with the neighbour nearer
 * (depth 50) it isn't; ID 8 uses EDGE_COLOR[1]; the screen border compares with the rear plane's ID (0) */
VCT_TEST(p19_edge_marking) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 5);
  vct_io16(s, VC_REG_EDGE_COLOR + 0, 0x001F); /* red */
  vct_io16(s, VC_REG_EDGE_COLOR + 2, 0x03E0); /* green */
  vct_io32(s, VC_REG_CLEAR_COLOR, 0);
  fill_bg(s, PX(20, 20, 20, 31), 0xFFFFFF, 1);
  /* (10,10) ID 1 depth 100, right neighbour (11,10) ID 2 depth 200 */
  s->f3d.opaque_id[10][11] = 2; s->f3d.depth[10][11] = 200;
  s->f3d.depth[10][10] = 100; s->f3d.flags[10][10] = VC_F3_EDGE;
  /* (20,10) ID 1 depth 300, right neighbour ID 2 depth 200 (nearer): not marked */
  s->f3d.opaque_id[10][21] = 2; s->f3d.depth[10][21] = 200;
  s->f3d.depth[10][20] = 300; s->f3d.flags[10][20] = VC_F3_EDGE;
  /* (30,30) ID 8 depth 1, neighbours ID 1 (background) farther */
  s->f3d.opaque_id[30][30] = 8; s->f3d.depth[30][30] = 1; s->f3d.flags[30][30] = VC_F3_EDGE;
  /* (40,40) edge flag but no neighbour with another ID */
  s->f3d.flags[40][40] = VC_F3_EDGE; s->f3d.depth[40][40] = 1;
  /* (0,50) ID 1 on the left border: the rear plane's ID is 0, so it is marked */
  s->f3d.flags[50][0] = VC_F3_EDGE; s->f3d.depth[50][0] = 1;
  /* (10,10)'s neighbours above/left/below are ID 1: same, so only the right one counts */
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[10][10], PX(63, 0, 0, 31));
  VCT_EQ(s->f3d.out[10][20], PX(20, 20, 20, 31));
  VCT_EQ(s->f3d.out[30][30], PX(0, 63, 0, 31));
  VCT_EQ(s->f3d.out[40][40], PX(20, 20, 20, 31));
  VCT_EQ(s->f3d.out[50][0], PX(63, 0, 0, 31));
  VCT_EQ(s->f3d.out[10][11], PX(20, 20, 20, 31)); /* the neighbour has no edge flag */
}

/* fog with FOG_TABLE[0..1] = 0, [2..31] = 64, offset 0, shift 0 (step 0x400; boundary n at 0x400 * (n + 1)):
 * depth 0x800 = boundary 1 -> density 0; 0xC00 = boundary 2 -> 64; 0xA00, half way -> 32. White pixel, fog
 * colour black with fog alpha 0: colour = 63 * (128 - D) / 128, alpha = 31 * (128 - D) / 128 */
VCT_TEST(p19_fog_table_interpolation) {
  VCoreState *s = vct_state();
  for (int i = 0; i < 32; i++) vct_io8(s, VC_REG_FOG_TABLE + i, i < 2 ? 0 : 64);
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 7);
  vct_io32(s, VC_REG_FOG_COLOR, 0);
  fill_bg(s, PX(63, 63, 63, 31), 0, 1);
  u32 depths[4] = {0x800, 0xC00, 0xA00, 0x100};
  for (int i = 0; i < 4; i++) {
    s->f3d.depth[0][i] = depths[i] << 9;
    s->f3d.flags[0][i] = VC_F3_FOG;
  }
  s->f3d.depth[0][4] = 0xC00 << 9; /* no fog flag */
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[0][0], PX(63, 63, 63, 31));  /* D = 0 */
  VCT_EQ(s->f3d.out[0][1], PX(31, 31, 31, 15));  /* D = 64: 63 * 64 / 128 = 31.5, 31 * 64 / 128 = 15.5 */
  VCT_EQ(s->f3d.out[0][2], PX(47, 47, 47, 23));  /* D = 32: 63 * 96 / 128 = 47.25, 31 * 96 / 128 = 23.25 */
  VCT_EQ(s->f3d.out[0][3], PX(63, 63, 63, 31));  /* before boundary 0: density[0] = 0 */
  VCT_EQ(s->f3d.out[0][4], PX(63, 63, 63, 31));
}

/* the fog shift: FOG_STEP = 0x400 >> 2 = 0x100: boundary 2 at 0x300; density 127 counts as 128 (full fog) */
VCT_TEST(p19_fog_shift_and_full_density) {
  VCoreState *s = vct_state();
  for (int i = 0; i < 32; i++) vct_io8(s, VC_REG_FOG_TABLE + i, i < 2 ? 0 : 127);
  vct_io16(s, VC_REG_DISP3DCNT, (1 << 7) | (2 << 8));
  vct_io32(s, VC_REG_FOG_COLOR, 0x7FFF | (31 << 16)); /* white, alpha 31 */
  fill_bg(s, PX(0, 0, 0, 0), 0, 1);
  s->f3d.depth[0][0] = 0x300 << 9; s->f3d.flags[0][0] = VC_F3_FOG;
  s->f3d.depth[0][1] = 0x7FFF << 9; s->f3d.flags[0][1] = VC_F3_FOG;
  s->f3d.depth[0][2] = 0x200 << 9; s->f3d.flags[0][2] = VC_F3_FOG; /* boundary 1: density 0 */
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[0][0], PX(63, 63, 63, 31)); /* D = 127 -> 128 at boundary 2 */
  VCT_EQ(s->f3d.out[0][1], PX(63, 63, 63, 31)); /* beyond boundary 31: density[31] */
  VCT_EQ(s->f3d.out[0][2], PX(0, 0, 0, 0));
}

/* DISP3DCNT bit 6: only the alpha is fogged, the colour is kept */
VCT_TEST(p19_fog_alpha_only) {
  VCoreState *s = vct_state();
  for (int i = 0; i < 32; i++) vct_io8(s, VC_REG_FOG_TABLE + i, 64);
  vct_io16(s, VC_REG_DISP3DCNT, (1 << 7) | (1 << 6));
  vct_io32(s, VC_REG_FOG_COLOR, 0x7FFF); /* white, alpha 0 */
  fill_bg(s, PX(10, 20, 30, 31), 0x7FFFFF, 1);
  s->f3d.flags[5][5] = VC_F3_FOG;
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[5][5], PX(10, 20, 30, 15)); /* 31 * 64 / 128 = 15.5 */
  VCT_EQ(s->f3d.out[5][6], PX(10, 20, 30, 31));
}

/* antialiasing: an edge pixel with coverage 20 over a red neighbour that is farther: a = 20,
 * R = (21 * 63 + 11 * 63) / 32 = 63, G = (21 * 63 + 11 * 0) / 32 = 41.3 -> 41, alpha max(20, 31) = 31 */
VCT_TEST(p19_antialias_coverage_blend) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_DISP3DCNT, 1 << 4);
  fill_bg(s, PX(0, 0, 0, 0), 0, 1);
  s->f3d.color[5][5] = PX(63, 63, 63, 31);
  s->f3d.depth[5][5] = 100;
  s->f3d.flags[5][5] = VC_F3_EDGE;
  s->f3d.coverage[5][5] = 20;
  s->f3d.color[5][6] = PX(63, 0, 0, 31);
  s->f3d.depth[5][6] = 200;
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[5][5], PX(63, 41, 41, 31));
  /* an edge pixel with nothing behind it (alpha 0): the colour stays, the alpha becomes the coverage */
  s->f3d.color[5][6] = 0;
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[5][5], PX(63, 63, 63, 20));
  /* not an edge pixel: untouched */
  VCT_EQ(s->f3d.out[7][7], 0);
}

/* edge marking + antialiasing: the edge colour's alpha is 16: R = (17 * 63 + 15 * 0) / 32 = 33, G = 15 * 63 / 32 = 29
 * over a green neighbour (the neighbour is farther and has another ID) */
VCT_TEST(p19_edge_with_antialias) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_DISP3DCNT, (1 << 4) | (1 << 5));
  vct_io16(s, VC_REG_EDGE_COLOR, 0x001F);
  fill_bg(s, PX(0, 0, 0, 0), 0, 0);
  s->f3d.color[5][5] = PX(63, 63, 63, 31);
  s->f3d.opaque_id[5][5] = 1;
  s->f3d.depth[5][5] = 100;
  s->f3d.flags[5][5] = VC_F3_EDGE;
  s->f3d.coverage[5][5] = 31;
  s->f3d.color[5][6] = PX(0, 63, 0, 31);
  s->f3d.depth[5][6] = 200;
  vc_3d_post(s);
  VCT_EQ(s->f3d.out[5][5], PX(33, 29, 0, 31));
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR): engine A BG0 = 3D, one white triangle or the bare rear plane ---- */
static VCoreState *scene_base(u16 d3, u32 clear) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  vct_eng32(s, VC_ENG_A, VC_REG_DISPCNT, 0x00010108);
  vct_eng32(s, VC_ENG_B, VC_REG_DISPCNT, 0x00010000);
  vct_io16(s, VC_REG_DISP3DCNT, d3);
  vct_io32(s, VC_REG_CLEAR_COLOR, clear);
  vct_io16(s, VC_REG_CLEAR_DEPTH, 0x4000);
  return s;
}
static void scene_tri(VCoreState *s, u32 pid) {
  u32 one = 0x1000, ident[16] = {one, 0, 0, 0, 0, one, 0, 0, 0, 0, one, 0, 0, 0, 0, one};
  u32 p;
  p = 0; vct_gx(s, VC_OP_MTX_MODE, 1, &p);
  vct_gx(s, VC_OP_MTX_LOAD_4x4, 16, ident);
  p = 2; vct_gx(s, VC_OP_MTX_MODE, 1, &p);
  vct_gx(s, VC_OP_MTX_IDENTITY, 0, NULL);
  p = 0xBFFF0000u; vct_gx(s, VC_OP_VIEWPORT, 1, &p);
  p = 0x001F00C0u | (pid << 24); vct_gx(s, VC_OP_POLYGON_ATTR, 1, &p);
  p = 0; vct_gx(s, VC_OP_TEXIMAGE_PARAM, 1, &p);
  p = 0x7FFF; vct_gx(s, VC_OP_COLOR, 1, &p);
  p = 0; vct_gx(s, VC_OP_BEGIN_VTXS, 1, &p);
  u32 v[3][2] = {{0xF800F800u, 0}, {0xF8000800u, 0}, {0x08000000u, 0}};
  vct_gx(s, VC_OP_VTX_16, 2, v[0]);
  vct_gx(s, VC_OP_VTX_16, 2, v[1]);
  vct_gx(s, VC_OP_VTX_16, 2, v[2]);
  vct_gx(s, VC_OP_END_VTXS, 0, NULL);
  p = 0; vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &p);
}
VCT_TEST(p19_scene_fog_rearplane) {
  VCoreState *s = scene_base(1 << 7, 0x001F8000 | 0x1F | (31 << 16));
  for (int i = 0; i < 32; i++) vct_io8(s, VC_REG_FOG_TABLE + i, i * 4);
  vct_io32(s, VC_REG_FOG_COLOR, 0x7FFF | (31 << 16));
  u32 p = 0; vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &p);
  vct_emit(s, "p19_fog_rearplane");
}
VCT_TEST(p19_scene_edge) {
  VCoreState *s = scene_base(1 << 5, 0x001F0000);
  vct_io16(s, VC_REG_EDGE_COLOR + 2, 0x001F);
  scene_tri(s, 8);
  vct_emit(s, "p19_edge");
}
VCT_TEST(p19_scene_antialias) {
  VCoreState *s = scene_base(1 << 4, 0x03E0 | (31 << 16));
  scene_tri(s, 1);
  vct_emit(s, "p19_antialias");
}
VCT_TEST(p19_scene_edge_antialias) {
  VCoreState *s = scene_base((1 << 4) | (1 << 5), 0x03E0 | (31 << 16));
  vct_io16(s, VC_REG_EDGE_COLOR, 0x001F);
  scene_tri(s, 1);
  vct_emit(s, "p19_edge_aa");
}
VCT_TEST(p19_scene_fog_triangle) {
  VCoreState *s = scene_base(1 << 7, 0x001F | (31 << 16));
  for (int i = 0; i < 32; i++) vct_io8(s, VC_REG_FOG_TABLE + i, i * 4);
  vct_io32(s, VC_REG_FOG_COLOR, 0x7FFF | (31 << 16));
  scene_tri(s, 1);
  vct_emit(s, "p19_fog_tri");
}
