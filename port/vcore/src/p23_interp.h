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
  /* its identity (docs/interp-sweep.md, the oracle; nothing in the pairing reads them yet): */
  u8 proj;   /* which projection matrix of the frame it was drawn with: VC_IpRec.projh's index (VC_IP_NPROJ - 1 = one of the rest) */
  u32 geo;   /* a hash of its vertices' object-space positions, in order (its shape) */
  u32 tex;   /* a hash of its vertices' TEXCOORD parameters, in order */
  s32 mv[12]; /* the position matrix at its first vertex: rows 0-3, columns 0-2 (20.12) */
  /* its tag (docs/interp-sweep.md M3; layer B): the stream's last tag before its BEGIN_VTXS, a GX3D entry op 0 with 3 parameters
   * (flag 8) the game side sends where the instance is at hand (SIM_G3Tag); owner 0 = untagged (no tag, or the tag (0, 0, 0) that ends
   * one). owner = kind << 24 | id (VC_IP_TAG_*; id: the billboard scene, emitter or effect manager, interned to a counter), inst = the
   * instance (actor, particle slot, cell node; a shadow carries its sprite's), state = a discrete state (a sprite's cell, a particle's age) */
  u32 owner, inst, state;
  u32 texh; /* M4: a hash of the bytes it samples (texels, 4x4 indices, palette) in its frame's VRAM (vc_ip_texsig; 0 = not taken) */
} VC_IpList;
enum { VC_IP_NPROJ = 8 };
enum { VC_IP_TAG_SPRITE = 1, VC_IP_TAG_SHADOW = 2, VC_IP_TAG_PARTICLE = 3, VC_IP_TAG_CELLS = 4, VC_IP_TAG_EFFECT = 5, VC_IP_TAG_OVERLAY = 6 };
#define VC_IP_TAG_KIND(owner) ((owner) >> 24)

typedef struct VC_IpRec {
  VC_IpVtx *v;
  u32 nv, capv;
  VC_IpList *l;
  u32 nl, capl;
  u32 swapped;   /* the stream held a complete frame (SWAP_BUFFERS) */
  u32 npoly;     /* polygons the geometry engine kept */
  u32 projh[VC_IP_NPROJ], nproj; /* the frame's projection matrices by first use (a hash of each) */
  s32 projm[VC_IP_NPROJ][16];    /* ... and each one (rows 0-3, columns 0-3, 20.12; index < nproj): vc_ip_windows' projection test */
  u32 ntag;                      /* lists with a tag (VC_IpList.owner != 0): the pairing takes tags only when both frames have some */
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

/* a list of the older keyframe b with no partner in a, kept (vc_ip_windows): drawn in the in-betweens until a, as without Smooth motion */
typedef struct VC_IpKeep {
  int list;     /* b's list */
  int after;    /* drawn after a's list `after` ends, before the next one begins (-1: before a's first list) */
  float dx, dy; /* its motion to a on the screen (px, 256 x 192, y up: a's place minus b's), m's mean (the sweep reads it) */
  int win;      /* VC_IP_LEAVING / VC_IP_GONE: moved by m */
  double m[16]; /* (win) a clip-space vertex of b (a row vector) to its place in a: v x m; drawn at v + (4096 - f) / 4096 (v x m - v) */
  int half;     /* (M4, VC_IP_SWAP's other side) drawn only in the older half of the in-betweens, f >= 2048 */
} VC_IpKeep;
enum { VC_IP_LEAVING = 1, VC_IP_GONE = 2 };

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
  const VC_IpKeep *keep; /* APPLY: b's lists to draw too (vc_ip_apply_keep), by after; nkeep of them, the next one kcur */
  int nkeep, kcur;
  const double (*enter)[16]; /* APPLY: the clip-space motion of this frame's entering lists by projection (vc_ip_apply_enter) or NULL */
  u32 tag[3];                /* RECORD: the current tag (owner, inst, state; vc_ip_tag), copied into each list at its BEGIN */
  /* APPLY (M4, vc_ip_state's texhold bits): the open list is drawn with b's texture state or a blended alpha (ovr); the tape's
   * texparam / pltt it replaced (sv bit 0 / 1: still to put back at the list's end, cleared when the stream sets them itself) */
  int ovr, sv;
  u32 svtex, svpltt;
} VC_P23;

/* what vc_ip_pair found between frame a and the next frame b */
typedef struct VC_IpStats {
  int nl_a, nl_b;
  u32 nv_a, nv_b;
  int strict, loose;           /* lists paired by identity or a tag / by the rest's rule (p23_interp.c rest_pair) */
  int removed, added;          /* a's lists with no partner, b's lists with no partner */
  int jumps, behind;           /* pairs dropped: a tag pair that jumped (more than half the screen) / crossing w = 0 with no vertex in front in both */
  u32 vt_paired, vt_removed, vt_added; /* vertices in the paired lists of a, the removed lists, the added lists */
  int same;                    /* every list paired and every vertex identical */
  int idcand;                  /* identity's candidate pairs weighed, both tiers (bounded: p23_interp.c IP_ID_VOTECAND) */
  int fresh;                   /* a's lists marked VC_IP_NEW (vc_ip_windows) */
  int ident;                   /* pairs identity proved (counted in strict too) */
  int camok;                   /* identity found the camera's D */
  int tagged, tagdec;          /* pairs the tags made (counted in strict too) / lists of a the tags decided, paired or not (vc_ip_tags) */
  int camproj;                 /* the projections with a world's D (bit VC_IpList.proj; vc_ip_windows reads them) ... */
  double camp[VC_IP_NPROJ][12]; /* ... each: the strongest D identity chose that a list drawn with it moved by, a point of a's camera
                                 * space to b's: v x camp[k][0..8] + camp[k][9..11] (world units) */
  int entering, leaving, gone; /* vc_ip_windows: a's lists VC_IP_ENTER, b's lists kept as VC_IP_LEAVING / VC_IP_GONE */
  int swapped;                 /* vc_ip_windows (M4): a's lists VC_IP_SWAP (b's lists kept for the older half: counted in gone) */
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
/* the texcoord step (1/16 texel) from a to b + d on axis c (0 s, 1 t) that the blend takes, a's texture state tpa, b's tpb: on an axis
 * that repeats in both with the same size and mirroring, the shortest of d + k x period (they sample the same texels), else d.
 * vc_ip_texwrap_on: 1 (default) or 0 = every step as it is (the blend before 2026-11-26), from BWPORT_INTERP_TEXWRAP */
int vc_ip_tex_step(u32 tpa, u32 tpb, int c, int d);
extern int vc_ip_texwrapon;
int vc_ip_texwrap_on(void);
/* vc_ip_restpx_on: 1 (default) or 0 = M5's rest (no screen measure: round 0 in draw order, no limit on the screen, round 2's draw
 * order within 12 world units), from BWPORT_INTERP_RESTPX (p23_interp.c rest_jumped) */
extern int vc_ip_restpxon;
int vc_ip_restpx_on(void);
void vc_ip_free(VC_IpRec *r);
/* runs the geometry engine over s's command stream and leaves the vertices in s->p23.rec (copy with vc_ip_take) */
void vc_ip_record(struct VCoreState *s);
void vc_ip_take(struct VCoreState *s, VC_IpRec *into);  /* moves s->p23.rec into *into */
/* pairs a's lists with b's: map[i] = b's list for a's list i, or -1 (no partner: drawn at a's place in the in-betweens; vc_ip_windows
 * marks it new, entering or switching form); limit: unused since M5 (the jump limit went; kept for the callers). The pairs, each list
 * once (docs/interp-sweep.md M1-M5; INTERP-ARCH 2.2):
 * - the tags (vc_ip_tags, both frames tagged): a list of one instance (owner, inst) with one of that instance in the other frame
 *   (p23_interp.c tag_pair), unless it moved more than half the screen beyond the world's motion (the game moved it in one step: no
 *   pair; a tagged list with no tagged partner is new or gone);
 * - identity (ident_pair; the rules of the sweep's oracle): a list's key, its shape in object space, its projection and the motion of
 *   its matrix between the frames;
 * - the rest (rest_pair): what both left, in three rounds. 0: within a loose key (the texture's address left out), the pairs whose
 *   motion identity would take (a chosen D, or none), nearest on the screen beyond that motion first (BWPORT_INTERP_RESTPX=0: the same
 *   key first, in draw order); 1: within one key, nearest first in world units after the camera's D, only near (identity's rule 5:
 *   IP_ID_FAR), a key whose lists are alike in draw order; 2: the same within a loose key, alike lists in draw order also further
 *   (an untagged thing flying on its own: debris); never a pair that jumped on the screen (48 px, rest_jumped).
 * A pair crossing the camera plane with no vertex in front in both frames is dropped (nothing measures its motion). */
void vc_ip_pair(const VC_IpRec *a, const VC_IpRec *b, int *map, double limit, VC_IpStats *st);
/* the same, and how a's list i was paired (how, room for a->nl; NULL: not wanted): VC_IP_HOW_ID identity, _TAG a tag, _REST the rest's
 * rule; _JUMPED a tag pair that jumped and _BEHIND one the camera plane dropped (both no pair now); 0 none */
void vc_ip_pair_how(const VC_IpRec *a, const VC_IpRec *b, int *map, u8 *how, double limit, VC_IpStats *st);
enum { VC_IP_HOW_ID = 1, VC_IP_HOW_TAG, VC_IP_HOW_REST, VC_IP_HOW_JUMPED, VC_IP_HOW_BEHIND };
/* map's mark for a list of a (the newer keyframe) with no partner that is new (vc_ip_windows: the game would have drawn it in b had it
 * been there: an effect the game starts, the grass a step shakes). Not drawn in the in-betweens before a (vc_ip_blend_list): it appears
 * with its keyframe, as without Smooth motion. (b's lists with no partner that the windows keep are drawn until a: VC_IpKeep.) */
#define VC_IP_NEW (-2)
/* map's mark for a list of a with no partner that is entering the screen (vc_ip_windows): drawn in the in-betweens blended toward
 * where it was in b had it stood still in the world, its vertices times the enter matrix of its projection k (vc_ip_apply_enter);
 * k >= VC_IP_NPROJ: a tagged list's, moved with its owner group (enter[k], one of VC_IP_NENTER - VC_IP_NPROJ such motions) */
enum { VC_IP_NENTER = VC_IP_NPROJ + 24 };
#define VC_IP_ENTER(k) (-16 - (k))
#define VC_IP_IS_ENTER(m) ((m) <= -16 && (m) > -16 - VC_IP_NENTER)
/* map's mark (M4, vc_ip_state_on; INTERP-ARCH 3.3) for a list of a with no partner whose instance (owner, inst; not a particle) has
 * unpaired lists in b too: the same thing in another form (another model, another cell count, an overlay, which never pairs). Drawn
 * only in the newer half of the in-betweens (f < 2048), moved back by enter[k] as an entering list; b's lists of it are kept for the
 * older half (VC_IpKeep.half): never both, never neither, the switch half a keyframe from where the game puts it either way (at f = .5
 * exactly the older, as the texcoord hold) */
#define VC_IP_SWAP(k) (-64 - (k))
#define VC_IP_IS_SWAP(m) ((m) <= -64 && (m) > -64 - VC_IP_NENTER)
/* The visibility windows (docs/interp-sweep.md, M2; INTERP-ARCH 3.1), after vc_ip_pair(a, b, map, ..., st): every list with no
 * partner is placed by the projection test, with the world's D of the list's projection (st->camp: identity's strongest D that some
 * list drawn with that projection moved by; a 2D layer over a scene of its own has its own) and that projection P (VC_IpRec.projm):
 * Dc = inverse(P) x D x P takes a clip-space vertex of a to where it was in b had it stood still in the world.
 * - a's list, projected back with Dc, outside b's view volume (vc_ip_list_seen): entering, blended toward that place (it slides in
 *   with the world), map VC_IP_ENTER(its projection), enter[k] = Dc for a's projection k;
 *   inside: new, VC_IP_NEW (not drawn before a: the game would have drawn it in b and did not);
 * - b's list, projected forward with the world's D, outside a's view volume: leaving, kept and moved by that projection
 *   (VC_IP_LEAVING); inside: gone, kept and moved with its rigid group (VC_IP_GONE): the D the b lists that moved as it did in the
 *   pair before (c, mc: b's map to c, or NULL) have now, else the world's.
 * Kept lists are drawn until a (window [0, 1)), new ones from a (window [1, 1]): never both, never neither. No D for the projection: no motion
 * (every list with no partner switches at its keyframe). Returns how many of b's lists are kept (keep: room for b->nl, sorted by after);
 * st->fresh, entering, leaving, gone count them. vc_ip_win 0 (BWPORT_INTERP_WINDOWS=0): no windows (every list with no
 * partner drawn at a's place, none of b's kept: Smooth motion's in-betweens before 2026-11-25; enter is left alone then).
 * A list the tags decided (vc_ip_tags: tagged, both frames tagged) with no partner is tested and moved with its owner group instead:
 * the motion most of the paired lists of that owner have (a shadow's: its sprite's, the paired list of another owner with its inst), an
 * overlay's none; no such pair: the world's, as above. enter: VC_IP_NENTER matrices. */
int vc_ip_windows(const VC_IpRec *a, const VC_IpRec *b, int *map, const VC_IpRec *c, const int *mc, VC_IpStats *st, VC_IpKeep *keep,
                  double (*enter)[16]);
/* 1 on, 0 off, -1 = from BWPORT_INTERP_WINDOWS (default on) */
extern int vc_ip_win;
/* Tags (docs/interp-sweep.md M3; INTERP-ARCH 2.3 layer B): vc_ip_pair pairs the lists both keyframes tagged by (owner, inst) before
 * identity (p23_interp.c tag_pair), and identity, the rest and the windows' world motion leave them; a tagged list with no
 * tagged partner is new or gone. 1 on, 0 off (the pairing reads no tag: M2's, and the bridges do not ask the game for tags; RECORD
 * keeps them either way, for the sweep's oracle), -1 = from BWPORT_INTERP_TAGS (default on) */
extern int vc_ip_tags;
int vc_ip_tags_on(void);
/* RECORD: a tag in the stream (op 0, 3 parameters: owner, inst, state; vc_gx_command) */
void vc_ip_tag(struct VCoreState *s, const u32 *param);
/* the lists of map (nl lists of a) with no partner, also in *unpaired (IpGather's log; the scene rule it was named for went in M5:
 * the identity cut, vc_ip_cut_id, says when a picture is another scene) */
int vc_ip_scene_cut(const int *map, int nl, int *unpaired);
/* 1 when some of list l of r may be on the screen: not all of its vertices outside one plane of the view volume (-w..w in x, y and z;
 * the polygons of a list are inside its vertices' hull, so a list all past one plane draws nothing) */
int vc_ip_list_seen(const VC_IpRec *r, const VC_IpList *l);
/* the cut of BWPORT_INTERP_CUTID=0 (vc_ip_cut_id's A/B): the pair map of key frame a shows the new frame at once when, over a's lists
 * that may be on the screen (vc_ip_list_seen), fewer than half the vertices pair (an entering list, VC_IP_ENTER, glides with the world:
 * its vertices count as paired). The lists past the view volume are left out: nothing of them is drawn (a map block HeartGold loads all
 * off the screen was a cut every time, 2026-11-25). *unpaired = the lists on the screen with no partner that are not new (VC_IP_NEW),
 * *seen = the lists on it (either may be NULL). Returns 0 (no cut) or 1. */
int vc_ip_cut(const VC_IpRec *a, const int *map, int *unpaired, int *seen);
/* M4's cut (INTERP-ARCH 3.4; vc_ip_cutid_on: 1 default, 0 = vc_ip_cut, from BWPORT_INTERP_CUTID; identity off: vc_ip_cut): over a's
 * lists that may be on the screen, fewer than half the vertices in lists paired through identity or a tag (how, vc_ip_pair_how) or
 * switching form (VC_IP_SWAP) is a cut (1); so is the world's motion: the mean screen motion of those pairs' vertices over half the
 * screen's width (2: a camera cut inside one map). No count of unpaired lists. *unpaired, *seen as vc_ip_cut's. */
extern int vc_ip_cutid;
int vc_ip_cutid_on(void);
int vc_ip_cut_id(const VC_IpRec *a, const VC_IpRec *b, const int *map, const u8 *how, int *unpaired, int *seen);
/* texhold[i] = 3 (both axes; bit 0 = s, bit 1 = t; a big list: only an axis that jumps by whole multiples of 8 texels) when a's list i, paired with b's list map[i], is at most maxpx (screen pixels, 256 wide) in size in both frames and changes a
 * texcoord by `thr` (1/16 texel) or more between them: a walking sprite's texture is an atlas of cells picked by texcoords, and blending two
 * cells' coordinates shows a strip of both. (A big surface that scrolls fast, the battle's floor, keeps its blend.) A pair of one tagged
 * sprite (both lists of one owner and instance, VC_IP_TAG_SPRITE) is held on both axes when its state (the image) or any texcoord differs,
 * else blended: the tag says what a cell step is, no threshold (M4's rule, after the rule above: before M5 in vc_ip_state; with
 * BWPORT_INTERP_STATE=0 not applied: M3). */
/* pstep: the pair before's nstep (2 ints per list of b, or NULL), nstep: out, 2 ints per list of a (zeroed by the caller): a big list's
 * axis step that is the same as in the pair before is a steady scroll and is not held */
void vc_ip_texhold(const VC_IpRec *a, const VC_IpRec *b, const int *map, u8 *texhold, int thr, double maxpx, const int *pstep,
                   int *nstep);
/* pass this as pstep for the first pair after a cut or an unpaired keyframe: there is no pair before to say the scroll is steady, and
 * holding its first step (8 texels at once) shows one jump of a scroll that then runs blended. (NULL holds, for one-pair tools.) */
extern const int vc_ip_nostep[1];

/* ---- M4: state and kinds (docs/interp-sweep.md m4; INTERP-ARCH 3.2, 3.3, 3.5). vc_ip_state_on: 1 (default) or 0 = M3, from
 * BWPORT_INTERP_STATE (-1 = not read yet): with it off nothing below runs and vc_ip_windows makes no VC_IP_SWAP ---- */
extern int vc_ip_stateon;
int vc_ip_state_on(void);
/* texhold's bits past the hold's two (vc_ip_state): an in-between at f >= 2048 draws the list with b's texparam and pltt (its texcoords
 * held: they go with their texture); at 0 < f its alpha is blended toward b's (attr bits 16-20; both 1..31: 0 is wireframe) */
enum { VC_IP_HOLD_STATE = 4, VC_IP_HOLD_ALPHA = 8 };
/* each list's texh from s's VRAM now (vc_vram_map; IpGather and vcore_ipsweep right after recording the frame): a hash of the texel,
 * 4x4 index and palette bytes its texparam and pltt sample (the 4x4 format's palette as far as its blocks reach; untextured: 1) */
void vc_ip_texsig(struct VCoreState *s, VC_IpRec *r);
/* after vc_ip_texhold and vc_ip_windows, with s holding a's VRAM: a paired list whose texture state (texparam, pltt) differs switches
 * to b's at f = .5 when the bytes b's state samples did not change between the keyframes (b's texh), else keeps a's throughout; alpha
 * blends (a tagged sprite's texcoords: vc_ip_texhold); a kept
 * list whose bytes changed is dropped (drawn with a's VRAM it would show the wrong image), and a VC_IP_SWAP list whose kept other
 * side all went is drawn in the whole interval (VC_IP_ENTER). Returns the kept lists left (keep compacted in place). */
int vc_ip_state(struct VCoreState *s, const VC_IpRec *a, const VC_IpRec *b, int *map, u8 *texhold, VC_IpKeep *keep, int nkeep);
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
/* after vc_ip_apply_begin: b's kept lists (vc_ip_windows) are drawn too, each after its anchor list of a (p14 at BEGIN_VTXS, p18 at
 * SWAP_BUFFERS: vc_ip_draw_kept), in the in-betweens (f > 0); vc_ip_apply_end clears them */
void vc_ip_apply_keep(struct VCoreState *s, const VC_IpKeep *keep, int n);
/* after vc_ip_apply_begin: the enter matrices of vc_ip_windows (VC_IP_NENTER of them) for the map's VC_IP_ENTER lists (NULL: drawn at
 * a's place); vc_ip_apply_end clears them */
void vc_ip_apply_enter(struct VCoreState *s, const double (*enter)[16]);
/* out = clip + t (clip x m - clip): a clip-space vertex moved by t (0..1) of the way the motion m takes it (vc_ip_windows' lists) */
void vc_ip_moved(const s32 *clip, const double *m, double t, s32 *out);
/* p14 / p18 (APPLY): draws the kept lists anchored after a's lists up to `upto` (INT_MAX: the rest), leaving p14's state as it was */
void vc_ip_draw_kept(struct VCoreState *s, int upto);
/* BWPORT_INTERP_STATS: one line per drawn frame to fp (the pair with the previous frame, and what moved in 2D) */
void vc_ip_log_frame(struct VCoreState *s, FILE *fp, unsigned frame);

#endif
