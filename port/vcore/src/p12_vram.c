/* port/vcore piece 12: VRAM bank mapping (docs/vcore.md, piece 12). OWNED BY PIECE 12.
 * Spec: GBATEK 22 (DS Memory Control - VRAM: VRAMCNT_A-I and the bank table), 27 (extended palettes), 21, 10.
 * VRAMCNT bytes: A-G at io9 0x240-0x246, (WRAMCNT 0x247 is ignored), H 0x248, I 0x249. Bit 7 enable, bits 3-4 OFS,
 * bits 0-2 MST (bit 2 unused by A, B, H, I). A page that several banks map reads their OR (VC_Page holds 4). */
#include "p12_vram.h"
#include "vc_shared.h"

#include <string.h>

enum { PG = VC_PAGE_SIZE };

/* map `n` pages of `bank` (from its page 0) into region `r` starting at page `at` */
static void put(VCoreState *s, int r, u32 at, int bank, u32 n) {
  VC_Region *g = &s->rgn[r];
  for (u32 i = 0; i < n; i++) {
    if (at + i >= g->npages) return; /* beyond the region */
    VC_Page *p = &g->page[at + i];
    if (p->n >= VC_PAGE_SRCS) continue; /* more than 4 banks on one page: the rest are dropped */
    p->src[p->n++] = vc_bank(s, bank) + (size_t)i * PG;
  }
}

static u32 bank_pages(int bank) { return (vc_bank_size[bank] + PG - 1) / PG; }

/* VRAMCNT register of a bank (WRAMCNT sits between G and H) */
static u8 cnt(const VCoreState *s, int bank) {
  return s->io9[VC_REG_VRAMCNT_A + (bank >= VC_BANK_H ? bank + 1 : bank)];
}

/* the MST of a bank: bit 2 is unused by A, B, H, I */
static int mst_of(int bank, u8 c) {
  int mst = c & 7;
  if (bank == VC_BANK_A || bank == VC_BANK_B || bank == VC_BANK_H || bank == VC_BANK_I) mst &= 3;
  return mst;
}

void vc_vram_map(VCoreState *s) {
  static const u32 npages[VC_NREGIONS] = {
      [VC_RGN_ABG] = 32,    [VC_RGN_AOBJ] = 16,    [VC_RGN_BBG] = 8,     [VC_RGN_BOBJ] = 8,
      [VC_RGN_LCDC] = 41,   [VC_RGN_ABGEXT] = 2,   [VC_RGN_AOBJEXT] = 1, [VC_RGN_BBGEXT] = 2,
      [VC_RGN_BOBJEXT] = 1, [VC_RGN_TEX] = 32,     [VC_RGN_TEXPAL] = 6,
  };
  for (int r = 0; r < VC_NREGIONS; r++) {
    memset(&s->rgn[r], 0, sizeof s->rgn[r]);
    s->rgn[r].npages = npages[r];
  }
  for (int b = 0; b < VC_NBANKS; b++) {
    u8 c = cnt(s, b);
    if (!(c & 0x80)) continue; /* disabled: unmapped */
    int mst = mst_of(b, c), ofs = (c >> 3) & 3;
    if (mst == 0) { /* LCDC: every bank at its own offset */
      put(s, VC_RGN_LCDC, vc_bank_offset[b] / PG, b, bank_pages(b));
      continue;
    }
    switch (b) {
    case VC_BANK_A:
    case VC_BANK_B:
    case VC_BANK_C:
    case VC_BANK_D:
      if (mst == 1) put(s, VC_RGN_ABG, (u32)ofs * 8, b, 8);                                   /* 6000000h + 20000h*OFS */
      else if (mst == 2 && b <= VC_BANK_B) put(s, VC_RGN_AOBJ, (u32)(ofs & 1) * 8, b, 8);      /* 6400000h + 20000h*OFS.0 */
      else if (mst == 3) put(s, VC_RGN_TEX, (u32)ofs * 8, b, 8);                              /* texture slot OFS */
      else if (mst == 4 && b == VC_BANK_C) put(s, VC_RGN_BBG, 0, b, 8);                       /* engine B BG, 6200000h */
      else if (mst == 4 && b == VC_BANK_D) put(s, VC_RGN_BOBJ, 0, b, 8);                      /* engine B OBJ, 6600000h */
      /* C, D with MST 2 are ARM7 work RAM: no video */
      break;
    case VC_BANK_E:
      if (mst == 1) put(s, VC_RGN_ABG, 0, b, 4);        /* 6000000h */
      else if (mst == 2) put(s, VC_RGN_AOBJ, 0, b, 4);  /* 6400000h */
      else if (mst == 3) put(s, VC_RGN_TEXPAL, 0, b, 4); /* texture palette slots 0-3 */
      else if (mst == 4) put(s, VC_RGN_ABGEXT, 0, b, 2); /* BG ext palette slots 0-3: the lower 32K */
      break;
    case VC_BANK_F:
    case VC_BANK_G: {
      u32 at = (u32)(ofs & 1) + (u32)(ofs >> 1) * 4; /* 4000h*OFS.0 + 10000h*OFS.1 in pages: 0, 1, 4, 5 */
      /* the oracle mirrors a 16K bank F/G 32K up in the 128K block (GBATEK is silent): pages at, at+2 */
      if (mst == 1) put(s, VC_RGN_ABG, at, b, 1), put(s, VC_RGN_ABG, at + 2, b, 1);
      else if (mst == 2) put(s, VC_RGN_AOBJ, at, b, 1), put(s, VC_RGN_AOBJ, at + 2, b, 1);
      else if (mst == 3) put(s, VC_RGN_TEXPAL, at, b, 1);           /* slot OFS.0*1 + OFS.1*4 */
      else if (mst == 4) put(s, VC_RGN_ABGEXT, (u32)(ofs & 1), b, 1); /* slots 0-1 (OFS 0), 2-3 (OFS 1) */
      else if (mst == 5) { /* the oracle keeps only the last bank mapped here (G over F), no OR, unlike the other regions */
        s->rgn[VC_RGN_AOBJEXT].page[0].n = 0;
        put(s, VC_RGN_AOBJEXT, 0, b, 1);
      }
      break;
    }
    case VC_BANK_H:
      /* the oracle repeats the 64K block H+I in the 128K engine B BG region: H at 0,1,4,5 */
      if (mst == 1) put(s, VC_RGN_BBG, 0, b, 2), put(s, VC_RGN_BBG, 4, b, 2);
      else if (mst == 2) put(s, VC_RGN_BBGEXT, 0, b, 2); /* slots 0-3 */
      break;
    case VC_BANK_I:
      if (mst == 1) { /* 6208000h, mirrored 16K up and repeated with the 64K block: pages 2, 3, 6, 7 */
        for (u32 q = 2; q < 8; q += (q == 3 ? 3 : 1)) put(s, VC_RGN_BBG, q, b, 1);
      } else if (mst == 2) for (u32 q = 0; q < 8; q++) put(s, VC_RGN_BOBJ, q, b, 1); /* the oracle repeats I over all of 6600000h-661FFFFh */
      else if (mst == 3) put(s, VC_RGN_BOBJEXT, 0, b, 1);
      break;
    }
  }
}

u8 *vc_vram_lcdc(VCoreState *s, int bank) {
  if (bank < 0 || bank >= VC_NBANKS) return NULL;
  u8 c = cnt(s, bank);
  if (!(c & 0x80) || mst_of(bank, c) != 0) return NULL;
  return vc_bank(s, bank);
}
