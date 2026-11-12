/* port/vcore piece 14: vertices, polygon assembly, clipping, viewport (docs/vcore.md, piece 14). OWNED BY PIECE 14:
 * its own state lives here; the functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P14_H
#define VC_P14_H

#include "vc_types.h"

/* a vertex waiting in the BEGIN_VTXS list (clip coordinates, attributes) and where it sits in vertex RAM (-1: not yet) */
typedef struct VC_P14Pend {
  VC_Vertex v;
  int ram;
  int vpok;  /* v's sx, sy, sz are its viewport transform for the viewport vpkey (a strip's vertex is in several polygons) */
  u32 vpkey;
} VC_P14Pend;

typedef struct VC_P14 {
  u32 attr_pending;   /* POLYGON_ATTR as written (takes effect at BEGIN_VTXS) */
  u32 attr;           /* POLYGON_ATTR in force */
  u32 texparam, pltt; /* TEXIMAGE_PARAM, PLTT_BASE */
  int vp_x1, vp_y1, vp_x2, vp_y2; /* VIEWPORT (y from the bottom) */
  int in_list;        /* between BEGIN_VTXS and END_VTXS */
  int type;           /* the list's primitive type */
  int count;          /* vertices received since BEGIN_VTXS */
  int polyn;          /* polygons made in this list (strip parity) */
  s32 cur[3];         /* the last vertex's x, y, z (20.12) for VTX_XY/XZ/YZ/DIFF */
  VC_P14Pend ring[4]; /* the last vertices of the list, oldest first */
  s32 pos_result[4];  /* POS_TEST's result (clip coordinates) */
} VC_P14;

/* ---- helpers the unit tests (and nobody else) use ---- */
/* clips a convex polygon of n vertices (clip coordinates, attributes) against the six planes of the view volume
 * (GBATEK 44, "Clipping"); out holds up to 10 vertices, new ones have clipped = 1. Returns the count (< 3: nothing
 * left). far_ok = POLYGON_ATTR bit 12: when 0 a polygon with a vertex beyond the far plane is dropped (returns 0). */
int vc_p14_clip(const VC_Vertex *in, int n, int far_ok, VC_Vertex *out);
/* sx, sy and sz of v from its clip coordinates and the viewport (x1, y1, x2, y2 as VIEWPORT sends them) */
void vc_p14_viewport(VC_Vertex *v, int x1, int y1, int x2, int y2);
/* the sign of the polygon's facing from its first three vertices: > 0 front (anti-clockwise), < 0 back, 0 degenerate */
int vc_p14_facing(const VC_Vertex *a, const VC_Vertex *b, const VC_Vertex *c);

#endif
