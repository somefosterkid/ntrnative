/* port/vcore piece 5's unit tests: normal OBJs and the OAM loop (docs/vcore.md, piece 5). OWNED BY PIECE 5.
 * Run: build/vcore/vcore_tests p05. Expected values worked by hand from GBATEK 14, 15, 17, 26, 27. */
#include "vct.h"
#include <string.h>

static u8 vr[0x40000];
static u8 ext[0x4000];

static void oam_set(VCoreState *s, int e, int i, u16 a0, u16 a1, u16 a2) {
  u8 *o = s->oam + e * 0x400 + i * 8;
  o[0] = (u8)a0; o[1] = (u8)(a0 >> 8);
  o[2] = (u8)a1; o[3] = (u8)(a1 >> 8);
  o[4] = (u8)a2; o[5] = (u8)(a2 >> 8);
}
/* a fresh state with OBJ VRAM mapped flat, engine e's DISPCNT = dispcnt (bit 12 added) */
static VCoreState *mk(int e, u32 dispcnt) {
  VCoreState *s = vct_state();
  memset(vr, 0, sizeof vr);
  memset(ext, 0, sizeof ext);
  vct_map_flat(s, e ? VC_RGN_BOBJ : VC_RGN_AOBJ, vr, e ? 0x20000 : 0x40000);
  vct_map_flat(s, e ? VC_RGN_BOBJEXT : VC_RGN_AOBJEXT, ext, 0x2000);
  vct_eng32(s, e, VC_REG_DISPCNT, dispcnt | (1u << 12));
  /* all OBJs hidden (a zero OAM entry is a visible 8x8 OBJ at 0,0) */
  for (int i = 0; i < 128; i++) s->oam[e * 0x400 + i * 8 + 1] = 2;
  return s;
}
static void px4(u32 addr, int x, int y, int idx) { /* 4bpp tile pixel */
  u32 a = addr + (u32)y * 4 + (u32)(x >> 1);
  if (x & 1) vr[a] = (u8)((vr[a] & 0x0F) | idx << 4); else vr[a] = (u8)((vr[a] & 0xF0) | idx);
}
static void px8(u32 addr, int x, int y, int idx) { vr[addr + (u32)y * 8 + (u32)x] = (u8)idx; }
#define OP(c) (VC_PX_OPAQUE | (c))

VCT_TEST(p05_sizes) {
  static const int W[12] = {8, 16, 32, 64, 16, 32, 32, 64, 8, 8, 16, 32};
  static const int H[12] = {8, 16, 32, 64, 8, 8, 16, 32, 16, 32, 32, 64};
  VCoreState *s = mk(0, 0);
  for (int shape = 0; shape < 3; shape++)
    for (int size = 0; size < 4; size++) {
      VC_ObjAttr o;
      oam_set(s, 0, 5, (u16)(shape << 14), (u16)(size << 14), 0);
      vc_obj_decode(s, 0, 5, &o);
      VCT_EQ(o.w, W[shape * 4 + size]);
      VCT_EQ(o.h, H[shape * 4 + size]);
      VCT_EQ(o.index, 5);
    }
}

VCT_TEST(p05_decode_fields) {
  VCoreState *s = mk(0, 0);
  VC_ObjAttr o;
  /* a0: y=0x12, mode 1, mosaic, 8bpp, shape 1; a1: x=0x0AB, hflip, size 2; a2: tile 0x2A5, prio 2, pal 9 */
  oam_set(s, 0, 3, 0x12 | 1 << 10 | 1 << 12 | 1 << 13 | 1 << 14, 0xAB | 1 << 12 | 2 << 14, 0x2A5 | 2 << 10 | 9 << 12);
  vc_obj_decode(s, 0, 3, &o);
  VCT_EQ(o.y, 0x12); VCT_EQ(o.x, 0xAB); VCT_EQ(o.mode, 1); VCT_EQ(o.mosaic, 1); VCT_EQ(o.bpp8, 1);
  VCT_EQ(o.hflip, 1); VCT_EQ(o.vflip, 0); VCT_EQ(o.tile, 0x2A5); VCT_EQ(o.prio, 2); VCT_EQ(o.palette, 9);
  VCT_EQ(o.affine, 0); VCT_EQ(o.hidden, 0); VCT_EQ(o.w, 32); VCT_EQ(o.h, 16);
  oam_set(s, 0, 4, 1 << 8 | 1 << 9, 7 << 9, 0);
  vc_obj_decode(s, 0, 4, &o);
  VCT_EQ(o.affine, 1); VCT_EQ(o.dbl, 1); VCT_EQ(o.hidden, 0); VCT_EQ(o.affine_param, 7);
  oam_set(s, 0, 4, 1 << 9, 0, 0);
  vc_obj_decode(s, 0, 4, &o);
  VCT_EQ(o.hidden, 1);
  VCT_EQ(o.tile, 0);
}

VCT_TEST(p05_x_wrap) {
  VCoreState *s = mk(0, 0);
  VC_ObjAttr o;
  oam_set(s, 0, 0, 0, 0x1FF, 0); /* x = 511 = -1 */
  vc_obj_decode(s, 0, 0, &o);
  VCT_EQ(o.x, -1);
  oam_set(s, 0, 0, 0, 0x100, 0); /* 256 -> -256 */
  vc_obj_decode(s, 0, 0, &o);
  VCT_EQ(o.x, -256);
  oam_set(s, 0, 0, 0, 0xFF, 0);
  vc_obj_decode(s, 0, 0, &o);
  VCT_EQ(o.x, 255);
  /* on screen: an 8x8 OBJ at x=-1 shows its texel columns 1..7 at pixels 0..6 */
  vct_pal(s, 0, 1, 1, 0x001F);
  for (int i = 1; i < 8; i++) px4(0, i, 0, 1); /* texel 0 transparent, 1-7 index 1 */
  oam_set(s, 0, 0, 0, 0x1FF, 0);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(0x001F));
  VCT_EQ(s->line.obj[6], OP(0x001F));
  VCT_EQ(s->line.obj[7], 0); /* texel 8 does not exist */
  /* at x=252 it is cut at the right edge: texels 0 (clear) and 1..3 at pixels 253..255 */
  oam_set(s, 0, 0, 0, 252, 0);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[252], 0);
  VCT_EQ(s->line.obj[253], OP(0x001F));
  VCT_EQ(s->line.obj[255], OP(0x001F));
  VCT_EQ(s->line.obj[0], 0); /* no wrap round to the left edge */
}

VCT_TEST(p05_y_wrap) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 1, 0x03E0);
  /* 16x16 OBJ, 4bpp, 2D, tile 0: every texel index 1 (tiles 0,1 and 32,33) */
  for (int t = 0; t < 4; t++)
    for (int i = 0; i < 32; i++) vr[(t & 1) * 32 + (t >> 1) * 32 * 32 + i] = 0x11;
  oam_set(s, 0, 0, 250, 1 << 14 | 20, 0);            /* y=250, 16x16 (size 1), x=20 */
  vc_obj_line(s, 0, 0, &s->line);                     /* (0-250)&255 = 6 < 16: row 6 */
  VCT_EQ(s->line.obj[20], OP(0x03E0));
  vc_obj_line(s, 0, 9, &s->line);                     /* row 15 */
  VCT_EQ(s->line.obj[20], OP(0x03E0));
  vc_obj_line(s, 0, 10, &s->line);                    /* row 16: past the end */
  VCT_EQ(s->line.obj[20], 0);
  vc_obj_line(s, 0, 191, &s->line);                   /* row (191-250)&255 = 197: no */
  VCT_EQ(s->line.obj[20], 0);
  /* a line before the OBJ starts (y=100 and line 99) is not covered */
  oam_set(s, 0, 0, 100, 1 << 14 | 20, 0);
  vc_obj_line(s, 0, 99, &s->line);
  VCT_EQ(s->line.obj[20], 0);
  vc_obj_line(s, 0, 100, &s->line);
  VCT_EQ(s->line.obj[20], OP(0x03E0));
}

VCT_TEST(p05_y_wrap_tall) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 2, 0x7C00);
  /* 8x32 (shape 2 size 1) at y=240: lines 240..255 show rows 0..15 and lines 0..15 rows 16..31.
   * 2D: tile row ty is at byte ty*32*32; tile row 2 texel (0,0) = index 2. */
  vr[2 * 32 * 32] = 0x02;
  oam_set(s, 0, 0, 240 | 2 << 14, 1 << 14, 0);
  vc_obj_line(s, 0, 0, &s->line); /* (0-240)&255 = 16: tile row 2 */
  VCT_EQ(s->line.obj[0], OP(0x7C00));
  vc_obj_line(s, 0, 1, &s->line); /* row 17: tile row 2, pixel row 1: empty */
  VCT_EQ(s->line.obj[0], 0);
  vc_obj_line(s, 0, 16, &s->line); /* row 32: outside */
  VCT_EQ(s->line.obj[0], 0);
}

VCT_TEST(p05_hidden_and_enable) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 1, 0x001F);
  vr[0] = 0x01;
  oam_set(s, 0, 0, 1 << 9, 0, 0); /* hidden */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], 0);
  oam_set(s, 0, 0, 0, 0, 0);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(0x001F));
  vct_eng32(s, 0, VC_REG_DISPCNT, 0); /* DISPCNT bit 12 off: nothing */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], 0);
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 12 | 1u << 23); /* bit 23 (OBJ in H-blank) changes nothing */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(0x001F));
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 23);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], 0);
}

VCT_TEST(p05_4bpp_palettes) {
  VCoreState *s = mk(0, 0);
  /* tile 1 (bytes 32..63): row 0 = indices 1,2,3,0, ..., 15 at x=7 */
  px4(32, 0, 0, 1); px4(32, 1, 0, 2); px4(32, 2, 0, 3); px4(32, 3, 0, 0); px4(32, 7, 0, 15);
  vct_pal(s, 0, 1, 0x30 + 1, 0x0001);
  vct_pal(s, 0, 1, 0x30 + 2, 0x0002);
  vct_pal(s, 0, 1, 0x30 + 3, 0x0003);
  vct_pal(s, 0, 1, 0x30 + 15, 0x7FFF);
  vct_pal(s, 0, 1, 0x30, 0x1234); /* colour 0 of palette 3: transparent */
  vct_pal(s, 0, 1, 1, 0x5555);    /* palette 0 must not be used */
  oam_set(s, 0, 0, 10, 100, 1 | 3 << 12); /* tile 1, palette 3, y=10, x=100 */
  vc_obj_line(s, 0, 10, &s->line);
  VCT_EQ(s->line.obj[100], OP(1));
  VCT_EQ(s->line.obj[101], OP(2));
  VCT_EQ(s->line.obj[102], OP(3));
  VCT_EQ(s->line.obj[103], 0);
  VCT_EQ(s->line.obj[107], OP(0x7FFF));
  VCT_EQ(s->line.obj[108], 0);
  vc_obj_line(s, 0, 11, &s->line); /* row 1 empty */
  VCT_EQ(s->line.obj[100], 0);
}

VCT_TEST(p05_8bpp) {
  VCoreState *s = mk(0, 0);
  px8(0, 0, 0, 7); px8(0, 3, 2, 200);
  vct_pal(s, 0, 1, 7, 0x0111);
  vct_pal(s, 0, 1, 200, 0x0222);
  vct_pal(s, 0, 1, 0, 0x7FFF);
  oam_set(s, 0, 0, 0 | 1 << 13, 0, 5 << 12); /* palette number ignored in 8bpp */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(0x0111));
  VCT_EQ(s->line.obj[1], 0);
  vc_obj_line(s, 0, 2, &s->line);
  VCT_EQ(s->line.obj[3], OP(0x0222));
}

VCT_TEST(p05_ext_palette) {
  VCoreState *s = mk(0, 1u << 31);
  px8(0, 1, 0, 5);
  vct_pal(s, 0, 1, 5, 0x0001); /* the standard palette must not be used */
  /* extended: palette number 2, index 5 -> halfword 2*256+5 = 517 -> byte 1034 */
  ext[1034] = 0xCD; ext[1035] = 0x2B;
  oam_set(s, 0, 0, 1 << 13, 0, 2 << 12);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[1], OP(0x2BCD));
  /* 4bpp ignores the extended palette: standard palette 2 */
  px4(32, 0, 0, 4);
  vct_pal(s, 0, 1, 2 * 16 + 4, 0x0777);
  oam_set(s, 0, 0, 0, 0, 1 | 2 << 12);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(0x0777));
  /* with DISPCNT bit 31 clear the 8bpp OBJ uses the standard palette */
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 12);
  oam_set(s, 0, 0, 1 << 13, 0, 2 << 12);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[1], OP(0x0001));
}

VCT_TEST(p05_map_1d_boundaries) {
  /* 16x16, 4bpp, tile 3; texel (8,8) = tile (1,1) pixel (0,0) */
  static const u32 bound[4] = {32, 64, 128, 256};
  for (int b = 0; b < 4; b++) {
    VCoreState *s = mk(0, 1u << 4 | (u32)b << 20);
    /* tile (1,1) of the OBJ is the 4th tile after the base (tiles follow each other in 32-byte steps) */
    px4(3 * bound[b] + 3 * 32, 0, 0, 9);
    vct_pal(s, 0, 1, 9, 0x0AAA);
    oam_set(s, 0, 0, 0, 1 << 14, 3);
    VC_ObjAttr o;
    vc_obj_decode(s, 0, 0, &o);
    VCT_EQ(o.w, 16);
    VCT_EQ(vc_obj_tile_texel(s, 0, &o, 8, 8), OP(0x0AAA));
    VCT_EQ(vc_obj_tile_texel(s, 0, &o, 0, 0), 0);
    VCT_EQ(vc_obj_tile_texel(s, 0, &o, 16, 8), 0);
    VCT_EQ(vc_obj_tile_texel(s, 0, &o, 8, -1), 0);
  }
}

VCT_TEST(p05_map_1d_8bpp) {
  /* 16x16, 8bpp, tile 4, boundary 32: tile (1,1) = base 128 + (1*2+1)*64 = 320 */
  VCoreState *s = mk(0, 1u << 4);
  px8(320, 2, 3, 77);
  vct_pal(s, 0, 1, 77, 0x0CCC);
  oam_set(s, 0, 0, 1 << 13, 1 << 14, 4);
  VC_ObjAttr o;
  vc_obj_decode(s, 0, 0, &o);
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 10, 11), OP(0x0CCC));
  /* boundary 256: base 4*256 = 1024 + 192 */
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 12 | 1u << 4 | 3u << 20);
  memset(vr, 0, sizeof vr);
  px8(1024 + 192, 2, 3, 77);
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 10, 11), OP(0x0CCC));
}

VCT_TEST(p05_map_2d) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 9, 0x0123);
  /* 4bpp 16x16 tile 4: tile (1,1) is tile number 4 + 32 + 1 = 37 -> byte 1184 */
  px4(1184, 5, 6, 9);
  oam_set(s, 0, 0, 0, 1 << 14, 4);
  VC_ObjAttr o;
  vc_obj_decode(s, 0, 0, &o);
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 13, 14), OP(0x0123));
  /* top-right tile (1,0) is tile 5 at byte 160 */
  px4(160, 0, 0, 9);
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 8, 0), OP(0x0123));
  /* 8bpp 16x16 tile 4: (1,0) is tile 6 (byte 192), (0,1) is tile 36 (byte 1152), (1,1) tile 38 (byte 1216) */
  oam_set(s, 0, 0, 1 << 13, 1 << 14, 4);
  vc_obj_decode(s, 0, 0, &o);
  memset(vr, 0, sizeof vr);
  px8(192, 0, 0, 9); px8(1152, 0, 0, 9); px8(1216, 7, 7, 9);
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 8, 0), OP(0x0123));
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 0, 8), OP(0x0123));
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 15, 15), OP(0x0123));
  /* in 2D the 1D boundary bits are ignored */
  vct_eng32(s, 0, VC_REG_DISPCNT, 1u << 12 | 3u << 20);
  VCT_EQ(vc_obj_tile_texel(s, 0, &o, 8, 0), OP(0x0123));
}

VCT_TEST(p05_flips) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 1, 0x0001);
  /* 16x8 OBJ (shape 1 size 0) at 0,0; texel tile 0 (0,0) -> OBJ pixel (0,0); tile 1 (7,7) -> (15,7) */
  px4(0, 0, 0, 1);
  px4(32, 7, 7, 1);
  oam_set(s, 0, 0, 1 << 14, 0, 0);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(1)); VCT_EQ(s->line.obj[15], 0);
  oam_set(s, 0, 0, 1 << 14, 1 << 12, 0); /* hflip: (0,0) shows at x=15 */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], 0); VCT_EQ(s->line.obj[15], OP(1));
  oam_set(s, 0, 0, 1 << 14, 1 << 13, 0); /* vflip: line 7 shows texel row 0 */
  vc_obj_line(s, 0, 7, &s->line);
  VCT_EQ(s->line.obj[0], OP(1));
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[15], OP(1)); /* texel row 7 on line 0 */
  oam_set(s, 0, 0, 1 << 14, 3 << 12, 0); /* both: (0,0) -> (15,7) */
  vc_obj_line(s, 0, 7, &s->line);
  VCT_EQ(s->line.obj[15], OP(1)); VCT_EQ(s->line.obj[0], 0);
}

VCT_TEST(p05_priority) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 1, 0x0001);
  vct_pal(s, 0, 1, 2, 0x0002);
  vr[0] = 0x11; vr[1] = 0x11;  /* tile 0: indices 1 */
  vr[32] = 0x22; vr[33] = 0x22; /* tile 1: indices 2 */
  /* equal priority: the lower OAM index wins */
  oam_set(s, 0, 0, 0, 0, 0 | 1 << 10);
  oam_set(s, 0, 1, 0, 0, 1 | 1 << 10);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(1) | 1u << VC_PX_OBJPRIO_SHIFT);
  /* a better (lower) priority number wins even from a later OAM index */
  oam_set(s, 0, 1, 0, 0, 1 | 0 << 10);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(2));
  /* a worse priority number at a later index loses */
  oam_set(s, 0, 1, 0, 0, 1 | 3 << 10);
  oam_set(s, 0, 0, 0, 0, 0 | 2 << 10);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(1) | 2u << VC_PX_OBJPRIO_SHIFT);
  /* a transparent pixel of a better OBJ lets the other show through */
  vr[0] = 0x10; /* texel 0 transparent, texel 1 index 1 */
  oam_set(s, 0, 0, 0, 0, 0 | 0 << 10);
  oam_set(s, 0, 1, 0, 0, 1 | 3 << 10);
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(2) | 3u << VC_PX_OBJPRIO_SHIFT);
  VCT_EQ(s->line.obj[1], OP(1));
}

VCT_TEST(p05_semi_flag) {
  VCoreState *s = mk(0, 0);
  vct_pal(s, 0, 1, 1, 0x0001);
  vr[0] = 0x11;
  oam_set(s, 0, 0, 1 << 10, 0, 0); /* mode 1 */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(1) | VC_PX_SEMI);
  oam_set(s, 0, 0, 0, 0, 0 | 2 << 10); /* mode 0, priority 2: no flag */
  vc_obj_line(s, 0, 0, &s->line);
  VCT_EQ(s->line.obj[0], OP(1) | 2u << VC_PX_OBJPRIO_SHIFT);
}

VCT_TEST(p05_engine_b) {
  VCoreState *s = mk(1, 1u << 4);
  vct_pal(s, 1, 1, 1, 0x0042);
  vct_pal(s, 0, 1, 1, 0x0099);
  px4(0, 0, 0, 1);
  oam_set(s, 1, 0, 3, 7, 0);
  oam_set(s, 0, 0, 3, 8, 0); /* engine A's OAM must not matter */
  vc_obj_line(s, 1, 3, &s->line);
  VCT_EQ(s->line.obj[7], OP(0x0042));
  VCT_EQ(s->line.obj[8], 0);
}

VCT_TEST(p05_plot_clip) {
  VCoreState *s = mk(0, 0);
  VC_ObjAttr o;
  oam_set(s, 0, 0, 0, 0, 0);
  vc_obj_decode(s, 0, 0, &o);
  vc_obj_plot(s, 0, &o, -1, OP(5), &s->line);
  vc_obj_plot(s, 0, &o, 256, OP(5), &s->line);
  vc_obj_plot(s, 0, &o, 255, OP(5), &s->line);
  VCT_EQ(s->line.obj[255], OP(5));
}

VCT_TEST(p05_emit_scene) {
  VCoreState *s = mk(0, 1u << 4);
  vct_pal(s, 0, 0, 0, 0x2108);
  for (int i = 0; i < 8; i++) vct_pal(s, 0, 1, 16 + i, (u16)(0x001F << (i % 3 * 5)));
  for (int i = 0; i < 0x800; i++) vr[i] = (u8)(0x10 + (i & 7) + ((i & 7) << 4));
  for (int i = 0; i < 6; i++)
    oam_set(s, 0, i, (u16)(20 + i * 20), (u16)((10 + i * 30) | (i & 3) << 14), (u16)(i * 4 | 1 << 12));
  oam_set(s, 0, 6, 250, 1 << 14 | 200, 8);
  vct_emit(s, "p05_scene");
}
