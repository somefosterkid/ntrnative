/* piece 15's oracle sweep: VCT_SWEEP=DIR vcore_tests p15_sweep writes N random lit-triangle scenes (one flat colour
 * each: three equal normals) as DIR/swNNN.vcs and the colour piece 15 computes for each in DIR/expect.txt
 * ("NNN r g b" 5-bit). tools: python3 sweep_cmp.py DIR runs the oracle on them and compares the centre pixel. Without
 * VCT_SWEEP the test does nothing. VCT_SWEEP_ROT=1 puts a rotated directional matrix in force. */
#include "vct.h"
#include <stdlib.h>

static u32 rng_state = 12345;
static u32 rnd(void) { rng_state = rng_state * 1664525u + 1013904223u; return rng_state >> 8; }
static void G(VCoreState *s, u32 op, u32 n, u32 a) { u32 p[1] = {a}; vct_gx(s, op, n, p); }
#define V10(x, y, z) ((((u32)(x) & 0x3FF)) | (((u32)(y) & 0x3FF) << 10) | (((u32)(z) & 0x3FF) << 20))

/* a random vector of length about len/512 */
static u32 rvec(int len) {
  int x = (int)(rnd() % 1001) - 500, y = (int)(rnd() % 1001) - 500, z = (int)(rnd() % 1001) - 500;
  double l = x * (double)x + y * (double)y + z * (double)z;
  if (l < 1) { x = 1; y = 0; z = 0; l = 1; }
  double sc = len / __builtin_sqrt(l);
  return V10((int)(x * sc), (int)(y * sc), (int)(z * sc));
}
static u32 rcol(void) { return (rnd() % 32) | (rnd() % 32) << 5 | (rnd() % 32) << 10; }

VCT_TEST(p15_sweep) {
  const char *dir = getenv("VCT_SWEEP");
  if (dir == NULL) return;
  int rot = getenv("VCT_SWEEP_ROT") != NULL;
  const char *ncs = getenv("VCT_SWEEP_N");
  int count = ncs ? atoi(ncs) : 60;
  char path[512];
  snprintf(path, sizeof path, "%s/expect.txt", dir);
  FILE *ex = fopen(path, "w");
  VCT_CHECK(ex != NULL);
  for (int k = 0; k < count; k++) {
    VCoreState *s = vcore_create();
    VCT_CHECK(s != NULL);
    vct_io32(s, VC_REG_DISPCNT, 0x00010108);
    vct_io16(s, VC_REG_POWCNT1, 0x820F);
    vct_io16(s, VC_REG_CLEAR_DEPTH, 0x7FFF);
    VC_Mtx id;
    memset(&id, 0, sizeof id);
    id.m[0] = id.m[5] = id.m[10] = id.m[15] = 0x1000;
    G(s, VC_OP_MTX_MODE, 1, 0);
    vct_gx(s, VC_OP_MTX_LOAD_4x4, 16, (const u32 *)id.m);
    G(s, VC_OP_MTX_MODE, 1, 2);
    VC_Mtx rm = id;
    if (rot) { /* about an arbitrary axis-ish: a rotation of ~37 degrees about z, then ~20 about x (rows orthonormal) */
      double a = 0.3 + 0.05 * k, b = 0.2 + 0.03 * k;
      double ca = __builtin_cos(a), sa = __builtin_sin(a), cb = __builtin_cos(b), sb = __builtin_sin(b);
      double r[3][3] = {{ca, sa * cb, sa * sb}, {-sa, ca * cb, ca * sb}, {0, -sb, cb}};
      for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) rm.m[i * 4 + j] = (s32)(r[i][j] * 4096.0);
      vct_gx(s, VC_OP_MTX_LOAD_4x4, 16, (const u32 *)rm.m);
    } else {
      G(s, VC_OP_MTX_IDENTITY, 0, 0);
    }
    G(s, VC_OP_VIEWPORT, 1, 0xBFFF0000u);
    u32 dif = rcol(), amb = rcol(), spe = rcol(), emi = rcol();
    if (getenv("VCT_SWEEP_NOSPEC")) spe = 0;
    int table = rnd() % 2;
    int lights = rnd() % 16;
    if (k % 4 == 0) lights = 1;
    G(s, VC_OP_DIF_AMB, 1, dif | amb << 16);
    G(s, VC_OP_SPE_EMI, 1, spe | (table ? 0x8000u : 0) | emi << 16);
    if (table) {
      u32 tb[32];
      for (int i = 0; i < 128; i++) ((u8 *)tb)[i] = (u8)(rnd() % 256);
      vct_gx(s, VC_OP_SHININESS, 32, tb);
    }
    for (u32 i = 0; i < 4; i++) {
      G(s, VC_OP_LIGHT_COLOR, 1, rcol() | i << 30);
      G(s, VC_OP_LIGHT_VECTOR, 1, rvec(k % 3 == 0 ? 511 : 300) | i << 30);
    }
    G(s, VC_OP_POLYGON_ATTR, 1, 0x001F00C0 | (u32)lights);
    u32 nrm = rvec(k % 5 == 0 ? 511 : 400);
    G(s, VC_OP_BEGIN_VTXS, 1, 0);
    static const s32 vx[3][2] = {{-0xE00, -0xE00}, {0xE00, -0xB00}, {-0x200, 0xE00}};
    for (int i = 0; i < 3; i++) {
      G(s, VC_OP_NORMAL, 1, nrm);
      u32 p[2] = {((u32)vx[i][0] & 0xFFFF) | ((u32)vx[i][1] & 0xFFFF) << 16, 0};
      vct_gx(s, VC_OP_VTX_16, 2, p);
    }
    G(s, VC_OP_END_VTXS, 0, 0);
    G(s, VC_OP_SWAP_BUFFERS, 1, 0);
    /* piece 15's colour: play the stream through vc_cmd_light only */
    vc_light_reset(s);
    VC_Mtx *vm = (VC_Mtx *)vc_mtx_vec(s);
    *vm = rm;
    VC_Vertex v;
    memset(&v, 0, sizeof v);
    s32 pos[4] = {0, 0, 0, 0x1000};
    for (u32 i = 0; i < s->gx_words;) {
      u32 h = s->gx[i++], op = h & 0xFF, n = (h >> 8) & 0xFF;
      if (op == VC_OP_VTX_16) vc_vertex_attrs(s, pos, &v);
      else if (op == VC_OP_NORMAL || (op >= VC_OP_DIF_AMB && op <= VC_OP_SHININESS)) vc_cmd_light(s, op, s->gx + i, n);
      i += n;
    }
    snprintf(path, sizeof path, "%s/sw%03d.vcs", dir, k);
    VCT_CHECK(vct_write_vcs(s, path) == 0);
    fprintf(ex, "%03d %d %d %d\n", k, v.color[0], v.color[1], v.color[2]);
  }
  fclose(ex);
}
