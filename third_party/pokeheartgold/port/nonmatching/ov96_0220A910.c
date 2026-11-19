#include "error_handling.h"
#include "touchscreen.h"
#include "touchscreen.h"
s32 _ll_mul(s32, s32, s32, s32);
s32 ov96_0220B148(s32, void *, void *);
extern s16 ov96_0221CDD8[];

typedef struct CourseBits { /* invented */
    u8 pad[0x180];
    u32 unk180_0 : 4;
    u32 unk180_4 : 4;
    u32 rest : 24;
} CourseBits;

s32 ov96_0220A910(void *arg0)
{
  u32 sp18;
  u32 sp14;
  s16 sp10[2];
  s32 sp1C[12];
  s32 spC;
  s32 sp8;
  s32 sp4;
  s32 sp0;
  s32 temp;
  s32 temp4;
  fx32 new_var;
  s32 temp9;
  s32 i;
  if (arg0 == 0L)
  {
    GF_AssertFail();
  }
  if (((CourseBits *) arg0)->unk180_0 == 1)
  {
    return 0;
  }
  if (((CourseBits *) arg0)->unk180_4 == 0)
  {
    GF_AssertFail();
  }
  ov96_0220B148(*((s32 *) ((((u8 *) arg0) + 0x150) + (((CourseBits *) arg0)->unk180_4 * 4))), &sp10[1], &sp10[0]);
  System_GetTouchNewCoords(&sp18, &sp14);
  sp0 = sp10[0];
  if ((sp14 >= ((u32) (sp0 - 0x12))) && (sp14 <= ((u32) (sp0 + 0xC))))
  {
    spC = (s32) ((sp18 != 0) ? (0.5f + ((f32) (sp18 << 0xC))) : (((f32) (sp18 << 0xC)) - 0.5f));
    sp8 = (s32) ((sp14 != 0) ? (0.5f + ((f32) (sp14 << 0xC))) : (((f32) (sp14 << 0xC)) - 0.5f));
    sp4 = sp10[1];
    for (i = 0; i < 4; i++)
    {
      temp = sp4 + ov96_0221CDD8[i * 2];
      sp1C[i * 3] = (s32) (((sp4 + ov96_0221CDD8[i * 2]) > 0) ? (0.5f + ((f32) ((sp4 + ov96_0221CDD8[i * 2]) << 0xC))) : (((f32) (temp << 0xC)) - 0.5f));
      temp = sp0 + ov96_0221CDD8[(i * 2) + 1];
      sp1C[(i * 3) + 1] = (s32) ((temp > 0) ? (0.5f + ((f32) (temp << 0xC))) : (((f32) (temp << 0xC)) - 0.5f));
    }

    temp4 = FX_Div(FX32_CAST(((((fx64) (sp1C[4] - sp1C[1])) * (spC - sp1C[0])) + 0x800LL) >> 12), sp1C[3] - sp1C[0]);
    temp4 = temp4 + sp1C[1];
    new_var = FX_Div(FX32_CAST(((((fx64) (sp1C[10] - sp1C[7])) * (spC - sp1C[6])) + 0x800LL) >> 12), sp1C[9] - sp1C[6]);
    temp9 = new_var + sp1C[7];
    if ((((spC >= sp1C[3]) && (spC <= sp1C[6])) || (((sp8 >= temp4) && (spC >= sp1C[0])) && (spC <= sp1C[3]))) || (((sp8 <= temp9) && (spC >= sp1C[6])) && (spC <= sp1C[9])))
    {
      return 1;
    }
  }
  return 0;
}

