#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon.h"
u32 _s32_div_f(u32, u32);
void ov96_021E8228(PokeathlonCourseData * param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5);
void ov96_021EAF78(u32, u32, u32, VecFx32 *, fx32 *, s32 *);
u32 ov96_0221341C(VecFx32 * param_1, s32 param_2, VecFx32 * param_3, s32 param_4);
void ov96_022134D4(u32 param_1, s32 param_2, VecFx32 * param_3);
u32 ov96_02213534(VecFx32 * param_1, VecFx32 * param_2, s32 param_3);
extern const u8 ov96_0221D4B4[];
extern const u8 ov96_0221D514[];
extern const u8 ov96_0221D5AA[];
void ov96_022127F4(PokeathlonCourseData *param_1) {
  s32 iStack_58;
  s32 uVar4;
  s32 iStack_60;
  u8 *pvVar3;
  u8 uVar2;
  u8 cVar1;
  u8 *elem;
  u8 *obj;
  s32 uVar6;
  s32 uVar12;
  s32 rem;
  s32 iStack_40;
  s32 iVar5;
  s32 iVar8;
  fx32 fVar9;

  pvVar3 = (u8 *)PokeathlonCourse_GetHeapAllocPtr4(param_1);
  elem = pvVar3 + 0x62c;
  for (iStack_58 = 0; iStack_58 < 2; iStack_58++) {
    elem = pvVar3 + 0x62c + iStack_58 * 0x4c;
    cVar1 = *(u8 *)(elem + 0x38);
    if (cVar1 == 0) {
      continue;
    }
    if (*(u8 *)(elem + 0x39) != 0 && *(u8 *)(elem + 0x39) != 2) {
      continue;
    }
    if (cVar1 == 1) {
      iStack_60 = 8;
    } else if (cVar1 == 2) {
      iStack_60 = 0xc;
    }
    for (uVar12 = 0; uVar12 < 12; uVar12++) {
      VecFx32 VStack_24 = {0, 0, 0};
      VecFx32 VStack_30;
      rem = uVar12 % 3;
      uVar4 = uVar12 / 3;
      obj = pvVar3 + 0x5c + uVar4 * 0x174 + rem * 0x7c;
      ov96_021EAF78(*(u32 *)obj, *(u32 *)(obj + 0x30), *(u32 *)(obj + 0x34), &VStack_24, &VStack_24.y, &iStack_40);
      iVar5 = ov96_0221341C(&VStack_24, iStack_40 << 12, (VecFx32 *)(elem + 8), iStack_60 << 12);
      if (iVar5 != 0) {
      if (((*(u8 *)(elem + uVar12 + 0x2c) == 0) || (*(s32 *)(elem + 0x48) == 0)) &&
          *(s32 *)(obj + 0x48) != 3 && *(s32 *)(obj + 0x48) != 2) {
        VEC_Subtract((VecFx32 *)(elem + 8), &VStack_24, &VStack_30);
        VEC_Normalize(&VStack_30, &VStack_30);
        if (*(u8 *)(elem + 0x39) == 2) {
          if (*(u8 *)(elem + 0x3b) == uVar12) {
            continue;
          }
          iVar5 = *(u8 *)(obj + 0x71) - *(u8 *)(elem + 0x45);
          if (iVar5 <= 0) {
            *(u8 *)(obj + 0x71) = 0;
            *(s32 *)(obj + 0x78) = 2;
            *(u8 *)(obj + 0x60) = 0x5a;
            *(s32 *)(obj + 0x48) = 3;
            *(u16 *)(obj + 0x5a) = 0x1e;
            *(VecFx32 *)(obj + 0x4c) = VStack_30;
            *(s32 *)(obj + 0x4c) *= -1;
            *(s32 *)(obj + 0x50) *= -1;
            *(u16 *)(obj + 0x58) = 7;
            ov96_021E8228(param_1, uVar4 & 0xff, rem & 0xff, 1, 1);
          } else {
            *(u8 *)(obj + 0x71) = iVar5;
            *(s32 *)(obj + 0x48) = 3;
            *(u16 *)(obj + 0x5a) = 0x1e;
            *(u16 *)(obj + 0x58) = 7;
            *(VecFx32 *)(obj + 0x4c) = VStack_30;
            *(s32 *)(obj + 0x4c) *= -1;
            *(s32 *)(obj + 0x50) *= -1;
          }
        } else {
          fVar9 = VEC_Mag((VecFx32 *)(obj + 0x3c));
          if (fVar9 != 0) {
            uVar6 = (*(u16 *)(obj + 0x6c) << 0x10) / 360;
            iVar5 = ov96_02213534((VecFx32 *)(obj + 0x3c), &VStack_30, uVar6 & 0xffff);
            if (*(s32 *)(obj + 0x78) == 1 && iVar5 != 0) {
              *(u8 *)(elem + 0x39) = 2;
              *(u8 *)(elem + 0x3e) = 2;
              *(u8 *)(elem + 0x41) = *(u8 *)(obj + 0x73);
              *(u8 *)(elem + 0x45) = *(u8 *)(obj + 0x72);
              *(u16 *)(elem + 0x46) = *(u8 *)(obj + 0x75);
              uVar2 = ov96_0221D514[*(u8 *)(elem + 0x41) * 0x1e];
              *(s32 *)(obj + 0x48) = 1;
              *(VecFx32 *)(obj + 0x4c) = VStack_30;
              *(s32 *)(obj + 0x4c) *= -1;
              *(s32 *)(obj + 0x50) *= -1;
              *(u16 *)(obj + 0x58) = 3;
              *(s32 *)(obj + 0x78) = 0;
              *(u8 *)(elem + 0x3c) = 1;
              *(u8 *)(elem + 0x3d) = 0x1e;
              ov96_021E8228(param_1, uVar4 & 0xff, rem & 0xff, 4, 1);
            } else {
              *(u8 *)(elem + 0x3e) = 1;
              *(u8 *)(elem + 0x41) = *(u8 *)(obj + 0x73);
              uVar2 = ov96_0221D5AA[*(u8 *)(elem + 0x41) * 0x1e];
              *(u8 *)(elem + 0x3c) = 1;
              *(u8 *)(elem + 0x3d) = 0x1e;
            }
            *(u8 *)(elem + 0x40) = uVar2;
          }
          ov96_022134D4((u32)pvVar3, (s32)elem, &VStack_30);
          *(u8 *)(elem + 0x3b) = uVar12;
        }
        *(u8 *)(elem + uVar12 + 0x2c) = 1;
      }
      } else {
        *(u8 *)(elem + uVar12 + 0x2c) = 0;
      }
    }
    for (iVar5 = 0; iVar5 < 8; iVar5++) {
      iVar8 = ov96_0221341C((VecFx32 *)((u8 *)ov96_0221D4B4 + (iVar5 % 4) * 0x18 + (iVar5 / 4) * 0xc), 0x8000, (VecFx32 *)(elem + 8), iStack_60 << 12);
      if (iVar8 != 0) {
        VecFx32 VStack_3c;
        VEC_Subtract((VecFx32 *)((u8 *)ov96_0221D4B4 + (iVar5 % 4) * 0x18 + (iVar5 / 4) * 0xc), (VecFx32 *)(elem + 8), &VStack_3c);
        VEC_Normalize(&VStack_3c, &VStack_3c);
        fVar9 = VEC_DotProduct((VecFx32 *)(elem + 0x14), &VStack_3c);
        if (fVar9 > 0) {
          VEC_MultAdd(fVar9 * -2, &VStack_3c, (VecFx32 *)(elem + 0x14), (VecFx32 *)(elem + 0x14));
          VEC_Normalize((VecFx32 *)(elem + 0x14), (VecFx32 *)(elem + 0x20));
        }
        break;
      }
    }
  }
}
