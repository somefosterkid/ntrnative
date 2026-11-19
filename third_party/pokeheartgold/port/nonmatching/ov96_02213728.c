#include "error_handling.h"
#include "unk_02020E80.h"
#include "unk_02020E80.h"
extern VecFx32 ov96_0221D214;
extern VecFx32 ov96_0221D274;
extern VecFx32 ov96_0221D244;
extern VecFx32 ov96_0221D22C;
BOOL ov96_02213D00(Vec2D * param_1, Vec2D * param_2, Vec2D * param_3, VecFx32 * param_4);
u32 ov96_02213D2C(VecFx32 * param_1, VecFx32 * param_2, VecFx32 * param_3, VecFx32 * param_4, VecFx32 * param_5);
s32 ov96_02213728(VecFx32 *param_1, VecFx32 *param_2, s32 param_3, s32 param_4, VecFx32 *param_5) {
  s32 iVar7;
  u32 uVar6;
  BOOL BVar3;
  BOOL bVar1;
  BOOL bVar2;
  s32 t;
  Vec2D VStack_98;
  Vec2D VStack_a0;
  Vec2D VStack_a8;
  Vec2D VStack_b0;
  Vec2D VStack_b8;
  VecFx32 VStack_24 = {0, 0, 0};
  VecFx32 VStack_30 = {0, 0, 0};
  VecFx32 VStack_3c;
  VecFx32 VStack_48;
  VecFx32 VStack_54 = {0, 0, 0};
  VecFx32 VStack_60 = {0, 0, 0};
  VecFx32 VStack_6c = ov96_0221D214;
  VecFx32 VStack_78 = ov96_0221D274;
  VecFx32 VStack_84 = ov96_0221D244;
  VecFx32 VStack_90 = ov96_0221D22C;

  param_5->x = 0;
  param_5->y = 0;
  param_5->z = 0;
  VStack_a8.x = param_2->x / 0x1000;
  VStack_a8.y = param_2->y / 0x1000;
  VStack_b0.x = param_1->x / 0x1000;
  VStack_b0.y = param_1->y / 0x1000;
  if (param_1->y < 0x70000) {
    if (param_1->x >= 0x80000) {
      uVar6 = 1;
    } else {
      uVar6 = 0;
    }
  } else if (param_1->x < 0x80000) {
    uVar6 = 2;
  } else {
    uVar6 = 3;
  }
  iVar7 = 0;
  bVar1 = FALSE;
  bVar2 = FALSE;
  if (param_4 != 0) {
    switch (uVar6) {
    case 0:
      VStack_54.x = -0x20000;
      VStack_54.y = 0x70000;
      VStack_60.x = 0x60000;
      VStack_60.y = -0x10000;
      VEC_Normalize(&VStack_6c, &VStack_48);
      iVar7 = 5;
      break;
    case 1:
      VStack_54.x = 0xa0000;
      VStack_54.y = -0x10000;
      VStack_60.x = 0x120000;
      VStack_60.y = 0x70000;
      VEC_Normalize(&VStack_78, &VStack_48);
      iVar7 = 6;
      break;
    case 2:
      VStack_54.x = 0x60000;
      VStack_54.y = 0xe0000;
      VStack_60.x = -0x20000;
      VStack_60.y = 0x60000;
      VEC_Normalize(&VStack_84, &VStack_48);
      iVar7 = 7;
      break;
    case 3:
      VStack_54.x = 0x120000;
      VStack_54.y = 0x60000;
      VStack_60.x = 0xa0000;
      VStack_60.y = 0xe0000;
      VEC_Normalize(&VStack_90, &VStack_48);
      iVar7 = 8;
      break;
    default:
      GF_AssertFail();
    }
    if (iVar7 != 0) {
      t = param_3 << 0xc;
      VEC_MultAdd(t, &VStack_48, &VStack_54, &VStack_54);
      VEC_MultAdd(t, &VStack_48, &VStack_60, &VStack_60);
      VStack_98.x = VStack_54.x / 0x1000;
      VStack_98.y = VStack_54.y / 0x1000;
      VStack_a0.x = VStack_60.x / 0x1000;
      VStack_a0.y = VStack_60.y / 0x1000;
      if (sub_02020F4C(&VStack_98, &VStack_a0, &VStack_a8, &VStack_b0, &VStack_b8)) {
        BVar3 = sub_02020E80(&VStack_98, &VStack_a0, &VStack_b8);
        if (BVar3 == 0) {
          *param_5 = *param_2;
          bVar1 = TRUE;
        } else {
          param_5->x = VStack_b8.x << 12;
          param_5->y = VStack_b8.y << 12;
        }
        bVar2 = TRUE;
      } else {
        iVar7 = 0;
      }
    }
  }
  if (param_1->y < 0x70000) {
    t = (param_3 + 0x10) * 0x1000;
    if (param_1->y < t && param_2->y == (0x10 - param_3) * 0x1000) {
      *param_5 = *param_2;
      return 3;
    }
    VStack_24.x = 0x8000;
    VStack_30.x = 0xf7000;
    VStack_24.y = t;
    VStack_30.y = t;
    if (ov96_02213D2C(&VStack_24, &VStack_30, param_2, param_1, &VStack_3c)) {
      if (bVar1) {
        return iVar7;
      }
      if (bVar2 && param_5->y >= VStack_3c.y) {
        return iVar7;
      }
      if (param_4 != 0 && ov96_02213D00(&VStack_98, &VStack_a0, &VStack_a8, &VStack_3c)) {
        *param_5 = *param_2;
        return 3;
      }
      *param_5 = VStack_3c;
      return 3;
    }
    if (param_1->x < 0x80000) {
      param_3 += 8;
      t = param_3 * 0x1000;
      if (param_1->x < t && param_2->x == t) {
        *param_5 = *param_2;
        return 1;
      }
      VStack_24.y = 0xb7000;
      VStack_30.y = 0x10000;
      VStack_24.x = t;
      VStack_30.x = t;
      if (ov96_02213D2C(&VStack_24, &VStack_30, param_2, param_1, &VStack_3c)) {
        if (bVar1) {
          return iVar7;
        }
        if (bVar2 && param_5->x >= VStack_3c.x) {
          return iVar7;
        }
        if (param_4 != 0 && ov96_02213D00(&VStack_98, &VStack_a0, &VStack_a8, &VStack_3c)) {
          *param_5 = *param_2;
          return 1;
        }
        *param_5 = VStack_3c;
        return 1;
      }
    } else {
      t = (0xf7 - param_3) * 0x1000;
      if (param_1->x > t && param_2->x == t) {
        *param_5 = *param_2;
        return 2;
      }
      VStack_24.y = 0x10000;
      VStack_30.y = 0xb7000;
      VStack_24.x = t;
      VStack_30.x = t;
      if (ov96_02213D2C(&VStack_24, &VStack_30, param_2, param_1, &VStack_3c)) {
        if (bVar1) {
          return iVar7;
        }
        if (bVar2 && param_5->x <= VStack_3c.x) {
          return iVar7;
        }
        if (param_4 != 0 && ov96_02213D00(&VStack_98, &VStack_a0, &VStack_a8, &VStack_3c)) {
          *param_5 = *param_2;
          return 2;
        }
        *param_5 = VStack_3c;
        return 2;
      }
    }
  } else {
    t = (0xb7 - param_3) * 0x1000;
    if (param_1->y > t && param_2->y == t) {
      *param_5 = *param_2;
      return 4;
    }
    VStack_24.x = 0xf7000;
    VStack_30.x = 0x8000;
    VStack_24.y = t;
    VStack_30.y = t;
    if (ov96_02213D2C(&VStack_24, &VStack_30, param_2, param_1, &VStack_3c)) {
      if (bVar1) {
        return iVar7;
      }
      if (bVar2 && param_5->y <= VStack_3c.y) {
        return iVar7;
      }
      if (param_4 != 0 && ov96_02213D00(&VStack_98, &VStack_a0, &VStack_a8, &VStack_3c)) {
        *param_5 = *param_2;
        return 4;
      }
      *param_5 = VStack_3c;
      return 4;
    }
    if (param_1->x < 0x80000) {
      param_3 += 8;
      t = param_3 * 0x1000;
      if (param_1->x < t && param_2->x == t) {
        *param_5 = *param_2;
        return 1;
      }
      VStack_24.y = 0xb7000;
      VStack_30.y = 0x10000;
      VStack_24.x = t;
      VStack_30.x = t;
      if (ov96_02213D2C(&VStack_24, &VStack_30, param_2, param_1, &VStack_3c)) {
        if (bVar1) {
          return iVar7;
        }
        if (bVar2 && param_5->x >= VStack_3c.x) {
          return iVar7;
        }
        if (param_4 != 0 && ov96_02213D00(&VStack_98, &VStack_a0, &VStack_a8, &VStack_3c)) {
          *param_5 = *param_2;
          return 1;
        }
        *param_5 = VStack_3c;
        return 1;
      }
    } else {
      t = (0xf7 - param_3) * 0x1000;
      if (param_1->x > t && param_2->x == t) {
        *param_5 = *param_2;
        return 2;
      }
      VStack_24.y = 0x10000;
      VStack_30.y = 0xb7000;
      VStack_24.x = t;
      VStack_30.x = t;
      if (ov96_02213D2C(&VStack_24, &VStack_30, param_2, param_1, &VStack_3c)) {
        if (bVar1) {
          return iVar7;
        }
        if (bVar2 && param_5->x <= VStack_3c.x) {
          return iVar7;
        }
        if (param_4 != 0 && ov96_02213D00(&VStack_98, &VStack_a0, &VStack_a8, &VStack_3c)) {
          *param_5 = *param_2;
          return 2;
        }
        *param_5 = VStack_3c;
        return 2;
      }
    }
  }
  return iVar7;
}
