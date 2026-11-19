#include "pokeathlon/pokeathlon.h"
s32 ov96_02205AFC(VecFx32 *, u8 *);
s32 ov96_02205DD4(VecFx32 *, u8, u8, u8);
s32 ov96_02205E30(PokeathlonCourseData *);
f32 ov96_022073F0(VecFx32 *, s32);
s32 ov96_02207400(VecFx32 *, VecFx32 *, VecFx32 *);
s32 ov96_02207870(VecFx32 *);
s8 ov96_022079B8(VecFx32 *);
s32 ov96_02208B34(PokeathlonCourseData *);

void ov96_022055AC(PokeathlonCourseData *arg0)
{
  VecFx32 sp6C;
  VecFx32 sp60;
  VecFx32 sp54;
  VecFx32 sp48;
  VecFx32 sp3C;
  f32 sp38;
  s32 sp34;
  s32 sp30;
  VecFx32 *sp2C;
  u8 *sp28;
  s32 sp24;
  f32 sp20;
  f32 temp_r0_6;
  f32 sp1C;
  u8 *sp18;
  VecFx32 *sp14;
  VecFx32 *sp10;
  VecFx32 *spC;
  VecFx32 *sp8;
  PokeathlonCourseData *sp0;
  VecFx32 *temp_r0_4;
  VecFx32 *var_r4_2;
  VecFx32 *var_r5;
  VecFx32 *var_r6;
  VecFx32 *sp4;
  f32 temp_r0_7;
  s32 temp_r0_8;
  s32 temp_r1_2;
  s32 temp_r1_4;
  s32 temp_r1_5;
  f32 temp_r7;
  u16 temp_r0_3;
  u8 *temp_r0;
  u8 *temp_r0_5;
  u8 *temp_r0_9;
  u8 *var_r3;
  u8 *var_r4;
  u8 temp_r1_3;
  void *temp_r2;
  void *temp_r2_2;
  void *temp_r2_3;
  void *temp_r2_4;
  void *temp_r2_5;
  void *temp_r2_6;
  void *temp_r2_7;
  void *temp_r2_8;
  void *temp_r3;
  void *temp_r3_2;
  void *temp_r3_3;
  void *temp_r3_4;
  void *temp_r3_5;
  void *temp_r3_6;
  VecFx32 *temp_r0_10;
  sp0 = arg0;
  sp28 = PokeathlonCourse_GetDataCopyArea(arg0);
  sp2C = PokeathlonCourse_GetHeapAllocPtr4(arg0);
  if (ov96_021E5F24(arg0) != 0)
  {
    return;
  }
  if ((*((u8 *) (((u8 *) sp2C) + 0x50E))) != 0)
  {
    temp_r0 = sp28 + 0x28;
    sp28 = temp_r0;
    ov96_02205AFC(sp2C, ov96_021E8A20(temp_r0));
    return;
  }
  var_r4 = ov96_021E8A20(sp28 + 0x50);
  var_r3 = ov96_021E8A20(sp28);
  *((VecFx32 (*)[3]) var_r4) = *((VecFx32 (*)[3]) var_r3);
  temp_r0_3 = *((u16 *) (((u8 *) sp2C) + 0x50C));
  if (temp_r0_3 != 0)
  {
    ;
    *((u16 *) (((u8 *) sp2C) + 0x50C)) = (u16) (temp_r0_3 - 1);
  }
  var_r5 = (var_r6 = sp2C);
  ov96_02208B34(*((void **) (((u8 *) sp2C) + 0x36C)));
  sp30 = 0;
  *((s8 *) (((u8 *) sp2C) + 0x51B)) = 0;
  sp18 = sp28;
  sp18 = sp28 + 0x50;
  sp14 = var_r5;
  sp14 = ((void *) var_r5) + 0x24;
  sp10 = var_r5;
  sp10 = ((void *) var_r5) + 0xA0;
  spC = var_r5;
  spC = ((void *) var_r5) + 0xAC;
  sp8 = var_r5;
  temp_r0_4 = ((void *) var_r5) + 0x88;
  sp8 = temp_r0_4;
  sp4 = temp_r0_4;
  var_r4_2 = var_r5;
  loop_9:
  temp_r0_5 = ov96_021E8A20(sp18);

  if ((*((s32 *) temp_r0_5)) != 0)
  {
    temp_r1_2 = *((s32 *) (((u8 *) var_r5) + 0x304));
    if ((temp_r1_2 != 0) && ((*((s32 *) (((u8 *) var_r5) + 0x308))) != 0))
    {
      *((s32 *) (((u8 *) var_r5) + 0x304)) = 0;
    }
    else
      if ((temp_r1_2 == 0) && ((*((s32 *) (((u8 *) var_r5) + 0x308))) == 0))
    {
      *((s32 *) (((u8 *) var_r5) + 0x304)) = 1;
      *((s32 *) (((u8 *) var_r5) + 0x308)) = 1;
    }
  }
  else
  {
    *((s32 *) (((u8 *) var_r5) + 0x304)) = 0;
    *((s32 *) (((u8 *) var_r5) + 0x308)) = 0;
  }
  if ((*((u8 *) (((u8 *) var_r4_2) + 0xC0))) != 0)
  {
    temp_r1_3 = *((u8 *) (((u8 *) var_r4_2) + 0xC1));
    if ((temp_r1_3 == 1) && ((*((u8 *) (((u8 *) temp_r0_5) + 8))) == 1))
    {
      *((u8 *) (((u8 *) var_r4_2) + 0xD5)) = (u8) (((s32) ((*((u8 *) (((u8 *) var_r4_2) + 0xD5))) + 1)) % 3);
      *((u8 *) (((u8 *) var_r4_2) + 0xC1)) = 2U;
      *((s8 *) (((u8 *) var_r4_2) + 0xD4)) = 1;
    }
    else
      if ((temp_r1_3 == 2) && ((*((u8 *) (((u8 *) temp_r0_5) + 8))) == 2))
    {
      *((u8 *) (((u8 *) var_r4_2) + 0xC1)) = 0U;
      *((u8 *) (((u8 *) var_r4_2) + 0xC0)) = 0U;
      *((u8 *) (((u8 *) var_r4_2) + 0xCF)) = 0U;
      *((s8 *) (((u8 *) var_r4_2) + 0xD4)) = 1;
    }
    *((s32 *) (((u8 *) var_r6) + 0x334)) = 0;
    *((u16 *) (((u8 *) var_r4_2) + 0x9E)) = 0U;
    *((s32 *) (((u8 *) var_r4_2) + 0xA0)) = 0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA4)) = 0;
    temp_r3 = ((void *) var_r4_2) + 0xA0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA8)) = 0;
    temp_r2 = ((void *) var_r4_2) + 0xAC;
    *((VecFx32 *) (((u8 *) var_r4_2) + 0xAC)) = *((VecFx32 *) (((u8 *) var_r4_2) + 0xA0));
  }
  else
    if ((*((u8 *) (((u8 *) var_r4_2) + 0xCA))) == 1)
  {
    *((s32 *) (((u8 *) var_r6) + 0x334)) = 0;
    *((u16 *) (((u8 *) var_r4_2) + 0x9E)) = 0U;
    *((s32 *) (((u8 *) var_r4_2) + 0xA0)) = 0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA4)) = 0;
    temp_r3_2 = ((void *) var_r4_2) + 0xA0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA8)) = 0;
    temp_r2_2 = ((void *) var_r4_2) + 0xAC;
    *((VecFx32 *) (((u8 *) var_r4_2) + 0xAC)) = *((VecFx32 *) (((u8 *) var_r4_2) + 0xA0));
  }
  else
    if ((*((u8 *) (((u8 *) var_r4_2) + 0xC8))) != 0)
  {
    *((s32 *) (((u8 *) var_r6) + 0x334)) = 0;
    *((u16 *) (((u8 *) var_r4_2) + 0x9E)) = 0U;
    *((s32 *) (((u8 *) var_r4_2) + 0xA0)) = 0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA4)) = 0;
    temp_r3_3 = ((void *) var_r4_2) + 0xA0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA8)) = 0;
    temp_r2_3 = ((void *) var_r4_2) + 0xAC;
    *((VecFx32 *) (((u8 *) var_r4_2) + 0xAC)) = *((VecFx32 *) (((u8 *) var_r4_2) + 0xA0));
  }
  else
    if ((*((u8 *) (((u8 *) var_r4_2) + 0xCF))) != 0)
  {
    *((s32 *) (((u8 *) var_r6) + 0x334)) = 0;
    *((u16 *) (((u8 *) var_r4_2) + 0x9E)) = 0U;
    *((s32 *) (((u8 *) var_r4_2) + 0xA0)) = 0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA4)) = 0;
    temp_r3_4 = ((void *) var_r4_2) + 0xA0;
    *((s32 *) (((u8 *) var_r4_2) + 0xA8)) = 0;
    temp_r2_4 = ((void *) var_r4_2) + 0xAC;
    *((VecFx32 *) (((u8 *) var_r4_2) + 0xAC)) = *((VecFx32 *) (((u8 *) var_r4_2) + 0xA0));
  }
  else
  {
    sp24 = 0;
    if ((*((s32 *) (((u8 *) var_r5) + 0x304))) != 0)
    {
      if (ov96_02205DD4(sp2C, (u8) sp30, *((u8 *) (((u8 *) temp_r0_5) + 4)), *((u8 *) (((u8 *) temp_r0_5) + 5))) != 0)
      {
        *((s32 *) (((u8 *) var_r6) + 0x334)) = 1;
        *((s32 *) (((u8 *) var_r4_2) + 0xA0)) = (s32) ((*((u8 *) (((u8 *) temp_r0_5) + 4))) << 0xC);
        temp_r3_5 = ((void *) var_r4_2) + 0xA0;
        temp_r2_5 = ((void *) var_r4_2) + 0xAC;
        *((s32 *) (((u8 *) var_r4_2) + 0xA4)) = (s32) ((*((u8 *) (((u8 *) temp_r0_5) + 5))) << 0xC);
        *((s32 *) (((u8 *) var_r4_2) + 0xA8)) = 0;
        *((VecFx32 *) (((u8 *) var_r4_2) + 0xAC)) = *((VecFx32 *) (((u8 *) var_r4_2) + 0xA0));
      }
      goto block_47;
    }
    if ((*((s32 *) (((u8 *) var_r5) + 0x308))) != 0)
    {
      if (((u32) (*((u16 *) (((u8 *) var_r4_2) + 0x9E)))) < 0xFFU)
      {
        *((u16 *) (((u8 *) var_r4_2) + 0x9E)) += 1;
      }
      else
      {
        *((s8 *) (((u8 *) sp2C) + 0x510)) = 1;
      }
      if ((*((s32 *) (((u8 *) var_r6) + 0x334))) == 0)
      {
      }
      else
      {
        *((s32 *) (((u8 *) var_r4_2) + 0xAC)) = (s32) ((*((u8 *) (((u8 *) temp_r0_5) + 4))) << 0xC);
        *((s32 *) (((u8 *) var_r4_2) + 0xB0)) = (s32) ((*((u8 *) (((u8 *) temp_r0_5) + 5))) << 0xC);
        goto block_47;
      }
    }
    else
    {
      if ((*((s32 *) (((u8 *) var_r6) + 0x334))) != 0)
      {
        if (((u32) (*((u16 *) (((u8 *) var_r4_2) + 0x9E)))) <= 0x1EU)
        {
          *((u8 *) (((u8 *) var_r4_2) + 0xC9)) = 1U;
        }
        *((s32 *) (((u8 *) var_r6) + 0x334)) = 0;
        *((s8 *) (((u8 *) sp2C) + 0x510)) = 0;
      }
      sp24 = 1;
      block_47:
      if ((*((u8 *) (((u8 *) var_r4_2) + 0xC9))) == 0)
      {
      }
      else
      {
        temp_r0_10 = (VecFx32 *) (&sp54);
        temp_r0_10->x = 0;
        temp_r0_10->y = 0;
        temp_r0_10->z = 0;
        temp_r2_6 = ((void *) var_r4_2) + (0x14 * (*((u8 *) (((u8 *) var_r4_2) + 0xD5))));
        temp_r0_6 = *((f32 *) (((u8 *) temp_r2_6) + 0x30));
        sp20 = temp_r0_6;
        sp1C = *((f32 *) (((u8 *) temp_r2_6) + 0x34));
        temp_r7 = ov96_022073F0(sp14, *((u8 *) (((u8 *) var_r4_2) + 0xD5)));
        sp34 = (s32) (4096.0 * (((double) (temp_r0_6 * sp1C)) / 8.0));
        VEC_Subtract(spC, sp10, &sp60);
        VEC_MultAdd(sp34, &sp60, &sp54, &sp60);
        sp20 = (f32) (VEC_Mag(&sp60) / 4096);
        temp_r2_7 = ((void *) var_r4_2) + 0x88;
        sp6C = *((VecFx32 *) (((u8 *) var_r4_2) + 0x88));
        ov96_02207400(&sp6C, &sp60, &sp6C);
        temp_r0_7 = (f32) VEC_Mag(&sp6C);
        sp38 = temp_r0_7;
        if (temp_r0_7 > (4096.0f * temp_r7))
        {
          temp_r0_10 = (VecFx32 *) (&sp3C);
          temp_r0_10->x = 0;
          temp_r0_10->y = 0;
          temp_r0_10->z = 0;
          VEC_Normalize(&sp6C, &sp6C);
          VEC_MultAdd((s32) (4096.0f * temp_r7), &sp6C, &sp3C, sp8);
          VEC_MultAdd((s32) (4096.0f * temp_r7), &sp6C, &sp3C, &sp48);
        }
        else
        {
          ov96_02207400(sp4, &sp60, sp8);
          sp48 = sp60;
        }
        *((s8 *) (((u8 *) var_r4_2) + 0xD4)) = ov96_022079B8(sp4);
        *((s8 *) (((u8 *) sp2C) + 0x51B)) = 1;
        temp_r1_4 = *((s32 *) (((u8 *) var_r4_2) + 0x7C));
        *((s16 *) (((u8 *) sp2C) + 0x522)) = (s16) (((s32) (temp_r1_4 + (((u32) (temp_r1_4 >> 0xB)) >> 0x14))) >> 0xC);
        temp_r1_5 = *((s32 *) (((u8 *) var_r4_2) + 0x80));
        *((s16 *) (((u8 *) sp2C) + 0x520)) = (s16) (((s32) (temp_r1_5 + (((u32) (temp_r1_5 >> 0xB)) >> 0x14))) >> 0xC);
        *((u8 *) (((u8 *) var_r4_2) + 0xC9)) = 0U;
      }

      if (sp24 != 0)
      {
        *((u16 *) (((u8 *) var_r4_2) + 0x9E)) = 0U;
        *((s32 *) (((u8 *) var_r4_2) + 0xA0)) = 0;
        *((s32 *) (((u8 *) var_r4_2) + 0xA4)) = 0;
        temp_r3_6 = ((void *) var_r4_2) + 0xA0;
        *((s32 *) (((u8 *) var_r4_2) + 0xA8)) = 0;
        temp_r2_8 = ((void *) var_r4_2) + 0xAC;
        *((VecFx32 *) (((u8 *) var_r4_2) + 0xAC)) = *((VecFx32 *) (((u8 *) var_r4_2) + 0xA0));
      }
    }
  }
  var_r5 = (void *) (((u8 *) var_r5) + 0xC);
  sp18 += 0x28;
  var_r4_2 = (void *) (((u8 *) var_r4_2) + 0xB8);
  sp14 = (void *) (((u8 *) sp14) + 0xB8);
  var_r6 = (void *) (((u8 *) var_r6) + 4);
  sp10 = (void *) (((u8 *) sp10) + 0xB8);
  spC = (void *) (((u8 *) spC) + 0xB8);
  sp8 = (void *) (((u8 *) sp8) + 0xB8);
  sp4 = (void *) (((u8 *) sp4) + 0xB8);
  temp_r0_8 = sp30 + 1;
  sp30 = temp_r0_8;
  if (temp_r0_8 < 4)
  {
    goto loop_9;
  }
  ov96_02205E30(sp0);
  ov96_02207870(sp2C);
  if ((*((u16 *) (((u8 *) sp2C) + 0x50C))) == 0)
  {
    *((s8 *) (((u8 *) sp2C) + 0x50E)) = 1;
  }
  else
  {
    *((s8 *) (((u8 *) sp2C) + 0x50E)) = 0;
  }
  temp_r0_9 = sp28 + 0x28;
  sp28 = temp_r0_9;
  ov96_02205AFC(sp2C, ov96_021E8A20(temp_r0_9));
}

