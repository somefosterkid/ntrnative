/* port/vcore piece 15: lighting, materials, normals, texture coordinates (docs/vcore.md, piece 15). OWNED BY PIECE 15:
 * its own state lives here; the functions other pieces call are declared in vc_shared.h. */
#ifndef VC_P15_H
#define VC_P15_H

#include "vc_types.h"

typedef struct VC_P15 {
  /* the colour in force (0-31 per channel) and the texture coordinates */
  u8 color[3];
  s16 texraw[2];  /* the last TEXCOORD, as sent (12.4) */
  s16 tex[2];     /* the texture coordinates a vertex gets now (12.4, after the transform of modes 1/2) */
  /* materials (GBATEK 45: DIF_AMB, SPE_EMI) */
  u8 dif[3], amb[3], spe[3], emi[3];
  u8 shin_table_on;
  u8 shin_table[128];
  /* lights 0-3: colour, direction (vector matrix applied, 1.12), half vector (1.12) */
  u8 lcolor[4][3];
  s16 ldir[4][3];
  s16 lhalf[4][3];
  s16 normal[3];     /* the NormalVector in force (1.12, after the directional matrix) */
  s16 vec_result[3]; /* VEC_TEST's result (GBATEK 53: 4-bit sign, 12-bit fraction) */
  /* POLYGON_ATTR (latched at BEGIN_VTXS; the lights enabled are bits 0-3) and TEXIMAGE_PARAM (bits 30-31 the texgen
   * mode) as they are in force. Piece 14 owns those commands and does not pass them on, so this piece follows the
   * frame's command stream (s->gx) itself up to the command it runs; a test with no stream sets these fields. */
  u32 polyattr, polyattr_pending, texparam;
  u32 cursor;      /* s->gx word index the stream has been followed to */
  u32 vtx_passed;  /* VTX commands behind the cursor */
  u32 nvtx_seen;   /* vertices vc_vertex_attrs has served */
} VC_P15;

#endif
