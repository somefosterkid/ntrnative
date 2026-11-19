typedef struct Words2 { u32 w[2]; } Words2;
void ov96_021FF1E0(s32 arg0, s32 *arg1, void *arg2)
{
  s32 temp_r1;
  s32 temp_r1_2;
  s32 temp_r1_3;
  s32 temp_r3;
  s32 temp_r4_3;
  s32 temp_r5_2;
  u16 temp_r5;
  u32 temp_r4_2;
  u32 var_r3;
  u8 temp_r4;
  temp_r3 = *arg1;
  temp_r4 = (u8) temp_r3;
  temp_r1 = ((u8) (temp_r3 >> 8)) + 0x20;
  temp_r5 = (u16) (temp_r3 >> 0x10);
  *((s32 *) arg2) = (s32) temp_r4;
  if (temp_r4 == 3)
  {
    *((s32 *) (((u8 *) arg2) + 4)) = (s32) (temp_r5 - 0x20);
    *((s32 *) (((u8 *) arg2) + 8)) = (s32) (temp_r1 - 0x20);
    return;
  }
  if (temp_r4 == 1)
  {
    var_r3 = 0x30;
  }
  else
  {
    var_r3 = 0x20;
  }
  temp_r4_2 = var_r3 >> 1;
  temp_r5_2 = temp_r5 - temp_r4_2;
  temp_r4_3 = temp_r1 - temp_r4_2;
  *((s32 *) (((u8 *) arg2) + 4)) = temp_r5_2;
  *((s32 *) (((u8 *) arg2) + 8)) = temp_r4_3;
  temp_r1_2 = temp_r5_2 + var_r3;
  *((s32 *) (((u8 *) arg2) + 0xC)) = temp_r1_2;
  *((s32 *) (((u8 *) arg2) + 0x10)) = temp_r4_3;
  *((s32 *) (((u8 *) arg2) + 0x14)) = temp_r1_2;
  temp_r1_3 = temp_r4_3 + var_r3;
  *((s32 *) (((u8 *) arg2) + 0x18)) = temp_r1_3;
  *((s32 *) (((u8 *) arg2) + 0x1C)) = temp_r5_2;
  *((s32 *) (((u8 *) arg2) + 0x20)) = temp_r1_3;
  *((Words2 *) (((u8 *) arg2) + 0x24)) = *((Words2 *) (((u8 *) arg2) + 0x4));
  *((Words2 *) (((u8 *) arg2) + 0x2C)) = *((Words2 *) (((u8 *) arg2) + 0xC));
  *((Words2 *) (((u8 *) arg2) + 0x34)) = *((Words2 *) (((u8 *) arg2) + 0xC));
  *((Words2 *) (((u8 *) arg2) + 0x3C)) = *((Words2 *) (((u8 *) arg2) + 0x14));
  *((Words2 *) (((u8 *) arg2) + 0x44)) = *((Words2 *) (((u8 *) arg2) + 0x14));
  *((Words2 *) (((u8 *) arg2) + 0x4C)) = *((Words2 *) (((u8 *) arg2) + 0x1C));
  *((Words2 *) (((u8 *) arg2) + 0x54)) = *((Words2 *) (((u8 *) arg2) + 0x1C));
  *((Words2 *) (((u8 *) arg2) + 0x5C)) = *((Words2 *) (((u8 *) arg2) + 0x4));
  *((s32 *) (((u8 *) arg2) + 0x28)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x28))) - arg0);
  *((s32 *) (((u8 *) arg2) + 0x30)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x30))) - arg0);
  *((s32 *) (((u8 *) arg2) + 0x34)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x34))) + arg0);
  *((s32 *) (((u8 *) arg2) + 0x3C)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x3C))) + arg0);
  *((s32 *) (((u8 *) arg2) + 0x48)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x48))) + arg0);
  *((s32 *) (((u8 *) arg2) + 0x50)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x50))) + arg0);
  *((s32 *) (((u8 *) arg2) + 0x54)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x54))) - arg0);
  *((s32 *) (((u8 *) arg2) + 0x5C)) = (s32) ((*((s32 *) (((u8 *) arg2) + 0x5C))) - arg0);
}

