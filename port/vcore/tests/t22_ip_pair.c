/* Smooth motion's pairing (p23_interp.c, docs/interp.md): the particle-run re-pairing (repair_runs) and the scene cut rule.
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

/* a particle emitter's run of 32 px quads, the newest first: KEY = OLD's three moved 4 px, one born in front */
VCT_TEST(p23_repair_runs_pairs_a_particle_run_by_motion) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  double ox[3] = {100, 140, 180};
  add(&key, 60, 90, 16, 4);
  for (int i = 0; i < 3; i++) {
    add(&old, ox[i], 90, 16, 4);
    add(&key, ox[i] + 4, 90, 16, 4);
  }
  /* (key's order: the newcomer, then OLD's three: 60, 104, 144, 184; the old run 100, 140, 180) */
  fin(&key);
  fin(&old);
  int map[NMAX], was = vc_ip_repair_runs;
  VC_IpStats st;
  vc_ip_repair_runs = 0;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], 0); /* the pairing by order: the newcomer takes the oldest's place (a 40 px sweep) */
  vc_ip_repair_runs = 1;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  vc_ip_repair_runs = was;
  VCT_EQ(map[0], -1); /* re-paired by motion: the newcomer alone, the others each with the one 4 px behind */
  VCT_EQ(map[1], 0);
  VCT_EQ(map[2], 1);
  VCT_EQ(map[3], 2);
  VCT_CHECK(st.repaired > 0);
}

/* the battle intro's swirl: rings 150-600 px big, each one step from the next: the order is the pairing */
VCT_TEST(p23_repair_runs_leaves_big_lists_alone) {
  Fx key, old;
  memset(&key, 0, sizeof key);
  memset(&old, 0, sizeof old);
  for (int i = 0; i < 5; i++) {
    add(&old, 128, 96, 100 + 40 * i, 6);
    add(&key, 128, 96, 135 + 40 * i, 6); /* every ring 35 px out: the next ring's old place is 5 px away */
  }
  fin(&key);
  fin(&old);
  int map[NMAX], was = vc_ip_repair_runs;
  VC_IpStats st;
  vc_ip_repair_runs = 1;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  vc_ip_repair_runs = was;
  for (int i = 0; i < 5; i++) VCT_EQ(map[i], i);
  VCT_EQ(st.repaired, 0);
}

VCT_TEST(p23_scene_cut_rule) {
  int map[600], un;
  for (int i = 0; i < 600; i++) map[i] = i;
  for (int i = 0; i < 29; i++) map[i] = -1;
  VCT_EQ(vc_ip_scene_cut(map, 100, &un), 0); /* 29 alone: under 30 */
  VCT_EQ(un, 29);
  map[29] = -1;
  VCT_EQ(vc_ip_scene_cut(map, 100, &un), 1); /* 30 of 100 */
  VCT_EQ(un, 30);
  VCT_EQ(vc_ip_scene_cut(map, 360, NULL), 1); /* 30 of 360 = exactly 1/12 */
  VCT_EQ(vc_ip_scene_cut(map, 361, NULL), 0); /* a little under 1/12 */
}

/* the same lists in another draw order (Black 2 / White 2 battles): the LCS keeps two of three, the third pairs by position */
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
  int map[NMAX], was = vc_ip_reorder;
  VC_IpStats st;
  vc_ip_reorder = 0;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], -1); /* by order: the 5-gon has no partner */
  VCT_EQ(st.reordered, 0);
  vc_ip_reorder = 1;
  vc_ip_pair(&key.r, &old.r, map, 48.0, &st);
  VCT_EQ(map[0], 2);
  VCT_EQ(map[1], 0);
  VCT_EQ(map[2], 1);
  VCT_EQ(st.reordered, 1);
  VCT_EQ(st.removed, 0);
  /* a candidate 40 px from where the camera motion (the 2 px of the other two) puts it stays alone */
  for (int k = 0; k < 5; k++) key.v[k].clip[0] += 40 * 32;
  vc_ip_pair(&key.r, &old.r, map, 96.0, &st);
  VCT_EQ(map[0], -1);
  VCT_EQ(st.reordered, 0);
  vc_ip_reorder = was;
}
