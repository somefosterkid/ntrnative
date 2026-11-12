/* port/vcore piece 10 unit tests: master brightness and the display modes (docs/vcore.md, piece 10). OWNED BY PIECE 10.
 * Expected values worked by hand from GBATEK (DS Video Stuff: MASTER_BRIGHT; LCD I/O Display Control; DS Power Control).
 * Run: build/vcore/vcore_tests p10 */
#include "vct.h"

#include <stdio.h>

#define WHITE VC_RGB6(63, 63, 63)

static void fill_out(VC_Line *ln, u32 px) {
  for (int x = 0; x < VC_W; x++) ln->out[x] = px;
}
static u32 one(VCoreState *s, int e, const VC_Line *ln) {
  u32 d[VC_W];
  vc_display_line(s, e, 5, ln, d);
  for (int x = 1; x < VC_W; x++) if (d[x] != d[0]) return 0xDEAD0000u;
  return d[0];
}
static void power(VCoreState *s) { vct_io16(s, VC_REG_POWCNT1, 0x820F); }
static void backdrop(VCoreState *s, int e, u16 c) { vct_pal(s, e, 0, 0, c); }

VCT_TEST(p10_mode1_graphics) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  power(s);
  fill_out(&ln, VC_RGB6(10, 20, 30));
  vct_io32(s, VC_REG_DISPCNT, 0x00010000);
  VCT_EQ(one(s, 0, &ln), VC_RGB6(10, 20, 30));
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010000);
  VCT_EQ(one(s, 1, &ln), VC_RGB6(10, 20, 30));
  backdrop(s, 0, 0x7C00 | 5 << 5 | 3);
  vct_emit(s, "p10_mode1_backdrop");
}

VCT_TEST(p10_mode0_white) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  power(s);
  fill_out(&ln, 0x123456 & 0x3F3F3F);
  vct_io32(s, VC_REG_DISPCNT, 0);
  VCT_EQ(one(s, 0, &ln), WHITE);
  vct_eng32(s, 1, VC_REG_DISPCNT, 0);
  VCT_EQ(one(s, 1, &ln), WHITE);
  backdrop(s, 0, 0x03E0);
  vct_emit(s, "p10_mode0_white");
}

VCT_TEST(p10_forced_blank) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  power(s);
  fill_out(&ln, VC_RGB6(1, 2, 3));
  vct_io32(s, VC_REG_DISPCNT, 0x00010080); /* mode 1 + forced blank */
  VCT_EQ(one(s, 0, &ln), WHITE);
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010080);
  VCT_EQ(one(s, 1, &ln), WHITE);
  backdrop(s, 0, 0x001F);
  vct_emit(s, "p10_forced_blank");
}

VCT_TEST(p10_mode2_vram_line) {
  /* 256 BGR555 pixels per line, row y at byte y*512; each channel c5 -> c5 times 2 */
  static u8 bank[0x20000];
  u32 d[VC_W];
  int y = 3;
  bank[y * 512 + 0] = 0x1F;   bank[y * 512 + 1] = 0x00;   /* pixel 0: red 31 */
  bank[y * 512 + 2] = 0xE0;   bank[y * 512 + 3] = 0x03;   /* pixel 1: green 31 */
  bank[y * 512 + 4] = 0x00;   bank[y * 512 + 5] = 0x7C;   /* pixel 2: blue 31 */
  bank[y * 512 + 6] = 0x21;   bank[y * 512 + 7] = 0x84;   /* pixel 3: 0x8421: r1 g1 b1, bit 15 ignored */
  bank[y * 512 + 510] = 0x0A; bank[y * 512 + 511] = 0x00; /* pixel 255: red 10 */
  bank[2 * 512] = 0x1F;                                   /* another line: not shown */
  vc_p10_vram_line(bank, y, d);
  VCT_EQ(d[0], VC_RGB6(62, 0, 0));
  VCT_EQ(d[1], VC_RGB6(0, 62, 0));
  VCT_EQ(d[2], VC_RGB6(0, 0, 62));
  VCT_EQ(d[3], VC_RGB6(2, 2, 2));
  VCT_EQ(d[4], 0);
  VCT_EQ(d[255], VC_RGB6(20, 0, 0));
  vc_p10_vram_line(NULL, y, d); /* a bank not in LCDC mode: zero */
  VCT_EQ(d[0], 0);
  VCT_EQ(d[255], 0);
}

VCT_TEST(p10_mode2_through_display_line) {
  /* the full path needs piece 12 for vc_vram_lcdc; with the stub (NULL) the line is zero and only that is checked */
  VCoreState *s = vct_state();
  static VC_Line ln;
  u32 d[VC_W];
  power(s);
  fill_out(&ln, WHITE);
  vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x80); /* bank C, LCDC */
  vc_bank(s, VC_BANK_C)[5 * 512] = 0x1F;
  vct_io32(s, VC_REG_DISPCNT, 0x00020000 | 2 << 18);
  vc_display_line(s, 0, 5, &ln, d);
  if (vc_vram_lcdc(s, VC_BANK_C)) {
    VCT_EQ(d[0], VC_RGB6(62, 0, 0));
    VCT_EQ(d[1], 0);
  } else {
    VCT_EQ(d[0], 0);
  }
  vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x03); /* bank C as engine A OBJ: not LCDC: zero */
  vc_display_line(s, 0, 5, &ln, d);
  VCT_EQ(d[0], 0);
  vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x80);
  for (int i = 0; i < 0x20000; i += 2) {
    vc_bank(s, VC_BANK_C)[i] = (u8)(i >> 3);
    vc_bank(s, VC_BANK_C)[i + 1] = (i >> 11) & 0x7F;
  }
  vct_emit(s, "p10_mode2_bankC");
  vct_io8(s, VC_REG_VRAMCNT_A + 2, 0x00); /* the same bank disabled */
  vct_emit(s, "p10_mode2_disabled");
}

VCT_TEST(p10_mode3_mainmem) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  power(s);
  fill_out(&ln, WHITE);
  vct_io32(s, VC_REG_DISPCNT, 0x00030000);
  VCT_EQ(one(s, 0, &ln), 0);
  backdrop(s, 0, 0x7FFF);
  vct_emit(s, "p10_mode3_mainmem");
}

VCT_TEST(p10_engine_b_modes) {
  /* engine B has display modes 0-1 only: bit 17 is not there */
  VCoreState *s = vct_state();
  static VC_Line ln;
  power(s);
  fill_out(&ln, VC_RGB6(7, 8, 9));
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00020000);
  VCT_EQ(one(s, 1, &ln), WHITE); /* bit 16 = 0: mode 0 */
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00030000);
  VCT_EQ(one(s, 1, &ln), VC_RGB6(7, 8, 9)); /* bit 16 = 1: mode 1 */
  backdrop(s, 1, 0x7C00);
  vct_emit(s, "p10_engineB_mode3bits");
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00020000);
  vct_emit(s, "p10_engineB_bit17");
}

VCT_TEST(p10_power_off) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  fill_out(&ln, VC_RGB6(7, 8, 9));
  vct_io32(s, VC_REG_DISPCNT, 0x00010000);
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010000);
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 0x200 | 1); /* A off (bit 1), B on */
  VCT_EQ(one(s, 0, &ln), 0); /* engine A off: black (found with the oracle) */
  VCT_EQ(one(s, 1, &ln), VC_RGB6(7, 8, 9));
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 2 | 1); /* B off (bit 9) */
  VCT_EQ(one(s, 0, &ln), VC_RGB6(7, 8, 9));
  VCT_EQ(one(s, 1, &ln), WHITE); /* engine B off: white (found with the oracle) */
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  VCT_EQ(one(s, 1, &ln), VC_RGB6(31, 31, 31)); /* brightness still applies */
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0);
  vct_io32(s, VC_REG_DISPCNT, 0x00010080); /* powered off beats forced blank */
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 0x200 | 1);
  VCT_EQ(one(s, 0, &ln), 0);
  backdrop(s, 0, 0x7FFF);
  backdrop(s, 1, 0x001F);
  vct_io32(s, VC_REG_DISPCNT, 0x00010000);
  vct_emit(s, "p10_poweroff_A");
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 2 | 1);
  vct_emit(s, "p10_poweroff_B");
  /* probes of the powered-off engine with brightness and forced blank */
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0x4000 | 8);
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  vct_emit(s, "p10_poweroff_B_bright");
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 0x200 | 1);
  vct_emit(s, "p10_poweroff_A_bright");
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010080);
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 2 | 1);
  vct_emit(s, "p10_poweroff_B_blank");
  vct_io16(s, VC_REG_POWCNT1, 0x8000 | 1);
  vct_emit(s, "p10_poweroff_both");
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0);
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0);
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010000);
  vct_io16(s, VC_REG_POWCNT1, 1 | 0x200); /* A off, B on, A on the bottom screen */
  vct_emit(s, "p10_poweroff_A_swapped");
  vct_io16(s, VC_REG_POWCNT1, 1 | 2); /* B off, A on, A on the bottom screen */
  vct_emit(s, "p10_poweroff_B_swapped");
}

/* the oracle's table (probed over POWCNT1 x mode x forced blank x MASTER_BRIGHT, down by 8): mode 0 white is not
 * darkened; forced blank / B off white is; A off mode 1 black; mode 2 shows VRAM and mode 3 black despite either */
VCT_TEST(p10_white_vs_brightness) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  fill_out(&ln, VC_RGB6(7, 8, 9));
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  vct_io32(s, VC_REG_DISPCNT, 0);
  VCT_EQ(one(s, 0, &ln), WHITE);
  vct_io32(s, VC_REG_DISPCNT, 0x00010080);
  VCT_EQ(one(s, 0, &ln), VC_RGB6(31, 31, 31));
  vct_io32(s, VC_REG_DISPCNT, 0x00030080);
  VCT_EQ(one(s, 0, &ln), 0);
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010080);
  VCT_EQ(one(s, 1, &ln), VC_RGB6(31, 31, 31));
  vct_eng32(s, 1, VC_REG_DISPCNT, 0);
  VCT_EQ(one(s, 1, &ln), WHITE);
  vct_io16(s, VC_REG_POWCNT1, 0x8201);
  vct_io32(s, VC_REG_DISPCNT, 0x00030000);
  VCT_EQ(one(s, 0, &ln), 0);
}

static u32 bright(u32 px, int mode, int f) { return vc_p10_bright(px, (u16)(mode * 0x4000 + f)); }

VCT_TEST(p10_bright_formula) {
  /* up: New = Old + (63-Old)*F/16; down: New = Old - Old*F/16 (the amount subtracted rounds up, as the oracle shows) */
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 0, 16), VC_RGB6(0, 31, 63)); /* mode 0: off */
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 1, 0), VC_RGB6(0, 31, 63));
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 2, 0), VC_RGB6(0, 31, 63));
  /* factor 8 up: 0 -> 63*8/16 = 31; 31 -> 31 + 32*8/16 = 47; 63 -> 63 */
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 1, 8), VC_RGB6(31, 47, 63));
  /* factor 8 down: 0 -> 0; 31 -> 31 - ceil(248/16 = 15.5) = 15; 63 -> 63 - ceil(31.5) = 31 */
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 2, 8), VC_RGB6(0, 15, 31));
  /* factor 16: up all 63, down all 0 */
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 1, 16), WHITE);
  VCT_EQ(bright(VC_RGB6(0, 31, 63), 2, 16), 0);
  /* factor > 16 is 16 */
  VCT_EQ(bright(VC_RGB6(5, 31, 63), 1, 17), WHITE);
  VCT_EQ(bright(VC_RGB6(5, 31, 63), 1, 31), WHITE);
  VCT_EQ(bright(VC_RGB6(5, 31, 63), 2, 20), 0);
  /* rounding: truncation. up F=1: 10 + 53/16 = 10 + 3 = 13; down F=1: 10 - ceil(0.625) = 9; 20 - ceil(1.25) = 18; 16 - 1 = 15 */
  VCT_EQ(bright(VC_RGB6(10, 10, 10), 1, 1), VC_RGB6(13, 13, 13));
  VCT_EQ(bright(VC_RGB6(10, 20, 16), 2, 1), VC_RGB6(9, 18, 15));
  /* F=5 up on 1: 1 + 62*5/16 = 1 + 19 = 20; on 63: 63; on 0: 315/16 = 19; down on 63: 63 - ceil(19.69) = 43 */
  VCT_EQ(bright(VC_RGB6(1, 63, 0), 1, 5), VC_RGB6(20, 63, 19));
  VCT_EQ(bright(VC_RGB6(1, 63, 0), 2, 5), VC_RGB6(0, 43, 0));
}

VCT_TEST(p10_bright_in_display) {
  VCoreState *s = vct_state();
  static VC_Line ln;
  power(s);
  fill_out(&ln, VC_RGB6(20, 40, 0));
  vct_io32(s, VC_REG_DISPCNT, 0x00010000);
  vct_eng32(s, 1, VC_REG_DISPCNT, 0x00010000);
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0x4000 | 8);
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  /* A up 8: 20 + 43*8/16 = 41; 40 + 23*8/16 = 51; 0 + 31 = 31. B down 8: 20 - 10 = 10; 40 - 20 = 20; 0 (exact halves) */
  VCT_EQ(one(s, 0, &ln), VC_RGB6(41, 51, 31));
  VCT_EQ(one(s, 1, &ln), VC_RGB6(10, 20, 0));
  /* each engine has its own register */
  vct_eng16(s, 1, VC_REG_MASTER_BRIGHT, 0);
  VCT_EQ(one(s, 1, &ln), VC_RGB6(20, 40, 0));
  /* applies to the white of forced blank: down 8 on 63 = 63 - ceil(31.5) = 31 */
  vct_io32(s, VC_REG_DISPCNT, 0x00010080);
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  VCT_EQ(one(s, 0, &ln), VC_RGB6(31, 31, 31));
  /* and to mode 3 zero: up 16 -> white */
  vct_io32(s, VC_REG_DISPCNT, 0x00030000);
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0x4000 | 16);
  VCT_EQ(one(s, 0, &ln), WHITE);
  /* reserved mode 3 does nothing */
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0xC000 | 8);
  vct_io32(s, VC_REG_DISPCNT, 0x00010000);
  VCT_EQ(one(s, 0, &ln), VC_RGB6(20, 40, 0));
}

VCT_TEST(p10_bright_scenes) {
  static const int fs[] = {0, 8, 16, 20, 5};
  for (int m = 1; m <= 2; m++)
    for (int i = 0; i < 5; i++) {
      char nm[48];
      VCoreState *t = vct_state();
      power(t);
      vct_io32(t, VC_REG_DISPCNT, 0x00010000);
      vct_eng32(t, 1, VC_REG_DISPCNT, 0x00010000);
      vct_pal(t, 0, 0, 0, 0x2A53);
      vct_pal(t, 1, 0, 0, 0x1D8E);
      vct_eng16(t, 0, VC_REG_MASTER_BRIGHT, (u16)(m * 0x4000 + fs[i]));
      vct_eng16(t, 1, VC_REG_MASTER_BRIGHT, (u16)((3 - m) * 0x4000 + fs[i]));
      snprintf(nm, sizeof nm, "p10_bright_m%d_f%d", m, fs[i]);
      vct_emit(t, nm);
    }
}

VCT_TEST(p10_bright_scenes2) {
  VCoreState *s = vct_state();
  power(s);
  vct_io32(s, VC_REG_DISPCNT, 0x00010000);
  vct_pal(s, 0, 0, 0, 0x2A53);
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0xC000 | 8);
  vct_emit(s, "p10_bright_mode3");
  vct_io32(s, VC_REG_DISPCNT, 0x00010080); /* forced blank with brightness */
  vct_eng16(s, 0, VC_REG_MASTER_BRIGHT, 0x8000 | 8);
  vct_emit(s, "p10_blank_bright");
}
