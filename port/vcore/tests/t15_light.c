/* port/vcore piece 15's unit tests: lighting, materials, normals, texture coordinates (docs/vcore.md, piece 15). OWNED BY PIECE 15.
 * Run: build/vcore/vcore_tests p15
 * Every expected value is worked by hand from GBATEK 45 (light formula), 49 (texture coordinates), 53 (VEC_TEST); the
 * working is in the comment above each test. Vectors: 10-bit command value v is v/512; internally 1.12 (v << 3). */
#include "vct.h"

static void set_mtx(const VC_Mtx *m, const VC_Mtx *v) { *(VC_Mtx *)m = *v; }
static VC_Mtx identity(void) {
  VC_Mtx m;
  memset(&m, 0, sizeof m);
  m.m[0] = m.m[5] = m.m[10] = m.m[15] = 0x1000;
  return m;
}
static void L(VCoreState *s, u32 op, u32 w) { vc_cmd_light(s, op, &w, 1); }
static VCoreState *lit_state(void) {
  VCoreState *s = vct_state();
  VC_Mtx id = identity();
  set_mtx(vc_mtx_vec(s), &id);
  set_mtx(vc_mtx_tex(s), &id);
  vc_light_reset(s);
  return s;
}
static void attrs(VCoreState *s, VC_Vertex *v) {
  s32 pos[4] = {0, 0, 0, 0x1000};
  memset(v, 0, sizeof *v);
  vc_vertex_attrs(s, pos, v);
}
#define RGB5(r, g, b) ((u32)(r) | (u32)(g) << 5 | (u32)(b) << 10)
#define VEC10(x, y, z) ((((u32)(x) & 0x3FF)) | (((u32)(y) & 0x3FF) << 10) | (((u32)(z) & 0x3FF) << 20))

/* COLOR: bits 0-4 R, 5-9 G, 10-14 B, taken by the next vertex */
VCT_TEST(p15_color) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_COLOR, RGB5(31, 0, 0));
  attrs(s, &v);
  VCT_EQ(v.color[0], 31); VCT_EQ(v.color[1], 0); VCT_EQ(v.color[2], 0);
  L(s, VC_OP_COLOR, 0x7C00 | 0x8000 | 0xFFFF0000u); /* bit 15 and up are not part of the colour */
  attrs(s, &v);
  VCT_EQ(v.color[0], 0); VCT_EQ(v.color[1], 0); VCT_EQ(v.color[2], 31);
}

/* DIF_AMB bit 15 = also set the vertex colour (the low 15 bits, as COLOR); without it the colour stays */
VCT_TEST(p15_dif_amb_sets_color) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_COLOR, RGB5(1, 2, 3));
  L(s, VC_OP_DIF_AMB, RGB5(5, 6, 7) | (RGB5(9, 9, 9) << 16)); /* bit 15 clear */
  attrs(s, &v);
  VCT_EQ(v.color[0], 1); VCT_EQ(v.color[1], 2); VCT_EQ(v.color[2], 3);
  L(s, VC_OP_DIF_AMB, RGB5(5, 6, 7) | 0x8000 | (RGB5(9, 9, 9) << 16));
  attrs(s, &v);
  VCT_EQ(v.color[0], 5); VCT_EQ(v.color[1], 6); VCT_EQ(v.color[2], 7);
  VCT_EQ(s->p15.amb[0], 9); VCT_EQ(s->p15.amb[2], 9); /* the ambient field is bits 16-30 */
}

/* NORMAL with no light enabled in POLYGON_ATTR: the colour is just the emission */
VCT_TEST(p15_normal_no_light) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_COLOR, RGB5(10, 10, 10));
  L(s, VC_OP_DIF_AMB, RGB5(31, 31, 31) | (RGB5(31, 31, 31) << 16));
  L(s, VC_OP_SPE_EMI, RGB5(31, 31, 31) | (RGB5(3, 4, 5) << 16));
  L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31));
  s->p15.polyattr = 0;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 3); VCT_EQ(v.color[1], 4); VCT_EQ(v.color[2], 5);
}

/* One light, light direction (0,0,-1.0) = raw z 0x200, normal (0,0,+511/512). 1.12: L = -0x1000, N = 0xFF8.
 * diffuse level = -(L.N) = 0x1000*0xFF8 = 0xFF8000 >> 16 = 255.
 * The terms are summed before the cut (checked against the oracle: 16/8 adds up to 23, not 15 + 7), each in 13
 * fraction bits: R: emi 2<<13 = 16384 + dif 16*31*255 = 126480 + amb (8*31)<<8 = 63488 = 206352 >> 13 = 25.
 * G: (31*31*255)>>13 = 245055>>13 = 29. B: dif 0, amb 0, emi 0 = 0. */
VCT_TEST(p15_one_light_diffuse_ambient) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_DIF_AMB, RGB5(16, 31, 0) | (RGB5(8, 0, 0) << 16));
  L(s, VC_OP_SPE_EMI, RGB5(0, 0, 0) | (RGB5(2, 0, 0) << 16));
  L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31));
  L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200));
  s->p15.polyattr = 1;
  VCT_EQ(s->p15.ldir[0][2], -0x1000);
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  VCT_EQ(s->p15.normal[2], 0xFF8);
  attrs(s, &v);
  VCT_EQ(v.color[0], 25); VCT_EQ(v.color[1], 29); VCT_EQ(v.color[2], 0);
}

/* A normal facing away (0,0,-1.0): -(L.N) = -(0x1000*-0x1000)... L=(0,0,-1), N=(0,0,-1): L.N = +1 so -(L.N) < 0, level 0:
 * only emission + ambient: R = 2 + 7 = 9 */
VCT_TEST(p15_light_facing_away) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_DIF_AMB, RGB5(16, 31, 0) | (RGB5(8, 0, 0) << 16));
  L(s, VC_OP_SPE_EMI, (RGB5(2, 0, 0) << 16));
  L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31));
  L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200));
  s->p15.polyattr = 1;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x200));
  attrs(s, &v);
  VCT_EQ(v.color[0], 9); VCT_EQ(v.color[1], 0);
}

/* Specular: L = (0,0,-1.0), N = (0,0,511/512): the oracle's level is 505-506 (/512); the fitted pipeline gives 506.
 * Table off: spe 31, lc 31: (31*31*506)>>14 = 29. Table on, entry 127 = 0x80: level>>1 = 127:
 * (31*31*128)>>13 = 123008>>13 = 15. */
VCT_TEST(p15_specular_and_table) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_SPE_EMI, RGB5(31, 0, 0));
  L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31));
  L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200));
  VCT_EQ(s->p15.lhalf[0][2], -0x1000);
  s->p15.polyattr = 1;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 29);
  u32 tbl[32];
  memset(tbl, 0, sizeof tbl);
  tbl[31] = 0x80800000u; /* entries 126 and 127 = 0x80 (the level here is 505..506: index 126) */
  vc_cmd_light(s, VC_OP_SHININESS, tbl, 32);
  VCT_EQ(s->p15.shin_table[127], 0x80);
  L(s, VC_OP_SPE_EMI, RGB5(31, 0, 0) | 0x8000);
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 15);
}

/* SHININESS: four 8-bit entries a word, entry 0 in the low byte; 32 words = 128 entries */
VCT_TEST(p15_shininess_table_layout) {
  VCoreState *s = lit_state();
  u32 tbl[32];
  for (int i = 0; i < 32; i++) tbl[i] = 0;
  tbl[0] = 0x04030201u;
  tbl[1] = 0x08070605u;
  tbl[31] = 0xAABBCCDDu;
  vc_cmd_light(s, VC_OP_SHININESS, tbl, 32);
  VCT_EQ(s->p15.shin_table[0], 1); VCT_EQ(s->p15.shin_table[3], 4); VCT_EQ(s->p15.shin_table[4], 5);
  VCT_EQ(s->p15.shin_table[7], 8);
  VCT_EQ(s->p15.shin_table[124], 0xDD); VCT_EQ(s->p15.shin_table[127], 0xAA);
}

/* Four lights, all (0,0,-1.0), normal facing them (terms summed, then cut once):
 * dif R 4, lc 31, level 255: 4*31*255 = 31620 a light; ambient 1*31<<8 = 7936 a light. All four: 4*(31620+7936) =
 * 158224 >> 13 = 19. Lights 0 and 2 only (mask 5): 79112 >> 13 = 9. Light 1's colour is set but it is disabled. */
VCT_TEST(p15_four_lights) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_DIF_AMB, RGB5(4, 0, 0) | (RGB5(1, 0, 0) << 16));
  for (u32 i = 0; i < 4; i++) {
    L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31) | i << 30);
    L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200) | i << 30);
  }
  s->p15.polyattr = 0xF;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 19);
  s->p15.polyattr = 0x5;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 9);
}

/* The sum clamps to 31: two lights with dif G 31: 29 + 29 = 58 -> 31. R: emission 31 + more stays 31 */
VCT_TEST(p15_clamp_31) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_DIF_AMB, RGB5(0, 31, 31));
  L(s, VC_OP_SPE_EMI, (RGB5(31, 0, 0) << 16));
  for (u32 i = 0; i < 2; i++) {
    L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31) | i << 30);
    L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200) | i << 30);
  }
  s->p15.polyattr = 3;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 31); VCT_EQ(v.color[1], 31); VCT_EQ(v.color[2], 31);
}

/* COLOR after lighting replaces the lit colour */
VCT_TEST(p15_color_after_lighting) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_SPE_EMI, (RGB5(7, 7, 7) << 16));
  L(s, VC_OP_NORMAL, VEC10(0, 0, 0x1FF));
  attrs(s, &v);
  VCT_EQ(v.color[0], 7);
  L(s, VC_OP_COLOR, RGB5(1, 2, 3));
  attrs(s, &v);
  VCT_EQ(v.color[0], 1); VCT_EQ(v.color[1], 2); VCT_EQ(v.color[2], 3);
}

/* LIGHT_VECTOR goes through the directional matrix (v' = v * M, 3x3). M swaps y and z: m[6] (row 1, col 2) and
 * m[9] (row 2, col 1) = 1.0, m[0] = 1.0. (0,0,-1.0) -> out[1] = v[2]*m[9] = -0x1000, out[2] = v[1]*m[6] = 0.
 * Half = (L + (0,0,-0x1000))/2 = (0, -0x800, -0x800). */
VCT_TEST(p15_light_vector_through_matrix) {
  VCoreState *s = lit_state();
  VC_Mtx m;
  memset(&m, 0, sizeof m);
  m.m[0] = 0x1000; m.m[6] = 0x1000; m.m[9] = 0x1000; m.m[15] = 0x1000;
  set_mtx(vc_mtx_vec(s), &m);
  L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200) | 2u << 30);
  VCT_EQ(s->p15.ldir[2][0], 0); VCT_EQ(s->p15.ldir[2][1], -0x1000); VCT_EQ(s->p15.ldir[2][2], 0);
  VCT_EQ(s->p15.lhalf[2][1], -0x800); VCT_EQ(s->p15.lhalf[2][2], -0x800);
  VCT_EQ(s->p15.ldir[0][1], 0); /* the other lights untouched */
}

/* VEC_TEST: result = vector * directional matrix, 1.12 with a 4-bit sign. Identity: x = 0x1FF -> 0xFF8,
 * y = -1.0 (0x200) -> -0x1000. With the matrix scaled by 2, x = 0x1FF -> 0x1FF0, which has bit 12 set: the 4-bit sign
 * extends it to 0xFFF0 = -16 (GBATEK 53: overflows). */
VCT_TEST(p15_vec_test) {
  VCoreState *s = lit_state();
  L(s, VC_OP_VEC_TEST, VEC10(0x1FF, 0x200, 0));
  VCT_EQ(s->p15.vec_result[0], 0xFF8); VCT_EQ(s->p15.vec_result[1], -0x1000); VCT_EQ(s->p15.vec_result[2], 0);
  VC_Mtx m = identity();
  m.m[0] = 0x2000;
  set_mtx(vc_mtx_vec(s), &m);
  L(s, VC_OP_VEC_TEST, VEC10(0x1FF, 0, 0));
  VCT_EQ(s->p15.vec_result[0], -16);
  VCT_EQ(s->io9[0x630], 0xF0); VCT_EQ(s->io9[0x631], 0xFF);
}

/* texgen 0: (S', T') = (S, T), the matrix is not used. TEXCOORD bits 0-15 S, 16-31 T, signed 12.4 */
VCT_TEST(p15_texgen0) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  VC_Mtx m = identity();
  m.m[0] = 0x4000;
  set_mtx(vc_mtx_tex(s), &m);
  L(s, VC_OP_TEXCOORD, 0x00200010);
  attrs(s, &v);
  VCT_EQ(v.tex[0], 16); VCT_EQ(v.tex[1], 32);
  L(s, VC_OP_TEXCOORD, 0xFFE0FFF0u);
  attrs(s, &v);
  VCT_EQ(v.tex[0], -16); VCT_EQ(v.tex[1], -32);
}

/* texgen 1 (TEXCOORD x matrix): s' = (S*m0 + T*m4 + m8 + m12) >> 12, t' = (S*m1 + T*m5 + m9 + m13) >> 12.
 * Scale 2 (m0 = m5 = 0x2000) and m12 = 0x10000, m13 = 0x20000, S = 16, T = 32:
 * s' = (16*0x2000 + 0x10000) >> 12 = 0x30000 >> 12 = 48; t' = (32*0x2000 + 0x20000) >> 12 = 0x60000 >> 12 = 96. */
VCT_TEST(p15_texgen1_scale_translate) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  VC_Mtx m = identity();
  m.m[0] = m.m[5] = 0x2000; m.m[12] = 0x10000; m.m[13] = 0x20000;
  set_mtx(vc_mtx_tex(s), &m);
  s->p15.texparam = 1u << 30;
  L(s, VC_OP_TEXCOORD, 32u << 16 | 16);
  attrs(s, &v);
  VCT_EQ(v.tex[0], 48); VCT_EQ(v.tex[1], 96);
}

/* texgen 1 with a swap matrix (m0 = m5 = 0, m4 = m1 = 1.0): s' = T = 32, t' = S = 16 */
VCT_TEST(p15_texgen1_rotate) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  VC_Mtx m;
  memset(&m, 0, sizeof m);
  m.m[4] = 0x1000; m.m[1] = 0x1000; m.m[15] = 0x1000;
  set_mtx(vc_mtx_tex(s), &m);
  s->p15.texparam = 1u << 30;
  L(s, VC_OP_TEXCOORD, 32u << 16 | 16);
  attrs(s, &v);
  VCT_EQ(v.tex[0], 32); VCT_EQ(v.tex[1], 16);
}

/* the result is masked to 16 bits: S = 0x7FFF, m0 = 4.0: 0x7FFF*0x4000 >> 12 = 0x1FFFC -> 0xFFFC = -4 */
VCT_TEST(p15_texgen1_16bit_mask) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  VC_Mtx m = identity();
  m.m[0] = 0x4000;
  set_mtx(vc_mtx_tex(s), &m);
  s->p15.texparam = 1u << 30;
  L(s, VC_OP_TEXCOORD, 0x7FFF);
  attrs(s, &v);
  VCT_EQ(v.tex[0], -4);
}

/* texgen 2 (normal source), at NORMAL: (N * matrix) >> 20 + (S, T) with N 1.12. TEXCOORD S = 16, T = 32 first.
 * m0 = m5 = 2.0 (0x2000); N = (0, 0x100 = 0.5 -> 0x800, 0): s' = 0 + 16 = 16; t' = (0x800*0x2000)>>20 = 16, + 32 = 48.
 * Before the NORMAL the vertex has just (S, T). */
VCT_TEST(p15_texgen2_normal) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  VC_Mtx m = identity();
  m.m[0] = m.m[5] = 0x2000;
  set_mtx(vc_mtx_tex(s), &m);
  s->p15.texparam = 2u << 30;
  L(s, VC_OP_TEXCOORD, 32u << 16 | 16);
  attrs(s, &v);
  VCT_EQ(v.tex[0], 16); VCT_EQ(v.tex[1], 32);
  L(s, VC_OP_NORMAL, VEC10(0, 0x100, 0));
  attrs(s, &v);
  VCT_EQ(v.tex[0], 16); VCT_EQ(v.tex[1], 48);
  /* in mode 0 a NORMAL leaves the texture coordinates alone */
  s->p15.texparam = 0;
  L(s, VC_OP_TEXCOORD, 32u << 16 | 16);
  L(s, VC_OP_NORMAL, VEC10(0, 0x100, 0));
  attrs(s, &v);
  VCT_EQ(v.tex[1], 32);
}

/* texgen 3 (vertex source), at the vertex: (V * matrix) >> 20 + (S, T). V = (1.0, 2.0, 0) = (0x1000, 0x2000, 0),
 * identity matrix: s' = (0x1000*0x1000)>>20 = 16 (one texel), t' = (0x2000*0x1000)>>20 = 32, plus S = 16, T = 32:
 * (32, 64).
 * V = (-1.0, 0, 0): (-0x1000*0x1000)>>20 = -16, + 16 = 0. */
VCT_TEST(p15_texgen3_vertex) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  s32 p1[4] = {0x1000, 0x2000, 0, 0x1000};
  s32 p2[4] = {-0x1000, 0, 0, 0x1000};
  s->p15.texparam = 3u << 30;
  L(s, VC_OP_TEXCOORD, 32u << 16 | 16);
  memset(&v, 0, sizeof v);
  vc_vertex_attrs(s, p1, &v);
  VCT_EQ(v.tex[0], 32); VCT_EQ(v.tex[1], 64);
  vc_vertex_attrs(s, p2, &v);
  VCT_EQ(v.tex[0], 0); VCT_EQ(v.tex[1], 32);
}

/* The lights in force and the texgen mode come from the command stream: POLYGON_ATTR is applied at the next
 * BEGIN_VTXS (a NORMAL between them is unlit), TEXIMAGE_PARAM at once. Played as piece 20 plays a stream. */
VCT_TEST(p15_stream_attr_latch) {
  VCoreState *s = vct_state();
  VC_Mtx id = identity();
  set_mtx(vc_mtx_vec(s), &id);
  set_mtx(vc_mtx_tex(s), &id);
  u32 w;
#define G(op, v) do { w = (v); vct_gx(s, op, 1, &w); } while (0)
  G(VC_OP_DIF_AMB, RGB5(16, 0, 0));
  G(VC_OP_SPE_EMI, RGB5(0, 0, 0) | (RGB5(1, 0, 0) << 16));
  G(VC_OP_LIGHT_COLOR, RGB5(31, 31, 31));
  G(VC_OP_LIGHT_VECTOR, VEC10(0, 0, 0x200));
  G(VC_OP_POLYGON_ATTR, 1);
  G(VC_OP_TEXIMAGE_PARAM, 3u << 30);
  G(VC_OP_TEXCOORD, 16);
  G(VC_OP_NORMAL, VEC10(0, 0, 0x1FF)); /* attr not yet applied: emission only */
  G(VC_OP_VTX_16, 0);
  G(VC_OP_BEGIN_VTXS, 0);
  G(VC_OP_NORMAL, VEC10(0, 0, 0x1FF)); /* lit: 1 + 15 = 16 */
  G(VC_OP_VTX_16, 0);
#undef G
  vc_light_reset(s);
  int nvtx = 0;
  u8 red[2] = {0, 0};
  for (u32 i = 0; i < s->gx_words;) {
    u32 h = s->gx[i++], op = h & 0xFF, n = (h >> 8) & 0xFF;
    if (op == VC_OP_VTX_16) {
      VC_Vertex v;
      s32 pos[4] = {0, 0, 0, 0x1000};
      memset(&v, 0, sizeof v);
      vc_vertex_attrs(s, pos, &v);
      red[nvtx++] = v.color[0];
      VCT_EQ(v.tex[0], 16); /* texgen 3 with the identity matrix and pos 0: just S (TEXIMAGE_PARAM followed) */
    } else if (op == VC_OP_COLOR || op == VC_OP_NORMAL || op == VC_OP_TEXCOORD ||
               (op >= VC_OP_DIF_AMB && op <= VC_OP_SHININESS) || op == VC_OP_VEC_TEST) {
      vc_cmd_light(s, op, s->gx + i, n);
    }
    i += n;
  }
  VCT_EQ(nvtx, 2);
  VCT_EQ(red[0], 1);
  VCT_EQ(red[1], 16);
}

/* ---- synthetic scenes for the oracle (VCT_EMIT=DIR vcore_tests p15): each a lit or textured triangle in an
 * otherwise plain frame (engine A BG0 = 3D, display mode 1). They also run as tests (no output without VCT_EMIT), and
 * the emitted stream is checked to hold the commands. ---- */
static void G(VCoreState *s, u32 op, u32 n, u32 a, u32 b, u32 c, u32 d) {
  u32 p[4] = {a, b, c, d};
  vct_gx(s, op, n, p);
}
static void G16(VCoreState *s, u32 op, const VC_Mtx *m) { vct_gx(s, op, 16, (const u32 *)m->m); }

static VCoreState *scene_base(void) {
  VCoreState *s = vct_state();
  vct_io16(s, VC_REG_DISPCNT, 0x0108); /* mode 0, BG0 on, 3D */
  vct_io32(s, VC_REG_DISPCNT, 0x00010108);
  vct_io16(s, VC_REG_POWCNT1, 0x820F);
    vct_io16(s, VC_REG_CLEAR_DEPTH, 0x7FFF);
  vct_io16(s, VC_REG_DISP3DCNT, 0x0000);
  VC_Mtx id = identity();
  G(s, VC_OP_MTX_MODE, 1, 0, 0, 0, 0);
  G16(s, VC_OP_MTX_LOAD_4x4, &id);
  G(s, VC_OP_MTX_MODE, 1, 2, 0, 0, 0);
  G(s, VC_OP_MTX_IDENTITY, 0, 0, 0, 0, 0);
  G(s, VC_OP_VIEWPORT, 1, 0xBFFF0000u, 0, 0, 0);
  return s;
}
/* a triangle with a normal and a texcoord at each corner */
static void tri3(VCoreState *s, const u32 nrm[3], const u32 tc[3]) {
  static const s32 vx[3][2] = {{-0xE00, -0xE00}, {0xE00, -0xB00}, {-0x200, 0xE00}};
  G(s, VC_OP_BEGIN_VTXS, 1, 0, 0, 0, 0);
  for (int i = 0; i < 3; i++) {
    if (nrm) G(s, VC_OP_NORMAL, 1, nrm[i], 0, 0, 0);
    if (tc) G(s, VC_OP_TEXCOORD, 1, tc[i], 0, 0, 0);
    G(s, VC_OP_VTX_16, 2, ((u32)vx[i][0] & 0xFFFF) | ((u32)vx[i][1] & 0xFFFF) << 16, 0, 0, 0);
  }
  G(s, VC_OP_END_VTXS, 0, 0, 0, 0, 0);
  G(s, VC_OP_SWAP_BUFFERS, 1, 0, 0, 0, 0);
}
static const u32 kNormals[3] = {0x00000000u | (0x1FFu << 20) /* +z */, 0x1FFu | (0x1FFu << 20) /* +x+z */,
                                (0x200u << 10) | (0x1FFu << 20) /* -y+z */};

VCT_TEST(p15_scene_light1) { /* one light, diffuse + ambient + emission, normals varying per corner */
  VCoreState *s = scene_base();
  G(s, VC_OP_DIF_AMB, 1, RGB5(24, 12, 20) | (RGB5(6, 3, 4) << 16), 0, 0, 0);
  G(s, VC_OP_SPE_EMI, 1, (RGB5(2, 1, 0) << 16), 0, 0, 0);
  G(s, VC_OP_LIGHT_COLOR, 1, RGB5(31, 28, 20), 0, 0, 0);
  G(s, VC_OP_LIGHT_VECTOR, 1, VEC10(100, -150, -400), 0, 0, 0);
  G(s, VC_OP_POLYGON_ATTR, 1, 0x001F00C0 | 1, 0, 0, 0);
  tri3(s, kNormals, NULL);
  VCT_CHECK(s->gx_words > 20);
  vct_emit(s, "p15_light1");
}

VCT_TEST(p15_scene_light4_spec) { /* four lights, specular with and without the shininess table */
  for (int table = 0; table < 2; table++) {
    VCoreState *s = scene_base();
    G(s, VC_OP_DIF_AMB, 1, RGB5(14, 18, 10) | (RGB5(3, 3, 3) << 16), 0, 0, 0);
    G(s, VC_OP_SPE_EMI, 1, RGB5(20, 16, 24) | (table ? 0x8000u : 0) | (RGB5(0, 1, 2) << 16), 0, 0, 0);
    if (table) {
      u32 tb[32];
      for (int i = 0; i < 128; i++) ((u8 *)tb)[i] = (u8)(i * i / 64);
      vct_gx(s, VC_OP_SHININESS, 32, tb);
    }
    for (u32 i = 0; i < 4; i++) {
      G(s, VC_OP_LIGHT_COLOR, 1, RGB5(8 + 6 * i, 31 - 7 * i, 5 + 8 * i) | i << 30, 0, 0, 0);
      G(s, VC_OP_LIGHT_VECTOR, 1, VEC10(150 * (int)i - 200, 100 - 80 * (int)i, -300 - 40 * (int)i) | i << 30, 0, 0, 0);
    }
    G(s, VC_OP_POLYGON_ATTR, 1, 0x001F00C0 | 0xF, 0, 0, 0);
    tri3(s, kNormals, NULL);
    vct_emit(s, table ? "p15_light4_spectable" : "p15_light4_spec");
  }
}

VCT_TEST(p15_scene_light_rotated) { /* the light vector and the normal through a rotated directional matrix */
  VCoreState *s = scene_base();
  VC_Mtx r = identity();
  r.m[0] = 0x0DDB; r.m[1] = 0x0800; r.m[4] = -0x0800; r.m[5] = 0x0DDB; /* about z by 30 degrees (cos .866, sin .5) */
  G(s, VC_OP_MTX_MODE, 1, 2, 0, 0, 0);
  G16(s, VC_OP_MTX_LOAD_4x4, &r);
  G(s, VC_OP_DIF_AMB, 1, RGB5(28, 28, 28) | (RGB5(2, 2, 2) << 16), 0, 0, 0);
  G(s, VC_OP_SPE_EMI, 1, RGB5(10, 10, 10), 0, 0, 0);
  G(s, VC_OP_LIGHT_COLOR, 1, RGB5(31, 31, 31), 0, 0, 0);
  G(s, VC_OP_LIGHT_VECTOR, 1, VEC10(-300, 200, -250), 0, 0, 0);
  G(s, VC_OP_POLYGON_ATTR, 1, 0x001F00C0 | 1, 0, 0, 0);
  tri3(s, kNormals, NULL);
  vct_emit(s, "p15_light_rot");
}

/* a 16x16 direct-colour texture in bank A (texture slot 0) */
static void tex_setup(VCoreState *s) {
  vct_io8(s, 0x240, 0x83);
  u16 *t = (u16 *)(s->vram + vc_bank_offset[VC_BANK_A]);
  for (int y = 0; y < 16; y++)
    for (int x = 0; x < 16; x++) t[y * 16 + x] = (u16)(0x8000 | (x * 2) | ((y * 2) << 5) | (((x ^ y) & 1) ? 0 : (0x10 << 10)));
  vct_io16(s, VC_REG_DISP3DCNT, 0x0001); /* textures on */
}
static void texgen_scene(int mode, const char *name) {
  VCoreState *s = scene_base();
  tex_setup(s);
  VC_Mtx m = identity();
  if (mode == 1) { m.m[0] = 0x0C00; m.m[1] = 0x0400; m.m[4] = -0x0400; m.m[5] = 0x0C00; m.m[12] = 0x4000; m.m[13] = 0x2000; }
  if (mode == 2) { m.m[0] = m.m[5] = 0x8000; }
  if (mode == 3) { m.m[0] = m.m[5] = 0x10000; m.m[1] = 0x2000; }
  G(s, VC_OP_MTX_MODE, 1, 3, 0, 0, 0);
  G16(s, VC_OP_MTX_LOAD_4x4, &m);
  G(s, VC_OP_POLYGON_ATTR, 1, 0x001F00C0, 0, 0, 0);
  G(s, VC_OP_TEXIMAGE_PARAM, 1, (7u << 26) | (1u << 23) | (1u << 20) | (3u << 16) | ((u32)mode << 30), 0, 0, 0);
  G(s, VC_OP_COLOR, 1, 0x7FFF, 0, 0, 0);
  static const u32 tc[3] = {0x00000000u, 0x00000100u, 0x01000000u};
  static const u32 tc2[3] = {0x00800080u, 0x00800080u, 0x00800080u};
  tri3(s, mode == 2 ? kNormals : NULL, mode == 2 ? tc2 : tc);
  vct_emit(s, name);
}
VCT_TEST(p15_scene_texgen0) { texgen_scene(0, "p15_texgen0"); }
VCT_TEST(p15_scene_texgen1) { texgen_scene(1, "p15_texgen1"); }
VCT_TEST(p15_scene_texgen2) { texgen_scene(2, "p15_texgen2"); }
VCT_TEST(p15_scene_texgen3) { texgen_scene(3, "p15_texgen3"); }

/* Oracle-measured rules (the black box, tabulated; GBATEK gives no widths). Diffuse level has 9 bits and each of the
 * three products is cut to 9 fraction bits before adding: L=(0,0,-511/512)... on the z axis L=-511, N=400 -> level
 * floor(511*400/512) = 399 (/512), dif 26 x light 30 -> 26*30*399 >> 14 = 18 (not 19, which a 256-step level gives). */
VCT_TEST(p15_diffuse_level_9bit) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_DIF_AMB, RGB5(26, 0, 0));
  L(s, VC_OP_LIGHT_COLOR, RGB5(30, 0, 0));
  L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, -511));
  s->p15.polyattr = 1;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 400));
  attrs(s, &v);
  VCT_EQ(v.color[0], 18);
}

/* Specular on the z axis, L=(0,0,-1.0): level = 2 n^2 - 1 (oracle: n=480/512 -> 0.748 of full; 256/512 -> 0). With
 * spe 31 x light 31 the colour is 961 * level9 >> 14: n=480: level9 ~ 385 -> 22. */
VCT_TEST(p15_specular_z_axis) {
  VCoreState *s = lit_state();
  VC_Vertex v;
  L(s, VC_OP_SPE_EMI, RGB5(31, 31, 31));
  L(s, VC_OP_LIGHT_COLOR, RGB5(31, 31, 31));
  L(s, VC_OP_LIGHT_VECTOR, VEC10(0, 0, -512));
  s->p15.polyattr = 1;
  L(s, VC_OP_NORMAL, VEC10(0, 0, 480));
  attrs(s, &v);
  VCT_CHECK(v.color[0] >= 21 && v.color[0] <= 23);
  L(s, VC_OP_NORMAL, VEC10(0, 0, 256));
  attrs(s, &v);
  VCT_EQ(v.color[0], 0);
}
