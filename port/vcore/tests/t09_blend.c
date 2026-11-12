/* port/vcore piece 9's unit tests: the compositor and the colour effects (docs/vcore.md, piece 9). OWNED BY PIECE 9.
 * Run: build/vcore/vcore_tests p09
 *
 * The expected values are worked by hand from GBATEK 09 (BLDCNT / BLDALPHA / BLDY, semi-transparent OBJs), 55 (the 3D
 * layer), 26 (bitmap OBJ alpha) and 04 (priority). GBATEK gives the formulas but not the rounding of the divide by
 * 16: where it matters the comment says "rounding: oracle", settled by running the melonDS oracle as a black box on
 * a synthetic frame (see the emitted scenes at the end of this file). The engine works in 6 bits per channel: a
 * 5-bit colour c5 is c5 << 1 (vc_555_to_rgb6).
 *   alpha:  c = (c1 * EVA + c2 * EVB + 8) >> 4, clamped to 63
 *   bright: c = c + ((63 - c) * EVY + 8) >> 4
 *   dark:   c = c - ((c * EVY + 7) >> 4)
 *   3D:     c = (c1 * (A + 1) + c2 * (31 - A) + 16) >> 5          (A = the 3D pixel's alpha, 1..31)
 */
#include "vct.h"
#include <string.h>

enum { E = VC_ENG_A };

static u16 c555(int r, int g, int b) { return (u16)(r | g << 5 | b << 10); }
static u32 px(int r5, int g5, int b5) { return VC_PX_OPAQUE | c555(r5, g5, b5); }

/* a line fixture: engine A, all layers off until a test turns them on */
typedef struct Fx {
  VCoreState *s;
  VC_Line *ln;
} Fx;

static Fx fx_new(void) {
  Fx f;
  f.s = vct_state();
  f.ln = &f.s->line;
  memset(f.ln, 0, sizeof *f.ln);
  memset(f.ln->winmask, 0x3F, sizeof f.ln->winmask);
  vct_eng32(f.s, E, VC_REG_DISPCNT, 1u << 12); /* OBJ on (the compositor checks DISPCNT bit 12 for the OBJ layer) */
  return f;
}
static void bg(Fx *f, int n, int prio, u32 pixel) {
  f->ln->bg_on[n] = 1;
  vct_eng16(f->s, E, VC_REG_BG0CNT + 2 * n, (u16)prio);
  for (int x = 0; x < VC_W; x++) f->ln->bg[n][x] = pixel;
}
static void obj(Fx *f, int prio, u32 pixel) {
  for (int x = 0; x < VC_W; x++) f->ln->obj[x] = pixel ? (pixel | (u32)prio << VC_PX_OBJPRIO_SHIFT) : 0;
}
static void bld(Fx *f, u16 cnt, int eva, int evb, int evy) {
  vct_eng16(f->s, E, VC_REG_BLDCNT, cnt);
  vct_eng16(f->s, E, VC_REG_BLDALPHA, (u16)(eva | evb << 8));
  vct_eng16(f->s, E, VC_REG_BLDY, (u16)evy);
}
static u32 run(Fx *f) {
  vc_compose_line(f->s, E, 0, f->ln);
  return f->ln->out[100];
}
#define RGB(r, g, b) VC_RGB6(r, g, b)

/* ---- layer order ---- */

VCT_TEST(p09_backdrop_only) {
  Fx f = fx_new();
  vct_pal(f.s, E, 0, 0, c555(10, 0, 31));
  VCT_EQ(run(&f), RGB(20, 0, 62)); /* the colour widening: c5 << 1, so 31 becomes 62 (not 63) */
}

VCT_TEST(p09_backdrop_is_bg_palette_0_of_the_engine) {
  Fx f = fx_new();
  vct_pal(f.s, 0, 0, 0, c555(1, 2, 3));
  vct_pal(f.s, 1, 0, 0, c555(4, 5, 6));
  vct_pal(f.s, 0, 1, 0, c555(7, 8, 9)); /* OBJ palette 0 is not the backdrop */
  VCT_EQ(run(&f), RGB(2, 4, 6));
  Fx g = fx_new();
  vct_pal(g.s, 0, 0, 0, c555(1, 2, 3));
  vct_pal(g.s, 1, 0, 0, c555(4, 5, 6));
  vc_compose_line(g.s, VC_ENG_B, 0, g.ln);
  VCT_EQ(g.ln->out[0], RGB(8, 10, 12));
}

VCT_TEST(p09_priority_lower_number_wins) {
  Fx f = fx_new();
  bg(&f, 0, 1, px(30, 0, 0));
  bg(&f, 1, 0, px(0, 30, 0)); /* BG1 priority 0 beats BG0 priority 1 */
  bg(&f, 2, 3, px(0, 0, 30));
  VCT_EQ(run(&f), RGB(0, 60, 0));
}

VCT_TEST(p09_priority_tie_lower_bg_number_wins) {
  Fx f = fx_new();
  bg(&f, 3, 2, px(0, 0, 30));
  bg(&f, 1, 2, px(0, 30, 0));
  bg(&f, 2, 2, px(30, 0, 0));
  VCT_EQ(run(&f), RGB(0, 60, 0)); /* BG1 before BG2 before BG3 */
  f.ln->bg[1][100] = 0;
  VCT_EQ(run(&f), RGB(60, 0, 0));
  f.ln->bg[2][100] = 0;
  VCT_EQ(run(&f), RGB(0, 0, 60));
}

VCT_TEST(p09_obj_in_front_at_equal_priority) {
  Fx f = fx_new();
  bg(&f, 0, 1, px(30, 0, 0));
  obj(&f, 1, px(0, 30, 0));
  VCT_EQ(run(&f), RGB(0, 60, 0)); /* OBJ priority 1 = BG0 priority 1: the OBJ is in front */
  obj(&f, 2, px(0, 30, 0));
  VCT_EQ(run(&f), RGB(60, 0, 0)); /* a worse OBJ priority loses */
  obj(&f, 0, px(0, 30, 0));
  bg(&f, 1, 0, px(0, 0, 30));
  VCT_EQ(run(&f), RGB(0, 60, 0)); /* OBJ 0 and BG1 0: the OBJ still first */
}

VCT_TEST(p09_transparent_pixels_show_what_is_behind) {
  Fx f = fx_new();
  bg(&f, 0, 0, 0);
  bg(&f, 1, 1, px(0, 30, 0));
  VCT_EQ(run(&f), RGB(0, 60, 0));
  f.ln->bg_on[1] = 0; /* a BG that was not rendered (DISPCNT) shows nothing, whatever the buffer holds */
  vct_pal(f.s, E, 0, 0, c555(3, 3, 3));
  VCT_EQ(run(&f), RGB(6, 6, 6));
}

VCT_TEST(p09_obj_layer_needs_dispcnt_bit_12) {
  Fx f = fx_new();
  obj(&f, 0, px(0, 30, 0));
  vct_pal(f.s, E, 0, 0, c555(3, 3, 3));
  VCT_EQ(run(&f), RGB(0, 60, 0));
  vct_eng32(f.s, E, VC_REG_DISPCNT, 0);
  VCT_EQ(run(&f), RGB(6, 6, 6));
}

VCT_TEST(p09_window_mask_hides_layers) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 0));
  bg(&f, 1, 1, px(0, 30, 0));
  obj(&f, 0, px(0, 0, 30));
  f.ln->winmask[100] = VC_WIN_BG0 | VC_WIN_BG1; /* OBJ hidden here */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  f.ln->winmask[100] = VC_WIN_BG1 | VC_WIN_OBJ; /* BG0 hidden: the OBJ then BG1 */
  VCT_EQ(run(&f), RGB(0, 0, 60));
  f.ln->winmask[100] = VC_WIN_BG1;
  VCT_EQ(run(&f), RGB(0, 60, 0));
  f.ln->winmask[100] = 0; /* nothing: the backdrop (a window never hides it) */
  VCT_EQ(run(&f), RGB(0, 0, 0));
}

/* ---- alpha blending ---- */

VCT_TEST(p09_alpha_blend_two_bgs) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 10));
  bg(&f, 1, 1, px(0, 20, 20));
  bld(&f, 0x0041 | 0x0200, 8, 8, 0); /* BG0 1st, mode 1, BG1 2nd */
  /* r: (60*8 + 0*8 + 8) >> 4 = 488 >> 4 = 30;  g: (0 + 40*8 + 8) >> 4 = 328 >> 4 = 20;
     b: (20*8 + 40*8 + 8) >> 4 = 488 >> 4 = 30 */
  VCT_EQ(run(&f), RGB(30, 20, 30));
}

VCT_TEST(p09_alpha_only_when_both_targets_match) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 0));
  bg(&f, 1, 1, px(0, 30, 0));
  bld(&f, 0x0041, 8, 8, 0); /* BG1 not a 2nd target: only the top pixel, at normal intensity */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  bld(&f, 0x0240, 8, 8, 0); /* BG0 not a 1st target */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  bld(&f, 0x0041 | 0x0100, 8, 8, 0); /* BG0 is 2nd, BG1 is 1st? the top is BG0: BG1 below is no 2nd target */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  bld(&f, 0x0042 | 0x0100, 8, 8, 0); /* BG1 as 1st target is not the top layer: no effect on BG0 */
  VCT_EQ(run(&f), RGB(60, 0, 0));
}

VCT_TEST(p09_alpha_all_layers_both_targets) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 0));
  bg(&f, 1, 0, px(0, 30, 0));
  bg(&f, 2, 0, px(0, 0, 30));
  bld(&f, 0x3F7F, 16, 0, 0); /* everything 1st and 2nd: the top one is 1st, the next lower the 2nd */
  VCT_EQ(run(&f), RGB(60, 0, 0));            /* EVA 16, EVB 0 */
  bld(&f, 0x3F3F | 0x40, 0, 16, 0);
  VCT_EQ(run(&f), RGB(0, 60, 0)); /* the second layer (BG1), not the third */
}

VCT_TEST(p09_alpha_backdrop_as_second_target) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 10, 0));
  vct_pal(f.s, E, 0, 0, c555(0, 10, 30)); /* backdrop (0,20,60) */
  bld(&f, 0x0041 | 0x2000, 4, 12, 0);
  /* r: (60*4 + 0 + 8) >> 4 = 248 >> 4 = 15; g: (20*4 + 20*12 + 8) >> 4 = 328 >> 4 = 20;
     b: (0 + 60*12 + 8) >> 4 = 728 >> 4 = 45 */
  VCT_EQ(run(&f), RGB(15, 20, 45));
}

VCT_TEST(p09_alpha_backdrop_as_first_target) {
  Fx f = fx_new();
  vct_pal(f.s, E, 0, 0, c555(30, 0, 0)); /* backdrop (60,0,0) is the top: BD 1st target */
  bld(&f, 0x0060 | 0x0100, 8, 8, 0);     /* nothing below the backdrop: no blending */
  VCT_EQ(run(&f), RGB(60, 0, 0));
}

VCT_TEST(p09_alpha_coefficients_above_16_clamp) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(20, 20, 20));
  bg(&f, 1, 1, px(10, 10, 10));
  bld(&f, 0x0041 | 0x0200, 31, 0, 0); /* EVA 17..31 = 16/16 */
  VCT_EQ(run(&f), RGB(40, 40, 40));
  bld(&f, 0x0041 | 0x0200, 17, 0, 0);
  VCT_EQ(run(&f), RGB(40, 40, 40));
  bld(&f, 0x0041 | 0x0200, 0, 31, 0); /* EVB 31 = 16/16: only the 2nd target (20,20,20 here) */
  VCT_EQ(run(&f), RGB(20, 20, 20));
  bld(&f, 0x0041 | 0x0200, 31, 31, 0); /* 40 + 20 = 60 */
  VCT_EQ(run(&f), RGB(60, 60, 60));
}

VCT_TEST(p09_alpha_sum_clamped) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(31, 31, 31)); /* 62 */
  bg(&f, 1, 1, px(31, 31, 31));
  bld(&f, 0x0041 | 0x0200, 16, 16, 0); /* 62 + 62 = 124, clamped to 63 */
  VCT_EQ(run(&f), RGB(63, 63, 63));
}

VCT_TEST(p09_alpha_rounding) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(7, 7, 7));    /* 14 */
  bg(&f, 1, 1, px(20, 20, 20)); /* 40 */
  bld(&f, 0x0041 | 0x0200, 5, 9, 0);
  /* 14*5 + 40*9 = 70 + 360 = 430; 430 / 16 = 26.875; rounding: oracle (the sum plus 8, then >> 4) gives 27 */
  VCT_EQ(run(&f), RGB(27, 27, 27));
  bld(&f, 0x0041 | 0x0200, 1, 0, 0); /* 14 / 16 = 0.875 -> 1 */
  VCT_EQ(run(&f), RGB(1, 1, 1));
}

/* ---- brighten / darken ---- */

VCT_TEST(p09_brighten) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 20)); /* (60, 0, 40) */
  bld(&f, 0x0001 | 0x80, 0, 0, 0);
  VCT_EQ(run(&f), RGB(60, 0, 40)); /* EVY 0: unchanged */
  bld(&f, 0x0001 | 0x80, 0, 0, 16);
  VCT_EQ(run(&f), RGB(63, 63, 63)); /* EVY 16: white */
  bld(&f, 0x0001 | 0x80, 0, 0, 8);
  /* c + (63 - c) * 8/16: r 60 + 3*8/16 (1.5 -> 2) = 62; g 0 + 31.5 -> 32; b 40 + 23*8/16 (11.5 -> 12) = 52
     (rounding: oracle, half up) */
  VCT_EQ(run(&f), RGB(62, 32, 52));
  bld(&f, 0x0001 | 0x80, 0, 0, 31); /* EVY above 16 = 16/16 */
  VCT_EQ(run(&f), RGB(63, 63, 63));
}

VCT_TEST(p09_darken) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 20));
  bld(&f, 0x0001 | 0xC0, 0, 0, 0);
  VCT_EQ(run(&f), RGB(60, 0, 40));
  bld(&f, 0x0001 | 0xC0, 0, 0, 16);
  VCT_EQ(run(&f), RGB(0, 0, 0));
  bld(&f, 0x0001 | 0xC0, 0, 0, 8);
  /* c - c * 8/16: r 60 - 30 = 30; b 40 - 20 = 20 */
  VCT_EQ(run(&f), RGB(30, 0, 20));
  bld(&f, 0x0001 | 0xC0, 0, 0, 31);
  VCT_EQ(run(&f), RGB(0, 0, 0));
}

VCT_TEST(p09_darken_rounding) {
  Fx f = fx_new();
  vct_pal(f.s, E, 0, 0, c555(4, 9, 20)); /* backdrop (8, 18, 40) */
  bld(&f, 0x0020 | 0xC0, 0, 0, 7);
  /* c - c*7/16: r 8 - 3.5 -> 5 (3.5 rounds down, so 3 is taken off); g 18 - 7.875 -> 10 (8 off);
     b 40 - 17.5 -> 23 (17 off) (rounding: oracle) */
  VCT_EQ(run(&f), RGB(5, 10, 23));
}

VCT_TEST(p09_brighten_rounding) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(25, 3, 7)); /* (50, 6, 14) */
  bld(&f, 0x0001 | 0x80, 0, 0, 5);
  /* r 50 + 13*5/16 = 54.06 -> 54; g 6 + 57*5/16 = 23.8 -> 24; b 14 + 49*5/16 = 29.3 -> 29 (rounding: oracle) */
  VCT_EQ(run(&f), RGB(54, 24, 29));
}

VCT_TEST(p09_brightness_only_first_targets) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 0));
  bg(&f, 1, 1, px(0, 30, 0));
  bld(&f, 0x0002 | 0x80, 0, 0, 16); /* BG1 is the target, but BG0 is on top: untouched */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  bld(&f, 0x0001 | 0x80, 0, 0, 16);
  VCT_EQ(run(&f), RGB(63, 63, 63));
}

VCT_TEST(p09_brightness_on_backdrop) {
  Fx f = fx_new();
  vct_pal(f.s, E, 0, 0, c555(10, 10, 10));
  bld(&f, 0x0020 | 0x80, 0, 0, 16);
  VCT_EQ(run(&f), RGB(63, 63, 63));
  bld(&f, 0x0000 | 0x80, 0, 0, 16); /* backdrop not selected */
  VCT_EQ(run(&f), RGB(20, 20, 20));
}

VCT_TEST(p09_window_effect_bit) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(30, 0, 0));
  bg(&f, 1, 1, px(0, 30, 0));
  bld(&f, 0x0001 | 0x0200 | 0x40, 8, 8, 0);
  f.ln->winmask[100] = 0x1F; /* the effect bit (5) clear: no blending here */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  f.ln->winmask[100] = 0x3F;
  VCT_EQ(run(&f), RGB(30, 30, 0));
  bld(&f, 0x0001 | 0x80, 0, 0, 16);
  f.ln->winmask[100] = 0x1F;
  VCT_EQ(run(&f), RGB(60, 0, 0));
}

/* ---- semi-transparent OBJs and bitmap OBJs ---- */

VCT_TEST(p09_semi_obj_blends_with_second_target) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_SEMI);
  bld(&f, 0x0200, 4, 12, 0); /* mode 0 (none), BG1 2nd target: the semi OBJ blends anyway; BLDCNT bit 4 not set */
  /* r (60*4 + 0 + 8) >> 4 = 15; g (0 + 60*12 + 8) >> 4 = 45 */
  VCT_EQ(run(&f), RGB(15, 45, 0));
}

VCT_TEST(p09_semi_obj_over_a_non_target_is_plain) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_SEMI);
  bld(&f, 0x0400, 4, 12, 0); /* BG2 is the 2nd target; below the OBJ is BG1 */
  VCT_EQ(run(&f), RGB(60, 0, 0));
}

VCT_TEST(p09_semi_obj_over_backdrop) {
  Fx f = fx_new();
  vct_pal(f.s, E, 0, 0, c555(0, 0, 20)); /* (0,0,40) */
  obj(&f, 0, px(30, 0, 0) | VC_PX_SEMI);
  bld(&f, 0x2000, 8, 8, 0);
  /* r (60*8 + 8) >> 4 = 30; b (40*8 + 8) >> 4 = 20 */
  VCT_EQ(run(&f), RGB(30, 0, 20));
}

VCT_TEST(p09_semi_obj_blend_beats_brightness) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_SEMI);
  /* BLDCNT: OBJ 1st target, mode 3 (darken), BG1 2nd target: a semi OBJ over a 2nd target blends, no darkening */
  bld(&f, 0x0010 | 0xC0 | 0x0200, 8, 8, 16);
  VCT_EQ(run(&f), RGB(30, 30, 0));
  /* over a non-target the darkening applies to the OBJ (it is a 1st target here) */
  bld(&f, 0x0010 | 0xC0, 8, 8, 16);
  VCT_EQ(run(&f), RGB(0, 0, 0));
}

VCT_TEST(p09_semi_obj_ignores_window_effect_bit) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_SEMI);
  bld(&f, 0x0200, 8, 8, 0);
  f.ln->winmask[100] = 0x1F; /* rounding/behaviour: oracle - the semi-transparency still happens */
  VCT_EQ(run(&f), RGB(30, 30, 0));
}

VCT_TEST(p09_semi_obj_below_a_bg_is_a_second_target) {
  Fx f = fx_new();
  bg(&f, 0, 0, px(0, 30, 0));
  obj(&f, 1, px(30, 0, 0) | VC_PX_SEMI);
  bld(&f, 0x0001 | 0x0040 | 0x1000, 8, 8, 0); /* BG0 1st, OBJ 2nd (bit 12) */
  VCT_EQ(run(&f), RGB(30, 30, 0));
}

VCT_TEST(p09_bitmap_obj_alpha) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  bld(&f, 0x0200, 0, 0, 0); /* BG1 2nd target; BLDALPHA ignored by a bitmap OBJ */
  /* attr 2 bits 12-15 = alpha a: EVA = a + 1, EVB = 16 - EVA  (rounding and the formula: oracle) */
  obj(&f, 0, px(30, 0, 0) | VC_PX_BMPOBJ | 7u << VC_PX_ALPHA_SHIFT);
  /* EVA 8, EVB 8: r (60*8 + 8) >> 4 = 30, g (60*8 + 8) >> 4 = 30 */
  VCT_EQ(run(&f), RGB(30, 30, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_BMPOBJ | 15u << VC_PX_ALPHA_SHIFT); /* EVA 16: opaque */
  VCT_EQ(run(&f), RGB(60, 0, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_BMPOBJ | 3u << VC_PX_ALPHA_SHIFT); /* EVA 4, EVB 12 */
  VCT_EQ(run(&f), RGB(15, 45, 0));
}

VCT_TEST(p09_bitmap_obj_alpha_zero_is_invisible) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  obj(&f, 0, px(30, 0, 0) | VC_PX_BMPOBJ);
  VCT_EQ(run(&f), RGB(0, 60, 0)); /* oracle: alpha 0 draws nothing, even with no blending at all */
}

VCT_TEST(p09_bitmap_obj_without_second_target) {
  Fx f = fx_new();
  bg(&f, 1, 1, px(0, 30, 0));
  bld(&f, 0x0400, 0, 0, 0);
  obj(&f, 0, px(30, 0, 0) | VC_PX_BMPOBJ | 3u << VC_PX_ALPHA_SHIFT);
  VCT_EQ(run(&f), RGB(60, 0, 0)); /* BG1 is no 2nd target: the OBJ shows at full intensity */
}

/* ---- the 3D layer ---- */

static void l3d(Fx *f, u32 rgb6, int alpha) {
  f->ln->bg_on[0] = 1;
  f->ln->bg3d = 1;
  for (int x = 0; x < VC_W; x++) f->ln->l3d[x] = rgb6 | (u32)alpha << 24;
}

VCT_TEST(p09_3d_alpha_blend_values) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 16);
  bg(&f, 1, 1, px(0, 0, 30)); /* (0, 0, 60) */
  f.ln->bg_on[0] = 1;
  bld(&f, 0x0200, 0, 0, 0); /* BG1 2nd target (BLDALPHA ignored) */
  /* A = 16: (c1 * 17 + c2 * 15 + 16) >> 5: r (680 + 16) >> 5 = 21; g (340 + 16) >> 5 = 11;
     b (170 + 900 + 16) >> 5 = 1086 >> 5 = 33  (formula GBATEK 55 "probably"; rounding: oracle) */
  VCT_EQ(run(&f), RGB(21, 11, 33));
  l3d(&f, RGB(40, 20, 10), 31); /* alpha 31: opaque */
  VCT_EQ(run(&f), RGB(40, 20, 10));
  l3d(&f, RGB(40, 20, 10), 1); /* (c1 * 2 + c2 * 30 + 16) >> 5: r 96 >> 5 = 3; g 56 >> 5 = 1; b 1836 >> 5 = 57 */
  VCT_EQ(run(&f), RGB(3, 1, 57));
  l3d(&f, RGB(40, 20, 10), 30); /* (c1 * 31 + c2 + 16) >> 5: r 1256 >> 5 = 39; g 636 >> 5 = 19; b 370 + 60 + 16 -> (310+60+16)>>5 = 12 */
  VCT_EQ(run(&f), RGB(39, 19, 12));
}

VCT_TEST(p09_3d_alpha_zero_is_transparent) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 0);
  bg(&f, 1, 1, px(0, 30, 0));
  f.ln->bg_on[0] = 1;
  VCT_EQ(run(&f), RGB(0, 60, 0));
}

VCT_TEST(p09_3d_blend_does_not_need_the_first_target_bit) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 16);
  bg(&f, 1, 1, px(0, 0, 30));
  f.ln->bg_on[0] = 1;
  bld(&f, 0x0200, 0, 0, 0); /* BLDCNT bit 0 clear: the 3D alpha still blends (oracle) */
  VCT_EQ(run(&f), RGB(21, 11, 33));
  bld(&f, 0x0200 | 0x0001 | 0x40, 2, 14, 0); /* EVA/EVB do not matter either */
  VCT_EQ(run(&f), RGB(21, 11, 33));
}

VCT_TEST(p09_3d_over_non_target_is_plain) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 16);
  bg(&f, 1, 1, px(0, 0, 30));
  f.ln->bg_on[0] = 1;
  bld(&f, 0x0400, 0, 0, 0); /* BG2 is the 2nd target; below the 3D is BG1 */
  VCT_EQ(run(&f), RGB(40, 20, 10));
}

VCT_TEST(p09_3d_over_backdrop) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 16);
  vct_pal(f.s, E, 0, 0, c555(0, 0, 30));
  bld(&f, 0x2000, 0, 0, 0);
  VCT_EQ(run(&f), RGB(21, 11, 33));
}

VCT_TEST(p09_3d_blend_beats_brightness) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 31);
  bg(&f, 1, 1, px(0, 0, 30));
  f.ln->bg_on[0] = 1;
  bld(&f, 0x0001 | 0xC0 | 0x0200, 0, 0, 16); /* darken BG0 with BG1 2nd target: the 3D blend (opaque) wins, oracle */
  VCT_EQ(run(&f), RGB(40, 20, 10));
  bld(&f, 0x0001 | 0xC0, 0, 0, 16); /* no 2nd target below: the darkening applies */
  VCT_EQ(run(&f), RGB(0, 0, 0));
}

VCT_TEST(p09_3d_brightness_alone) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 31);
  bld(&f, 0x0001 | 0x80, 0, 0, 16);
  VCT_EQ(run(&f), RGB(63, 63, 63));
}

VCT_TEST(p09_3d_priority_and_obj) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 31);
  vct_eng16(f.s, E, VC_REG_BG0CNT, 1);
  bg(&f, 1, 0, px(0, 30, 0));
  f.ln->bg_on[0] = 1;
  VCT_EQ(run(&f), RGB(0, 60, 0)); /* BG1 priority 0 in front of the 3D at priority 1 */
  obj(&f, 1, px(30, 0, 0));
  f.ln->bg_on[1] = 0;
  VCT_EQ(run(&f), RGB(60, 0, 0)); /* equal priority: the OBJ before BG0 = the 3D */
}

VCT_TEST(p09_3d_window_hides_bg0) {
  Fx f = fx_new();
  l3d(&f, RGB(40, 20, 10), 31);
  vct_pal(f.s, E, 0, 0, c555(1, 1, 1));
  f.ln->winmask[100] = 0x3E; /* BG0 not shown here */
  VCT_EQ(run(&f), RGB(2, 2, 2));
}

/* ---- a whole line: each pixel on its own ---- */

VCT_TEST(p09_line_pixels_independent) {
  Fx f = fx_new();
  bg(&f, 0, 0, 0);
  for (int x = 0; x < VC_W; x++) f.ln->bg[0][x] = (x & 1) ? px(30, 0, 0) : 0;
  bg(&f, 1, 1, px(0, 30, 0));
  bld(&f, 0x0001 | 0x0200 | 0x40, 8, 8, 0);
  vc_compose_line(f.s, E, 0, f.ln);
  for (int x = 0; x < VC_W; x++) VCT_EQ(f.ln->out[x], (x & 1) ? RGB(30, 30, 0) : RGB(0, 60, 0));
}

/* ---- synthetic frames for the oracle (VCT_EMIT=DIR) ----
 * Each scene: 32 columns of 8 pixels. BG n shows (opaque) in column g when bit n of g is clear... (see build below),
 * the OBJ in the columns 16-31: so every combination of layers occurs once. Check 2 compares them with the oracle
 * once the BG and OBJ pieces exist; the probes that settled the rounding rules used the same frames. */
static void scene(const char *name, const int prio[4], u16 bldcnt, u16 balpha, u16 bldy, int oprio, int omode,
                  int winin, int a3d) {
  VCoreState *s = vcore_create();
  const int bgc[4][3] = {{25, 3, 7}, {6, 28, 11}, {13, 17, 30}, {30, 27, 2}};
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
  vct_pal(s, 0, 0, 0, c555(4, 9, 20));
  for (int n = 0; n < 4; n++) vct_pal(s, 0, 0, n + 1, c555(bgc[n][0], bgc[n][1], bgc[n][2]));
  vct_pal(s, 0, 1, 1, c555(21, 22, 23));
  vct_io8(s, VC_REG_VRAMCNT_A, 0x81);     /* bank A: engine A BG */
  vct_io8(s, VC_REG_VRAMCNT_A + 1, 0x82); /* bank B: engine A OBJ */
  u8 *a = vc_bank(s, VC_BANK_A);
  for (int n = 0; n < 4; n++) {
    int cb = 4 + n, sb = n;
    vct_io16(s, VC_REG_BG0CNT + 2 * n, (u16)(prio[n] | cb << 2 | 1 << 7 | sb << 8));
    memset(a + cb * 0x4000, n + 1, 64); /* tile 0 opaque (index n+1), tile 1 transparent */
    for (int row = 0; row < 32; row++)
      for (int g = 0; g < 32; g++) {
        u16 ent = (g >> n) & 1 ? 0 : 1;
        a[sb * 0x800 + (row * 32 + g) * 2] = (u8)ent;
      }
  }
  u8 *b = vc_bank(s, VC_BANK_B);
  if (omode == 3) {
    for (int i = 0; i < 64; i++) { b[i * 2] = (u8)(c555(21, 22, 23) & 255); b[i * 2 + 1] = (u8)(0x80 | c555(21, 22, 23) >> 8); }
  } else memset(b, 1, 64);
  u8 *oam = s->oam;
  for (int g = 0; g < 128; g++) {
    u16 a0 = 0x200, a1 = 0, a2 = 0;
    if (g >= 16 && g < 32) {
      a0 = (u16)((a3d ? 96 : 0) | omode << 10 | (omode != 3 ? 1 << 13 : 0));
      a1 = (u16)(g * 8);
      a2 = (u16)(oprio << 10 | (omode == 3 ? ((g + 1) & 15) << 12 : 0));
    }
    oam[g * 8] = (u8)a0; oam[g * 8 + 1] = (u8)(a0 >> 8);
    oam[g * 8 + 2] = (u8)a1; oam[g * 8 + 3] = (u8)(a1 >> 8);
    oam[g * 8 + 4] = (u8)a2; oam[g * 8 + 5] = (u8)(a2 >> 8);
  }
  u32 dispcnt = 0x00011F00u | (omode == 3 ? 0x40 : 0) | (a3d ? 8 : 0) | (winin >= 0 ? 0x2000 : 0);
  vct_eng32(s, E, VC_REG_DISPCNT, dispcnt);
  vct_eng16(s, E, VC_REG_BLDCNT, bldcnt);
  vct_eng16(s, E, VC_REG_BLDALPHA, balpha);
  vct_eng16(s, E, VC_REG_BLDY, bldy);
  if (winin >= 0) {
    vct_eng16(s, E, VC_REG_WIN0H, 0x0080);
    vct_eng16(s, E, VC_REG_WIN0V, 0x00C0);
    vct_eng16(s, E, VC_REG_WININ, (u16)(winin | 0x3F00));
    vct_eng16(s, E, VC_REG_WINOUT, 0x3F);
  }
  if (a3d) {
    vct_io16(s, VC_REG_DISP3DCNT, 8);
    vct_io32(s, VC_REG_CLEAR_COLOR, 0);
    vct_io16(s, VC_REG_CLEAR_DEPTH, 0x7FFF);
    u32 id[16] = {0x1000, 0, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0x1000};
    u32 p;
    p = 0; vct_gx(s, 0x10, 1, &p);
    vct_gx(s, 0x16, 16, id);
    p = 2; vct_gx(s, 0x10, 1, &p);
    vct_gx(s, 0x15, 0, NULL);
    p = 0xBFFF0000u; vct_gx(s, 0x60, 1, &p);
    p = 0x00C0u | (u32)a3d << 16; vct_gx(s, 0x29, 1, &p);
    p = 0; vct_gx(s, 0x2A, 1, &p);
    p = c555(20, 10, 5); vct_gx(s, 0x20, 1, &p);
    p = 1; vct_gx(s, 0x40, 1, &p);
    const int xy[4][2] = {{-0xF00, -0xF00}, {0xF00, -0xF00}, {0xF00, 0xF00}, {-0xF00, 0xF00}};
    for (int i = 0; i < 4; i++) {
      u32 v[2] = {(u32)(xy[i][0] & 0xFFFF) | (u32)(xy[i][1] & 0xFFFF) << 16, 0};
      vct_gx(s, 0x23, 2, v);
    }
    vct_gx(s, 0x41, 0, NULL);
    p = 0; vct_gx(s, 0x50, 1, &p);
  }
  vct_emit(s, name);
  vcore_destroy(s);
}

VCT_TEST(p09_emit_scenes) {
  const int p0[4] = {0, 1, 2, 3}, p1[4] = {2, 1, 3, 0}, p2[4] = {1, 1, 1, 1}, p3[4] = {3, 0, 2, 1};
  scene("p09_plain", p0, 0, 0, 0, 0, 0, -1, 0);
  scene("p09_prio", p1, 0, 0, 0, 1, 0, -1, 0);
  scene("p09_prio_tie", p2, 0, 0, 0, 1, 0, -1, 0);
  scene("p09_alpha", p0, 0x3F41, 5 | 9 << 8, 0, 0, 0, -1, 0);
  scene("p09_alpha_prio", p3, 0x3F7F, 12 | 7 << 8, 0, 0, 0, -1, 0);
  scene("p09_alpha_clamp", p0, 0x3F41, 31 | 25 << 8, 0, 0, 0, -1, 0);
  scene("p09_bright", p0, 0x00BF, 0, 5, 0, 0, -1, 0);
  scene("p09_bright16", p0, 0x00BF, 0, 16, 0, 0, -1, 0);
  scene("p09_dark", p0, 0x00FF, 0, 7, 0, 0, -1, 0);
  scene("p09_dark31", p0, 0x00FF, 0, 31, 0, 0, -1, 0);
  scene("p09_semi", p0, 0x3F00, 6 | 10 << 8, 0, 0, 1, -1, 0);
  scene("p09_semi_nontarget", p0, 0x0100, 6 | 10 << 8, 0, 0, 1, -1, 0);
  scene("p09_semi_bright", p0, 0x01BF, 6 | 10 << 8, 8, 0, 1, -1, 0);
  scene("p09_bmpobj", p0, 0x3F00, 6 | 10 << 8, 0, 0, 3, -1, 0);
  scene("p09_bmpobj_bright", p0, 0x01BF, 0, 5, 0, 3, -1, 0);
  scene("p09_win_alpha", p0, 0x3F41, 9 | 7 << 8, 0, 0, 0, 0x1F, 0);
  scene("p09_win_bright", p0, 0x00BF, 0, 6, 0, 0, 0x1F, 0);
  scene("p09_win_semi", p0, 0x3F00, 6 | 10 << 8, 0, 0, 1, 0x1F, 0);
  scene("p09_3d_a31", p0, 0x3F00, 0, 0, 0, 0, -1, 31);
  scene("p09_3d_a30", p0, 0x3F00, 0, 0, 0, 0, -1, 30);
  scene("p09_3d_a16", p0, 0x3F01, 0, 0, 0, 0, -1, 16);
  scene("p09_3d_a5", p0, 0x3F41, 3 | 13 << 8, 0, 0, 0, -1, 5);
  scene("p09_3d_a1_obj", p0, 0x3F00, 0, 0, 1, 1, -1, 1);
  scene("p09_3d_dark", p0, 0x3FC1, 0, 6, 0, 0, -1, 31);
  scene("p09_3d_dark_nobase", p0, 0x00C1, 0, 6, 0, 0, -1, 31);
}
