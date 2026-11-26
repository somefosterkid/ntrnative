/* Smooth motion's pairing (p23_interp.c, docs/interp.md): identity, the tags, the rest (rest_pair), the windows and the cuts.
 * Run: build/vcore/vcore_tests p23 */
#include "vct.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 16
typedef struct Fx {
  VC_IpRec r;
  VC_IpVtx v[NMAX * 8];
  VC_IpList l[NMAX];
} Fx;

/* a list of n vertices on a circle (n = 4: a square) of radius rad px around (cx, cy), screen px of a 256 x 192 screen */
static void add(Fx *f, double cx, double cy, double rad, int n) {
  VC_IpList *l = &f->l[f->r.nl++];
  l->first = f->r.nv;
  l->n = (u32)n;
  l->type = 1;
  l->attr = 0x001F0080;
  for (int k = 0; k < n; k++) {
    double a = 6.283185307179586 * k / n + 0.7;
    double x = cx + rad * cos(a), y = cy + rad * sin(a);
    VC_IpVtx *v = &f->v[f->r.nv++];
    v->clip[3] = 4096;
    v->clip[0] = (s32)((x - 128.0) * 32.0);
    v->clip[1] = (s32)((96.0 - y) * 4096.0 / 96.0);
  }
}
static void fin(Fx *f) {
  f->r.v = f->v;
  f->r.l = f->l;
}

/* a list as add's with its own texture (another key) */
static void addt(Fx *f, double cx, double cy, double rad, int n, u32 tex) {
  add(f, cx, cy, rad, n);
  f->l[f->r.nl - 1].texparam = tex;
}

/* a 4-vertex list at clip x0..x1 (w = 4096: -4096..4096 is the screen's width), depth z, into a big fixture; map: its partner or -1 */
static void cut_list(VC_IpRec *r, int *map, s32 x0, s32 x1, s32 z, int paired) {
  VC_IpList *l = &r->l[r->nl];
  l->first = r->nv;
  l->n = 4;
  l->type = 1;
  for (int k = 0; k < 4; k++) {
    VC_IpVtx *v = &r->v[r->nv++];
    memset(v, 0, sizeof *v);
    v->clip[0] = k & 1 ? x1 : x0;
    v->clip[1] = k & 2 ? 300 : -300;
    v->clip[2] = z;
    v->clip[3] = 4096;
  }
  map[r->nl] = paired ? (int)r->nl : -1;
  r->nl++;
}

/* the cut counts only the lists that may be on the screen (vc_ip_cut, 2026-11-25): the map block HeartGold loads as the player walks
 * west past Cherrygrove's x 560 adds 303 lists, all past the view volume, and was a cut every time (no in-between: a hitch) */
VCT_TEST(p23_cut_counts_lists_on_the_screen) {
  static VC_IpVtx v[400 * 4];
  static VC_IpList l[400];
  static int map[400];
  VC_IpRec r;
  memset(&r, 0, sizeof r);
  r.v = v;
  r.l = l;
  int un = -1, seen = -1;
  for (int i = 0; i < 70; i++) cut_list(&r, map, -2000 + 40 * i, -1900 + 40 * i, 0, 1); /* on the screen, paired */
  for (int i = 0; i < 100; i++) cut_list(&r, map, -9000 - 20 * i, -5000, 0, 0);       /* all left of the screen */
  for (int i = 0; i < 100; i++) cut_list(&r, map, -1000, 1000, 5000 + i, 0);         /* past the far plane */
  VCT_EQ(vc_ip_cut(&r, map, &un, &seen), 0);
  VCT_EQ(un, 0);
  VCT_EQ(seen, 70);
  /* a list across the screen (one corner left of it, one right) is on it */
  cut_list(&r, map, -6000, 6000, 0, 0);
  for (int i = 0; i < 68; i++) cut_list(&r, map, -3000 + 80 * i, -2950 + 80 * i, 0, 0);
  VCT_EQ(vc_ip_cut(&r, map, &un, NULL), 0); /* 70 of 139 on the screen pair */
  VCT_EQ(un, 69);
  cut_list(&r, map, 3000, 3050, 0, 0); /* 70 of 140: not more than half; one more alone is */
  VCT_EQ(vc_ip_cut(&r, map, &un, NULL), 0);
  cut_list(&r, map, 3100, 3150, 0, 0);
  VCT_EQ(vc_ip_cut(&r, map, &un, NULL), 1);
  VCT_EQ(un, 71);
}


/* ---- identity first (p23_interp.c ident_pair; docs/interp-sweep.md 2026-11-26) ---- */

/* a 4-vertex quad of half-size h (object units) as RECORD records it: its key (tex, pal), its shape (geo), its position matrix m (4x3,
 * v' = v x m[0..8] + m[9..11]; camera space looks down -z) and its corners' clip coordinates (x and y two units a unit at distance 1) */
static void addm(Fx *f, u32 tex, u32 pal, u32 geo, const double *m, double h) {
  VC_IpList *l = &f->l[f->r.nl++];
  l->first = f->r.nv;
  l->n = 4;
  l->type = 1;
  l->attr = 0x001F00C0;
  l->texparam = tex;
  l->pltt = pal;
  l->geo = geo;
  for (int k = 0; k < 12; k++) l->mv[k] = (s32)lround(m[k] * 4096);
  static const double c[4][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
  for (int k = 0; k < 4; k++) {
    double ox = c[k][0] * h, oy = c[k][1] * h, p[3];
    for (int j = 0; j < 3; j++) p[j] = ox * m[j] + oy * m[3 + j] + m[9 + j];
    VC_IpVtx *v = &f->v[f->r.nv++];
    v->clip[0] = (s32)lround(p[0] * 2 * 4096);
    v->clip[1] = (s32)lround(p[1] * 2 * 4096 * 4 / 3);
    v->clip[3] = (s32)lround(-p[2] * 4096);
  }
}
/* the position matrix of a thing at world (x, y, z) seen by a camera at (cx, cy, cz) turned by yaw (radians) about y, scaled by s */
static void cam_mv(double *m, double x, double y, double z, double cx, double cy, double cz, double yaw, double s) {
  double c = cos(yaw), n = sin(yaw), r[9] = {c, 0, n, 0, 1, 0, -n, 0, c}; /* world -> camera (rows: the world axes in camera space) */
  double d[3] = {x - cx, y - cy, z - cz};
  for (int i = 0; i < 9; i++) m[i] = r[i] * s;
  for (int j = 0; j < 3; j++) m[9 + j] = d[0] * r[j] + d[1] * r[3 + j] + d[2] * r[6 + j];
}
static const double kBg[6][2] = {{-60, -20}, {50, -30}, {-30, 25}, {40, 20}, {0, 30}, {-55, 10}};
/* a scene: six things of their own (keys 0x100..) and a row of six copies of one model (key 0x200) one tile (16 units) apart from
 * x = -40, but copy "skip" (not drawn: culled; -1 = none) */
static void scene(Fx *f, double cx, double cy, double cz, double yaw, int skip) {
  double m[12];
  for (int i = 0; i < 6; i++) {
    cam_mv(m, kBg[i][0], kBg[i][1], -100, cx, cy, cz, yaw, 1);
    addm(f, 0x100u + (u32)i, 0, 0x7000u + (u32)i, m, 4);
  }
  for (int i = 0; i < 6; i++) {
    if (i == skip) continue;
    cam_mv(m, -40 + 16 * i, 0, -100, cx, cy, cz, yaw, 1);
    addm(f, 0x200, 0, 0x8000, m, 4);
  }
  fin(f);
}

/* Copies of one model (a row of fences, the windows of a row of houses) pair with themselves through the camera's motion, whatever
 * the camera does: the order passes paired them by draw order, so a culled copy shifted each copy after it onto its neighbour (60-160
 * px at Cherrygrove at night) until the camera's alignment undid it. Pan, zoom and turn, then a copy culled while the camera pans */
VCT_TEST(p23_identity_pairs_copies_through_the_camera) {
  static const double cams[4][4] = {{6, 0, 0, 0}, {0, 0, -12, 0}, {0, 0, 0, 0.05}, {6, 0, 0, 0}}; /* the newer camera: x, y, z, yaw */
  for (int t = 0; t < 4; t++) {
    Fx key, old;
    memset(&key, 0, sizeof key);
    memset(&old, 0, sizeof old);
    scene(&old, 0, 0, 0, 0, -1);
    scene(&key, cams[t][0], cams[t][1], cams[t][2], cams[t][3], t == 3 ? 0 : -1);
    int map[NMAX];
    VC_IpStats st;
    vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
    for (int i = 0; i < 6; i++) VCT_EQ(map[i], i);
    for (int i = 6; i < (int)key.r.nl; i++) VCT_EQ(map[i], i + (t == 3)); /* the same copy */
    VCT_EQ(st.ident, (int)key.r.nl);
  }
}

/* HeartGold's rings at the water (docs/interp-sweep.md; scratch/ipform's DIAG 2.3): walking north with the follower a tile behind,
 * the rings at the player's and the follower's feet are one texture and palette whose image steps through phases, the follower's a
 * phase behind, so the newer keyframe's follower ring has the older one's player ring's key and shape: one signature, drawn once in
 * each frame. It is not that ring: 16 units from where the camera's motion puts it. The billboards are scaled (here by 2): the test
 * is made in camera space (in the ring's own units the gap was 8, and they were paired: a 15 px slide). Each ring then pairs with
 * its own, a phase on, by the rest's loose key (one palette and texture family; untagged: tags pair them first) */
VCT_TEST(p23_identity_rings_a_phase_apart_are_two_things) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    double cz = f ? -2 : 0; /* the camera follows the player north (-z), 2 units a keyframe */
    for (int i = 0; i < 6; i++) {
      cam_mv(m, kBg[i][0], kBg[i][1], -100 - 2 * i, 0, 0, cz, 0, 1);
      addm(r, 0x100u + (u32)i, 0, 0x7000u + (u32)i, m, 4);
    }
    /* phases 0x2400 / 0x2440 / 0x2480: OLD's player ring 0x2440 and follower ring 0x2400, KEY's 0x2480 and 0x2440 */
    cam_mv(m, 0, -10, -100 + cz, 0, 0, cz, 0, 2); /* the player's ring, at its feet */
    addm(r, f ? 0x2480 : 0x2440, 0x13e, 0x9000, m, 4);
    cam_mv(m, 0, -10, -84 + cz, 0, 0, cz, 0, 2); /* the follower's, a tile behind */
    addm(r, f ? 0x2440 : 0x2400, 0x13e, 0x9000, m, 4);
    fin(r);
  }
  int map[NMAX];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[6], 6);
  VCT_EQ(map[7], 7);
  VCT_EQ(st.ident, 6);
}

/* Run 4x with the follower (docs/interp-sweep.md, 2026-11-26): the camera moves a tile (16 units) a keyframe, the follower walks a
 * tile behind the player, and their shadows are copies of one model. The newer follower's shadow stands where the older player's
 * stood in the world (the camera's motion fits) and the newer player's where it stood on the screen (no motion fits): two plausible
 * motions, so identity leaves the shadows to the rest, which pairs each with its own (draw order) (through the camera's motion first the
 * player's shadow was hidden and the follower's drawn twice, one sliding a tile) */
VCT_TEST(p23_identity_leaves_copies_two_motions_fit) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    double cx = f ? 16 : 0; /* the camera and the player: x 0, then 16 */
    for (int i = 0; i < 6; i++) {
      cam_mv(m, kBg[i][0] + 3, kBg[i][1], -100, cx, 0, 0, 0, 1);
      addm(r, 0x100u + (u32)i, 0, 0x7000u + (u32)i, m, 4);
    }
    cam_mv(m, cx, -10, -100, cx, 0, 0, 0, 1); /* the player's shadow */
    addm(r, 0x300, 0x20, 0xA000, m, 3);
    cam_mv(m, cx - 16, -10, -100, cx, 0, 0, 0, 1); /* the follower's */
    addm(r, 0x300, 0x20, 0xA000, m, 3);
    fin(r);
  }
  int map[NMAX];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[6], 6);
  VCT_EQ(map[7], 7);
  VCT_EQ(st.ident, 6);
}

/* Many copies of one particle (lib/spl draws each as its own list; the M1 review, finding 2): 256 in each keyframe, each moving on its
 * own, its image a cell on. Identity weighed every pair of the group (65536: a bin each, then a sort) and the gap pass every pair of
 * the one gap they leave (65536 list_cost), 10-20 ms a keyframe while the effect lasts, pairing nothing. A group of more than
 * IP_ID_VOTECAND pairs now weighs only those a plausible motion predicts, and the rest pairs a key with as many lists in both frames
 * in draw order: each particle with its own */
typedef struct FxB {
  VC_IpRec r;
  VC_IpVtx v[300 * 4];
  VC_IpList l[300];
} FxB;
static void addmb(FxB *f, u32 tex, u32 geo, const double *m, double h) {
  Fx *t = (Fx *)calloc(1, sizeof(Fx)); /* addm's quad, moved into f */
  addm(t, tex, 0x40, geo, m, h);
  t->l[0].first = f->r.nv;
  f->l[f->r.nl++] = t->l[0];
  memcpy(&f->v[f->r.nv], t->v, 4 * sizeof *t->v);
  f->r.nv += 4;
  f->r.v = f->v, f->r.l = f->l;
  free(t);
}
VCT_TEST(p23_identity_many_copies_cost_bounded) {
  static FxB key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  u32 rnd = 12345;
  double px[256], py[256], dx[256], dy[256];
  for (int k = 0; k < 256; k++) {
    rnd = rnd * 1103515245u + 12345u, px[k] = -60 + (rnd >> 8) % 1200 / 10.0;
    rnd = rnd * 1103515245u + 12345u, py[k] = -40 + (rnd >> 8) % 800 / 10.0;
    rnd = rnd * 1103515245u + 12345u, dx[k] = 2 + (rnd >> 8) % 40 / 10.0;
    rnd = rnd * 1103515245u + 12345u, dy[k] = -3 + (rnd >> 8) % 60 / 10.0;
  }
  for (int f = 0; f < 2; f++) {
    FxB *r = f ? &key : &old;
    const double cx = f ? 4 : 0;
    for (int i = 0; i < 6; i++) {
      cam_mv(m, kBg[i][0], kBg[i][1], -100, cx, 0, 0, 0, 1);
      addmb(r, 0x100u + (u32)i, 0x7000u + (u32)i, m, 4);
    }
    for (int k = 0; k < 256; k++) {
      cam_mv(m, px[k] + f * dx[k], py[k] + f * dy[k], -100, cx, 0, 0, 0, 1);
      addmb(r, f ? 0x2010 : 0x2000, 0x9000, m, 2);
    }
  }
  int map[300];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  for (int i = 0; i < 6; i++) VCT_EQ(map[i], i);
  int own = 0;
  for (int k = 0; k < 256; k++) own += map[6 + k] == 6 + k;
  VCT_EQ(own, 256);
  VCT_CHECK(st.idcand < 4096);
}

/* The camera's D when the view is copies only (the M1 review, finding 5): a row of six copies of a block of two lists, the camera
 * panning. The true D and each shift by a copy get one vote from each signature (two), so the bin table's order chose; a shift chosen
 * first locks every block with its neighbour. The tie now goes to the D that explains more candidate pairs (every copy, not a row
 * fewer): each block pairs with itself */
VCT_TEST(p23_identity_camera_d_of_copies_only) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    const double cx = f ? 6 : 0;
    for (int i = 0; i < 6; i++) {
      cam_mv(m, -40 + 16 * i, 0, -100, cx, 0, 0, 0, 1);
      addm(r, 0x200, 0, 0x8000, m, 4); /* the block's two lists */
      cam_mv(m, -40 + 16 * i, 8, -100, cx, 0, 0, 0, 1);
      addm(r, 0x201, 0, 0x8001, m, 4);
    }
    fin(r);
  }
  int map[NMAX];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  for (int i = 0; i < 12; i++) VCT_EQ(map[i], i);
  VCT_EQ(st.ident, 12);
}

/* ---- the visibility windows (vc_ip_windows; docs/interp-sweep.md, M2) ---- */

/* a perspective projection (row vectors: clip = (x, y, z, 1) x P; 90 degrees across, near 1) and a quad recorded with it (VC_IpRec
 * projm: the projection test needs the frame's projection) of half-size h at world (x, y, -100), the camera at x = cx */
static const double kP[16] = {2, 0, 0, 0, 0, 8.0 / 3, 0, 0, 0, 0, -1.02, -1, 0, 0, -2.02, 0};
static void addq(Fx *f, int k, const double *P, u32 tex, u32 geo, double x, double y, double z, double cx, double h) {
  double m[12];
  cam_mv(m, x, y, z, cx, 0, 0, 0, 1);
  VC_IpList *l = &f->l[f->r.nl++];
  l->first = f->r.nv, l->n = 4, l->type = 1, l->attr = 0x001F00C0, l->texparam = tex, l->pltt = 0x40, l->geo = geo, l->proj = (u8)k;
  for (int k = 0; k < 12; k++) l->mv[k] = (s32)lround(m[k] * 4096);
  static const double c[4][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
  for (int k = 0; k < 4; k++) {
    double p[3];
    for (int j = 0; j < 3; j++) p[j] = c[k][0] * h * m[j] + c[k][1] * h * m[3 + j] + m[9 + j];
    VC_IpVtx *v = &f->v[f->r.nv++];
    for (int j = 0; j < 4; j++) v->clip[j] = (s32)lround((p[0] * P[j] + p[1] * P[4 + j] + p[2] * P[8 + j] + P[12 + j]) * 4096);
  }
  if ((u32)k >= f->r.nproj) f->r.nproj = (u32)k + 1;
  f->r.projh[k] = 1u + (u32)k;
  for (int j = 0; j < 16; j++) f->r.projm[k][j] = (s32)lround(P[j] * 4096);
  fin(f);
}
static void addp(Fx *f, u32 tex, u32 geo, double x, double y, double cx, double h) { addq(f, 0, kP, tex, geo, x, y, -100, cx, h); }
static double scr_x(const s32 *c) { return 128.0 * c[0] / c[3]; }

/* ---- behaviour M5 keeps, from the tests of the order passes before it (the M5 review, finding 4) ---- */

/* Cherrygrove at Run 4x (docs/interp-sweep.md, 2026-11-24): the camera moves 16 px a keyframe, a mailbox's flag and post are three
 * lists of one key 8 px apart, the shadow under the player two lists of another key that stay put on the screen. Zero matrices: no
 * D says how anything moved. Every list keeps its partner: by plain displacement the mailbox's last list took the first's place (it
 * stands there on the screen now); measured beyond the world's motion (here the proven pairs' on the screen) it keeps its own */
VCT_TEST(p23_camera_motion_keeps_copies_paired) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  static const double bg[6][2] = {{100, 40}, {160, 40}, {200, 60}, {100, 120}, {160, 130}, {60, 80}};
  for (int i = 0; i < 6; i++) { /* the map around them, moved by the camera */
    addt(&old, bg[i][0], bg[i][1], 20, 4, 0x100u + (u32)i);
    addt(&key, bg[i][0] - 16, bg[i][1], 20, 4, 0x100u + (u32)i);
  }
  for (int i = 0; i < 3; i++) { /* the mailbox's run */
    addt(&old, 140 + 8 * i, 60, 5, 4, 0x200);
    addt(&key, 124 + 8 * i, 60, 5, 4, 0x200);
  }
  for (int i = 0; i < 2; i++) { /* the player's shadow */
    addt(&old, 128 - 10 * i, 100, 7, 6, 0x300);
    addt(&key, 128 - 10 * i, 100, 7, 6, 0x300);
  }
  fin(&key);
  fin(&old);
  int map[NMAX], n = (int)key.r.nl;
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  for (int i = 0; i < n; i++) VCT_EQ(map[i], i);
}

/* the same lists in another draw order (Black 2 / White 2 battles): identity pairs them, each key drawn once in each frame, whatever
 * the order */
VCT_TEST(p23_reordered_lists_pair_by_key_and_position) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double ox[3] = {40, 120, 200};
  for (int i = 0; i < 3; i++) add(&old, ox[i], 90, 20, 3 + i); /* three keys (vertex counts 3, 4, 5) */
  add(&key, ox[2] + 2, 90, 20, 5);                             /* drawn first now, the camera moved 2 px */
  add(&key, ox[0] + 2, 90, 20, 3);
  add(&key, ox[1] + 2, 90, 20, 4);
  fin(&key);
  fin(&old);
  int map[NMAX];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], 2);
  VCT_EQ(map[1], 0);
  VCT_EQ(map[2], 1);
  VCT_EQ(st.ident, 3);
}

/* A list drawn elsewhere in the newer keyframe's order (X, drawn once, last in the older keyframe and third in the newer) is identity's
 * pair whatever the order, and R, whose image changed while it moved on its own (identity leaves it), pairs with its own by the rest's
 * loose key (before M5 the gap pass's anchors: with X as the nearest locked list before it R's gap was empty and R was drawn at the
 * newer keyframe's place) */
VCT_TEST(p23_identity_gap_anchors_skip_a_list_drawn_out_of_order) {
  static const int ordo[6] = {0, 1, 6, 2, 3, 5}, ordk[6] = {0, 1, 5, 6, 2, 3}; /* 0-3 things of their own, 5 X, 6 R */
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    const double cx = f ? 6 : 0; /* the camera pans 6 units */
    for (int k = 0; k < 6; k++) {
      const int t = (f ? ordk : ordo)[k];
      if (t < 5) {
        cam_mv(m, kBg[t][0], kBg[t][1], -100, cx, 0, 0, 0, 1);
        addm(r, 0x100u + (u32)t, 0, 0x7000u + (u32)t, m, 4);
      } else if (t == 5) {
        cam_mv(m, kBg[4][0], kBg[4][1], -100, cx, 0, 0, 0, 1);
        addm(r, 0x180, 0, 0x7100, m, 4);
      } else {
        cam_mv(m, 10 + 3 * f, -20, -100, cx, 0, 0, 0, 1);
        addm(r, f ? 0x2040 : 0x2000, 0x40, 0x9000, m, 4);
      }
    }
    fin(r);
  }
  int map[NMAX];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[2], 5); /* X: identity's pair, out of order */
  VCT_EQ(map[3], 2); /* R with its own */
  VCT_EQ(st.ident, 5);
}

/* A1, A2 of the newer keyframe and B1, B2 of the older, left by identity, share the key without the texture's address. A1 and B1 are
 * one texture whose image changed: a pair; A2's texture is another size than B2's: never a pair across a texture family (the rest's
 * loose key keeps the size and format; before M5 the run re-pairing paired them) */
VCT_TEST(p23_repair_runs_keep_the_rest_rules) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    const double cx = f ? 6 : 0;
    for (int i = 0; i < 6; i++) {
      cam_mv(m, kBg[i][0], kBg[i][1], -100, cx, 0, 0, 0, 1);
      addm(r, 0x100u + (u32)i, 0, 0x7000u + (u32)i, m, 4);
    }
    cam_mv(m, 10 + 3 * f, -20, -100, cx, 0, 0, 0, 1);
    addm(r, f ? 0x00102040u : 0x00102000u, 0x40, 0x9000, m, 4); /* A1 / B1 */
    cam_mv(m, 24 + 3 * f, -20, -100, cx, 0, 0, 0, 1);
    addm(r, f ? 0x00202080u : 0x00102080u, 0x40, 0x9100, m, 4); /* A2 / B2 */
    fin(r);
  }
  int map[NMAX];
  VC_IpStats st;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[6], 6);
  VCT_EQ(map[7], -1);
}


/* ---- the rest's measure on the screen (rest_jumped; the M5 review, finding 1) ---- */

/* a quad of half-size h placed on the CPU at object x ox (its own shape: geo), drawn under the one matrix of the world's origin at
 * (0, 0, -100) seen by the camera at x = cx (addq's projection recorded) */
static void addr(Fx *f, u32 geo, double ox, double cx, double h) {
  addq(f, 0, kP, 0x500, geo, 0, 0, -100, cx, h);
  const VC_IpList *l = &f->l[f->r.nl - 1];
  for (u32 k = 0; k < 4; k++) { /* the vertices again, moved by ox in object space (the matrix's first row is x) */
    VC_IpVtx *v = &f->v[l->first + k];
    double p[3];
    for (int j = 0; j < 3; j++) p[j] = (ox + (k == 1 || k == 2 ? h : -h)) * (j == 0) + (k >= 2 ? h : -h) * (j == 1) + (j == 0 ? -cx : j == 2 ? -100 : 0);
    for (int j = 0; j < 4; j++) v->clip[j] = (s32)lround((p[0] * kP[j] + p[1] * kP[4 + j] + p[2] * kP[8 + j] + kP[12 + j]) * 4096);
  }
}

/* the rest's round 0 (the test phone's untagged file, M5, 2fed29f48): the camera pans, the player stays on the screen, its sprite's cell
 * changed (another texture address and shape: identity leaves it), and the follower beside it now draws the player's old cell (the
 * same key). In draw order by key the follower took the player's old list and the player blinked; round 0 takes first the pairs whose
 * motion identity would take (the player's: none), so each keeps its own and the follower is new. With their own matrices the
 * follower's motion fits no D (10 units off the camera's: M5's round 0 leaves it); placed on the CPU under one matrix every motion fits
 * (none), and the distance on the screen beyond it decides (M5: the same key first, the follower took the player's list) */
VCT_TEST(p23_rest_round0_keeps_a_sprite_whose_cell_changed) {
  const int was = vc_ip_restpxon;
  for (int one = 0; one < 2; one++)
    for (int px = 0; px < 2; px++) {
      Fx key, old;
      memset(&key, 0, sizeof key);
      memset(&old, 0, sizeof old);
      for (int f = 0; f < 2; f++) {
        Fx *r = f ? &key : &old;
        const double cx = f ? 6 : 0; /* the camera pans 6 units */
        for (int i = 0; i < 6; i++) addq(r, 0, kP, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], -100, cx, 4);
        if (one)
          addr(r, f ? 0x9200u : 0x9000u, 0, 0, 4); /* 6: the player, cell 0x420 then 0x440 (another shape too) */
        else
          addq(r, 0, kP, 0, f ? 0x9200u : 0x9000u, cx, 0, -100, cx, 4);
        r->l[6].texparam = 0x0D200000u | (f ? 0x440u : 0x420u);
      }
      if (one)
        addr(&key, 0x9100, 3, 0, 4); /* 7 of KEY: the follower, 3 units beside the player */
      else
        addq(&key, 0, kP, 0, 0x9100, 10, 0, -100, 6, 4);
      key.l[7].texparam = 0x0D200420u;
      fin(&key), fin(&old);
      int map[NMAX];
      VC_IpStats st;
      vc_ip_restpxon = px;
      vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
      VCT_EQ(st.ident, 6);
      VCT_EQ(map[6], one && !px ? -1 : 6);
      VCT_EQ(map[7], one && !px ? 6 : -1);
    }
  vc_ip_restpxon = was;
}

/* A run of quads alike placed on the CPU under one matrix, each drifting 3 units in object space (so its shape changes and identity
 * leaves it), the camera panning 6 units: every pair of the run moves by the camera's D, which is plausible, so round 0 weighed them
 * all alike and paired in draw order, and with the run's first quad gone each quad took its neighbour's place (13 units, 33 px). By
 * the distance from where its D puts it (3 units) each pairs with its own */
VCT_TEST(p23_rest_run_under_one_matrix_pairs_by_place) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    const double cx = f ? 6 : 0;
    for (int i = 0; i < 6; i++) addp(r, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], cx, 4);
    for (int i = f; i < 6; i++) addr(r, 0x8100u + 16u * (u32)f + (u32)i, -40 + 16 * i + 3 * f, cx, 4); /* KEY: the first gone */
  }
  int map[NMAX], was = vc_ip_restpxon;
  VC_IpStats st;
  vc_ip_restpxon = 0;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[6], 6); /* M5's rest: draw order, each quad on its neighbour */
  vc_ip_restpxon = 1;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  vc_ip_restpxon = was;
  for (int i = 0; i < 6; i++) VCT_EQ(map[i], i);
  for (int i = 6; i < 11; i++) VCT_EQ(map[i], i + 1);
  VCT_EQ(st.ident, 6);
  VCT_EQ(st.loose, 5);
}

/* a particle emitter's run of 32 px quads under zero matrices (no motion is known), the newest first: KEY = OLD's three moved 4 px, one
 * born in front. In draw order the newcomer took the oldest's place (a 40 px sweep) and every particle the one ahead of it; nearest
 * on the screen first, the newcomer is alone (new: vc_ip_windows) and the others pair with the one 4 px behind */
VCT_TEST(p23_rest_pairs_a_particle_run_by_motion) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double ox[3] = {100, 140, 180};
  add(&key, 60, 90, 16, 4);
  for (int i = 0; i < 3; i++) {
    add(&old, ox[i], 90, 16, 4);
    add(&key, ox[i] + 4, 90, 16, 4);
  }
  fin(&key);
  fin(&old);
  int map[NMAX], was = vc_ip_restpxon;
  VC_IpStats st;
  vc_ip_restpxon = 0;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], 0);
  vc_ip_restpxon = 1;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  vc_ip_restpxon = was;
  VCT_CHECK(map[0] < 0);
  VCT_EQ(map[1], 0);
  VCT_EQ(map[2], 1);
  VCT_EQ(map[3], 2);
}

/* the limit: a pair of the rest further than 48 px on the screen plainly, beyond the world's motion and beyond its own D is no pair
 * (zero matrices: every D is unknown, so nothing else limits it). Two quads of one key at x 20 and 200 and only the one at 200 left:
 * not a 180 px glide; one quad placed at x 250, then at x 10 (it wrapped the screen): two things */
VCT_TEST(p23_rest_jump_is_no_pair) {
  Fx key, old, k2, o2;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  memset(&k2, 0, sizeof k2);
  memset(&o2, 0, sizeof o2);
  add(&old, 20, 90, 8, 4);
  add(&old, 200, 90, 8, 4);
  add(&key, 200, 90, 8, 4);
  add(&o2, 250, 60, 8, 4);
  add(&k2, 10, 60, 8, 4);
  o2.l[0].geo = 1, k2.l[0].geo = 2; /* (placed on the CPU: its shape in object space moved too, so identity leaves it) */
  fin(&key), fin(&old), fin(&k2), fin(&o2);
  int map[NMAX], was = vc_ip_restpxon;
  VC_IpStats st;
  vc_ip_restpxon = 0;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], 0);
  vc_ip_pair(&k2.r, &o2.r, map, 48.0, &st);
  VCT_EQ(map[0], 0);
  vc_ip_restpxon = 1;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], 1);
  vc_ip_pair(&k2.r, &o2.r, map, 48.0, &st);
  VCT_CHECK(map[0] < 0);
  vc_ip_restpxon = was;
}

/* Rock Smash's debris (untagged; the M5 review, finding 2): two pieces of one image fly up on their own, 15 units (38 px) a keyframe at
 * stride 4. Identity leaves them (copies, no plausible motion), and the rest's draw order took a pair only within 12 world units, so
 * the pieces popped from keyframe to keyframe. Round 2's draw order (as many left in both frames) is bounded on the screen only: each
 * piece glides; one thrown 26 units (67 px) a keyframe is still no pair */
VCT_TEST(p23_rest_equal_counts_fly_on_their_own) {
  for (int t = 0; t < 2; t++) {
    const double dy = t ? 26 : 15;
    Fx key, old;
    memset(&key, 0, sizeof key);
    memset(&old, 0, sizeof old);
    for (int f = 0; f < 2; f++) {
      Fx *r = f ? &key : &old;
      for (int i = 0; i < 6; i++) addp(r, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], 0, 4);
      addp(r, 0x600, 0x9100, -6, -20 + dy * f, 0, 2);
      addp(r, 0x600, 0x9100, 8, -24 + dy * f, 0, 2);
    }
    int map[NMAX], was = vc_ip_restpxon;
    VC_IpStats st;
    vc_ip_restpxon = 0;
    vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
    VCT_CHECK(map[6] < 0 && map[7] < 0);
    vc_ip_restpxon = 1;
    vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
    vc_ip_restpxon = was;
    VCT_EQ(map[6], t ? -1 : 6);
    VCT_EQ(map[7], t ? -1 : 7);
  }
}

/* The newer keyframe's lists with no partner (the camera pans 20 units, 51 px): one past the older view's right edge there is entering
 * (it slides in with the world: blended toward where it was, off the screen), one inside it is new (the game would have drawn it and
 * did not: not drawn before its keyframe) */
VCT_TEST(p23_windows_entering_and_new) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  int waswin = vc_ip_win;
  vc_ip_win = 1;
  for (int f = 0; f < 2; f++)
    for (int i = 0; i < 6; i++) addp(f ? &key : &old, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], f ? 20 : 0, 4);
  addp(&key, 0x300, 0x9000, 66, 0, 20, 4); /* 6: entering at the right edge */
  addp(&key, 0x301, 0x9001, 10, 0, 20, 4); /* 7: new */
  int map[NMAX];
  VC_IpStats st;
  VC_IpKeep keep[NMAX];
  double enter[VC_IP_NENTER][16];
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_CHECK(st.camok);
  VCT_EQ(vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter), 0);
  for (int i = 0; i < 6; i++) VCT_EQ(map[i], i);
  VCT_EQ(map[6], VC_IP_ENTER(0));
  VCT_EQ(map[7], VC_IP_NEW);
  VCT_EQ(st.entering, 1);
  VCT_EQ(st.fresh, 1);
  s32 o[4];
  const s32 *kv = key.v[key.l[6].first].clip; /* its first corner: world x 62, at 107.5 px in the newer frame, 158.7 in the older */
  vc_ip_moved(kv, enter[0], 1.0, o);
  VCT_CHECK(fabs(scr_x(o) - 128.0 * 2 * 62 / 100) < 0.05);
  VCoreState *s = vct_state(); /* the in-between halfway: halfway there */
  VC_Vertex in[4], out[4];
  memset(in, 0, sizeof in);
  for (int k = 0; k < 4; k++)
    for (int c = 0; c < 4; c++) in[k].clip[c] = key.v[key.l[6].first + k].clip[c];
  vc_ip_apply_begin(s, &old.r, map, NULL, 2048);
  vc_ip_apply_enter(s, (const double (*)[16])enter);
  s->p23.list = 6;
  s->p23.open = 1;
  VCT_EQ(vc_ip_blend_list(s, in, out, 4), 0);
  VCT_CHECK(fabs(scr_x(out[0].clip) - 128.0 * 2 * 52 / 100) < 0.05);
  vc_ip_apply_end(s);
  vc_ip_win = waswin;
}

/* The older keyframe's lists with no partner, the camera panning 20 units a keyframe with the player: one leaving the screen at the
 * left edge slides off with the world; the player's shadow, gone as the player steps into tall grass, stays under the player (in the
 * pair before it moved with the player: its rigid group, which stands still on the screen); one gone with no pair before moves with
 * the world. All three drawn until the newer keyframe (kept) */
VCT_TEST(p23_windows_leaving_and_gone_with_its_group) {
  Fx fr[3]; /* the keyframe before (camera x 0), the older (20), the newer (40) */
  memset(fr, 0, sizeof fr);
  int waswin = vc_ip_win;
  vc_ip_win = 1;
  for (int f = 0; f < 3; f++) {
    const double cx = 20.0 * f;
    for (int i = 0; i < 6; i++) addp(&fr[f], 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], cx, 4);
    addp(&fr[f], 0x400, 0xB000, cx, -10, cx, 4); /* 6: the player */
    if (f < 2) addp(&fr[f], 0x401, 0xB001, cx, -18, cx, 3); /* 7: its shadow */
    if (f == 1) addp(&fr[f], 0x402, 0xB002, -28, 0, cx, 4); /* 8: leaving at the left edge */
    if (f == 1) addp(&fr[f], 0x403, 0xB003, 10, 20, cx, 4); /* 9: gone, no pair before */
  }
  int mc[NMAX], map[NMAX];
  VC_IpStats st;
  VC_IpKeep keep[NMAX];
  double enter[VC_IP_NENTER][16];
  vc_ip_pair(&fr[1].r, &fr[0].r, mc, 48.0, &st);
  VCT_EQ(mc[7], 7);
  vc_ip_pair(&fr[2].r, &fr[1].r, map, 48.0, &st);
  VCT_EQ(vc_ip_windows(&fr[2].r, &fr[1].r, map, &fr[0].r, mc, &st, keep, enter), 3);
  VCT_EQ(st.leaving, 1);
  VCT_EQ(st.gone, 2);
  for (int k = 0; k < 3; k++) {
    const VC_IpKeep *q = &keep[k];
    VCT_EQ(q->win, q->list == 8 ? VC_IP_LEAVING : VC_IP_GONE);
    VCT_CHECK(fabs(q->dx - (q->list == 7 ? 0 : -51.2)) < 0.05 && fabs(q->dy) < 0.05);
  }
  vc_ip_win = waswin;
}

/* A 2D layer drawn with a projection of its own over a scene that has no common motion (Platinum's battle intro: an ortho layer whose
 * two lists slide 8 px together over perspective shards that each drift their own way): the layer's D is the only one identity chooses,
 * and it is not the shards' world. The older frame's shard with no partner, near the camera, is gone where it is (no D for its
 * projection), not moved off the screen by the layer's D (it was: "leaving", 205 px) */
static const double kO[16] = {1.0 / 128, 0, 0, 0, 0, 1.0 / 96, 0, 0, 0, 0, -1.0 / 64, 0, 0, 0, 0, 1};
VCT_TEST(p23_windows_world_d_by_projection) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  int waswin = vc_ip_win;
  vc_ip_win = 1;
  for (int f = 0; f < 2; f++) {
    Fx *x = f ? &key : &old;
    for (int i = 0; i < 6; i++) addq(x, 0, kP, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0] + f * 0.5 * (i + 1), kBg[i][1], -100, 0, 4);
    for (int i = 0; i < 2; i++) addq(x, 1, kO, 0x500u + (u32)i, 0xC000u + (u32)i, -40.0 + 30 * i, 20, -10, f ? -8 : 0, 8);
  }
  addq(&old, 0, kP, 0x600, 0xD000, 2, 0, -10, 0, 0.4); /* 8: gone */
  int map[NMAX];
  VC_IpStats st;
  VC_IpKeep keep[NMAX];
  double enter[VC_IP_NENTER][16];
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_CHECK(st.camok);
  VCT_EQ(st.camproj, 2);
  VCT_EQ(vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter), 1);
  VCT_EQ(keep[0].list, 8);
  VCT_EQ(keep[0].win, VC_IP_GONE);
  VCT_CHECK(fabs(keep[0].dx) < 0.05 && fabs(keep[0].dy) < 0.05);
  vc_ip_win = waswin;
}


/* ---- tags (layer B: docs/interp-sweep.md M3; p23_interp.c tag_pair) ---- */

/* the last list added is tagged (owner = kind << 24 | id, inst, state) */
static void tagl(Fx *f, u32 kind, u32 id, u32 inst, u32 state) {
  VC_IpList *l = &f->l[f->r.nl - 1];
  l->owner = kind << 24 | id, l->inst = inst, l->state = state;
  f->r.ntag++;
}

/* A tag in the stream (op 0 with 3 parameters, flag 8) tags every list up to the next one, (0, 0, 0) ends it, and nothing else reads
 * it: the geometry is what the stream without the tags makes. RECORD keeps them with BWPORT_INTERP_TAGS=0 too (the pairing ignores
 * them then) */
VCT_TEST(p23_tags_recorded_from_the_stream) {
  static const u32 tri[3][2] = {{0x0000 | 0x0000u << 16, 0xF800}, {0x0800, 0xF800}, {0x0800u << 16, 0xF800}};
  int wastags = vc_ip_tags;
  u32 npoly[2] = {0, 0};
  for (int t = 0; t < 3; t++) { /* tags on, tags off, no tags in the stream */
    VCoreState *s = vct_state();
    vc_ip_tags = t != 1;
    for (int k = 0; k < 3; k++) {
      const u32 tag[3] = {k == 2 ? 0 : (5u << 24 | 1), k == 2 ? 0 : 7u + (u32)k, k == 2 ? 0 : 9};
      if (t < 2) vct_gx(s, VC_OP_NOP | 8u << 16, 3, tag);
      const u32 begin = 0;
      vct_gx(s, VC_OP_BEGIN_VTXS, 1, &begin);
      for (int v = 0; v < 3; v++) vct_gx(s, VC_OP_VTX_16, 2, tri[v]);
      vct_gx(s, VC_OP_END_VTXS, 0, NULL);
    }
    const u32 sw = 0;
    vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &sw);
    vc_ip_record(s);
    const VC_IpRec *r = &s->p23.rec;
    VCT_EQ(r->nl, 3u);
    VCT_EQ(r->ntag, t < 2 ? 2u : 0u);
    VCT_EQ(r->l[0].owner, t < 2 ? (5u << 24 | 1) : 0u);
    VCT_EQ(r->l[1].inst, t < 2 ? 8u : 0u);
    VCT_EQ(r->l[1].state, t < 2 ? 9u : 0u);
    VCT_EQ(r->l[2].owner, 0u);
    VCT_EQ(r->nv, 9u);
    if (t < 2) npoly[t] = s->geo.npoly;
    else {
      VCT_EQ(s->geo.npoly, npoly[0]);
      VCT_EQ(s->geo.npoly, npoly[1]);
    }
  }
  vc_ip_tags = wastags;
}

/* two NPCs of one sprite (copies: one signature) swap places while the camera stands still: identity pairs each with the one that
 * stood where it stands (the camera's D fits both), the tags with itself. BWPORT_INTERP_TAGS=0 gives identity's pairs, as the frames
 * with no tags do */
VCT_TEST(p23_tags_copies_swap_places) {
  int wastags = vc_ip_tags;
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    for (int i = 0; i < 6; i++) {
      cam_mv(m, kBg[i][0], kBg[i][1], -100, 0, 0, 0, 0, 1);
      addm(r, 0x100u + (u32)i, 0, 0x7000u + (u32)i, m, 4);
    }
    for (int k = 0; k < 2; k++) { /* inst 1 at x -8 then 8, inst 2 the other way (41 px) */
      cam_mv(m, (k == f ? -8 : 8), -5, -100, 0, 0, 0, 0, 1);
      addm(r, 0x300, 0x20, 0xA000, m, 4);
      tagl(r, VC_IP_TAG_SPRITE, 1, 1 + (u32)k, 0);
    }
    fin(r);
  }
  int map[NMAX], map0[NMAX];
  VC_IpStats st;
  for (int t = 1; t >= 0; t--) {
    vc_ip_tags = t;
    vc_ip_pair(&key.r, &old.r, t ? map : map0, 48.0, &st);
    VCT_EQ(st.tagged, t ? 2 : 0);
    VCT_EQ(st.tagdec, t ? 2 : 0);
  }
  for (int i = 0; i < 6; i++) {
    VCT_EQ(map[i], i);
    VCT_EQ(map0[i], i);
  }
  VCT_EQ(map[6], 6);
  VCT_EQ(map[7], 7);
  VCT_EQ(map0[6], 7); /* identity: the copy that stood there */
  VCT_EQ(map0[7], 6);
  vc_ip_tags = 1;
  key.r.ntag = 0; /* one frame without tags: the tags decide nothing */
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  for (int i = 0; i < 8; i++) VCT_EQ(map[i], map0[i]);
  vc_ip_tags = wastags;
}

/* M5: a tag pair whose list moved more than half the screen, plainly and beyond the world's motion both, is no pair (Platinum's ledge:
 * NPC shadows and effects drawn far off the screen for one keyframe); one that moves with the world (the camera pans 200 px) or stands
 * still on the screen (the player the camera follows) is */
VCT_TEST(p23_tags_jump_is_no_pair) {
  int wastags = vc_ip_tags;
  vc_ip_tags = 1;
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    const double cx = f ? 80 : 0; /* 2.56 px a unit at z = -100: 205 px */
    for (int i = 0; i < 6; i++) addp(r, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], cx, 4);
    addp(r, 0x300, 0xA000, 0, -5, cx, 4); /* stands in the world */
    tagl(r, VC_IP_TAG_SPRITE, 1, 1, 0);
    addp(r, 0x300, 0xA000, cx, 5, cx, 4); /* stands on the screen */
    tagl(r, VC_IP_TAG_SPRITE, 1, 2, 0);
    addp(r, 0x340, 0xA100, f ? 310 : 10, 0, cx, 4); /* moved 300 units in one step */
    tagl(r, VC_IP_TAG_SHADOW, 1, 1, 0);
  }
  int map[NMAX];
  u8 how[NMAX];
  VC_IpStats st;
  vc_ip_pair_how(&key.r, &old.r, map, how, 48.0, &st);
  for (int i = 0; i < 6; i++) {
    VCT_EQ(map[i], i);
    VCT_EQ(how[i], VC_IP_HOW_ID);
  }
  VCT_EQ(map[6], 6);
  VCT_EQ(how[6], VC_IP_HOW_TAG);
  VCT_EQ(map[7], 7);
  VCT_EQ(how[7], VC_IP_HOW_TAG);
  VCT_EQ(map[8], -1);
  VCT_EQ(how[8], VC_IP_HOW_JUMPED);
  VCT_EQ(st.jumps, 1);
  VCT_EQ(st.tagged, 2);
  vc_ip_tags = wastags;
}

/* M5: identity's pairs have no jump limit (the 48 px filter over every pair went): a model of three lists moving 60 units (154 px) by
 * one D while the camera stands still is that model (route1-wild-faint: the wild Pokémon's model, its pair dropped and the older list
 * kept, drawn twice) */
VCT_TEST(p23_identity_pair_has_no_jump_limit) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double m[12];
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    for (int i = 0; i < 6; i++) {
      cam_mv(m, kBg[i][0], kBg[i][1], -100, 0, 0, 0, 0, 1);
      addm(r, 0x100u + (u32)i, 0, 0x7000u + (u32)i, m, 4);
    }
    for (int k = 0; k < 3; k++) {
      cam_mv(m, -30 + 4 * k + (f ? 60 : 0), 8 * k, -100, 0, 0, 0, 0, 1);
      addm(r, 0x400u + (u32)k, 0x30, 0xB000u + (u32)k, m, 4);
    }
    fin(r);
  }
  int map[NMAX];
  u8 how[NMAX];
  VC_IpStats st;
  vc_ip_pair_how(&key.r, &old.r, map, how, 48.0, &st);
  for (int i = 0; i < 9; i++) {
    VCT_EQ(map[i], i);
    VCT_EQ(how[i], VC_IP_HOW_ID);
  }
  VCT_EQ(st.jumps, 0);
}

/* Run 4x (docs/interp-sweep.md 2026-11-26): the camera follows the player a tile (16 units) a keyframe and the follower walks a tile
 * behind, their shadows copies of one model: two motions fit, so identity leaves them to the rest. The tags pair each shadow
 * with its own. Then the player steps into tall grass and its shadow is gone: kept until the newer keyframe and moved with its sprite
 * (its inst: it stands still on the screen), not with the other shadows' motion (an NPC's, standing in the world: 20 px a keyframe)
 * nor the world's */
VCT_TEST(p23_tags_shadow_follows_its_sprite) {
  int waswin = vc_ip_win, wastags = vc_ip_tags;
  vc_ip_win = 1, vc_ip_tags = 1;
  for (int gone = 0; gone < 2; gone++) {
    Fx key, old;
    memset(&key, 0, sizeof key);
    memset(&old, 0, sizeof old);
    for (int f = 0; f < 2; f++) {
      Fx *r = f ? &key : &old;
      const double cx = f ? 16 : 0;
      for (int i = 0; i < 6; i++) addp(r, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], cx, 4);
      addp(r, 0x400, 0xB000, cx, -6, cx, 4), tagl(r, VC_IP_TAG_SPRITE, 1, 1, 0);       /* 6: the player */
      addp(r, 0x400, 0xB000, cx - 16, -6, cx, 4), tagl(r, VC_IP_TAG_SPRITE, 1, 2, 0);  /* 7: the follower */
      addp(r, 0x400, 0xB000, -30, 6, cx, 4), tagl(r, VC_IP_TAG_SPRITE, 1, 3, 0);       /* 8: an NPC */
      addp(r, 0x401, 0xB001, -30, -2, cx, 3), tagl(r, VC_IP_TAG_SHADOW, 2, 3, 0);      /* 9: the shadows: the NPC's, */
      addp(r, 0x401, 0xB001, cx - 16, -14, cx, 3), tagl(r, VC_IP_TAG_SHADOW, 2, 2, 0); /* 10: the follower's, */
      if (!(gone && f)) addp(r, 0x401, 0xB001, cx, -14, cx, 3), tagl(r, VC_IP_TAG_SHADOW, 2, 1, 0); /* 11: the player's */
    }
    int map[NMAX];
    VC_IpStats st;
    VC_IpKeep keep[NMAX];
    double enter[VC_IP_NENTER][16];
    vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
    for (int i = 0; i < (int)key.r.nl; i++) VCT_EQ(map[i], i);
    VCT_EQ(st.tagged, gone ? 5 : 6);
    VCT_EQ(vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter), gone);
    if (gone) {
      VCT_EQ(keep[0].list, 11);
      VCT_EQ(keep[0].win, VC_IP_GONE);
      VCT_CHECK(fabs(keep[0].dx) < 0.05 && fabs(keep[0].dy) < 0.05);
      vc_ip_tags = 0; /* M2: the world's motion (no pair before) */
      vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
      VCT_EQ(vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter), 1);
      VCT_CHECK(fabs(keep[0].dx + 128.0 * 2 * 16 / 100) < 0.05);
      vc_ip_tags = 1;
    }
  }
  vc_ip_win = waswin, vc_ip_tags = wastags;
}

/* A sprite turns as it walks on its own (the next cell: another image address, so another signature) while an NPC of the same sprite
 * turns into the pose it had where it was (one signature each, a tile apart): the tags keep each sprite's pair through its change of
 * state. A new effect (tagged, no partner) inside the view is new:
 * not drawn before its keyframe; a particle whose age went down is a new one in the same slot */
VCT_TEST(p23_tags_pose_change_keeps_its_pair) {
  int waswin = vc_ip_win, wastags = vc_ip_tags;
  vc_ip_win = 1, vc_ip_tags = 1;
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  for (int f = 0; f < 2; f++) {
    Fx *r = f ? &key : &old;
    for (int i = 0; i < 6; i++) addp(r, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], 0, 4);
    addp(r, f ? 0x540 : 0x500, 0xB000, f ? 4 : -12, -6, 0, 4), tagl(r, VC_IP_TAG_SPRITE, 1, 1, f ? 0x540 : 0x500); /* 6 */
    addp(r, f ? 0x500 : 0x580, 0xB000, 20, -6, 0, 4), tagl(r, VC_IP_TAG_SPRITE, 1, 2, f ? 0x500 : 0x580);        /* 7 */
    addp(r, 0x600, 0xC000, 30, 10, 0, 1), tagl(r, VC_IP_TAG_PARTICLE, 4, 0, f ? 2 : 9);                         /* 8: slot 0 */
    if (f) addp(r, 0x700, 0xD000, 4, -10, 0, 4), tagl(r, VC_IP_TAG_EFFECT, 5, 1, 0);                             /* 9: new */
  }
  int map[NMAX];
  VC_IpStats st;
  VC_IpKeep keep[NMAX];
  double enter[VC_IP_NENTER][16];
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  for (int i = 0; i < 8; i++) VCT_EQ(map[i], i);
  VCT_EQ(map[8], -1);
  VCT_EQ(map[9], -1);
  VCT_EQ(st.tagged, 2);
  VCT_EQ(st.tagdec, 4);
  VCT_EQ(vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter), 1); /* the old particle, kept */
  VCT_EQ(keep[0].list, 8);
  VCT_EQ(map[8], VC_IP_NEW);
  VCT_EQ(map[9], VC_IP_NEW);
  vc_ip_win = waswin, vc_ip_tags = wastags;
}


/* ---- M4: state and kinds (vc_ip_state, the windows' VC_IP_SWAP and kinds, vc_ip_cut_id; docs/interp-sweep.md m4) ---- */

/* textures in bank A (texture slot 0) and palettes in bank E (palette slots 0-3) of s: 8 x 8 16-colour images */
static void tex_vram(VCoreState *s) {
  s->io9[VC_REG_VRAMCNT_A] = 0x83;
  s->io9[VC_REG_VRAMCNT_A + VC_BANK_E] = 0x83;
  u8 *t = vc_bank(s, VC_BANK_A), *p = vc_bank(s, VC_BANK_E);
  for (int k = 0; k < 256; k++) t[k] = (u8)(k * 7 + 1), p[k] = (u8)(k * 3 + 5);
}
#define TEX4(addr) (3u << 26 | (u32)(addr) >> 3) /* 16 colours, 8 x 8, at addr */

/* A tagged sprite changes its image by another texture address (Gen 4's billboards): the pair switches to the older image at the
 * middle while that image's bytes are still in VRAM, and keeps the newer throughout once they were overwritten; its texcoords are held
 * on any change of its state (vc_ip_texhold, STATE on or off). A kept list whose bytes changed is not drawn. BWPORT_INTERP_STATE=0: none of
 * the rest */
VCT_TEST(p23_state_switches_at_the_middle) {
  int waswin = vc_ip_win, wastags = vc_ip_tags, wasst = vc_ip_stateon;
  vc_ip_win = 1, vc_ip_tags = 1;
  VCoreState *s = vct_state();
  tex_vram(s);
  for (int on = 0; on < 2; on++)
    for (int over = 0; over < 2; over++) {
      vc_ip_stateon = on;
      Fx key, old;
      memset(&key, 0, sizeof key);
      memset(&old, 0, sizeof old);
      for (int f = 0; f < 2; f++) {
        Fx *r = f ? &key : &old;
        for (int i = 0; i < 6; i++) addp(r, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], 0, 4);
        addp(r, TEX4(f ? 0x20 : 0x00), 0xB000, 4, -6, 0, 4), tagl(r, VC_IP_TAG_SPRITE, 1, 1, f ? 1 : 0); /* 6: the sprite */
        r->l[6].pltt = 0;
        r->l[6].attr = (r->l[6].attr & ~0x001F0000u) | (f ? 31u : 11u) << 16; /* (fading in) */
      }
      addp(&old, TEX4(0x40), 0xC000, -20, 0, 0, 4); /* 7 of old: gone, kept */
      old.l[7].pltt = 0;
      vc_ip_texsig(s, &old.r);
      if (over) vc_bank(s, VC_BANK_A)[0x00] ^= 0xFF, vc_bank(s, VC_BANK_A)[0x40] ^= 0xFF; /* the game wrote over them */
      vc_ip_texsig(s, &key.r);
      int map[NMAX];
      u8 th[NMAX];
      VC_IpStats st;
      VC_IpKeep keep[NMAX];
      double enter[VC_IP_NENTER][16];
      vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
      VCT_EQ(map[6], 6);
      int nk = vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter);
      VCT_EQ(nk, 1);
      vc_ip_texhold(&key.r, &old.r, map, th, 128, 64.0, NULL, NULL);
      VCT_EQ(th[6], on ? 3 : 0); /* (the same texcoords, another state: the image changed: the tag's rule; STATE=0, M3: the 8-texel rule) */
      nk = vc_ip_state(s, &key.r, &old.r, map, th, keep, nk);
      VCT_EQ(nk, on && over ? 0 : 1);
      VCT_EQ(th[6], !on ? 0 : over ? 3 | VC_IP_HOLD_ALPHA : 3 | VC_IP_HOLD_STATE | VC_IP_HOLD_ALPHA);
      if (over) vc_bank(s, VC_BANK_A)[0x00] ^= 0xFF, vc_bank(s, VC_BANK_A)[0x40] ^= 0xFF;
    }
  vc_ip_win = waswin, vc_ip_tags = wastags, vc_ip_stateon = wasst;
}

/* one list of a recorded stream: texture tp, alpha al */
static void state_stream(VCoreState *s, u32 tp, u32 al) {
  static const u32 tri[3][2] = {{0x0000, 0xF800}, {0x0800, 0xF800}, {0x0800u << 16, 0xF800}};
  s->gx_words = 0;
  const u32 attr = al << 16 | 0xC0, pl = 0, begin = 0, sw = 0;
  vct_gx(s, VC_OP_TEXIMAGE_PARAM, 1, &tp);
  vct_gx(s, VC_OP_PLTT_BASE, 1, &pl);
  vct_gx(s, VC_OP_POLYGON_ATTR, 1, &attr);
  vct_gx(s, VC_OP_BEGIN_VTXS, 1, &begin);
  for (int v = 0; v < 3; v++) vct_gx(s, VC_OP_VTX_16, 2, tri[v]);
  vct_gx(s, VC_OP_END_VTXS, 0, NULL);
  vct_gx(s, VC_OP_SWAP_BUFFERS, 1, &sw);
}

/* The replay of an in-between: a list with vc_ip_state's bits is drawn with the older keyframe's texture state from the middle on (and
 * the tape's state is back for what follows), its alpha blended (fades step no more at the keyframes' rate); a VC_IP_SWAP list only in
 * the newer half */
VCT_TEST(p23_state_replay_texture_and_alpha) {
  VCoreState *s = vct_state();
  const u32 t0 = TEX4(0x00), t1 = TEX4(0x20);
  state_stream(s, t0, 11);
  vc_ip_record(s);
  VC_IpRec ob, kb;
  memset(&ob, 0, sizeof ob), memset(&kb, 0, sizeof kb);
  vc_ip_take(s, &ob);
  state_stream(s, t1, 31);
  vc_ip_record(s);
  vc_ip_take(s, &kb);
  VCT_EQ(kb.nl, 1u);
  static const struct { int map; u8 th; s32 f; int npoly; u32 tp, al; } cs[] = {
      {0, VC_IP_HOLD_STATE | VC_IP_HOLD_ALPHA | 3, 3072, 1, TEX4(0x00), 16}, {0, VC_IP_HOLD_STATE | VC_IP_HOLD_ALPHA | 3, 2048, 1, TEX4(0x00), 21},
      {0, VC_IP_HOLD_STATE | VC_IP_HOLD_ALPHA | 3, 1024, 1, TEX4(0x20), 26}, {0, 3, 3072, 1, TEX4(0x20), 31},
      {VC_IP_SWAP(0), 0, 3072, 0, 0, 0},                                     {VC_IP_SWAP(0), 0, 1024, 1, TEX4(0x20), 31}};
  for (int c = 0; c < (int)(sizeof cs / sizeof cs[0]); c++) {
    int map[1] = {cs[c].map};
    u8 th[1] = {cs[c].th};
    vc_ip_apply_begin(s, &ob, map, th, cs[c].f);
    vc_geo_replay(s);
    VCT_EQ(s->geo.npoly, (u32)cs[c].npoly);
    if (cs[c].npoly) {
      VCT_EQ(s->geo.poly[0].texparam, cs[c].tp);
      VCT_EQ(s->geo.poly[0].attr >> 16 & 31, cs[c].al);
    }
    VCT_EQ(s->p14.texparam, t1); /* (the tape's again) */
    vc_ip_apply_end(s);
  }
  vc_ip_free(&ob), vc_ip_free(&kb);
}

/* Kinds and forms (INTERP-ARCH 3.3, 3.5), the camera panning 20 units: a particle born past the older view's edge is new, not
 * entering; an effect drawn as one list, then as two of other shapes, switches at the middle (its new lists from the middle on, moved
 * back with the world, its old one kept until then); an overlay never pairs and never glides: in both frames it switches at the middle
 * in place, an old one alone is gone where it was. BWPORT_INTERP_STATE=0: as M3 */
VCT_TEST(p23_state_kinds_and_forms) {
  int waswin = vc_ip_win, wastags = vc_ip_tags, wasst = vc_ip_stateon;
  vc_ip_win = 1, vc_ip_tags = 1;
  for (int on = 0; on < 2; on++) {
    vc_ip_stateon = on;
    Fx key, old;
    memset(&key, 0, sizeof key);
    memset(&old, 0, sizeof old);
    for (int f = 0; f < 2; f++)
      for (int i = 0; i < 6; i++) addp(f ? &key : &old, 0x100u + (u32)i, 0x7000u + (u32)i, kBg[i][0], kBg[i][1], f ? 20 : 0, 4);
    addp(&key, 0x600, 0xC000, 66, 0, 20, 1), tagl(&key, VC_IP_TAG_PARTICLE, 4, 10, 0);   /* 6: a newborn particle */
    addp(&key, 0x700, 0xE100, 10, -10, 20, 4), tagl(&key, VC_IP_TAG_EFFECT, 5, 3, 0);    /* 7, 8: the effect's new form */
    addp(&key, 0x700, 0xE200, 14, -10, 20, 4), tagl(&key, VC_IP_TAG_EFFECT, 5, 3, 0);
    addp(&key, 0x800, 0xF000, 20, 20, 20, 4), tagl(&key, VC_IP_TAG_OVERLAY, 6, 2, 0);    /* 9: an overlay in both */
    addp(&old, 0x700, 0xE000, 10, -10, 0, 4), tagl(&old, VC_IP_TAG_EFFECT, 5, 3, 0);     /* 6: the effect's old form */
    addp(&old, 0x800, 0xF000, 0, 20, 0, 4), tagl(&old, VC_IP_TAG_OVERLAY, 6, 2, 0);      /* 7 */
    addp(&old, 0x801, 0xF001, -10, -20, 0, 4), tagl(&old, VC_IP_TAG_OVERLAY, 6, 1, 0);   /* 8: an overlay gone */
    int map[NMAX];
    VC_IpStats st;
    VC_IpKeep keep[NMAX];
    double enter[VC_IP_NENTER][16];
    vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
    for (int i = 6; i < 10; i++) VCT_EQ(map[i], -1);
    const int nk = vc_ip_windows(&key.r, &old.r, map, NULL, NULL, &st, keep, enter);
    VCT_EQ(nk, 3);
    VCT_EQ(map[6], on ? VC_IP_NEW : VC_IP_ENTER(0));
    VCT_EQ(map[7], on ? VC_IP_SWAP(0) : VC_IP_NEW);
    VCT_EQ(map[8], on ? VC_IP_SWAP(0) : VC_IP_NEW);
    VCT_EQ(st.swapped, on ? 3 : 0);
    if (on) {
      VCT_CHECK(VC_IP_IS_SWAP(map[9]) && -64 - map[9] >= VC_IP_NPROJ);
      const double *m = enter[-64 - map[9]];
      for (int k = 0; k < 16; k++) VCT_CHECK(fabs(m[k] - (k % 5 == 0)) < 1e-9); /* in place */
    }
    for (int k = 0; k < nk; k++) {
      const int j = keep[k].list;
      VCT_EQ(keep[k].half, on && j != 8);
      if (on && j >= 7)
        for (int q = 0; q < 16; q++) VCT_CHECK(fabs(keep[k].m[q] - (q % 5 == 0)) < 1e-9);
      if (on) VCT_EQ(keep[k].win, VC_IP_GONE);
    }
    if (on) { /* the new form: hidden from the middle back, moved back with the world before it */
      VCoreState *s = vct_state();
      VC_Vertex in[4], out[4];
      memset(in, 0, sizeof in);
      for (int k = 0; k < 4; k++)
        for (int c = 0; c < 4; c++) in[k].clip[c] = key.v[key.l[7].first + k].clip[c];
      vc_ip_apply_begin(s, &old.r, map, NULL, 3072);
      vc_ip_apply_enter(s, (const double (*)[16])enter);
      s->p23.list = 7, s->p23.open = 1;
      VCT_EQ(vc_ip_blend_list(s, in, out, 4), 0);
      VCT_CHECK(out[0].clip[0] > out[0].clip[3]); /* (right of the view volume: clipped away) */
      s->p23.f = 1024, s->p23.k = 0;
      VCT_EQ(vc_ip_blend_list(s, in, out, 4), 0);
      VCT_CHECK(fabs(scr_x(out[0].clip) - scr_x(in[0].clip) - 0.25 * 128.0 * 2 * 20 / 100) < 0.1);
      vc_ip_apply_end(s);
    }
  }
  vc_ip_win = waswin, vc_ip_tags = wastags, vc_ip_stateon = wasst;
}

/* M4's cut: the share of the screen's vertices paired through identity or a tag, not of all pairs (the rest's pairs prove nothing); the
 * world's motion over half the screen is a cut too. BWPORT_INTERP_CUTID=0: vc_ip_cut */
VCT_TEST(p23_cut_by_identity) {
  int wascut = vc_ip_cutid;
  for (int shift = 0; shift < 2; shift++) {
    Fx key, old;
    memset(&key, 0, sizeof key);
    memset(&old, 0, sizeof old);
    for (int i = 0; i < 10; i++) {
      add(&key, 20 + 20 * i, 96, 6, 4);
      add(&old, 20 + 20 * i + (shift ? 140 : 2), 96, 6, 4);
    }
    fin(&key), fin(&old);
    int map[NMAX];
    u8 how[NMAX];
    for (int i = 0; i < 10; i++) map[i] = i, how[i] = 0;
    for (int n = 4; n <= 6; n += 2) {
      for (int i = 0; i < 10; i++) how[i] = i < n ? VC_IP_HOW_ID : VC_IP_HOW_REST;
      int un = -1, seen = -1;
      vc_ip_cutid = 1;
      VCT_EQ(vc_ip_cut_id(&key.r, &old.r, map, how, &un, &seen), shift ? (n == 4 ? 1 : 2) : n == 4 ? 1 : 0);
      VCT_EQ(un, 0);
      VCT_EQ(seen, 10);
      vc_ip_cutid = 0;
      VCT_EQ(vc_ip_cut_id(&key.r, &old.r, map, how, &un, &seen), 0);
    }
  }
  vc_ip_cutid = wascut;
}

/* A repeating texture's scroll that wrapped between the keyframes (HG/SS's sea: a 32-texel layer's texture matrix steps -0.19 texels a
 * keyframe and jumps back by two periods, +63.81 texels): the in-between moves it by the small step, not back across itself; a
 * texture that clamps, another size in the older keyframe and BWPORT_INTERP_TEXWRAP=0 blend the step as it is; mirrored, the period is
 * twice the size */
VCT_TEST(p23_texcoords_across_a_wrap) {
  const int was = vc_ip_texwrapon;
  const u32 rep = 3u << 16 | 2u << 20 | 2u << 23 | 3u << 26; /* repeat s and t, 32 x 32, 4 colours */
  static const struct { u32 tpa, tpb; int on, ds; } cs[] = {
      {rep, rep, 1, -1},                       /* -3 x 1024 >> 12: the step (-0.75 rounds down) */
      {rep, rep, 0, 255},                      /* 1021 x 1024 >> 12: the slide */
      {rep & ~(1u << 16), rep & ~(1u << 16), 1, 255}, /* s clamps */
      {rep | 1u << 18, rep | 1u << 18, 1, -1}, /* s mirrored: the period is 64 texels, one of them back */
      {rep, (rep & ~(7u << 20)) | 1u << 20, 1, 255}, /* b's texture is 16 wide */
  };
  for (unsigned c = 0; c < sizeof cs / sizeof cs[0]; c++) {
    vc_ip_texwrapon = cs[c].on;
    Fx key, old;
    memset(&key, 0, sizeof key);
    memset(&old, 0, sizeof old);
    addt(&key, 100, 100, 80, 4, cs[c].tpa);
    addt(&old, 100, 100, 80, 4, cs[c].tpb);
    for (int k = 0; k < 4; k++) {
      key.v[k].tex[0] = (s16)(100 * 16 + 512 * k), key.v[k].tex[1] = 40 * 16;
      old.v[k].tex[0] = (s16)(key.v[k].tex[0] + 1021), old.v[k].tex[1] = 40 * 16 + 2; /* (t: an ordinary small step) */
    }
    fin(&key), fin(&old);
    int map[NMAX] = {0};
    VCoreState *s = vct_state();
    VC_Vertex in[4], out[4];
    memset(in, 0, sizeof in);
    for (int k = 0; k < 4; k++) {
      for (int q = 0; q < 4; q++) in[k].clip[q] = key.v[k].clip[q];
      in[k].tex[0] = key.v[k].tex[0], in[k].tex[1] = key.v[k].tex[1];
    }
    vc_ip_apply_begin(s, &old.r, map, NULL, 1024);
    s->p23.list = 0, s->p23.open = 1;
    s->p14.texparam = cs[c].tpa;
    VCT_EQ(vc_ip_blend_list(s, in, out, 4), 0);
    for (int k = 0; k < 4; k++) {
      VCT_EQ(out[k].tex[0] - in[k].tex[0], cs[c].ds);
      VCT_EQ(out[k].tex[1] - in[k].tex[1], 0); /* (2 x 1024 >> 12) */
    }
    vc_ip_apply_end(s);
  }
  vc_ip_texwrapon = 1;
  VCT_EQ(vc_ip_tex_step(rep, rep, 1, -1532), 4); /* 3 periods (96 texels) back: + 0.25 texel */
  VCT_EQ(vc_ip_tex_step(rep, rep, 0, 200), 200);  /* within half a period (16 texels = 256): as it is */
  VCT_EQ(vc_ip_tex_step(rep, rep, 0, 256), 256);  /* exactly half: as it is */
  VCT_EQ(vc_ip_tex_step(rep, rep, 0, 512), 0);    /* one period */
  VCT_EQ(vc_ip_tex_step(rep, rep, 0, 509), -3);
  VCT_EQ(vc_ip_tex_step(rep | 1u << 18, rep | 1u << 18, 0, 509), 509); /* mirrored: under half of 64 texels */
  VCT_EQ(vc_ip_tex_step(rep & ~(7u << 26), rep & ~(7u << 26), 0, 1021), 1021); /* untextured */
  vc_ip_texwrapon = was;
}
