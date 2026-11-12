/* piece 17's unit tests: textures. Expected values worked by hand from GBATEK 47, 48, 50. */
#include "vct.h"
#include <string.h>

static u8 tx[0x80000], tp[0x18000];

static VCoreState *setup(u32 fmt, u32 sz_s, u32 sz_t, u32 flags, VC_Poly *p) {
  VCoreState *s = vct_state();
  memset(tx, 0, sizeof tx); memset(tp, 0, sizeof tp);
  vct_map_flat(s, VC_RGN_TEX, tx, sizeof tx);
  vct_map_flat(s, VC_RGN_TEXPAL, tp, sizeof tp);
  memset(p, 0, sizeof *p);
  p->texparam = flags | sz_s << 20 | sz_t << 23 | fmt << 26;
  return s;
}
static void pal(u32 byteaddr, u16 c) { tp[byteaddr] = c & 255; tp[byteaddr + 1] = c >> 8; }
/* a 5-bit channel c becomes c*2+1 (0 stays 0): red 31 -> 63, 1 -> 3, 2 -> 5 */
#define PX(r, g, b, a) (VC_RGB6(r, g, b) | (u32)(a) << 24)

VCT_TEST(p17_a3i5) {
  VC_Poly p; VCoreState *s = setup(1, 0, 0, 0, &p);
  tx[1] = 0xE0 | 3; /* alpha 7 -> 28+3 = 31, index 3 */
  tx[2] = 0x40 | 1; /* alpha 2 -> 8+1 = 9 */
  tx[3] = 0x00 | 1; /* alpha 0 */
  pal(6, 0x001F); pal(2, 0x7C00);
  VCT_EQ(vc_tex_sample(s, &p, 16, 0), PX(63, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 32, 0), PX(0, 0, 63, 9));
  VCT_EQ(vc_tex_sample(s, &p, 48, 0) >> 24, 0);
}
VCT_TEST(p17_pal4) {
  VC_Poly p; VCoreState *s = setup(2, 0, 0, 0, &p);
  tx[0] = 0xE4; /* texels 0,1,2,3 = 0,1,2,3 */
  p.pltt = 2; /* 8-byte units -> 16 */
  pal(16, 0x0001); pal(18, 0x0020); pal(20, 0x0400); pal(22, 0x7FFF);
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), PX(3, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 16, 0), PX(0, 3, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 32, 0), PX(0, 0, 3, 31));
  VCT_EQ(vc_tex_sample(s, &p, 48, 0), PX(63, 63, 63, 31));
  p.texparam |= 1u << 29; /* colour 0 transparent */
  VCT_EQ(vc_tex_sample(s, &p, 0, 0) >> 24, 0);
  VCT_EQ(vc_tex_sample(s, &p, 16, 0) >> 24, 31);
}
VCT_TEST(p17_pal16) {
  VC_Poly p; VCoreState *s = setup(3, 0, 0, 0, &p);
  tx[0] = 0x5A; /* texel0 = 0xA, texel1 = 5 */
  p.pltt = 1; /* 16 bytes */
  pal(16 + 20, 0x0002); pal(16 + 10, 0x0040);
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), PX(5, 0, 0, 31));  /* index A: red 2 */
  VCT_EQ(vc_tex_sample(s, &p, 16, 0), PX(0, 5, 0, 31)); /* index 5: green 2 */
}
VCT_TEST(p17_pal256) {
  VC_Poly p; VCoreState *s = setup(4, 0, 0, 0, &p);
  tx[7] = 200;
  pal(400, 0x03E0);
  VCT_EQ(vc_tex_sample(s, &p, 7 << 4, 0), PX(0, 63, 0, 31));
  tx[6] = 0; pal(0, 0x7FFF);
  VCT_EQ(vc_tex_sample(s, &p, 6 << 4, 0), PX(63, 63, 63, 31));
  p.texparam |= 1u << 29;
  VCT_EQ(vc_tex_sample(s, &p, 6 << 4, 0) >> 24, 0);
}
VCT_TEST(p17_a5i3) {
  VC_Poly p; VCoreState *s = setup(6, 0, 0, 0, &p);
  tx[0] = (17 << 3) | 5;
  pal(10, 0x0421); /* 1,1,1 -> 3 */
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), PX(3, 3, 3, 17));
}
VCT_TEST(p17_direct) {
  VC_Poly p; VCoreState *s = setup(7, 0, 0, 0, &p);
  tx[2] = 0x1F; tx[3] = 0x80;               /* 0x801F: red 31, alpha bit */
  tx[4] = 0x1F; tx[5] = 0x00;               /* 0x001F: alpha bit clear */
  VCT_EQ(vc_tex_sample(s, &p, 16, 0), PX(63, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 32, 0) >> 24, 0);
}
VCT_TEST(p17_notex) {
  VC_Poly p; VCoreState *s = setup(0, 0, 0, 0, &p);
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), 0);
}
/* 4x4: 8x8 texture = 2x2 blocks; block 1 (x 4-7, y 0-3) */
static void comp_setup(VC_Poly *p, VCoreState **s, u32 mode) {
  *s = setup(5, 0, 0, 0, p);
  tx[4] = 0xE4; /* block 1 row 0: texels 0,1,2,3 */
  /* slot0 offset 0 -> index data at 0x20000; block 1 at +2: palette offset 2 (x4 = 8 bytes) and the mode */
  u16 v = 2 | mode << 14;
  tx[0x20000 + 2] = v & 255; tx[0x20000 + 3] = v >> 8;
  p->pltt = 1; /* 16 bytes */
  pal(16 + 8, 0x001F); pal(16 + 10, 0x7C00); pal(16 + 12, 0x03E0); pal(16 + 14, 0x0001);
}
VCT_TEST(p17_comp_mode0) {
  VC_Poly p; VCoreState *s; comp_setup(&p, &s, 0);
  VCT_EQ(vc_tex_sample(s, &p, 4 << 4, 0), PX(63, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 5 << 4, 0), PX(0, 0, 63, 31));
  VCT_EQ(vc_tex_sample(s, &p, 6 << 4, 0), PX(0, 63, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 7 << 4, 0) >> 24, 0);
}
VCT_TEST(p17_comp_mode1) {
  VC_Poly p; VCoreState *s; comp_setup(&p, &s, 1);
  /* texel 2 = (c0+c1)/2: red (31+0)/2 = 15, blue (0+31)/2 = 15 -> 31 */
  VCT_EQ(vc_tex_sample(s, &p, 6 << 4, 0), PX(31, 0, 31, 31));
  VCT_EQ(vc_tex_sample(s, &p, 7 << 4, 0) >> 24, 0);
  VCT_EQ(vc_tex_sample(s, &p, 4 << 4, 0), PX(63, 0, 0, 31));
}
VCT_TEST(p17_comp_mode2) {
  VC_Poly p; VCoreState *s; comp_setup(&p, &s, 2);
  VCT_EQ(vc_tex_sample(s, &p, 7 << 4, 0), PX(3, 0, 0, 31)); /* colour 3 = 0x0001: red 1 -> 3 */
}
VCT_TEST(p17_comp_mode3) {
  VC_Poly p; VCoreState *s; comp_setup(&p, &s, 3);
  /* texel 2 = (c0*5+c1*3)/8: red 155/8 = 19, blue 93/8 = 11 -> 39, 23 */
  VCT_EQ(vc_tex_sample(s, &p, 6 << 4, 0), PX(39, 0, 23, 31));
  /* texel 3 = (c0*3+c1*5)/8: red 93/8 = 11, blue 155/8 = 19 -> 23, 39 */
  VCT_EQ(vc_tex_sample(s, &p, 7 << 4, 0), PX(23, 0, 39, 31));
}
VCT_TEST(p17_comp_slot2) {
  /* texture at slot 2 + 0x100 (byte 0x40100): index data at 0x20000 + 0x10000 + 0x80 */
  VC_Poly p; VCoreState *s = setup(5, 0, 0, 0, &p);
  p.texparam |= (0x40100 / 8);
  tx[0x40100] = 0x01; /* block 0 row 0: texel 0 = 1 */
  pal(0, 0x7FFF); pal(2, 0x001F);
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), PX(63, 0, 0, 31));
}
VCT_TEST(p17_repeat_flip_clamp) {
  VC_Poly p; VCoreState *s = setup(4, 0, 0, 0, &p); /* 8x8 */
  for (int i = 0; i < 64; i++) tx[i] = i & 7;      /* x value */
  for (int i = 0; i < 8; i++) pal(i * 2, (u16)(i + 1)); /* red i+1 -> 2i+3 */
  VCT_EQ(vc_tex_sample(s, &p, -16, 0), PX(3, 0, 0, 31));      /* clamp low: x 0 */
  VCT_EQ(vc_tex_sample(s, &p, 9 * 16, 0), PX(17, 0, 0, 31));  /* clamp high: x 7 */
  p.texparam |= 1u << 16; /* repeat S: 9 -> 1, -1 -> 7 */
  VCT_EQ(vc_tex_sample(s, &p, 9 * 16, 0), PX(5, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, -16, 0), PX(17, 0, 0, 31));
  p.texparam |= 1u << 18; /* flip: 8 -> 7, 9 -> 6, 16 -> 0 */
  VCT_EQ(vc_tex_sample(s, &p, 8 * 16, 0), PX(17, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 9 * 16, 0), PX(15, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 16 * 16, 0), PX(3, 0, 0, 31));
  VCT_EQ(vc_tex_sample(s, &p, 31, 0), PX(5, 0, 0, 31)); /* fraction ignored: 1.9 -> texel 1 */
  /* T: flip without repeat does nothing (clamp): t = 9 -> row 7 */
  for (int i = 0; i < 64; i++) tx[i] = i >> 3;
  p.texparam &= ~(1u << 16); p.texparam &= ~(1u << 18); p.texparam |= 1u << 19;
  VCT_EQ(vc_tex_sample(s, &p, 0, 9 * 16), PX(17, 0, 0, 31));
  p.texparam |= 1u << 17; /* repeat+flip T: 8 -> row 7, 9 -> row 6 */
  VCT_EQ(vc_tex_sample(s, &p, 0, 9 * 16), PX(15, 0, 0, 31));
}
VCT_TEST(p17_sizes) {
  /* 16 x 32 (S size 1, T size 2): texel (3, 20) at index 20*16+3 */
  VC_Poly p; VCoreState *s = setup(4, 1, 2, 0, &p);
  tx[20 * 16 + 3] = 1; pal(2, 0x001F);
  VCT_EQ(vc_tex_sample(s, &p, 3 * 16, 20 * 16), PX(63, 0, 0, 31));
  /* offset field 8 -> byte 64: the texel there is 0 */
  p.texparam |= 8;
  VCT_EQ(vc_tex_sample(s, &p, 3 * 16, 20 * 16), PX(0, 0, 0, 31));
  /* 1024 wide, 8 high: texel 1023 */
  VC_Poly q; VCoreState *s2 = setup(4, 7, 0, 0, &q);
  tx[1023] = 1; pal(2, 0x03E0);
  VCT_EQ(vc_tex_sample(s2, &q, 1023 * 16, 0), PX(0, 63, 0, 31));
}
VCT_TEST(p17_modulate) {
  VC_Poly p; VCoreState *s = setup(7, 0, 0, 0, &p);
  vct_io16(s, VC_REG_DISP3DCNT, 1);
  u32 v = VC_RGB6(63, 31, 0) | 31u << 24, t = VC_RGB6(63, 63, 15) | 31u << 24;
  /* R: 64*64-1 = 4095/64 = 63; G: 64*32-1 = 2047/64 = 31; B: 16*1-1 = 15/64 = 0; A: 32*32-1 = 1023/32 = 31 */
  VCT_EQ(vc_tex_blend(s, &p, v, t), VC_RGB6(63, 31, 0) | 31u << 24);
  t = VC_RGB6(32, 10, 63) | 16u << 24; v = VC_RGB6(32, 63, 7) | 20u << 24;
  /* R: 33*33-1 = 1088/64 = 17; G: 11*64-1 = 703/64 = 10; B: 64*8-1 = 511/64 = 7; A: 17*21-1 = 356/32 = 11 */
  VCT_EQ(vc_tex_blend(s, &p, v, t), VC_RGB6(17, 10, 7) | 11u << 24);
}
VCT_TEST(p17_decal) {
  VC_Poly p; VCoreState *s = setup(7, 0, 0, 0, &p);
  vct_io16(s, VC_REG_DISP3DCNT, 1);
  p.attr = 1 << 4;
  u32 v = VC_RGB6(10, 20, 30) | 25u << 24;
  VCT_EQ(vc_tex_blend(s, &p, v, VC_RGB6(60, 0, 0)), v);
  VCT_EQ(vc_tex_blend(s, &p, v, VC_RGB6(60, 40, 8) | 31u << 24), VC_RGB6(60, 40, 8) | 25u << 24);
  /* oracle (GBATEK's /64 is wrong): (Rt*At + Rv*(31-At)) / 32. At = 15: R = (40*15 + 10*16)/32 = 23; G = (0 + 20*16)/32 = 10;
   * B = (62*15 + 30*16)/32 = 44 */
  VCT_EQ(vc_tex_blend(s, &p, v, VC_RGB6(40, 0, 62) | 15u << 24), VC_RGB6(23, 10, 44) | 25u << 24);
  /* shadow polygons (mode 3) texture exactly like decal (oracle) */
  p.attr = 3 << 4;
  VCT_EQ(vc_tex_blend(s, &p, v, VC_RGB6(40, 0, 62) | 15u << 24), VC_RGB6(23, 10, 44) | 25u << 24);
}
VCT_TEST(p17_palette_wrap) {
  /* oracle: texture palette addresses wrap at 128 KB; slots 6 and 7 (96-128 KB) read as colour 0 */
  VC_Poly p; VCoreState *s = setup(4, 0, 0, 0, &p);
  tx[0] = 0; pal(0x20, 0x001F);
  p.pltt = 0x2002; /* 0x20020 -> wraps to 0x20 */
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), PX(63, 0, 0, 31));
  p.pltt = 0x1800; /* 0x18000: past the 96 KB of VRAM, black */
  VCT_EQ(vc_tex_sample(s, &p, 0, 0), PX(0, 0, 0, 31));
}
VCT_TEST(p17_disable) {
  VC_Poly p; VCoreState *s = setup(7, 0, 0, 0, &p);
  u32 v = VC_RGB6(10, 20, 30) | 25u << 24;
  VCT_EQ(vc_tex_blend(s, &p, v, VC_RGB6(63, 63, 63) | 31u << 24), v); /* DISP3DCNT bit 0 off */
  vct_io16(s, VC_REG_DISP3DCNT, 1);
  p.texparam = 0; /* format 0 */
  VCT_EQ(vc_tex_blend(s, &p, v, VC_RGB6(63, 63, 63) | 31u << 24), v);
}
