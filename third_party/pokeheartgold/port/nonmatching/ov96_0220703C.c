#include "unk_02020E80.h"
#include "unk_02020E80.h"
fx32 ov96_02207390(VecFx32 * param_1, VecFx32 * param_2, VecFx32 * param_3, s32 param_4);
typedef struct {
    s32 a[4];
} ov96_0221CB00_Array;
extern const ov96_0221CB00_Array ov96_0221CB00;
typedef struct {
    s32 a[4];
} ov96_0221CAE0_Array;
extern const ov96_0221CAE0_Array ov96_0221CAE0;
s32 ov96_0220703C(VecFx32 *param_1, VecFx32 *param_2, s32 param_3, s32 *param_4, fx32 *param_5) {
  BOOL bVar1;
  BOOL BVar2;
  fx32 fVar3;
  s32 iVar4;
  s32 iVar5;
  s32 iVar6;
  s32 *piVar7;
  s32 iVar8;
  Vec2D *arg1;
  s32 iVar9;
  Vec2D *arg0;
  Vec2D VStack_8c;
  Vec2D VStack_94;
  Vec2D VStack_9c;
  BOOL xflag;
  BOOL yflag;
  s32 x0, x1, y0, y1;

  VStack_94.x = (s32)(param_1->x + ((u32)(param_1->x >> 0xb) >> 0x14)) >> 0xc;
  VStack_94.y = (s32)(param_1->y + ((u32)(param_1->y >> 0xb) >> 0x14)) >> 0xc;
  VStack_9c.x = (s32)(param_2->x + ((u32)(param_2->x >> 0xb) >> 0x14)) >> 0xc;
  VStack_9c.y = (s32)(param_2->y + ((u32)(param_2->y >> 0xb) >> 0x14)) >> 0xc;
  arg1 = (Vec2D *)(param_4 + 10);
  arg0 = (Vec2D *)(param_4 + 8);
  piVar7 = param_4;
  iVar6 = 0;
  for (; iVar6 < 4;) {
    ov96_0221CB00_Array aiStack_24 = ov96_0221CB00;
    BVar2 = sub_02020F4C(arg0,arg1,&VStack_94,&VStack_9c,&VStack_8c);
    if (BVar2 != 0) {
      *param_5 = VStack_8c.x << 0xc;
      param_5[1] = VStack_8c.y << 0xc;
      param_5[2] = 0;
      return aiStack_24.a[iVar6];
    }
    bVar1 = FALSE;
    if (iVar6 == 0 && iVar6 == 2) {
      if (VStack_94.y == piVar7[9]) {
        iVar9 = piVar7[10];
        iVar8 = piVar7[8];
        if (iVar8 < iVar9) {
          iVar4 = iVar8;
        } else {
          iVar4 = iVar9;
          iVar9 = iVar8;
        }
        if ((iVar4 <= VStack_94.x) && (VStack_94.x <= iVar9)) {
          bVar1 = TRUE;
        }
      }
    } else if (VStack_94.x == piVar7[8]) {
      iVar5 = piVar7[0xb];
      iVar8 = piVar7[9];
      if (iVar8 < iVar5) {
        iVar9 = iVar8;
        iVar4 = iVar5;
      } else {
        iVar5 = iVar8;
        iVar4 = iVar5;
      }
      if ((iVar4 <= VStack_94.y) && (VStack_94.y <= iVar5)) {
        bVar1 = TRUE;
      }
    }
    if (bVar1) {
      *(VecFx32 *)param_5 = *param_1;
      return aiStack_24.a[iVar6];
    }
    iVar6 = iVar6 + 1;
    arg1 = (Vec2D *)((s32 *)arg1 + 4);
    arg0 = (Vec2D *)((s32 *)arg0 + 4);
    piVar7 = piVar7 + 4;
  }
      iVar6 = 0;
      piVar7 = param_4;
      for (; iVar6 < 4;) {
        ov96_0221CAE0_Array aiStack_34 = ov96_0221CAE0;
        VecFx32 VStack_40;
        VecFx32 VStack_4c;
        VecFx32 VStack_58;
        VecFx32 VStack_64;
        VStack_40.x = *piVar7 << 0xc;
        VStack_40.y = piVar7[1] << 0xc;
        VStack_40.z = 0;
        VEC_Subtract(param_2,&VStack_40,&VStack_4c);
        fVar3 = VEC_Mag(&VStack_4c);
        if (fVar3 <= param_3 * 0x1000) {
          fVar3 = ov96_02207390(param_1,param_2,&VStack_40,param_3 << 0xc);
          VEC_Subtract(param_2,param_1,&VStack_58);
          VEC_MultAdd(fVar3,&VStack_58,param_1,&VStack_64);
          *param_5 = VStack_64.x;
          param_5[1] = VStack_64.y;
          param_5[2] = 0;
          return aiStack_34.a[iVar6];
        }
        iVar6 = iVar6 + 1;
        piVar7 = piVar7 + 2;
      }
      iVar6 = param_2->x;
      if (!(param_4[0] * 0x1000 > iVar6 || iVar6 > param_4[4] * 0x1000 ||
          param_4[1] * 0x1000 > param_2->y || param_2->y > param_4[5] * 0x1000)) {

      *(VecFx32 *)param_5 = *param_2;
      yflag = FALSE;
      x0 = param_4[0];
      iVar8 = (x0 << 12) - param_2->x;
      x1 = param_4[4];
      iVar4 = (x1 << 12) - param_2->x;
        xflag = FALSE;
      if (iVar8 < 0) {
        iVar8 *= -1;
      }
      if (iVar4 < 0) {
        iVar4 *= -1;
      }
      if (iVar8 < iVar4) {
        iVar9 = 0xc;
      } else {
        iVar8 = iVar4;
        iVar9 = 10;
      }
      y0 = param_4[1];
      iVar4 = (y0 << 12) - param_2->y;
      y1 = param_4[5];
      iVar5 = (y1 << 12) - param_2->y;
      if (iVar4 < 0) {
        iVar4 *= -1;
      }
      if (iVar5 < 0) {
        iVar5 *= -1;
      }
      if (iVar4 < iVar5) {
        iVar6 = 9;
      } else {
        iVar4 = iVar5;
        iVar6 = 0xb;
      }
      if (iVar8 <= iVar4) {
        xflag = TRUE;
        iVar5 = iVar9;
      } else {
        yflag = TRUE;
        iVar5 = iVar6;
      }
      if (xflag) {
        if (iVar9 == 0xc) {
          param_5[0] = (x0 - param_3) << 12;
        } else {
          param_5[0] = (x1 + param_3) << 12;
        }
      } else if (yflag) {
        if (iVar6 == 9) {
          param_5[1] = (y0 - param_3) << 12;
        } else {
          param_5[1] = (y1 + param_3) << 12;
        }
      }
      return iVar5;
      }
  return 0;
}
