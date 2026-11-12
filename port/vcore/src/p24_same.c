/* port/vcore piece 24: is an engine's 2D picture the same as another state's? (docs/gpu-2d.md, option 1). OWNED BY PIECE 24.
 *
 * An engine's 1x lines (vc_render_line; the GPU path's engine A layers, g07_stage_build) are a function of the state's inputs
 * alone (no piece keeps state between frames: VC_P01-VC_P12 are empty). Of those, engine e reads: its 2D registers
 * (0x000-0x06F of its block: DISPCNT, the BG and window registers, mosaic, blending, DISPCAPCNT for A, MASTER_BRIGHT), POWCNT1
 * (the engines' power and the screen swap), VRAMCNT A-I (which banks its regions show), its OAM and its palettes, the bytes of
 * the banks mapped in its BG, OBJ and extended palette regions, and for engine A in display mode 2 the bank it shows. The 3D
 * layer (s->f3d) is not compared: the GPU path composes it apart, and vcore's bridge asks only about engine B. */
#include "vc_shared.h"

#include <string.h>

int vc_2d_same(const VCoreState *a, const VCoreState *b, int e) {
  const u8 *ra = e ? a->io9b : a->io9, *rb = e ? b->io9b : b->io9;
  if (memcmp(ra, rb, VC_IO9B_SIZE) != 0) return 0;
  if (memcmp(a->io9 + VC_REG_POWCNT1, b->io9 + VC_REG_POWCNT1, 2) != 0) return 0;
  if (memcmp(a->io9 + VC_REG_VRAMCNT_A, b->io9 + VC_REG_VRAMCNT_A, 10) != 0) return 0; /* A-G, WRAMCNT, H, I */
  if (memcmp(a->oam + e * 0x400, b->oam + e * 0x400, 0x400) != 0) return 0;
  if (memcmp(a->pltt + e * 0x400, b->pltt + e * 0x400, 0x400) != 0) return 0;
  /* the same VRAMCNT maps the same banks: the pages are at the same offsets in both states' vram */
  static const int kRgn[2][4] = {{VC_RGN_ABG, VC_RGN_AOBJ, VC_RGN_ABGEXT, VC_RGN_AOBJEXT},
                                 {VC_RGN_BBG, VC_RGN_BOBJ, VC_RGN_BBGEXT, VC_RGN_BOBJEXT}};
  for (int k = 0; k < 4; k++) {
    const VC_Region *r = &a->rgn[kRgn[e][k]];
    for (u32 pg = 0; pg < r->npages; pg++)
      for (int i = 0; i < r->page[pg].n; i++) {
        size_t at = (size_t)(r->page[pg].src[i] - a->vram);
        if (memcmp(a->vram + at, b->vram + at, VC_PAGE_SIZE) != 0) return 0;
      }
  }
  u32 dispcnt = vc_eng32(a, e, VC_REG_DISPCNT);
  if (e == VC_ENG_A && ((dispcnt >> 16) & 3) == 2) { /* display mode 2: a bank in LCDC mode is the picture */
    int bank = VC_BANK_A + (int)((dispcnt >> 18) & 3);
    size_t at = vc_bank_offset[bank];
    if (memcmp(a->vram + at, b->vram + at, vc_bank_size[bank]) != 0) return 0;
  }
  return 1;
}
