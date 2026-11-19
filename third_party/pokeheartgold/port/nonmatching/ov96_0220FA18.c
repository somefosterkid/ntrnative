#include "pokeathlon/pokeathlon.h"
s32 _s32_div_f(u32, u32);
void ov96_021EB06C(s32 param_1, u32 param_2, s32 param_3, u32 * param_4, s32 * param_5);
typedef struct BF_ov96_0220FA18_0 { u32 f0_24:24; u32 pad24:6; u32 f30_2:2; } BF_ov96_0220FA18_0; /* invented: bitfield word (hk9) */
void ov96_0220FA18(u8 *param_1, PokeathlonCourseData *param_2) {
  u8 *out;
  u32 *w1;
  u32 *w2;
  u32 i;
  u32 sh;
  u8 *in;
  s32 q;
  s32 v;
  s32 x;
  s32 y;
  u32 cnt;

  out = PokeathlonCourse_GetDataCopyArea(param_2);
  out = ov96_021E8A20(out + 0x28);
  w1 = (u32 *)(out + 0x1c);
  i = 0;
  cnt = (*w1 >> 0x1e) + 1;
  ((BF_ov96_0220FA18_0 *)w1)->f30_2 = cnt;
  w2 = (u32 *)(out + 0x20);
  sh = 0;
  *w2 = (((*w2 & 0xffffff80) | ((*(u32 *)(param_1 + (*w1 >> 0x1e) * 0xe4 + 0x170) << 0xe) >> 0x10 & 0x7f)) & 0x8007ffff);
  ((BF_ov96_0220FA18_0 *)w1)->f0_24 = 0;
  *w2 &= 0xfff8007f;
  param_1 += 0x98;
  do {
    q = (s32)i / 3;
    in = param_1 + q * 0xe4 + (s32)i % 3 * 0x48;
    ov96_021EB06C(*(s32 *)(in + 4), (*(s32 *)(in + 0x1c) << 4) >> 0x10,
                  (*(s32 *)(in + 0x20) << 4) >> 0x10, (u32 *)&x, &y);
    if (x > 0xff) {
      v = 0xff;
    } else {
      v = x;
      if (x < 0) {
        v = 0;
      }
    }
    out[i + 4] = v;
    if (y > 0xff) {
      v = 0xff;
    } else {
      v = y;
      if (y < 0) {
        v = 0;
      }
    }
    out[i + 0x10] = v;
    if ((*(u32 *)(in + 0x40) & 0xffff) >> 8 != 0) {
      *w2 = ((((*w2 << 1) >> 0x14) | (1 << i)) << 0x14) >> 1 | (*w2 & 0x8007ffff);
      *(u32 *)(in + 0x40) = (*(u32 *)(in + 0x40) & 0xffff00ff) |
                            (((((*(u32 *)(in + 0x40) << 0x10) >> 0x18) - 1) << 0x18) >> 0x10);
    }
    if (*(s32 *)(in + 0xc) == 3) {
      *w2 = ((((*w2 << 0xd) >> 0x14) | (1 << i)) << 0x14) >> 0xd | (*w2 & 0xfff8007f);
    }
    i++;
    *w1 = (*w1 & 0xff000000) |
          (((((BF_ov96_0220FA18_0 *)w1)->f0_24) + ((((*(u32 *)(in + 0x40) << 8) >> 0x1c) - 1) << sh)) & 0xffffff);
    sh += 2;
  } while ((s32)i < 0xc);
}
