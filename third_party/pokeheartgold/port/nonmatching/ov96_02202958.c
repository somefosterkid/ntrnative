#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "unk_02005D10.h"
#include "pokeathlon/pokeathlon.h"
#include "unk_02005D10.h"
void ov96_021E6454(PokeathlonCourseData *, u32);
void ov96_021EAC5C(u32, u32);
s32 ov96_021EAD78(u32);
void ov96_021EB52C(u32, u32, u32);
void ov96_021EB564(u32, u32);
void ov96_021EB588(u32, VecFx32 *);
VecFx32 *ov96_021EB594(u32);
void ov96_021EB5A0(u32, VecFx32 *, u32);
void ov96_022038A0(void *, u32);
void ov96_02203970(void *, u32);
void ov96_02203BC0(u32, u32, u32);
void ov96_02203BD0(u32, u16);
void ov96_02203CA4(u32, u32, u32);
s32 ov96_02203CC4(u32, u32);
void ov96_02203CD4(u32);
void ov96_02204134(f32, f32, s32 *, s32 *);
extern VecFx32 ov96_0221C7C4;
extern VecFx32 ov96_0221C98C[];

void ov96_02202958(PokeathlonCourseData *param_1) {
  BOOL bVar1;
  void *pvVar2;
  u8 *puVar3;
  u32 uVar4;
  u8 uVar5;
  s32 iVar6;
  u32 uVar12;
  u8 iVar13;
  u8 rot;
  u16 bits;
  u8 flag;
  u8 state;
  s32 lo;
  s32 hi;
  u32 remainder_00;
  u32 remainder_01;
  u32 remainder_02;
  s32 remainder_03;
  s32 iVar15;
  u8 *puVar14;
  void *pvVar16;
  void *pvVar17;
  s32 iVar18;
  u32 uVar19;
  s32 iStack_14c;
  u32 uStack_12c;
  u32 uStack_128;
  u8 *puStack_124;
  u32 uStack_120;
  s32 iStack_11c;
  VecFx32 pos;
  MtxFx43 MStack_50;
  VecFx32 VStack_5c;
  VecFx32 VStack_68;
  VecFx32 pos2;
  MtxFx43 MStack_a4;
  VecFx32 VStack_b0;
  VecFx32 VStack_bc;
  VecFx32 scl;
  VecFx32 sclBase;
  s32 iStack_d8;
  s32 iStack_dc;
  s32 iStack_e0;
  s32 iStack_e4;
  f32 fA;

  pvVar2 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
  puVar3 = PokeathlonCourse_GetDataCopyArea(param_1);
  puVar3 = ov96_021E8A20(puVar3 + 0xf0);
  uVar4 = ov96_021E5F24(param_1);
  uVar4 = uVar4 & 0xff;
  uVar5 = *(s32 *)(puVar3 + 0x20) >> 0x19 & 0x7f;
  if (uVar5 == 0) {
    uVar12 = 0;
  }
  else {
    uVar12 = uVar5 - 1 & 0xff;
  }
  ov96_022038A0(pvVar2,uVar12);
  ov96_02203BD0(*(u32 *)((s32)pvVar2 + 0x5e0),*(u16 *)(puVar3 + uVar4 * 2) & 0x3ff);
  rot = 4 - uVar4;
  iVar15 = 0;
  puVar14 = puVar3;
  do {
    if ((u8)((*(u16 *)(puVar14 + 8) >> 13) & 1) != 0 && uVar4 == iVar15 / 3) {
      uVar12 = (u8)(iVar15 % 3);
      ov96_021EB52C(*(u32 *)((u8 *)pvVar2 + uVar12 * 4 + 100),1,1);
      ov96_021EB564(*(u32 *)((u8 *)pvVar2 + uVar12 * 4 + 100),6);
      iVar18 = ((*(u16 *)(puVar14 + 8) >> 6) & 0x3f) + 1;
      iVar6 = (*(u16 *)(puVar14 + 8) & 0x3f) + 1;
      if (uVar4 != 0) {
        VStack_5c.x = iVar6 << 12;
        VStack_5c.z = iVar18 << 12;
        VStack_5c.y = 0;
        VStack_5c.x -= 0x21000;
        VStack_5c.z -= 0x21000;
        MTX_RotY43_(&MStack_50,FX_SinIdx(rot << 14),FX_CosIdx(rot << 14));
        MTX_MultVec43(&VStack_5c,&MStack_50,&VStack_68);
        VStack_68.x += 0x21000;
        VStack_68.z += 0x21000;
        iVar6 = VStack_68.x / 0x1000;
        iVar18 = VStack_68.z / 0x1000;
      }
      ov96_02204134((f32)iVar6,(f32)iVar18,&iStack_d8,&iStack_dc);
      iVar6 = iStack_dc;
      pos.x = iStack_d8 << 12;
      pos.y = iVar6 << 12;
      pos.z = 0;
      pos.y = ((iVar6 - 8) - iVar6 / 20) << 12;
      ov96_021EB588(*(u32 *)((u8 *)pvVar2 + uVar12 * 4 + 100),&pos);
      PlaySE(0x88f);
    }
    iVar15 = iVar15 + 1;
    puVar14 = puVar14 + 2;
  } while (iVar15 < 0xc);
  uVar12 = 0;
  uStack_12c = uVar4 * 3;
  do {
    uVar19 = uStack_12c & 0xff;
    iVar13 = (u8)((*(u16 *)(puVar3 + uVar19 * 2 + 8) >> 14) & 3);
    if (iVar13 == 1) {
      pvVar16 = (void *)((s32)pvVar2 + uVar19 * 0x20);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0x418),1,1);
      ov96_02203BC0(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,0);
    }
    else if (iVar13 == 2) {
      pvVar16 = (void *)((s32)pvVar2 + uVar19 * 0x20);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0x418),1,0);
      ov96_02203BC0(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,1);
    }
    else {
      pvVar16 = (void *)((s32)pvVar2 + uVar19 * 0x20);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0x418),1,0);
      ov96_02203BC0(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,0);
    }
    flag = (u8)((*(u16 *)(puVar3 + uVar19 * 2 + 8) >> 12) & 1);
    state = (u8)((*(s32 *)(puVar3 + 0x20) >> (uVar19 * 2)) & 3);
    if (iVar13 == 1) {
      ov96_02203CA4(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,10);
      *(u16 *)((s32)pvVar16 + 0x434) = 10;
      if (IsSEPlaying(0x890) == 0) {
        PlaySE(0x890);
      }
      if (flag) {
        PlaySE(0x8b0);
      }
    }
    else if (flag) {
      ov96_02203CA4(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,7);
      *(u16 *)((s32)pvVar16 + 0x434) = 7;
      PlaySE(0x8b0);
      ov96_02203CD4(*(u32 *)((s32)pvVar2 + 0x5e0));
    }
    else if (state == 1 && *(u16 *)((s32)pvVar16 + 0x436) == 0) {
      if (iVar13 == 1) {
        GF_AssertFail();
      }
      ov96_02203CA4(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,4);
      *(u16 *)((s32)pvVar2 + uVar19 * 0x20 + 0x434) = 4;
      *(u16 *)((s32)pvVar2 + uVar19 * 0x20 + 0x436) = 1;
      if (*(u8 *)((s32)pvVar2 + uVar12 + 0x5ef) == 0) {
        PlaySE(0x8ac);
      }
    }
    else {
      bVar1 = FALSE;
      if (*(u16 *)((s32)pvVar16 + 0x434) == 10) {
        bVar1 = TRUE;
      }
      else if (ov96_02203CC4(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff) == 0) {
        bVar1 = TRUE;
      }
      if (bVar1) {
        ov96_02203CA4(*(u32 *)((s32)pvVar2 + 0x5e0),uVar12 & 0xff,1);
        *(u16 *)((s32)pvVar16 + 0x434) = 1;
      }
    }
    if (state != 1) {
      *(u16 *)((s32)pvVar16 + 0x436) = 0;
    }
    uVar12 = uVar12 + 1;
    uStack_12c = uStack_12c + 1;
  } while ((s32)uVar12 < 3);
  iStack_14c = 0;
  remainder_00 = (s32)((uVar4 + 1) * 3) % (s32)(0xc);
  uStack_128 = remainder_00 & 0xff;
  pvVar16 = pvVar2;
  do {
    remainder_01 = (s32)(uStack_128) % (s32)(0xc);
    uVar12 = remainder_01 & 0xff;
    iVar13 = (u8)((*(u16 *)(puVar3 + uVar12 * 2 + 8) >> 14) & 3);
    if (iVar13 == 1) {
      pvVar17 = (void *)((s32)pvVar2 + uVar12 * 0x20);
      ov96_021EB52C(*(u32 *)((s32)pvVar17 + 0x418),1,1);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0x70),1,0);
    }
    else if (iVar13 == 2) {
      pvVar17 = (void *)((s32)pvVar2 + uVar12 * 0x20);
      ov96_021EB52C(*(u32 *)((s32)pvVar17 + 0x418),1,0);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0x70),1,1);
    }
    else {
      pvVar17 = (void *)((s32)pvVar2 + uVar12 * 0x20);
      ov96_021EB52C(*(u32 *)((s32)pvVar17 + 0x418),1,0);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0x70),1,0);
    }
    bits = *(u16 *)(puVar3 + uVar12 * 2 + 8);
    flag = (u8)((bits >> 12) & 1);
    state = (u8)((*(s32 *)(puVar3 + 0x20) >> (uVar12 * 2)) & 3);
    lo = bits & 0x3f;
    hi = (bits >> 6) & 0x3f;
    if (iVar13 == 1) {
      ov96_021EAC5C(*(u32 *)((s32)pvVar16 + 0x94),0x14);
      *(u16 *)((s32)pvVar17 + 0x434) = 0x14;
      if (IsSEPlaying(0x890) == 0) {
        PlaySE(0x890);
      }
    }
    else if (flag) {
      ov96_021EAC5C(*(u32 *)((s32)pvVar16 + 0x94),0x16);
      *(u16 *)((s32)pvVar17 + 0x434) = 0x16;
    }
    else if (state == 1 && lo + 1 == ov96_0221C98C[uVar12].x / 0x1000 && hi + 1 == ov96_0221C98C[uVar12].z / 0x1000) {
      if (iVar13 == 1) {
        GF_AssertFail();
      }
      ov96_021EAC5C(*(u32 *)((s32)pvVar16 + 0x94),0x10);
      *(u16 *)((s32)pvVar17 + 0x434) = 0x10;
    }
    else {
      bVar1 = FALSE;
      if (*(u16 *)((s32)pvVar17 + 0x434) == 0x14) {
        bVar1 = TRUE;
      }
      else if (ov96_021EAD78(*(u32 *)((s32)pvVar16 + 0x94)) == 0) {
        bVar1 = TRUE;
      }
      if (bVar1) {
        ov96_021EAC5C(*(u32 *)((s32)pvVar16 + 0x94),0);
        *(u16 *)((s32)pvVar17 + 0x434) = 0;
      }
    }
    pvVar16 = (void *)((s32)pvVar16 + 4);
    uStack_128 = uStack_128 + 1;
    iStack_14c = iStack_14c + 1;
  } while (iStack_14c < 9);
  iVar15 = 0;
  uStack_120 = 0;
  pvVar16 = pvVar2;
  puStack_124 = puVar3;
  sclBase = ov96_0221C7C4;
  do {
    state = (u8)((*(s32 *)(puVar3 + 0x20) >> uStack_120) & 3);
    if ((u8)(state - 1) <= 1) {
      *(u8 *)((s32)pvVar16 + 0xfa) = 0;
      if (state != 2) {
        ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0xbc),1,1);
      }
      else {
        ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0xbc),1,0);
      }
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0xb8),1,1);
      uVar12 = iVar15 / 3;
      remainder_02 = iVar15 % 3;
      uVar19 = (u8)(((u8)((*(u16 *)(puVar3 + uVar12 * 2) >> 10) & 0x3f) >> (remainder_02 * 2)) & 3);
      iVar6 = (*(u16 *)(puStack_124 + 8) & 0x3f) + 1;
      iStack_11c = ((s32)(u32)*(u16 *)(puStack_124 + 8) >> 6 & 0x3fU) + 1;
      if (uVar4 != 0) {
        VStack_b0.x = iVar6 << 12;
        VStack_b0.z = iStack_11c << 12;
        VStack_b0.y = 0;
        VStack_b0.x -= 0x21000;
        VStack_b0.z -= 0x21000;
        MTX_RotY43_(&MStack_a4,FX_SinIdx(rot << 14),FX_CosIdx(rot << 14));
        MTX_MultVec43(&VStack_b0,&MStack_a4,&VStack_bc);
        VStack_bc.x += 0x21000;
        VStack_bc.z += 0x21000;
        iVar6 = VStack_bc.x / 0x1000;
        iStack_11c = VStack_bc.z / 0x1000;
      }
      ov96_02204134((f32)iVar6,(f32)iStack_11c,&iStack_e0,&iStack_e4);
      pos2.x = iStack_e0 << 12;
      pos2.y = iStack_e4 << 12;
      pos2.z = 0;
      ov96_021EB588(*(u32 *)((s32)pvVar16 + 0xbc),&pos2);
      iVar6 = iStack_e4;
      pos2.y = ((iVar6 - 8) - iVar6 / 20) << 12;
      ov96_021EB588(*(u32 *)((s32)pvVar16 + 0xb8),&pos2);
      scl = sclBase;
      fA = (((f32)iStack_e4 + 312.0) / 752.0) * (1.0 + (f32)uVar19 / 3.0);
      scl.x = fA * 4096.0f;
      scl.y = fA * 4096.0f;
      ov96_021EB5A0(*(u32 *)((s32)pvVar16 + 0xbc),&scl,2);
      ov96_021EB5A0(*(u32 *)((s32)pvVar16 + 0xb8),&scl,2);
      ov96_021EB5A0(*(u32 *)((s32)pvVar16 + 0xc0),&scl,2);
      if ((uVar12 == uVar4) && (uVar19 != *(u8 *)((s32)pvVar2 + remainder_02 + 0x5ec))) {
        PlaySE(0x8b1);
        *(s8 *)((s32)pvVar2 + remainder_02 + 0x5ec) = (s8)uVar19;
      }
    }
    else {
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0xbc),1,0);
      ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0xb8),1,0);
      if (state == 3 && *(u8 *)((s32)pvVar16 + 0xfa) == 0) {
        *(u8 *)((s32)pvVar16 + 0xfa) = 1;
        ov96_021EB588(*(u32 *)((s32)pvVar16 + 0xc0),ov96_021EB594(*(u32 *)((s32)pvVar16 + 0xb8)));
        ov96_021EB564(*(u32 *)((s32)pvVar16 + 0xc0),3);
        ov96_021EB52C(*(u32 *)((s32)pvVar16 + 0xc0),1,1);
        uVar12 = (iVar15) / 3;
        if (uVar12 == uVar4) {
          PlaySE(0x8b3);
          remainder_03 = (s32)(iVar15) % (s32)(3);
          *(u8 *)((s32)pvVar2 + remainder_03 + 0x5ef) = 0;
          *(u8 *)((s32)pvVar2 + remainder_03 + 0x5ec) = 0;
        }
      }
    }
    iVar15 = iVar15 + 1;
    uStack_120 = uStack_120 + 2;
    pvVar16 = (void *)((s32)pvVar16 + 0x48);
    puStack_124 = puStack_124 + 2;
  } while (iVar15 < 0xc);
  ov96_02203970(pvVar2,uVar4);
  ov96_021E6454(param_1,uVar5 * 0x1e);
  return;
}
