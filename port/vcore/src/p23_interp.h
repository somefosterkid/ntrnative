/* port/vcore p23: in-between 3D frames (docs/interp.md, ip75). A research module: it records the vertices a
 * frame's command stream submits (pre-clip clip coordinates, colours, texture coordinates), pairs the BEGIN_VTXS..END_VTXS
 * lists of two consecutive frames, and lets the geometry engine blend a frame's vertices toward the next frame's before
 * clipping. Off unless a tool or BWPORT_INTERP_STATS turns it on; nothing in the player's path uses it. */
#ifndef VC_P23_H
#define VC_P23_H

#include "vc_types.h"
#include <stdio.h>

/* a vertex as new_vertex builds it, before clipping */
typedef struct VC_IpVtx {
  s32 clip[4];
  s16 tex[2];
  u8 color[3];
} VC_IpVtx;

/* a BEGIN_VTXS..END_VTXS list */
typedef struct VC_IpList {
  u32 first, n; /* its vertices in VC_IpRec.v */
  u32 attr, texparam, pltt, viewport;
  u8 type;
  u8 behind; /* some vertex has w <= 0 */
} VC_IpList;

typedef struct VC_IpRec {
  VC_IpVtx *v;
  u32 nv, capv;
  VC_IpList *l;
  u32 nl, capl;
  u32 swapped;   /* the stream held a complete frame (SWAP_BUFFERS) */
  u32 npoly;     /* polygons the geometry engine kept */
} VC_IpRec;

enum { VC_IP_OFF = 0, VC_IP_RECORD = 1, VC_IP_APPLY = 2 };

/* the tape (vc_ip_record writes it, vc_geo_replay reads it): what reaches the polygon assembly from the stream, in order. A vertex
 * before the blend is a function of the stream alone (matrices, lights, the texture state), so an in-between picture's geometry is
 * the tape's vertices blended and assembled again, with the matrix work, lighting and texcoord generation done once a keyframe */
enum { VC_TAPE_BEGIN, VC_TAPE_END, VC_TAPE_TEX, VC_TAPE_PLTT, VC_TAPE_VIEWPORT, VC_TAPE_VTX, VC_TAPE_SWAP };
typedef struct VC_IpTapeOp {
  u8 op, b; /* VC_TAPE_*; BEGIN: the list's type */
  u32 a;    /* BEGIN: POLYGON_ATTR; TEX, PLTT, VIEWPORT: the value as p14 holds it; SWAP: its parameter */
} VC_IpTapeOp;
/* a list on the tape: its ops [op0, op1) from its BEGIN to the END or BEGIN that ends it, its vertices [v0, v1) and what the
 * recorded run made of them: polygons [p0, p1) and vertex RAM entries [r0, r1) of the run's s->geo */
typedef struct VC_IpTapeList {
  u32 op0, op1, v0, v1, p0, p1, r0, r1;
  int swap; /* a SWAP_BUFFERS inside it */
} VC_IpTapeList;
typedef struct VC_IpTape {
  VC_IpTapeOp *op;
  u32 nop, capop;
  VC_Vertex *v; /* the VTX ops' vertices, as new_vertex built them (before the blend) */
  VC_Vertex *bv; /* the replay's: them blended */
  u32 nv, capv;
  VC_IpTapeList *l;
  u32 nl, capl;
  int open;     /* the last list is not ended yet (recording) */
  VC_Poly *gp;  /* the recorded run's s->geo: its polygons and vertex RAM (a list the blend leaves alone is copied from them) */
  VC_Vertex *gv;
  u32 gnp, gnv, capgp, capgv;
  u32 copied;                /* the last replay: lists copied */
  int gok;                   /* they can be copied (the run did not run out of polygon or vertex RAM) */
  int ok;                    /* the tape is the stream's whole run (vc_ip_record finished it) */
  s32 cur[3], pos_result[4]; /* p14's at the stream's end (the replay leaves them as a full run does) */
} VC_IpTape;

typedef struct VC_P23 {
  int mode;
  VC_IpRec rec;          /* RECORD: this frame's vertices */
  const VC_IpRec *dst;   /* APPLY: the other frame ... */
  const int *map;        /* ... and for each of this frame's lists the paired list there, or -1 */
  const u8 *texhold;     /* ... and for each of this frame's lists 1 = its texcoords step (a sprite's cell), do not blend them */
  s32 f;                 /* APPLY: the blend, 0..4096 (0 = this frame, 4096 = the other) */
  int list;              /* the list being submitted (both modes) */
  u32 k;                 /* vertices submitted in it */
  int open;
  VC_IpTape tape;        /* RECORD writes it (vc_ip_record) */
} VC_P23;

/* what vc_ip_pair found between frame a and the next frame b */
typedef struct VC_IpStats {
  int nl_a, nl_b;
  u32 nv_a, nv_b;
  int strict, loose;           /* lists paired by the full key / by shape (type, vertex count) only */
  int removed, added;          /* a's lists with no partner, b's lists with no partner */
  int jumps, behind;           /* paired lists dropped: moved more than the limit / crossing w = 0 with no vertex in front in both */
  u32 vt_paired, vt_removed, vt_added; /* vertices in the paired lists of a, the removed lists, the added lists */
  int ch_alpha, ch_attr, ch_tex, ch_pltt, ch_viewport; /* loose pairs by what differs */
  int same;                    /* every list paired and every vertex identical */
  double disp_mean, disp_max;  /* screen pixels (256 wide), over paired vertices */
  int swapped_a, swapped_b;
} VC_IpStats;

struct VCoreState;
void vc_ip_reset(struct VCoreState *s);                 /* the geometry engine starts a stream */
void vc_ip_begin(struct VCoreState *s);                 /* BEGIN_VTXS (p14) */
void vc_ip_end(struct VCoreState *s);                   /* END_VTXS */
void vc_ip_vertex(struct VCoreState *s, VC_Vertex *v);  /* a vertex built, pre-clip (p14 new_vertex) */
/* APPLY's blend of the n vertices of the open list from its k-th on, in into out (in may be out; vc_ip_vertex is n = 1): 1 when it
 * left them as they are (clip, colour and texcoords: what it changes) */
int vc_ip_blend_list(struct VCoreState *s, const VC_Vertex *in, VC_Vertex *out, u32 n);
void vc_ip_free(VC_IpRec *r);
/* runs the geometry engine over s's command stream and leaves the vertices in s->p23.rec (copy with vc_ip_take) */
void vc_ip_record(struct VCoreState *s);
void vc_ip_take(struct VCoreState *s, VC_IpRec *into);  /* moves s->p23.rec into *into */
/* pairs a's lists with b's: map[i] = b's list for a's list i or -1; limit = the most a paired list may move (px, 256 wide) */
void vc_ip_pair(const VC_IpRec *a, const VC_IpRec *b, int *map, double limit, VC_IpStats *st);
/* texhold[i] = 1 when a's list i, paired with b's list map[i], is at most maxpx (screen pixels, 256 wide) in size in both frames and changes a
 * texcoord by `thr` (1/16 texel) or more between them: a walking sprite's texture is an atlas of cells picked by texcoords, and blending two
 * cells' coordinates shows a strip of both. (A big surface that scrolls fast, the battle's floor, keeps its blend.) */
void vc_ip_texhold(const VC_IpRec *a, const VC_IpRec *b, const int *map, u8 *texhold, int thr, double maxpx);
/* RECORD: append to s->p23.tape (p14, p18) */
void vc_ip_tape_op(struct VCoreState *s, int op, int b, u32 a);
void vc_ip_tape_vtx(struct VCoreState *s, const VC_Vertex *v);
void vc_ip_tape_free(struct VCoreState *s);
/* the geometry of s's last vc_ip_record again from its tape (blended when vc_ip_apply_begin is in force): s->geo as
 * gpu_geometry_again / a full run over the stream leaves it, byte for byte (p14). Returns s->geo.swapped; -1: no tape */
int vc_geo_replay(struct VCoreState *s);
/* the next vc_3d_frame / geometry run blends toward b (f 0..4096), then call vc_ip_apply_end */
void vc_ip_apply_begin(struct VCoreState *s, const VC_IpRec *b, const int *map, const u8 *texhold, s32 f);
void vc_ip_apply_end(struct VCoreState *s);
/* BWPORT_INTERP_STATS: one line per drawn frame to fp (the pair with the previous frame, and what moved in 2D) */
void vc_ip_log_frame(struct VCoreState *s, FILE *fp, unsigned frame);

#endif
