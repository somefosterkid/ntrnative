/* port/gpu 2D filter: the C reference (tools/up2d_ref.c). The same key, analysis and pick as shaders/g11_up*.glsl, in plain
 * integer C, applied to pass C's own inputs (its plan and 1x lines read back, the 3D layer at N x): up2d_ref_frame draws the
 * screens the way g07_compose.frag's UP program must, so a GPU picture can be compared with it pixel for pixel. */
#ifndef UP2D_REF_H
#define UP2D_REF_H

#include <stdint.h>

typedef struct UpRefIn {
  int scale;                            /* N */
  int compose, atop, have3d, hofs, evy; /* g07_up_inputs' fields (pass C's cCompose, cATop, cHave3d, cHofs, cEvy) */
  uint32_t bright;                      /* cBright */
  const uint32_t *plan;                 /* 256 x 192 x (x, y): the plan texture */
  const uint32_t *src;                  /* 256 x 384: the 1x lines (cSrc1x) */
  const uint32_t *l3d;                  /* the 3D layer at N x (256 N x 192 N), RGB6 | alpha5 << 24, antialiasing off */
  uint32_t *mm;                         /* MMPX's analysis (up2d_ref_prepare; NULL before) */
} UpRefIn;

typedef struct UpRefPick { /* one N x pixel's pick: show d + dA (mixed with d + dB by w / 64) */
  int8_t ax, ay, bx, by, w;
} UpRefPick;

typedef struct UpRefStats {
  long px, moved, blended;
} UpRefStats;

/* style: g07_compose.h's G07_UP_* (1 identity, 2 scale2x, 3 mmpx). rgba: 256 N x 384 N x 4 bytes (the top row first, the A
 * screen where cATop says); picks (or NULL): the same count of UpRefPick. */
void up2d_ref_frame(const UpRefIn *in, int style, uint8_t *rgba, UpRefPick *picks, UpRefStats *st);

/* a style's whole-frame analysis (MMPX: both stages, as the GPU's info textures), kept in in->mm until up2d_ref_release;
 * up2d_ref_pick needs it (up2d_ref_frame makes its own when in->mm is NULL). Nothing for the other styles. */
void up2d_ref_prepare(UpRefIn *in, int style);
void up2d_ref_release(UpRefIn *in);

/* one pick: DS pixel (dx, dy) (dy in engine order, 0-383), sub-pixel centre (fx, fy) in 1/2N steps */
UpRefPick up2d_ref_pick(const UpRefIn *in, int style, int dx, int dy, int fx, int fy);

/* up_key at DS pixel (x, y) (y 0-383, engine order), for the invariants */
uint32_t up2d_ref_key(const UpRefIn *in, int x, int y);

#endif
