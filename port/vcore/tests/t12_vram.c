/* port/vcore piece 12's unit tests: VRAM bank mapping (docs/vcore.md, piece 12). OWNED BY PIECE 12.
 * Run: build/vcore/vcore_tests p12
 * Expected values are worked from GBATEK 22's table: each row = bank, VRAMCNT byte, region, first page, page count
 * (pages of 16K; the bank's pages 0.. go in order). Everything else must stay unmapped. */
#include "vct.h"

#include <string.h>

#define EN 0x80
#define CNT(mst, ofs) (EN | (ofs) << 3 | (mst))

typedef struct { int bank; u8 cnt; int rgn; u32 at, n; } Row;

static const Row rows[] = {
  /* LCDC (MST 0): pages at bank offset / 16K */
  {VC_BANK_A, CNT(0, 0), VC_RGN_LCDC, 0, 8},  {VC_BANK_B, CNT(0, 0), VC_RGN_LCDC, 8, 8},
  {VC_BANK_C, CNT(0, 0), VC_RGN_LCDC, 16, 8}, {VC_BANK_D, CNT(0, 0), VC_RGN_LCDC, 24, 8},
  {VC_BANK_E, CNT(0, 0), VC_RGN_LCDC, 32, 4}, {VC_BANK_F, CNT(0, 0), VC_RGN_LCDC, 36, 1},
  {VC_BANK_G, CNT(0, 0), VC_RGN_LCDC, 37, 1}, {VC_BANK_H, CNT(0, 0), VC_RGN_LCDC, 38, 2},
  {VC_BANK_I, CNT(0, 0), VC_RGN_LCDC, 40, 1},
  /* engine A BG: A-D 128K at 20000h*OFS */
  {VC_BANK_A, CNT(1, 0), VC_RGN_ABG, 0, 8},  {VC_BANK_A, CNT(1, 1), VC_RGN_ABG, 8, 8},
  {VC_BANK_A, CNT(1, 2), VC_RGN_ABG, 16, 8}, {VC_BANK_A, CNT(1, 3), VC_RGN_ABG, 24, 8},
  {VC_BANK_B, CNT(1, 0), VC_RGN_ABG, 0, 8},  {VC_BANK_B, CNT(1, 3), VC_RGN_ABG, 24, 8},
  {VC_BANK_C, CNT(1, 1), VC_RGN_ABG, 8, 8},  {VC_BANK_C, CNT(1, 2), VC_RGN_ABG, 16, 8},
  {VC_BANK_D, CNT(1, 0), VC_RGN_ABG, 0, 8},  {VC_BANK_D, CNT(1, 3), VC_RGN_ABG, 24, 8},
  {VC_BANK_E, CNT(1, 0), VC_RGN_ABG, 0, 4},
  /* F, G: 4000h*OFS.0 + 10000h*OFS.1 = pages 0, 1, 4, 5 */
  {VC_BANK_F, CNT(1, 0), VC_RGN_ABG, 0, 1}, {VC_BANK_F, CNT(1, 1), VC_RGN_ABG, 1, 1},
  {VC_BANK_F, CNT(1, 2), VC_RGN_ABG, 4, 1}, {VC_BANK_F, CNT(1, 3), VC_RGN_ABG, 5, 1},
  {VC_BANK_G, CNT(1, 0), VC_RGN_ABG, 0, 1}, {VC_BANK_G, CNT(1, 1), VC_RGN_ABG, 1, 1},
  {VC_BANK_G, CNT(1, 2), VC_RGN_ABG, 4, 1}, {VC_BANK_G, CNT(1, 3), VC_RGN_ABG, 5, 1},
  /* engine A OBJ: A, B 20000h*OFS.0 (OFS.1 ignored); E; F, G as above */
  {VC_BANK_A, CNT(2, 0), VC_RGN_AOBJ, 0, 8}, {VC_BANK_A, CNT(2, 1), VC_RGN_AOBJ, 8, 8},
  {VC_BANK_B, CNT(2, 0), VC_RGN_AOBJ, 0, 8}, {VC_BANK_B, CNT(2, 1), VC_RGN_AOBJ, 8, 8},
  {VC_BANK_E, CNT(2, 0), VC_RGN_AOBJ, 0, 4},
  {VC_BANK_F, CNT(2, 0), VC_RGN_AOBJ, 0, 1}, {VC_BANK_F, CNT(2, 1), VC_RGN_AOBJ, 1, 1},
  {VC_BANK_F, CNT(2, 2), VC_RGN_AOBJ, 4, 1}, {VC_BANK_F, CNT(2, 3), VC_RGN_AOBJ, 5, 1},
  {VC_BANK_G, CNT(2, 0), VC_RGN_AOBJ, 0, 1}, {VC_BANK_G, CNT(2, 3), VC_RGN_AOBJ, 5, 1},
  /* texture image slots (128K = 8 pages each): A-D MST 3 */
  {VC_BANK_A, CNT(3, 0), VC_RGN_TEX, 0, 8},  {VC_BANK_A, CNT(3, 3), VC_RGN_TEX, 24, 8},
  {VC_BANK_B, CNT(3, 1), VC_RGN_TEX, 8, 8},  {VC_BANK_C, CNT(3, 2), VC_RGN_TEX, 16, 8},
  {VC_BANK_D, CNT(3, 0), VC_RGN_TEX, 0, 8},  {VC_BANK_D, CNT(3, 3), VC_RGN_TEX, 24, 8},
  /* texture palette slots (16K each): E slots 0-3, F/G slot OFS.0 + OFS.1*4 */
  {VC_BANK_E, CNT(3, 0), VC_RGN_TEXPAL, 0, 4}, {VC_BANK_E, CNT(3, 2), VC_RGN_TEXPAL, 0, 4},
  {VC_BANK_F, CNT(3, 0), VC_RGN_TEXPAL, 0, 1}, {VC_BANK_F, CNT(3, 1), VC_RGN_TEXPAL, 1, 1},
  {VC_BANK_F, CNT(3, 2), VC_RGN_TEXPAL, 4, 1}, {VC_BANK_F, CNT(3, 3), VC_RGN_TEXPAL, 5, 1},
  {VC_BANK_G, CNT(3, 0), VC_RGN_TEXPAL, 0, 1}, {VC_BANK_G, CNT(3, 1), VC_RGN_TEXPAL, 1, 1},
  {VC_BANK_G, CNT(3, 2), VC_RGN_TEXPAL, 4, 1}, {VC_BANK_G, CNT(3, 3), VC_RGN_TEXPAL, 5, 1},
  /* engine A BG extended palette: E (lower 32K = 2 pages); F, G OFS 0 = slots 0-1, OFS 1 = slots 2-3 */
  {VC_BANK_E, CNT(4, 0), VC_RGN_ABGEXT, 0, 2},
  {VC_BANK_F, CNT(4, 0), VC_RGN_ABGEXT, 0, 1}, {VC_BANK_F, CNT(4, 1), VC_RGN_ABGEXT, 1, 1},
  {VC_BANK_G, CNT(4, 0), VC_RGN_ABGEXT, 0, 1}, {VC_BANK_G, CNT(4, 1), VC_RGN_ABGEXT, 1, 1},
  /* engine A OBJ extended palette: F, G MST 5 */
  {VC_BANK_F, CNT(5, 0), VC_RGN_AOBJEXT, 0, 1}, {VC_BANK_G, CNT(5, 0), VC_RGN_AOBJEXT, 0, 1},
  /* engine B: C -> BG (MST 4), D -> OBJ (MST 4), H BG (MST 1) at 0, I BG (MST 1) at 8000h, I OBJ (MST 2) */
  {VC_BANK_C, CNT(4, 0), VC_RGN_BBG, 0, 8},  {VC_BANK_D, CNT(4, 0), VC_RGN_BOBJ, 0, 8},
  {VC_BANK_H, CNT(1, 0), VC_RGN_BBG, 0, 2},  {VC_BANK_I, CNT(1, 0), VC_RGN_BBG, 2, 1},
  {VC_BANK_I, CNT(2, 0), VC_RGN_BOBJ, 0, 1},
  /* engine B extended palettes: H BG (MST 2), I OBJ (MST 3) */
  {VC_BANK_H, CNT(2, 0), VC_RGN_BBGEXT, 0, 2}, {VC_BANK_I, CNT(3, 0), VC_RGN_BOBJEXT, 0, 1},
  /* OFS where it is not used */
  {VC_BANK_E, CNT(1, 3), VC_RGN_ABG, 0, 4}, {VC_BANK_H, CNT(1, 3), VC_RGN_BBG, 0, 2},
  {VC_BANK_I, CNT(1, 3), VC_RGN_BBG, 2, 1},
  /* unmapped combinations: C/D MST 2 (ARM7), invalid MST, A/B MST bit 2 ignored (A MST 4 = LCDC, B MST 7 = 3) */
  {VC_BANK_C, CNT(2, 0), VC_RGN_LCDC, 0, 0}, {VC_BANK_D, CNT(2, 1), VC_RGN_LCDC, 0, 0},
  {VC_BANK_E, CNT(5, 0), VC_RGN_LCDC, 0, 0}, {VC_BANK_F, CNT(6, 0), VC_RGN_LCDC, 0, 0},
  {VC_BANK_A, CNT(4, 0), VC_RGN_LCDC, 0, 8}, {VC_BANK_B, CNT(7, 0), VC_RGN_TEX, 0, 8},
};

/* the oracle's mirrors (probed, GBATEK is silent): further starts at which the bank's pages appear again.
 * F, G in A BG / A OBJ: 32K up; H in B BG: 64K up; I in B BG: pages 2, 3, 6, 7; I in B OBJ: every page of the region */
static int mirrors(const Row *w, u32 *x) {
  int n = 0;
  if ((w->bank == VC_BANK_F || w->bank == VC_BANK_G) && (w->rgn == VC_RGN_ABG || w->rgn == VC_RGN_AOBJ)) x[n++] = w->at + 2;
  if (w->bank == VC_BANK_H && w->rgn == VC_RGN_BBG) x[n++] = 4;
  if (w->bank == VC_BANK_I && w->rgn == VC_RGN_BBG) x[n++] = 3, x[n++] = 6, x[n++] = 7;
  if (w->bank == VC_BANK_I && w->rgn == VC_RGN_BOBJ) for (u32 q = 1; q < 8; q++) x[n++] = q;
  return n;
}

/* exactly the row's pages mapped (each to the bank's pages in order, plus the mirrors), nothing else anywhere */
static int check_row(const Row *w, VCoreState *s) {
  u32 x[8];
  int nx = mirrors(w, x);
  for (int r = 0; r < VC_NREGIONS; r++) {
    const VC_Region *g = &s->rgn[r];
    for (u32 p = 0; p < g->npages; p++) {
      u32 from = 0;
      int in = 0;
      if (r == w->rgn) {
        if (p >= w->at && p < w->at + w->n) in = 1, from = w->at;
        for (int k = 0; k < nx; k++)
          if (p >= x[k] && p < x[k] + w->n) in = 1, from = x[k];
      }
      if (in) {
        if (g->page[p].n != 1) return 0;
        if (g->page[p].src[0] != vc_bank(s, w->bank) + (size_t)(p - from) * VC_PAGE_SIZE) return 0;
      } else if (g->page[p].n != 0) return 0;
    }
  }
  return 1;
}

VCT_TEST(p12_every_row) {
  VCoreState *s = vct_state();
  int nrows = (int)(sizeof rows / sizeof rows[0]);
  for (int i = 0; i < nrows; i++) {
    const Row *w = &rows[i];
    memset(s->io9 + VC_REG_VRAMCNT_A, 0, 10);
    s->io9[VC_REG_VRAMCNT_A + (w->bank >= VC_BANK_H ? w->bank + 1 : w->bank)] = w->cnt;
    vc_vram_map(s);
    if (!check_row(w, s)) {
      vct_fail(__FILE__, __LINE__, "row %d (bank %d, cnt 0x%02x) maps wrongly", i, w->bank, w->cnt);
      return;
    }
  }
}

VCT_TEST(p12_region_sizes) {
  VCoreState *s = vct_state();
  vc_vram_map(s);
  static const u32 n[VC_NREGIONS] = {32, 16, 8, 8, 41, 2, 1, 2, 1, 32, 6};
  for (int r = 0; r < VC_NREGIONS; r++) VCT_EQ(s->rgn[r].npages, n[r]);
}

VCT_TEST(p12_disabled_unmapped) {
  VCoreState *s = vct_state();
  for (int b = 0; b < 10; b++) s->io9[VC_REG_VRAMCNT_A + b] = 0x01; /* MST 1, enable bit clear */
  vc_vram_map(s);
  for (int r = 0; r < VC_NREGIONS; r++)
    for (u32 p = 0; p < s->rgn[r].npages; p++) VCT_EQ(s->rgn[r].page[p].n, 0);
  VCT_CHECK(vc_vram_lcdc(s, VC_BANK_A) == NULL);
}

VCT_TEST(p12_overlap_or) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 0] = CNT(1, 0); /* A: engine A BG 0 */
  s->io9[VC_REG_VRAMCNT_A + 1] = CNT(1, 0); /* B: also at 0 */
  s->io9[VC_REG_VRAMCNT_A + 4] = CNT(1, 0); /* E: also at 0, pages 0-3 */
  vc_bank(s, VC_BANK_A)[5] = 0x01;
  vc_bank(s, VC_BANK_B)[5] = 0x10;
  vc_bank(s, VC_BANK_E)[5] = 0x40;
  vc_bank(s, VC_BANK_B)[0x4005] = 0x22; /* page 1: A, B and E all there */
  vc_bank(s, VC_BANK_E)[0x4005] = 0x04;
  vc_vram_map(s);
  VCT_EQ(s->rgn[VC_RGN_ABG].page[0].n, 3);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_ABG], 5), 0x51);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_ABG], 0x4005), 0x26);
  VCT_EQ(s->rgn[VC_RGN_ABG].page[4].n, 2); /* only A and B reach page 4 */
  VCT_EQ(vc_rd16(&s->rgn[VC_RGN_ABG], 4), 0x5100);
}

VCT_TEST(p12_read_through_region) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 5] = CNT(1, 2); /* F at engine A BG page 4 (10000h) */
  u8 *f = vc_bank(s, VC_BANK_F);
  f[0] = 0xAB; f[1] = 0xCD; f[2] = 0x12; f[3] = 0x34; f[0x3FFF] = 0x77;
  vc_vram_map(s);
  const VC_Region *g = vc_rgn_bg(s, 0);
  VCT_EQ(vc_rd8(g, 0x10000), 0xAB);
  VCT_EQ(vc_rd16(g, 0x10000), 0xCDAB);
  VCT_EQ(vc_rd32(g, 0x10000), 0x3412CDAB);
  VCT_EQ(vc_rd8(g, 0x13FFF), 0x77);
  VCT_EQ(vc_rd8(g, 0x14000), 0); /* next page unmapped */
  VCT_EQ(vc_rd8(g, 0x18000), 0xAB); /* the oracle mirrors a 16K bank 32K up */
  VCT_EQ(vc_rd8(g, 0x1BFFF), 0x77);
  VCT_EQ(vc_rd8(g, 0xFFFF), 0);
  VCT_EQ(vc_rd8(g, 0x80000 + 0x10000), 0xAB); /* the address wraps at 512K */
}

VCT_TEST(p12_engine_b_bg_and_wrap) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 8] = CNT(1, 0); /* H: B BG 0..32K */
  s->io9[VC_REG_VRAMCNT_A + 9] = CNT(1, 0); /* I: B BG 32K..48K */
  vc_bank(s, VC_BANK_H)[0x7FFF] = 0x11;
  vc_bank(s, VC_BANK_I)[0] = 0x22;
  vc_vram_map(s);
  const VC_Region *g = vc_rgn_bg(s, 1);
  VCT_EQ(vc_rd8(g, 0x7FFF), 0x11);
  VCT_EQ(vc_rd8(g, 0x8000), 0x22);
  VCT_EQ(vc_rd8(g, 0x20000 + 0x8000), 0x22); /* wraps at 128K */
  VCT_EQ(vc_rd8(g, 0xC000), 0x22);           /* I is mirrored 16K up (the oracle) */
  VCT_EQ(vc_rd8(g, 0x1FFFF - 0x8000), 0x11);  /* H repeats at 64K */
  VCT_EQ(vc_rd8(g, 0x17FFF), 0x11);
  VCT_EQ(vc_rd8(g, 0x1C000), 0x22);
}

VCT_TEST(p12_wramcnt_ignored) {
  VCoreState *s = vct_state();
  s->io9[0x247] = 0xFF;
  s->io9[VC_REG_VRAMCNT_A + 6] = CNT(1, 0); /* G */
  vc_vram_map(s);
  VCT_EQ(s->rgn[VC_RGN_ABG].page[0].n, 1);
  VCT_CHECK(s->rgn[VC_RGN_ABG].page[0].src[0] == vc_bank(s, VC_BANK_G));
  VCT_EQ(s->rgn[VC_RGN_BBG].page[0].n, 0); /* H reads 0x248, not 0x247 */
}

VCT_TEST(p12_lcdc_access) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 0] = CNT(0, 0);
  s->io9[VC_REG_VRAMCNT_A + 1] = CNT(1, 0);
  s->io9[VC_REG_VRAMCNT_A + 2] = 0x00; /* C disabled */
  s->io9[VC_REG_VRAMCNT_A + 3] = CNT(0, 0);
  VCT_CHECK(vc_vram_lcdc(s, VC_BANK_A) == vc_bank(s, VC_BANK_A));
  VCT_CHECK(vc_vram_lcdc(s, VC_BANK_B) == NULL);
  VCT_CHECK(vc_vram_lcdc(s, VC_BANK_C) == NULL);
  VCT_CHECK(vc_vram_lcdc(s, VC_BANK_D) == vc_bank(s, VC_BANK_D));
  VCT_CHECK(vc_vram_lcdc(s, 9) == NULL);
  s->io9[0x248] = CNT(0, 0);
  VCT_CHECK(vc_vram_lcdc(s, VC_BANK_H) == vc_bank(s, VC_BANK_H));
  vc_bank(s, VC_BANK_D)[0x10] = 0x5A;
  vc_vram_map(s);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_LCDC], 0x60010), 0x5A);
}

VCT_TEST(p12_extpal_reads) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 4] = CNT(4, 0);   /* E: A BG ext palettes */
  s->io9[VC_REG_VRAMCNT_A + 5] = CNT(5, 0);   /* F: A OBJ ext palette */
  s->io9[VC_REG_VRAMCNT_A + 8] = CNT(2, 0);   /* H: B BG ext */
  s->io9[VC_REG_VRAMCNT_A + 9] = CNT(3, 0);   /* I: B OBJ ext */
  vc_bank(s, VC_BANK_E)[0x2000 * 3 + 2] = 0x31; /* slot 3 */
  vc_bank(s, VC_BANK_F)[0x1FFE] = 0x42;
  vc_bank(s, VC_BANK_H)[0x6000] = 0x53;
  vc_bank(s, VC_BANK_I)[0x10] = 0x64;
  vc_vram_map(s);
  VCT_EQ(vc_rd8(vc_rgn_bgext(s, 0), 0x6002), 0x31);
  VCT_EQ(vc_rd8(vc_rgn_objext(s, 0), 0x1FFE), 0x42);
  VCT_EQ(vc_rd8(vc_rgn_bgext(s, 1), 0x6000), 0x53);
  VCT_EQ(vc_rd8(vc_rgn_objext(s, 1), 0x10), 0x64);
}

VCT_TEST(p12_texture_reads) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 2] = CNT(3, 2); /* C -> texture slot 2 */
  s->io9[VC_REG_VRAMCNT_A + 5] = CNT(3, 3); /* F -> texture palette slot 5 */
  vc_bank(s, VC_BANK_C)[0x1FFF0] = 0x9C;
  vc_bank(s, VC_BANK_F)[0x20] = 0x3E;
  vc_vram_map(s);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_TEX], 0x40000 + 0x1FFF0), 0x9C);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_TEX], 0x1FFF0), 0);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_TEXPAL], 5 * 0x4000 + 0x20), 0x3E);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_TEXPAL], 0x20), 0);
}

VCT_TEST(p12_remap_clears) {
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 0] = CNT(1, 0);
  vc_vram_map(s);
  VCT_EQ(s->rgn[VC_RGN_ABG].page[0].n, 1);
  s->io9[VC_REG_VRAMCNT_A + 0] = CNT(3, 1);
  vc_vram_map(s);
  VCT_EQ(s->rgn[VC_RGN_ABG].page[0].n, 0);
  VCT_EQ(s->rgn[VC_RGN_TEX].page[8].n, 1);
}

VCT_TEST(p12_aobjext_last_bank_wins) {
  /* the oracle keeps only the last of F, G mapped to the A OBJ ext palette: no OR (probed with tagged palettes) */
  VCoreState *s = vct_state();
  s->io9[VC_REG_VRAMCNT_A + 5] = CNT(5, 0);
  s->io9[VC_REG_VRAMCNT_A + 6] = CNT(5, 1);
  vc_bank(s, VC_BANK_F)[0x10] = 0x0F;
  vc_bank(s, VC_BANK_G)[0x10] = 0xF0;
  vc_vram_map(s);
  VCT_EQ(s->rgn[VC_RGN_AOBJEXT].page[0].n, 1);
  VCT_EQ(vc_rd8(&s->rgn[VC_RGN_AOBJEXT], 0x10), 0xF0);
}
