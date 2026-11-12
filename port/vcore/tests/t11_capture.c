/* port/vcore piece 11's unit tests: display capture (docs/vcore.md, piece 11). OWNED BY PIECE 11.
 * Expected values are worked by hand from GBATEK 28. Run: build/vcore/vcore_tests p11 */
#include "vct.h"

u16 vc_cap_rgb6_to_555(u32 rgb6);
u16 vc_cap_blend(u16 a, u16 b, int eva, int evb);

static void put16(u8 *b, u32 a, u16 v) {
  b[a] = (u8)v;
  b[a + 1] = (u8)(v >> 8);
}
static u16 get16(const u8 *b, u32 a) { return (u16)(b[a] | b[a + 1] << 8); }

/* DISPCAPCNT: EVA 0-4, EVB 8-12, write block 16-17, write offset 18-19, size 20-21, source A 24, source B 25, read
 * offset 26-27, mode 29-30, enable 31 */
#define CAP(eva, evb, wblk, woff, size, srca, srcb, roff, mode)                                                       \
  (0x80000000u | (eva) | ((evb) << 8) | ((wblk) << 16) | ((woff) << 18) | ((size) << 20) | ((srca) << 24) |          \
   ((srcb) << 25) | ((roff) << 26) | ((mode) << 29))

/* a state with banks A-D in LCDC mode (VRAMCNT = 0x80), engine A in display mode 1, DISPCNT read bank rb */
static VCoreState *mk(u32 cap, int rb) {
  VCoreState *s = vct_state();
  for (int b = 0; b < 4; b++) vct_io8(s, VC_REG_VRAMCNT_A + b, 0x80);
  vct_eng32(s, 0, VC_REG_DISPCNT, 0x10000 | ((u32)rb << 18));
  vct_io32(s, VC_REG_DISPCAPCNT, cap);
  return s;
}
/* halfword i of the test pattern: opaque, a value that differs at every index */
static u16 pat(u32 i) { return (u16)(((i * 7 + 3) & 0x7FFF) | 0x8000); }
static void fill(VCoreState *s, int bank) {
  u8 *b = vc_bank(s, bank);
  for (u32 i = 0; i < 0x10000; i++) put16(b, i * 2, pat(i));
}
static VC_Line gLn;
static void run_lines(VCoreState *s, int n) {
  for (int y = 0; y < n; y++) vc_capture_line(s, y, &gLn);
}
static void set_out(u32 rgb6) {
  for (int x = 0; x < VC_W; x++) gLn.out[x] = rgb6;
}

/* GBATEK 28 "15bit color depth": RGB6 (r6 | g6 << 8 | b6 << 16) -> r5 = r6 >> 1 ... */
VCT_TEST(p11_rgb6_to_555) {
  VCT_EQ(vc_cap_rgb6_to_555(VC_RGB6(63, 0, 32)), 31 | 0 | (16 << 10));
  VCT_EQ(vc_cap_rgb6_to_555(VC_RGB6(1, 2, 3)), 0 | (1 << 5) | (1 << 10));
  VCT_EQ(vc_cap_rgb6_to_555(VC_RGB6(0, 0, 0)), 0);
}

/* source A = the graphics screen, capture A: Dest = SrcA, alpha 1 (bit 15 set) */
VCT_TEST(p11_source_a_graphics) {
  VCoreState *s = mk(CAP(16, 0, 1, 0, 3, 0, 0, 0, 0), 0);
  set_out(VC_RGB6(63, 0, 32));
  gLn.out[5] = VC_RGB6(2, 4, 6);
  run_lines(s, 192);
  const u8 *d = vc_bank(s, 1);
  VCT_EQ(get16(d, 0), 0x8000 | 31 | (16 << 10));
  VCT_EQ(get16(d, 10), 0x8000 | 1 | (2 << 5) | (3 << 10));
  /* last pixel of the last line: 256x192 is 0x18000 bytes */
  VCT_EQ(get16(d, 0x18000 - 2), 0x8000 | 31 | (16 << 10));
  VCT_EQ(get16(d, 0x18000), 0); /* nothing past the capture */
}

/* source A = the 3D screen (bit 24): its alpha (0-31) becomes the alpha bit: 0 -> clear, otherwise set */
VCT_TEST(p11_source_a_3d) {
  VCoreState *s = mk(CAP(16, 0, 0, 0, 2, 1, 0, 0, 0), 0);
  set_out(VC_RGB6(63, 63, 63)); /* ignored */
  s->f3d.out[0][0] = VC_RGB6(62, 0, 0) | (31u << 24);
  s->f3d.out[0][1] = VC_RGB6(0, 62, 0) | (1u << 24);
  s->f3d.out[0][2] = VC_RGB6(0, 0, 62) | (0u << 24);
  run_lines(s, 1);
  const u8 *d = vc_bank(s, 0);
  VCT_EQ(get16(d, 0), 0x8000 | 31);
  VCT_EQ(get16(d, 2), 0x8000 | (31 << 5));
  VCT_EQ(get16(d, 4), 31 << 10); /* alpha 0: bit 15 clear */
}

/* capture B: Dest = SrcB, the VRAM word copied as it is (its bit 15 too) */
VCT_TEST(p11_source_b_vram) {
  VCoreState *s = mk(CAP(0, 16, 2, 0, 3, 0, 0, 0, 1), 0);
  fill(s, 0);
  put16(vc_bank(s, 0), 6, 0x1234); /* bit 15 clear: stays clear */
  set_out(0x3FFFF);
  run_lines(s, 192);
  const u8 *d = vc_bank(s, 2);
  VCT_EQ(get16(d, 0), pat(0));
  VCT_EQ(get16(d, 2), pat(1));
  VCT_EQ(get16(d, 6), 0x1234);
  VCT_EQ(get16(d, 2 * (100 * 256 + 77)), pat(100 * 256 + 77));
}

/* source B = main memory (bit 25): no data, zero; capture B writes zero */
VCT_TEST(p11_source_b_mainmem) {
  VCoreState *s = mk(CAP(0, 16, 3, 0, 2, 0, 1, 0, 1), 0);
  fill(s, 0);
  fill(s, 3);
  run_lines(s, 128);
  const u8 *d = vc_bank(s, 3);
  VCT_EQ(get16(d, 0), 0);
  VCT_EQ(get16(d, 2 * 5000), 0);
}

/* the blend: Dest = (A * alphaA * EVA + B * alphaB * EVB + 8) / 16 per channel (the oracle rounds half up; GBATEK's plain /16 differs), alpha = (aA and EVA > 0) or (aB and EVB > 0) */
VCT_TEST(p11_blend_formula) {
  u16 a = 0x8000 | 31 | (0 << 5) | (10 << 10);
  u16 b = 0x8000 | 0 | (31 << 5) | (20 << 10);
  /* EVA 8, EVB 8: (31*8+8)/16 = 16, 16, (10*8 + 20*8+8)/16 = 15 */
  VCT_EQ(vc_cap_blend(a, b, 8, 8), 0x8000 | 16 | (16 << 5) | (15 << 10));
  /* EVA 16 EVB 16: 31 + 0 = 31; 0 + 31; 10 + 20 = 30 */
  VCT_EQ(vc_cap_blend(a, b, 16, 16), 0x8000 | 31 | (31 << 5) | (30 << 10));
  /* the sum clamps at 31: 31 + 31 (both red) */
  VCT_EQ(vc_cap_blend(0x8000 | 31, 0x8000 | 31, 16, 16), 0x8000 | 31);
  /* rounding: (5 * 3 + 8) / 16 = 1; (31 * 3 + 31 * 5 + 8) / 16 = 256 / 16 = 16; (15 + 7) / 16 = 1 but (4*3+3)/16 = 0 */
  VCT_EQ(vc_cap_blend(0x8000 | 5 | (31 << 5), 0x8000 | (31 << 5), 3, 5), 0x8000 | 1 | (16 << 5));
  VCT_EQ(vc_cap_blend(0x8000 | 4, 0x8000, 3, 5), 0x8000 | 1);
  VCT_EQ(vc_cap_blend(0x8000 | 1, 0x8000, 7, 0), 0x8000 | 0);
  /* EVA, EVB above 16 act as 16 */
  VCT_EQ(vc_cap_blend(0x8000 | 2, 0x8000 | 3, 31, 20), 0x8000 | 5);
  /* alpha 0 sources contribute nothing */
  VCT_EQ(vc_cap_blend(31 | (31 << 5), 0x8000 | 7, 16, 16), 0x8000 | 7);
  VCT_EQ(vc_cap_blend(31, 7, 16, 16), 0); /* both alpha 0: all zero, alpha 0 */
  /* alpha needs EVx > 0: A opaque with EVA 0, B transparent -> alpha 0 and colour 0 */
  VCT_EQ(vc_cap_blend(0x8000 | 31, 0x0000 | 9, 0, 16), 0);
  /* A opaque EVA 0, B opaque EVB 16: alpha from B only */
  VCT_EQ(vc_cap_blend(0x8000 | 31, 0x8000 | 9, 0, 16), 0x8000 | 9);
}

/* mode 2/3 through the line function: A (graphics) and B (VRAM) blended */
VCT_TEST(p11_blend_line) {
  VCoreState *s = mk(CAP(8, 8, 1, 0, 2, 0, 0, 0, 2), 0);
  u8 *b = vc_bank(s, 0);
  put16(b, 0, 0x8000 | (31 << 5)); /* green, opaque */
  put16(b, 2, 0x0000 | (31 << 5)); /* green, alpha 0 */
  set_out(VC_RGB6(62, 0, 0));      /* red 31 */
  run_lines(s, 1);
  const u8 *d = vc_bank(s, 1);
  VCT_EQ(get16(d, 0), 0x8000 | 16 | (16 << 5));
  VCT_EQ(get16(d, 2), 0x8000 | 16); /* B's alpha is 0: A only, at EVA 8 */
  /* mode 3 behaves as mode 2 */
  vct_io32(s, VC_REG_DISPCAPCNT, CAP(8, 8, 2, 0, 2, 0, 0, 0, 3));
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 2), 0), 0x8000 | 16 | (16 << 5));
}

/* the four capture sizes: 128x128, 256x64, 256x128, 256x192; lines beyond the height and pixels beyond the width
 * are not written; the destination is packed at the capture's own width */
VCT_TEST(p11_sizes) {
  static const int W[4] = {128, 256, 256, 256}, H[4] = {128, 64, 128, 192};
  for (int sz = 0; sz < 4; sz++) {
    VCoreState *s = mk(CAP(16, 0, 1, 0, (u32)sz, 0, 0, 0, 0), 0);
    set_out(VC_RGB6(62, 62, 62));
    run_lines(s, 192);
    const u8 *d = vc_bank(s, 1);
    int total = W[sz] * H[sz];
    VCT_EQ(get16(d, 0), 0xFFFF);
    VCT_EQ(get16(d, 2 * (total - 1)), 0xFFFF);
    VCT_EQ(get16(d, 2 * total), 0);
  }
}

/* a 128-wide capture packs its lines 128 pixels apart: line 1 starts at byte 256 */
VCT_TEST(p11_width128_packed) {
  VCoreState *s = mk(CAP(16, 0, 1, 0, 0, 0, 0, 0, 0), 0);
  set_out(VC_RGB6(0, 62, 0));
  run_lines(s, 1);
  gLn.out[0] = VC_RGB6(62, 0, 0);
  vc_capture_line(s, 1, &gLn);
  const u8 *d = vc_bank(s, 1);
  VCT_EQ(get16(d, 254), 0x8000 | (31 << 5)); /* line 0, x = 127 */
  VCT_EQ(get16(d, 256), 0x8000 | 31);        /* line 1, x = 0 */
  VCT_EQ(get16(d, 256 + 254), 0x8000 | (31 << 5));
  VCT_EQ(get16(d, 256 + 256), 0);
}

/* write offset 0-3 = 0, 8000h, 10000h, 18000h; it wraps to 0 past 1FFFFh */
VCT_TEST(p11_write_offset_wrap) {
  VCoreState *s = mk(CAP(16, 0, 1, 3, 2, 0, 0, 0, 0), 0);
  set_out(VC_RGB6(62, 0, 0));
  run_lines(s, 128); /* 256x128 = 0x10000 bytes from 0x18000: the last 0x8000 bytes wrap to 0 */
  const u8 *d = vc_bank(s, 1);
  VCT_EQ(get16(d, 0x18000), 0x8000 | 31);
  VCT_EQ(get16(d, 0x1FFFE), 0x8000 | 31);
  VCT_EQ(get16(d, 0), 0x8000 | 31);
  VCT_EQ(get16(d, 0x7FFE), 0x8000 | 31);
  VCT_EQ(get16(d, 0x8000), 0);
  VCT_EQ(get16(d, 0x17FFE), 0);
  /* offset 1 and 2 */
  s = mk(CAP(16, 0, 0, 2, 0, 0, 0, 0, 0), 0);
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 0), 0x10000), 0x8000 | 31);
  VCT_EQ(get16(vc_bank(s, 0), 0xFFFE), 0);
  s = mk(CAP(16, 0, 0, 1, 0, 0, 0, 0, 0), 0);
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 0), 0x8000), 0x8000 | 31);
}

/* the read offset (bits 26-27) moves source B; the read address wraps at 128K too */
VCT_TEST(p11_read_offset) {
  VCoreState *s = mk(CAP(0, 16, 1, 0, 0, 0, 0, 2, 1), 0);
  fill(s, 0);
  set_out(0);
  run_lines(s, 1);
  /* offset 2 = 0x10000 bytes = halfword 0x8000; 128 wide, line 0 */
  VCT_EQ(get16(vc_bank(s, 1), 0), pat(0x8000));
  VCT_EQ(get16(vc_bank(s, 1), 254), pat(0x8000 + 127));
  /* offset 3, 256 wide, line 128: 0x18000 + 128 * 512 = 0x28000 wraps to 0x8000 */
  s = mk(CAP(0, 16, 1, 0, 3, 0, 0, 3, 1), 0);
  fill(s, 0);
  for (int y = 0; y < 129; y++) vc_capture_line(s, y, &gLn);
  VCT_EQ(get16(vc_bank(s, 1), 2 * 128 * 256), pat(0x4000));
}

/* "VRAM Read Offset is ignored (zero) in VRAM Display Mode" (DISPCNT bits 16-17 = 2) */
VCT_TEST(p11_read_offset_vram_mode) {
  VCoreState *s = mk(CAP(0, 16, 1, 0, 0, 0, 0, 2, 1), 0);
  vct_eng32(s, 0, VC_REG_DISPCNT, 0x20000);
  fill(s, 0);
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 1), 0), pat(0));
}

/* the read bank is DISPCNT bits 18-19 */
VCT_TEST(p11_read_bank) {
  VCoreState *s = mk(CAP(0, 16, 0, 0, 2, 0, 0, 0, 1), 3);
  fill(s, 3);
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 0), 0), pat(0));
  VCT_EQ(get16(vc_bank(s, 0), 20), pat(10));
}

/* source B and the destination the same bank: each line is read before it is written, so with the same offsets
 * line y reads its own old data; the capture of A+B then trails the old picture ("traces") */
VCT_TEST(p11_same_bank) {
  VCoreState *s = mk(CAP(8, 8, 0, 0, 2, 0, 0, 0, 2), 0);
  u8 *b = vc_bank(s, 0);
  for (int y = 0; y < 2; y++)
    for (int x = 0; x < 256; x++) put16(b, 2 * (y * 256 + x), 0x8000 | 20); /* red 20 */
  set_out(VC_RGB6(2, 0, 0));                                                /* red 1 */
  run_lines(s, 2);
  /* (1 * 8 + 20 * 8 + 8) / 16 = 11 */
  VCT_EQ(get16(b, 0), 0x8000 | 11);
  VCT_EQ(get16(b, 2 * 256 + 2 * 255), 0x8000 | 11);
  /* a second frame: (1 * 8 + 11 * 8 + 8) / 16 = 6 */
  vct_io32(s, VC_REG_DISPCAPCNT, CAP(8, 8, 0, 0, 2, 0, 0, 0, 2));
  run_lines(s, 2);
  VCT_EQ(get16(b, 0), 0x8000 | 6);
}

/* a bank that is not in LCDC mode is neither written nor read as source B */
VCT_TEST(p11_banks_not_lcdc) {
  VCoreState *s = mk(CAP(0, 16, 1, 0, 0, 0, 0, 0, 1), 0);
  fill(s, 0);
  vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x81); /* destination B: enabled, MST 1 (BG) */
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 1), 0), 0);
  vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x00); /* disabled */
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 1), 0), 0);
  /* source B not LCDC: reads zero */
  vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x80);
  vct_io8(s, VC_REG_VRAMCNT_A + 0, 0x81);
  run_lines(s, 1);
  VCT_EQ(get16(vc_bank(s, 1), 0), 0);
}

/* bit 31 clear: nothing; vc_capture_end clears bit 31 and keeps the other bits */
VCT_TEST(p11_enable_bit) {
  VCoreState *s = mk(CAP(16, 0, 1, 0, 3, 0, 0, 0, 0) & 0x7FFFFFFFu, 0);
  set_out(VC_RGB6(62, 0, 0));
  run_lines(s, 192);
  VCT_EQ(get16(vc_bank(s, 1), 0), 0);
  u32 c = CAP(16, 0, 1, 0, 3, 0, 0, 0, 0);
  vct_io32(s, VC_REG_DISPCAPCNT, c);
  vc_capture_end(s);
  VCT_EQ(vc_io32(s, VC_REG_DISPCAPCNT), c & 0x7FFFFFFFu);
}

/* synthetic frames for the oracle: whole-frame cases */
static void emit_case(const char *name, u32 cap, int rb, u32 dispmode) {
  VCoreState *s = mk(cap, rb);
  vct_eng32(s, 0, VC_REG_DISPCNT, dispmode | ((u32)rb << 18));
  fill(s, 0);
  u8 *c = vc_bank(s, 2);
  for (u32 i = 0; i < 0x10000; i++) put16(c, i * 2, (u16)((i * 13 + 5) & 0xFFFF));
  vct_pal(s, 0, 0, 0, 0x1234);
  vct_emit(s, name);
}
VCT_TEST(p11_emit_scenes) {
  emit_case("p11_s_b_copy128", CAP(0, 16, 1, 0, 0, 0, 0, 0, 1), 0, 0x10000);
  emit_case("p11_s_b_copy256x64", CAP(0, 16, 1, 1, 1, 0, 0, 1, 1), 0, 0x10000);
  emit_case("p11_s_b_copy256x128", CAP(0, 16, 3, 2, 2, 0, 0, 2, 1), 0, 0x10000);
  emit_case("p11_s_b_copy256x192_wrap", CAP(0, 16, 1, 3, 3, 0, 0, 3, 1), 0, 0x10000);
  emit_case("p11_s_b_vrammode_ro", CAP(0, 16, 1, 0, 3, 0, 0, 3, 1), 0, 0x20000);
  emit_case("p11_s_b_mainmem", CAP(0, 16, 1, 0, 3, 0, 1, 0, 1), 0, 0x10000);
  emit_case("p11_s_same_bank", CAP(0, 16, 0, 1, 3, 0, 0, 0, 1), 0, 0x10000);
  emit_case("p11_s_srcb_c", CAP(0, 16, 1, 0, 3, 0, 0, 0, 1), 2, 0x10000);
}
VCT_TEST(p11_emit_scenes2) {
  emit_case("p11_s_a_graphics", CAP(16, 0, 1, 0, 3, 0, 0, 0, 0), 0, 0x10000);
  emit_case("p11_s_a_graphics128", CAP(16, 0, 3, 2, 0, 0, 0, 0, 0), 0, 0x10000);
  emit_case("p11_s_blend_8_8", CAP(8, 8, 1, 0, 3, 0, 0, 0, 2), 0, 0x10000);
  emit_case("p11_s_blend_3_13", CAP(3, 13, 1, 0, 3, 0, 0, 0, 3), 0, 0x10000);
  emit_case("p11_s_blend_over", CAP(31, 20, 1, 0, 3, 0, 0, 0, 2), 0, 0x10000);
  emit_case("p11_s_blend_16_16", CAP(16, 16, 1, 0, 3, 0, 0, 0, 2), 0, 0x10000);
  emit_case("p11_s_blend_same_bank", CAP(8, 8, 0, 0, 3, 0, 0, 0, 2), 0, 0x10000);
  emit_case("p11_s_a_3d_empty", CAP(16, 0, 1, 0, 3, 1, 0, 0, 0), 0, 0x10000);
  VCoreState *s = mk(CAP(16, 0, 1, 0, 3, 0, 0, 0, 0), 0);
  vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x81); /* destination B is a BG bank, not LCDC: nothing is written */
  fill(s, 1);
  vct_emit(s, "p11_s_dest_nolcdc");
}
