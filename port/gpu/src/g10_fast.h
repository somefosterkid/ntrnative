/* port/gpu piece G10: the fast 3D renderer's CPU half (docs/gpu-hwraster.md): the frame's polygons as triangles for the GPU's own
 * rasterizer, in drawing order, instead of span records. Its fragments run pass R's per-pixel rules (G4, G5) on the pixel's state
 * read from the colour attachment (Metal's programmable blending: src/g10_fast_mtl.inc, shaders/g10_*).
 *
 * A triangle vertex is 3 float4 (GPU_FAST_V floats): (x, y, z, w), (c0, c1, c2, s), (t, k, flags, 0): the position on the N k
 * grid, the 24-bit z, the clip w, the 9-bit vertex colour (16 c5 + 15, p16's), s and t in 12.4, k the polygon's index in the
 * table (drawing order), flags GPU_FAST_*. A polygon is a fan, its vertices shifted by half a pixel so the GPU's pixel-centre
 * sample tests the DS's integer point (the DS fills from the left edge's pixel to the right edge's, top row included); a 1-dot
 * polygon is a block, a line polygon (zero area) and each edge of a wire-frame one (alpha 0) a band one DS pixel (Line=wide) or
 * one N pixel (Line=thin) wide, all of their pixels edge pixels as p16 draws them. A filled polygon's fragment finds its edge
 * pixels (the edge flag, the coverage) from the polygon's edge functions (pe): a pixel less than a pixel from an edge along the
 * edge's minor axis, so a 4-neighbour outside: the DS's edge runs and each row's ends. Fan triangle i (vertices 0, i, i + 1)
 * looks at polygon edges i - 1, i and i + 1 only (edge j: vertex j to j + 1), so a pixel at vertex 0 of a middle triangle may
 * miss the edge flag of the two edges there. */
#ifndef GPU_G10_FAST_H
#define GPU_G10_FAST_H

#include "g02_spans.h"

enum { GPU_FAST_V = 12 };
enum { GPU_FAST_ALLEDGE = 1, GPU_FAST_COV0 = 2 }; /* every pixel an edge pixel (lines, dots); coverage 0 (dots); flags >> 4 & 15:
                                                    * a filled polygon's vertices, flags >> 8: the fan triangle's i */

typedef struct GpuFast {
  float *tri;  /* triangles: ntri vertices */
  int ntri, captri;
  float (*pe)[4]; /* polygon k's edge j at VC_POLY_MAXV k + j: a, b, c of a x + b y + c, the distance inside along the minor axis */
  int cappe;
} GpuFast;

/* the polygon table (sp->poly, sp->npoly: as gpu_spans_build makes it, without rows) and f's vertices for the polygons of s->geo
 * at scale (the N k grid), aa = k, dot_block / line_wide the Dot and Line looks. -1 when out of memory */
int gpu_fast_build(GpuFast *f, GpuSpans *sp, const VCoreState *s, int scale);
void gpu_fast_free(GpuFast *f);

#endif
