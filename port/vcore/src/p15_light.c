/* port/vcore piece 15: lighting, materials, normals, texture coordinates (docs/vcore.md, piece 15). OWNED BY PIECE 15.
 * Spec: GBATEK 45 (DS 3D Polygon Light Parameters), 49 (Texture Coordinates), 53 (Tests: VEC_TEST), 36 (commands).
 * Vectors are kept 1.12 as the hardware does (VEC_RESULT, GBATEK 53, has a 12-bit fraction): a 10-bit command
 * vector (1 sign + 9 fraction bits) is widened by << 3 before the directional matrix. */
#include "vc_shared.h"

static inline s32 sext10(u32 v) { return (s32)(v << 22) >> 22; }

void vc_light_reset(VCoreState *s) {
  memset(&s->p15, 0, sizeof s->p15);
  /* GBATEK gives no power-on values; zero is what a reset geometry engine leaves. */
}

/* Piece 14 handles POLYGON_ATTR / TEXIMAGE_PARAM / BEGIN_VTXS and does not tell piece 15. The lights in force
 * (POLYGON_ATTR bits 0-3, applied at the next BEGIN_VTXS: GBATEK 45 "Additional Light Registers") and the texgen mode
 * (TEXIMAGE_PARAM bits 30-31) are read by following the frame's command stream. Follows entries from the cursor until
 * the entry at word index `end`, or (stop_vtx) until the VTX command number `stop_vtx - 1` (0-based, counted over the
 * frame) is reached. */
static void p15_follow(VCoreState *s, u32 end, int stop_vtx, u32 vtx_no) {
  VC_P15 *p = &s->p15;
  u32 i = p->cursor;
  while (i < end && i < s->gx_words) {
    u32 h = s->gx[i], op = h & 0xFF, n = (h >> 8) & 0xFF;
    if (n > s->gx_words - i - 1) n = s->gx_words - i - 1;
    if (op >= VC_OP_VTX_16 && op <= VC_OP_VTX_DIFF) {
      if (stop_vtx && p->vtx_passed == vtx_no) break;
      p->vtx_passed++;
    } else if (op == VC_OP_POLYGON_ATTR && n >= 1) p->polyattr_pending = s->gx[i + 1];
    else if (op == VC_OP_BEGIN_VTXS) p->polyattr = p->polyattr_pending;
    else if (op == VC_OP_TEXIMAGE_PARAM && n >= 1) p->texparam = s->gx[i + 1];
    i += 1 + n;
  }
  if (i > p->cursor) p->cursor = i;
}

static void p15_sync_param(VCoreState *s, const u32 *param) {
  if (s->gx_words == 0 || param == NULL) return;
  if (param <= s->gx || param > s->gx + s->gx_words) return;
  p15_follow(s, (u32)(param - s->gx) - 1, 0, 0);
}

static void p15_sync_vtx(VCoreState *s) {
  if (s->gx_words == 0) return;
  p15_follow(s, s->gx_words, 1, s->p15.nvtx_seen);
}

static void set_color(VC_P15 *p, u32 v) {
  p->color[0] = v & 31; p->color[1] = (v >> 5) & 31; p->color[2] = (v >> 10) & 31;
}

/* v * M with the upper-left 3x3 of a matrix (GBATEK 45 "Maths Notes"); 1.12 in, 1.12 out */
static void vec_mtx3(const VC_Mtx *m, const s32 v[3], s32 out[3]) {
  for (int j = 0; j < 3; j++) {
    s64 sum = (s64)v[0] * m->m[j] + (s64)v[1] * m->m[4 + j] + (s64)v[2] * m->m[8 + j];
    out[j] = (s32)(sum >> 12);
  }
}

static s16 mask16(s64 v) { return (s16)(u16)((u64)v & 0xFFFF); }

static void light_normal(VCoreState *s, u32 param) {
  VC_P15 *p = &s->p15;
  s32 n[3] = {sext10(param) << 3, sext10(param >> 10) << 3, sext10(param >> 20) << 3};
  /* texgen mode 2: TexCoord = (Nx Ny Nz 1.0) * matrix with (S T) as the last row (GBATEK 49), on the normal before
   * the directional matrix. 1.12 normal * 1.12 matrix = 24 fraction bits -> 4 (>> 20), then + S/T. */
  if ((p->texparam >> 30) == 2) {
    const VC_Mtx *tm = vc_mtx_tex(s);
    for (int c = 0; c < 2; c++) {
      s64 sum = (s64)n[0] * tm->m[c] + (s64)n[1] * tm->m[4 + c] + (s64)n[2] * tm->m[8 + c];
      p->tex[c] = mask16((sum >> 20) + p->texraw[c]);
    }
  }
  s32 nv[3];
  vec_mtx3(vc_mtx_vec(s), n, nv);
  for (int i = 0; i < 3; i++) p->normal[i] = (s16)nv[i];

  /* GBATEK 45 "Internal Operation on Normal Command". The terms are summed unrounded and cut once at the end (the
   * black-box oracle shows 16+8 diffuse/ambient adding to 23, not 15 + 7). Colours are 5 bits (/32); GBATEK does not
   * give the light levels' width, the oracle shows 9 bits (a diffuse level of 199.5/256 for a dot of 199.6: the dot
   * is cut at 1/512, not 1/256), so a product colour x colour x level has 14 fraction bits. */
  s64 acc[3];
  for (int c = 0; c < 3; c++) acc[c] = (s64)p->emi[c] << 14;
  for (int i = 0; i < 4; i++) {
    if (!((p->polyattr >> i) & 1)) continue;
    /* each product is cut to 9 fraction bits before the three are added (the oracle: 60/60 scenes), the negation
     * taken before the cut (floor of the negated product) */
    s64 d = 0;
    for (int k = 0; k < 3; k++) d += (-((s64)p->ldir[i][k] * p->normal[k])) >> 15;
    s32 dlevel = d < 0 ? 0 : (s32)d;
    if (dlevel > 511) dlevel = 511;
    /* Specular level. GBATEK 45 squares the half-vector dot product. The oracle (tabulated over 3600 single-light
     * probes: z axis, random unit lights, normals near the half vector, short lights) shows level = 2 h^2 / Y - 1
     * with h = -(H.N), Y = -Hz, H = (L + (0,0,-1)) / 2. Integer form that fits 96% of the probes to the 1/512 (the
     * rest are 1 off): with D = L + (0,0,-1.0) (= 2H) and the three products D.N each cut to 10 fraction bits, h2 =
     * sum of those, Yq = -Dz at 10 bits: level = h2*h2 / Yq - 514 (the 2 is a fitted constant, not derived). */
    s64 h = 0;
    for (int k = 0; k < 3; k++) h += (-((s64)(p->ldir[i][k] - (k == 2 ? 0x1000 : 0)) * p->normal[k])) >> 15;
    s64 y = (0x1000 - (s64)p->ldir[i][2]) >> 3;
    s32 slevel = 0;
    if (h > 0 && y > 0) {
      s64 l = h * h / y - 514;
      if (l < 0) l = 0;
      if (l > 511) l = 511;
      slevel = (s32)l;
      if (p->shin_table_on) slevel = p->shin_table[slevel >> 2] << 1;
    }
    for (int c = 0; c < 3; c++) {
      acc[c] += (s64)p->spe[c] * p->lcolor[i][c] * slevel;
      acc[c] += (s64)p->dif[c] * p->lcolor[i][c] * dlevel;
      acc[c] += ((s64)p->amb[c] * p->lcolor[i][c]) << 9;
    }
  }
  for (int c = 0; c < 3; c++) {
    s64 v = acc[c] >> 14;
    p->color[c] = (u8)(v > 31 ? 31 : v);
  }
}

void vc_cmd_light(VCoreState *s, u32 op, const u32 *param, u32 nparam) {
  VC_P15 *p = &s->p15;
  u32 w = nparam ? param[0] : 0;
  p15_sync_param(s, param);
  switch (op) {
  case VC_OP_COLOR: set_color(p, w); break;
  case VC_OP_TEXCOORD:
    p->texraw[0] = (s16)(w & 0xFFFF);
    p->texraw[1] = (s16)(w >> 16);
    if ((p->texparam >> 30) == 1) {
      /* mode 1: (S T 1/16 1/16) * matrix: the two 1/16 are the raw value 1 (4 fraction bits); the sum has 16
       * fraction bits (4 + 12), shifted to 4 and masked to 16 bits */
      const VC_Mtx *tm = vc_mtx_tex(s);
      for (int c = 0; c < 2; c++)
        p->tex[c] = mask16(((s64)p->texraw[0] * tm->m[c] + (s64)p->texraw[1] * tm->m[4 + c] + tm->m[8 + c] +
                            tm->m[12 + c]) >> 12);
    } else {
      p->tex[0] = p->texraw[0];
      p->tex[1] = p->texraw[1];
    }
    break;
  case VC_OP_NORMAL: light_normal(s, w); break;
  case VC_OP_DIF_AMB:
    p->dif[0] = w & 31; p->dif[1] = (w >> 5) & 31; p->dif[2] = (w >> 10) & 31;
    p->amb[0] = (w >> 16) & 31; p->amb[1] = (w >> 21) & 31; p->amb[2] = (w >> 26) & 31;
    if (w & 0x8000) set_color(p, w);
    break;
  case VC_OP_SPE_EMI:
    p->spe[0] = w & 31; p->spe[1] = (w >> 5) & 31; p->spe[2] = (w >> 10) & 31;
    p->shin_table_on = (w >> 15) & 1;
    p->emi[0] = (w >> 16) & 31; p->emi[1] = (w >> 21) & 31; p->emi[2] = (w >> 26) & 31;
    break;
  case VC_OP_LIGHT_VECTOR: {
    int i = w >> 30;
    s32 v[3] = {sext10(w) << 3, sext10(w >> 10) << 3, sext10(w >> 20) << 3}, d[3];
    vec_mtx3(vc_mtx_vec(s), v, d);
    for (int k = 0; k < 3; k++) p->ldir[i][k] = (s16)d[k];
    /* HalfVector = (LightVector + LineOfSight) / 2, LineOfSight = (0, 0, -1.0) always */
    p->lhalf[i][0] = (s16)(p->ldir[i][0] >> 1);
    p->lhalf[i][1] = (s16)(p->ldir[i][1] >> 1);
    p->lhalf[i][2] = (s16)((p->ldir[i][2] - 0x1000) >> 1);
    break;
  }
  case VC_OP_LIGHT_COLOR: {
    int i = w >> 30;
    p->lcolor[i][0] = w & 31; p->lcolor[i][1] = (w >> 5) & 31; p->lcolor[i][2] = (w >> 10) & 31;
    break;
  }
  case VC_OP_SHININESS:
    /* 32 words, four 8-bit entries each, entry 0 in the lowest byte of the first word */
    for (u32 k = 0; k < nparam && k < 32; k++)
      for (int b = 0; b < 4; b++) p->shin_table[k * 4 + b] = (u8)(param[k] >> (8 * b));
    break;
  case VC_OP_VEC_TEST: {
    /* GBATEK 53: (x y z 0) * directional matrix, results 4-bit sign + 12-bit fraction */
    s32 v[3] = {sext10(w) << 3, sext10(w >> 10) << 3, sext10(w >> 20) << 3}, d[3];
    vec_mtx3(vc_mtx_vec(s), v, d);
    for (int k = 0; k < 3; k++) {
      p->vec_result[k] = (s16)(((s32)((u32)d[k] << 19)) >> 19); /* 13 bits: sign + 12 fraction, sign-extended */
      s->io9[0x630 + 2 * k] = (u8)p->vec_result[k];
      s->io9[0x631 + 2 * k] = (u8)((u16)p->vec_result[k] >> 8);
    }
    break;
  }
  default: break;
  }
}

void vc_vertex_attrs(VCoreState *s, const s32 pos[4], VC_Vertex *out) {
  VC_P15 *p = &s->p15;
  p15_sync_vtx(s);
  p->nvtx_seen++;
  out->color[0] = p->color[0]; out->color[1] = p->color[1]; out->color[2] = p->color[2];
  if ((p->texparam >> 30) == 3) {
    /* mode 3: (Vx Vy Vz 1.0) * matrix, the last row (S T): 1.12 * 1.12 = 24 fraction bits -> 4 */
    const VC_Mtx *tm = vc_mtx_tex(s);
    for (int c = 0; c < 2; c++) {
      s64 sum = (s64)pos[0] * tm->m[c] + (s64)pos[1] * tm->m[4 + c] + (s64)pos[2] * tm->m[8 + c];
      p->tex[c] = mask16((sum >> 20) + p->texraw[c]);
    }
  }
  out->tex[0] = p->tex[0];
  out->tex[1] = p->tex[1];
}
