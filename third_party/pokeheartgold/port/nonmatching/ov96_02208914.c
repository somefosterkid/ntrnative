#include "sprite.h"
s32 ov96_021EB408(s32, s32, s32, u8, s32);
s32 ov96_021EB4F4(s32, u8, u8);
s32 ov96_021EB52C(s32, s32, s32);
Sprite *ov96_021EB5B8(s32);
s32 ov96_022089D0(s32, void *);
typedef struct Words2 { u32 w[2]; } Words2;
extern Words2 ov96_0221CBCC[];

void ov96_02208914(s32 arg0, void *arg1)
{
  Words2 sp14 = {0x66, 0x67};
  Words2 spC = {4, 5};
  VecFx32 sp1C;
  void *sp8;
  s32 sp4;
  Sprite *temp_r0;
  s32 var_r6;
  u32 *var_r4;
  u32 *var_r5;
  u32 *var_r7;
  u32 *var_r6_2;
  int new_var2;
  u8 *new_var3;
  void *var_r4_2;
  sp4 = arg0;
  sp8 = arg1;
  var_r5 = sp14.w;
  var_r4 = spC.w;
  var_r6 = 0;
  do
  {
    ov96_021EB408(sp4, 3, 2, (u8) (*var_r5), (s32) ((u8) (*var_r4)));
    var_r6 = var_r6 + 1;
    var_r4 += 1;
    var_r5 += 1;
  }
  while (2 > var_r6);
  var_r4_2 = sp8;
  var_r6 = 0;
  var_r7 = spC.w;
  var_r6_2 = sp14.w;
  do
  {
    new_var3 = ((u8 *) var_r4_2) + 0x130;
    *((s32 *) new_var3) = ov96_021EB4F4(sp4, (u8) (*var_r6_2), (u8) (*var_r7));
    temp_r0 = ov96_021EB5B8(*((s32 *) (((u8 *) var_r4_2) + 0x130)));
    sp1C.z = 0;
    new_var2 = ((0x58 * (1 - var_r6)) + 0x30) << 0xC;
    sp1C.x = 0x28000;
    sp1C.y = new_var2 + 0x2BC000;
    Sprite_SetMatrix(temp_r0, &sp1C);
    ov96_021EB52C(*((s32 *) (((u8 *) var_r4_2) + 0x130)), 1, 1);
    var_r6 += 1;
    var_r7 += 1;
    var_r6_2 += 1;
    var_r4_2 += 4;
  }
  while (var_r6 < 2);
  ov96_022089D0(sp4, sp8);
}
