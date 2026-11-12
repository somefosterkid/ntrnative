/* port/acore piece 2: IMA-ADPCM (docs/acore.md; GBATEK "IMA-ADPCM Format", "DS Sound Notes"). OWNED BY PIECE 2.
 * Per channel: the predictor (PCM16), the table index, and the pair saved at the loop start. The header word at SAD
 * gives the first pair; the 4-bit codes follow, low nibble first. */
#include "ac_shared.h"

static const u8 k_index_adj[8] = {0xFF, 0xFF, 0xFF, 0xFF, 2, 4, 6, 8}; /* -1,-1,-1,-1,2,4,6,8 */

/* AdpcmTable[0..88]; [89..127] = 0 (the header's 7-bit index can exceed 88: such a step has diff 0 until clamped) */
static const u16 k_step[128] = {
    0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x0010, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019,
    0x001C, 0x001F, 0x0022, 0x0025, 0x0029, 0x002D, 0x0032, 0x0037, 0x003C, 0x0042, 0x0049, 0x0050, 0x0058, 0x0061,
    0x006B, 0x0076, 0x0082, 0x008F, 0x009D, 0x00AD, 0x00BE, 0x00D1, 0x00E6, 0x00FD, 0x0117, 0x0133, 0x0151, 0x0173,
    0x0198, 0x01C1, 0x01EE, 0x0220, 0x0256, 0x0292, 0x02D4, 0x031C, 0x036C, 0x03C3, 0x0424, 0x048E, 0x0502, 0x0583,
    0x0610, 0x06AB, 0x0756, 0x0812, 0x08E0, 0x09C3, 0x0ABD, 0x0BD0, 0x0CFF, 0x0E4C, 0x0FBA, 0x114C, 0x1307, 0x14EE,
    0x1706, 0x1954, 0x1BDC, 0x1EA5, 0x21B6, 0x2515, 0x28CA, 0x2CDF, 0x315B, 0x364B, 0x3BB9, 0x41B2, 0x4844, 0x4F7E,
    0x5771, 0x602F, 0x69CE, 0x7462, 0x7FFF};

void ac_adpcm_start(AcoreState *s, int ch) {
  AC_P02 *p = &s->p02;
  u32 sad = ac_ch_reg32(s, ch, AC_CH_SAD) & 0x07FFFFFCu;
  u32 hdr = ac_rd32(s, sad);
  p->sad[ch] = sad;
  p->pred[ch] = (s32)(s16)(hdr & 0xFFFF);
  p->idx[ch] = (hdr >> 16) & 0x7F;
  p->spred[ch] = p->pred[ch];
  p->sidx[ch] = p->idx[ch];
}

/* one 4-bit code d on the pair (pred, idx): the new sample. The channels' decoder below and the stream's (ac_strm.c) */
s32 ac_adpcm_step(s32 *pred, u32 *idx, u32 d) {
  u32 t = k_step[*idx];
  s32 diff = (s32)(t >> 3); /* "in practice": each bit adds its own truncated share */
  if (d & 1) diff += (s32)(t >> 2);
  if (d & 2) diff += (s32)(t >> 1);
  if (d & 4) diff += (s32)t;
  s32 v = *pred;
  if (d & 8) {
    v -= diff;
    if (v < -0x7FFF) v = -0x7FFF; /* clips -8000h too */
  } else {
    v += diff;
    if (v > 0x7FFF) v = 0x7FFF; /* a start value of -8000h is not clipped upward */
  }
  *pred = v;
  s32 i = (s32)*idx + (s32)(s8)k_index_adj[d & 7];
  *idx = i < 0 ? 0 : i > 88 ? 88 : (u32)i;
  return v;
}

s32 ac_adpcm_next(AcoreState *s, int ch, u32 nib) {
  AC_P02 *p = &s->p02;
  u8 b = ac_rd8(s, p->sad[ch] + 4 + (nib >> 1));
  u32 d = (nib & 1) ? (u32)(b >> 4) : (u32)(b & 15);
  return ac_adpcm_step(&p->pred[ch], &p->idx[ch], d);
}

void ac_adpcm_loop_save(AcoreState *s, int ch) {
  s->p02.spred[ch] = s->p02.pred[ch];
  s->p02.sidx[ch] = s->p02.idx[ch];
}

void ac_adpcm_loop_restore(AcoreState *s, int ch) {
  s->p02.pred[ch] = s->p02.spred[ch];
  s->p02.idx[ch] = s->p02.sidx[ch];
}
