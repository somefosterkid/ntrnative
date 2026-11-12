/* port/vcore piece 9: the compositor and the colour effects (docs/vcore.md, piece 9). OWNED BY PIECE 9.
 * Spec: GBATEK 09 (BLDCNT/BLDALPHA/BLDY, semi-transparent OBJs), 55 (the 3D layer), 26 (bitmap OBJ alpha), 29 (the
 * layering block), 04 (BGxCNT priority). */
#include "vc_shared.h"

/* ---- colour arithmetic (6 bits per channel) ---- */
static inline u32 mk(int r, int g, int b) { return VC_RGB6(r, g, b); }

u32 vc_blend_alpha(u32 c1, u32 c2, int eva, int evb) {
  /* GBATEK 09: I = MIN(31, I1st*EVA + I2nd*EVB), EVA/EVB in 1/16 */
  int r = (int)((VC_R6(c1) * eva + VC_R6(c2) * evb + 8) >> 4);
  int g = (int)((VC_G6(c1) * eva + VC_G6(c2) * evb + 8) >> 4);
  int b = (int)((VC_B6(c1) * eva + VC_B6(c2) * evb + 8) >> 4);
  if (r > 63) r = 63;
  if (g > 63) g = 63;
  if (b > 63) b = 63;
  return mk(r, g, b);
}

u32 vc_blend_bright(u32 c, int evy) {
  /* GBATEK 09: I = I1st + (31-I1st)*EVY */
  int r = VC_R6(c), g = VC_G6(c), b = VC_B6(c);
  r += ((63 - r) * evy + 8) >> 4;
  g += ((63 - g) * evy + 8) >> 4;
  b += ((63 - b) * evy + 8) >> 4;
  return mk(r, g, b);
}

u32 vc_blend_dark(u32 c, int evy) {
  /* GBATEK 09: I = I1st - I1st*EVY */
  int r = VC_R6(c), g = VC_G6(c), b = VC_B6(c);
  r -= (r * evy + 7) >> 4;
  g -= (g * evy + 7) >> 4;
  b -= (b * evy + 7) >> 4;
  return mk(r, g, b);
}

u32 vc_blend_3d(u32 c1, u32 c2, int a) {
  /* GBATEK 55: "probably EVA = A/2 .. EVB = 16 - A/2"; the 5-bit alpha taken as (a+1)/32 and (31-a)/32 */
  int r = (VC_R6(c1) * (a + 1) + VC_R6(c2) * (31 - a) + 16) >> 5;
  int g = (VC_G6(c1) * (a + 1) + VC_G6(c2) * (31 - a) + 16) >> 5;
  int b = (VC_B6(c1) * (a + 1) + VC_B6(c2) * (31 - a) + 16) >> 5;
  return mk(r, g, b);
}

/* ---- the compositor ---- */
enum { K_OBJ = 4, K_BD = 5 };

typedef struct Cand {
  int kind;  /* 0-3 BG, 4 OBJ, 5 backdrop */
  u32 color; /* RGB6 */
  u32 px;    /* the OBJ pixel (flags) or the 3D alpha in bits 24-28 */
  int is3d;
} Cand;

static inline int clamp16(int v) { return v > 16 ? 16 : v; }

static inline Cand mkcand(const VC_Line *ln, int slot, int x, u32 objpx) {
  Cand c;
  if (slot == 0) {
    c.kind = K_OBJ; c.color = vc_555_to_rgb6((u16)objpx); c.px = objpx; c.is3d = 0;
  } else {
    int n = slot - 1;
    if (n == 0 && ln->bg3d) {
      u32 a3 = ln->l3d[x];
      c.kind = 0; c.color = a3 & 0xFFFFFF; c.px = a3; c.is3d = 1;
    } else {
      u32 px = ln->bg[n][x];
      c.kind = n; c.color = vc_555_to_rgb6((u16)px); c.px = px; c.is3d = 0;
    }
  }
  return c;
}

void vc_compose_line(VCoreState *s, int e, int y, VC_Line *ln) {
  (void)y;
  u32 dispcnt = vc_eng32(s, e, VC_REG_DISPCNT);
  u16 bgcnt[4];
  for (int n = 0; n < 4; n++) bgcnt[n] = vc_eng16(s, e, VC_REG_BG0CNT + 2 * n);
  u16 bld = vc_eng16(s, e, VC_REG_BLDCNT);
  u16 alpha = vc_eng16(s, e, VC_REG_BLDALPHA);
  int evy = clamp16(vc_eng16(s, e, VC_REG_BLDY) & 0x1F);
  int eva = clamp16(alpha & 0x1F), evb = clamp16((alpha >> 8) & 0x1F);
  int mode = (bld >> 6) & 3;
  u32 backdrop = vc_555_to_rgb6(vc_pal(s, e, 0, 0));
  int obj_on = (dispcnt >> 12) & 1;
  int bgkey[4]; /* priority * 8 + slot of each BG that was drawn this line, -1 for none */
  for (int n = 0; n < 4; n++) bgkey[n] = ln->bg_on[n] ? (bgcnt[n] & 3) * 8 + n + 1 : -1;

  for (int x = 0; x < VC_W; x++) {
    u32 mask = ln->winmask[x];
    int ncand = 0;
    u32 objpx = obj_on && (mask & VC_WIN_OBJ) ? ln->obj[x] : 0;
    /* front to back: priority 0..3; at one priority the OBJ, then BG0..BG3 (GBATEK 04, 14). A layer's key is
     * priority * 8 + slot (slot 0 the OBJ, 1-4 the BGs), unique, so the two smallest keys are the first two found
     * in that order. */
    int k1 = 99, k2 = 99;
    if ((objpx & VC_PX_OPAQUE) && !((objpx & VC_PX_BMPOBJ) && !((objpx >> VC_PX_ALPHA_SHIFT) & 15))) {
      /* (a bitmap OBJ with alpha 0 is invisible: oracle) */
      k1 = (int)VC_PX_OBJPRIO(objpx) * 8;
    }
    for (int n = 0; n < 4; n++) {
      if (bgkey[n] < 0 || !(mask & (1u << n))) continue;
      if (n == 0 && ln->bg3d) {
        if (!VC_A5(ln->l3d[x])) continue; /* 3D alpha 0: transparent */
      } else if (!(ln->bg[n][x] & VC_PX_OPAQUE)) {
        continue;
      }
      int k = bgkey[n];
      if (k < k1) { k2 = k1; k1 = k; }
      else if (k < k2) k2 = k;
    }
    Cand top, sec;
    if (k1 < 99) {
      top = mkcand(ln, k1 & 7, x, objpx);
      ncand = 1;
      if (k2 < 99) {
        sec = mkcand(ln, k2 & 7, x, objpx);
        ncand = 2;
      }
    }
    /* the backdrop lies below every layer */
    Cand bd = {K_BD, backdrop, 0, 0};
    if (ncand == 0) { top = bd; sec.kind = -1; sec.color = 0; sec.px = 0; sec.is3d = 0; }
    else if (ncand == 1) sec = bd;

    int tgt1 = (bld >> top.kind) & 1;
    int tgt2 = sec.kind >= 0 && ((bld >> (8 + sec.kind)) & 1);
    int fx = (mask & VC_WIN_EFFECT) != 0;
    u32 col = top.color;
    int semi = top.kind == K_OBJ && (top.px & (VC_PX_SEMI | VC_PX_BMPOBJ));
    if (semi && tgt2 && sec.kind >= 0 && !(top.px & VC_PX_BMPOBJ)) {
      col = vc_blend_alpha(top.color, sec.color, eva, evb);
    } else if (semi && tgt2 && sec.kind >= 0) {
      int a = (int)((top.px >> VC_PX_ALPHA_SHIFT) & 15);
      col = vc_blend_alpha(top.color, sec.color, a + 1, 16 - (a + 1));
    } else if (top.is3d && tgt2 && sec.kind >= 0) {
      col = vc_blend_3d(top.color, sec.color, (int)VC_A5(top.px));
    } else if (fx && tgt1) {
      if (mode == 1) {
        if (tgt2 && sec.kind >= 0) col = vc_blend_alpha(top.color, sec.color, eva, evb);
      } else if (mode == 2) col = vc_blend_bright(top.color, evy);
      else if (mode == 3) col = vc_blend_dark(top.color, evy);
    }
    ln->out[x] = col;
  }
}
