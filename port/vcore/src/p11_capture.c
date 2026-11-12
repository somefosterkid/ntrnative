/* port/vcore piece 11: display capture (docs/vcore.md, piece 11). OWNED BY PIECE 11.
 * Spec: GBATEK 28 (DISPCAPCNT, the capture formulas), 29 (the pipeline), 22 (VRAMCNT: LCDC = MST 0). */
#include "vc_shared.h"

/* GBATEK 28: capture size field -> width, height */
static const u16 kCapW[4] = {128, 256, 256, 256};
static const u16 kCapH[4] = {128, 64, 128, 192};

/* GBATEK 22: a bank is in LCDC mode when VRAMCNT's enable bit (7) is set and MST (bits 0-2) is 0. Banks A-D only here.
 * (Same rule as vc_vram_lcdc, kept local so this piece stands alone.) */
static u8 *cap_bank(VCoreState *s, int bank) {
  u8 cnt = s->io9[VC_REG_VRAMCNT_A + bank];
  if (!(cnt & 0x80) || (cnt & 7) != 0) return NULL;
  return vc_bank(s, bank);
}

/* GBATEK 28: "Capture data is 15bit color depth": an RGB6 value loses its low bit */
u16 vc_cap_rgb6_to_555(u32 rgb6) {
  return (u16)(((rgb6 >> 1) & 0x1F) | (((rgb6 >> 9) & 0x1F) << 5) | (((rgb6 >> 17) & 0x1F) << 10));
}

/* GBATEK 28 "Capture A+B (blending)": per channel (A * alphaA * EVA + B * alphaB * EVB) / 16, the alpha an on/off bit.
 * a, b: 15-bit colour in bits 0-14, alpha in bit 15. Result likewise. */
u16 vc_cap_blend(u16 a, u16 b, int eva, int evb) {
  if (eva > 16) eva = 16;
  if (evb > 16) evb = 16;
  int aa = (a >> 15) & 1, ab = (b >> 15) & 1;
  u32 out = 0;
  for (int c = 0; c < 3; c++) {
    int ca = (a >> (5 * c)) & 31, cb = (b >> (5 * c)) & 31;
    int v = (ca * aa * eva + cb * ab * evb + 8) / 16; /* oracle: rounds half up (GBATEK says /16) */
    if (v > 31) v = 31;
    out |= (u32)v << (5 * c);
  }
  if ((aa && eva > 0) || (ab && evb > 0)) out |= 0x8000;
  return (u16)out;
}

void vc_capture_line(VCoreState *s, int y, const VC_Line *ln) {
  u32 cnt = vc_io32(s, VC_REG_DISPCAPCNT);
  if (!(cnt & 0x80000000u)) return;
  int size = (cnt >> 20) & 3;
  int w = kCapW[size], h = kCapH[size];
  if (y >= h) return;
  int mode = (cnt >> 29) & 3;
  u8 *dst = cap_bank(s, (cnt >> 16) & 3);
  if (dst == NULL) return; /* destination must be allocated to LCDC */
  u32 woff = ((cnt >> 18) & 3) * 0x8000u;
  int eva = cnt & 0x1F, evb = (cnt >> 8) & 0x1F;

  u32 dispcnt = vc_eng32(s, 0, VC_REG_DISPCNT);
  const u8 *src = NULL;
  u32 roff = 0;
  if (mode != 0) {
    if (!(cnt & (1u << 25))) src = cap_bank(s, (dispcnt >> 18) & 3);
    /* GBATEK 28: the read offset is ignored (zero) in VRAM display mode */
    if (((dispcnt >> 16) & 3) != 2) roff = ((cnt >> 26) & 3) * 0x8000u;
  }
  for (int x = 0; x < w; x++) {
    u16 pa = 0, pb = 0;
    if (mode != 1) {
      if (cnt & (1u << 24)) {
        u32 p = s->f3d.out[y][x];
        pa = vc_cap_rgb6_to_555(p);
        if ((p >> 24) & 31) pa |= 0x8000;
      } else {
        pa = vc_cap_rgb6_to_555(ln->out[x]) | 0x8000;
      }
    }
    if (mode != 0 && src != NULL) {
      u32 a = (roff + (u32)(y * VC_W + x) * 2) & 0x1FFFF;
      pb = (u16)(src[a] | src[a + 1] << 8);
    }
    u16 v = mode == 0 ? pa : mode == 1 ? pb : vc_cap_blend(pa, pb, eva, evb);
    u32 d = (woff + (u32)(y * w + x) * 2) & 0x1FFFF;
    dst[d] = (u8)v;
    dst[d + 1] = (u8)(v >> 8);
  }
}

void vc_capture_end(VCoreState *s) {
  /* GBATEK 28: the enable/busy bit clears itself at line 192 */
  s->io9[VC_REG_DISPCAPCNT + 3] &= 0x7F;
}
