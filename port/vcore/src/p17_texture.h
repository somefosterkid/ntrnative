/* port/vcore piece 17: textures (docs/vcore.md, piece 17). OWNED BY PIECE 17: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P17_H
#define VC_P17_H

#include "vc_types.h"

struct VCoreState;

typedef struct VC_P17 {
  int unused; /* add the piece's state here */
} VC_P17;

/* a polygon's texture, decoded once for the rasterizer's per-pixel vc_tex_sample_c: the parameters, the
 * regions, the converted palette entries seen so far and the last texel. Built by vc_tex_ctx_init; read-only
 * VRAM, so every band thread has its own. */
typedef struct VC_TexCtx {
  const struct VCoreState *s;
  const struct VC_Poly *p;
  const VC_Region *tex, *pal;
  u32 fmt, sw, sh, base, pb, palsz, tmask;
  int rx, ry, fx, fy, t0, fast;
  s32 lx, ly; /* the last texel position (lx = -1: none) and its colour */
  u32 last;
  u32 lblk, bc[4]; /* format 5: the last 4x4 block and its four colours */
  u32 pc[256]; /* converted palette entries by index byte, 0xFFFFFFFF = not seen yet */
} VC_TexCtx;
void vc_tex_ctx_init(VC_TexCtx *c, const struct VCoreState *s, const struct VC_Poly *p);
u32 vc_tex_sample_c(VC_TexCtx *c, s32 ts, s32 tt);

#endif
