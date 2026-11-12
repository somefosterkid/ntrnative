/* port/vcore piece 21: the 3D at N x the DS resolution on the CPU (docs/vcore.md, "vcore at N x"). OWNED BY PIECE 21.
 *
 * The 3D pipeline (p16 rows, p17 texture, p18 plot, p19 clear/edge/fog/AA) on a 256 N x 192 N grid, and engine A's compositing
 * at N x around it, following port/gpu's N x rules (docs/gpu.md, "The DS rasterizer at N x"): the polygons' vertices land on
 * the N x grid (the viewport transform of the clip coordinates at N times the viewport), p16's own edge walk runs on that
 * grid, everything per pixel is the 1x code's. The 2D layers stay the DS's pixels (nearest, N x N blocks); engine B and the
 * other display modes are the 1x screens scaled by N. At N = 1 (when forced with BWPORT_VCORE_NPATH=1, for checking) the
 * picture equals the 1x core's. The 1x code is not touched by this piece: scale 1 runs vc_render_frame. */
#ifndef VC_P21_H
#define VC_P21_H

#include "vc_types.h"

enum { VC_MAX_SCALE = 8 };

struct VCoreState;
struct VC_NFrame;

typedef struct VC_P21 {
  int scale;     /* 0 / 1: the DS's resolution (vc_render_frame); 2..VC_MAX_SCALE: the grid the 3D is drawn on (the picture's
                    scale N times aa) */
  int aa;        /* supersampling k = 1, 2, 4: the picture is the k x k box average of the grid (scale = N k) */
  int out_want, aa_want; /* vcore_set_scale / vcore_set_aa (aa is lowered when N k > VC_MAX_SCALE) */
  int dot_fine;  /* a polygon that collapses to one DS pixel draws one N x pixel (default: an N x N block, the DS look) */
  int line_thin; /* lines and wire-frames are 1 N x pixel wide (default: N wide, walked on the DS grid) */
  int edge_fine; /* edge marking outlines one N x pixel (default: N pixels, one DS pixel) */
  int filter_bilinear; /* textures sampled bilinear on the decoded texels at N > 1 (default: nearest, the DS look) */
  int force_n;   /* run the N path at scale 1 (checking: BWPORT_VCORE_NPATH=1) */
  struct VC_NFrame *f;
} VC_P21;

/* 1 when vcore_render runs the N x frame (vc_render_frame_n) */
int vc_p21_active(const struct VCoreState *s);
void vc_p21_destroy(struct VCoreState *s);

/* ---- the pieces of the N x frame, called by p20's vc_render_frame_n ---- */
/* the buffers for s->p21.scale (kept between frames); 0 when out of memory */
int vc_n_begin(struct VCoreState *s);
/* the frame has no complete 3D frame: the N x 3D layer is empty */
void vc_n_none(struct VCoreState *s);
/* after the geometry (s->geo is complete and sorted): each polygon's walk on the N x grid */
void vc_n_plan(struct VCoreState *s);
void vc_n_clear(struct VCoreState *s);
void vc_n_clear_rows(struct VCoreState *s, int y0, int y1);
/* the rows of the N x frame cut into `nstrip` strips of about equal cost: ys[0..nstrip] */
void vc_n_plan_strips(struct VCoreState *s, int nstrip, int *ys);
/* all polygons, in order, on the N x rows y0 <= y < y1 (rows are independent) */
void vc_n_raster_rows(struct VCoreState *s, int y0, int y1);
void vc_n_post(struct VCoreState *s);
int vc_n_post_begin(struct VCoreState *s); /* the number of phases */
void vc_n_post_rows(struct VCoreState *s, int phase, int y0, int y1);
/* the picture's rows of DS rows y0 <= y < y1: engine A (a_top: on the top screen) composited at N x when its line has the
 * 3D layer, else its 1x line scaled; engine B's 1x line scaled */
void vc_n_picture_rows(struct VCoreState *s, int a_top, int y0, int y1);
/* the picture from the 1x screens (s->screen) scaled by N */
void vc_n_picture_from_screens(struct VCoreState *s);
void vc_n_picture_done(struct VCoreState *s); /* the picture is complete (vcore_picture returns it) */

#endif
