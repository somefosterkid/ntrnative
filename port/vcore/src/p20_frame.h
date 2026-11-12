/* port/vcore piece 20: integration: the frame, the 3D layer, the screens (docs/vcore.md, piece 20). OWNED BY PIECE 20: its own state lives here; the
 * functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P20_H
#define VC_P20_H

#include "vc_types.h"

typedef struct VC_P20 {
  VC_Line stage[VC_H]; /* engine A's lines between the halves of vc_render_line_a1 / _a2 */
  int stage_ok;        /* the last render left stage[] valid and the 2D done: 1 = 1x split frame, 2 = N x split frame (vc_render_3d_again) */
  VC_Line line_b;      /* engine B's line buffer (its thread) */
  void *pool;          /* the rasterizer's persistent threads (p20_frame.c; freed by vc_p20_destroy) */
  const u32 (*keep_b)[VC_W]; /* set by the caller: engine B's picture of an earlier state whose engine B inputs equal this one's
                                (vc_2d_same): copied into eng_out[1], not drawn (port/bridge/vcore_bridge.c); NULL: drawn */
} VC_P20;

enum { VC_ST_MAP, VC_ST_GEO, VC_ST_CLEAR, VC_ST_RASTER, VC_ST_POST, VC_ST_2DA, VC_ST_2DB, VC_NSTAGE };
extern int vc_threads;
extern int vc_bands; /* rows split in this many bands for the rasterizer threads (default 4; env BWPORT_VCORE_BANDS) */
enum { VC_NWALL = 5 }; /* N x: plan, clear, raster, post, picture */
extern double vc_nwall_ns[VC_NWALL];
struct VCoreState;
int vc_render_3d_again(struct VCoreState *s);
extern double vc_stage_ns[VC_NSTAGE]; /* summed per stage by vc_render_frame, for vcore_render --bench */

struct VCoreState;
void vc_p20_destroy(struct VCoreState *s);

#endif
