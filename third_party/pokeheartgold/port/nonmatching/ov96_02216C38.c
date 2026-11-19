/* PORTOK hk42 */
#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
u32 _d2f(void);

u64 _dfltu(u32);

u64 _dmul(u32, u32, s32, s32);

u64 _dsub(u32, u32, s32, s32);
u64 _f2d();

u32 _fdiv(u32, u32);

u32 _fflt(u32);

u32 _fsub(u32, u32);

void _s32_div_f(u8, u32);

s32 ov96_021EAA04(s32 param_1, s32 param_2);

void ov96_021EAB38(s32 * param_1, BOOL param_2);

void ov96_021EABA8(s32 * param_1, u32 param_2);

void ov96_021EAC0C(s32 * param_1, s32 param_2);

void ov96_021EAC5C(s32 * param_1, u32 param_2);

void ov96_021EAF60(s32 param_1, s32 param_2, s32 param_3, void * param_4);

void ov96_021EB01C(s32 * param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_021EB06C(s32 param_1, u32 param_2, s32 param_3, u32 * param_4, s32 * param_5);

void ov96_021EB10C(u32 * param_1, f32 param_2, f32 param_3);

void ov96_02215884(s32 * param_1, s32 param_2, s32 param_3);

void ov96_02216AA4(u8 * param_1, u32 * param_2, u32 param_3, PokeathlonCourseData * param_4, u32 param_5);

void ov96_02218510(s32 * param_1, s32 param_2);

s32 ov96_0221935C(PokeathlonCourseData * param_1, u32 param_2, u32 param_3);

void ov96_022193CC(u16 param_1, s32 param_2);

void ov96_02219770(s32 param_1, s32 param_2, s32 param_3);

u32 ov96_02219794(u32 * param_1, u8 param_2, u8 param_3);

u32 ov96_02219940(s32 param_1, u8 param_2, u8 param_3);

void ov96_0221996C(s32 param_1, u32 param_2, u32 param_3, s32 param_4);

void sub_02006118(u32, u32);

void ov96_02216C00();

typedef struct BF_ov96_02216C38_0 { u32 pad0:7; u32 b7:1; } BF_ov96_02216C38_0; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_1 { u8 b0:1; } BF_ov96_02216C38_1; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_2 { u8 f0_4:4; u8 f4_2:2; u8 f6_2:2; } BF_ov96_02216C38_2; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_3 { u32 f0_8:8; u32 pad8:12; u32 b20:1; u32 pad21:2; u32 f23_2:2; } BF_ov96_02216C38_3; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_4 { u8 f0_6:6; u8 f6_2:2; } BF_ov96_02216C38_4; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_5 { u8 pad0:3; u8 f3_2:2; u8 f5_2:2; } BF_ov96_02216C38_5; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_6 { u8 f0_4:4; } BF_ov96_02216C38_6; /* invented: bitfield word (hk9) */
typedef struct BF_ov96_02216C38_7 { u8 pad0:6; u8 f6_2:2; } BF_ov96_02216C38_7; /* invented: bitfield word (hk9) */
void ov96_02216C38(u32 *param_1, u8 *param_2, PokeathlonCourseData *param_3, u32 param_4) {
  u8 *pbVar1;
  s32 bVar2;
  s32 bVar3;
  BOOL bVar4;
  u8 bVar5;
  void *pvVar6;
  BOOL BVar7;
  u32 uVar8;
  u32 uVar9;
  u32 uVar10;
  u32 kind;
  s32 *piVar11;
  u32 uVar12;
  u32 uVar13;
  s32 iVar15;
  s32 remainder;
  s32 remainder_00;
  s32 iVar16;
  s32 iVar17;
  u16 uVar19;
  u32 *puVar20;
  u8 *pbVar21;
  u32 uVar22;
  BOOL bVar24;
  u8 isThree;
  u32 uStack_98;
  f32 uStack_94;
  s16 iStack_84;
  f32 uStack_74;
  u32 uStack_48;
  u32 uStack_44;
  u32 uStack_40;
  u32 uStack_3c;
  u32 uStack_38[8] = {0};
  f32 fVar;
  
  pvVar6 = PokeathlonCourse_GetHeapAllocPtr4(param_3);
  if ((((BF_ov96_02216C38_0 *)(param_2 + 0x1c))->b7) != 0 &&
     (((BF_ov96_02216C38_1 *)((u8 *)param_1 + 0x3b9))->b0) == 0) {
    PlaySE(0x8d4);
  }
  uVar22 = 0;
  iStack_84 = 0;
  ((BF_ov96_02216C38_1 *)((u8 *)param_1 + 0x3b9))->b0 = (u8)((u32)(*(u32 *)(param_2 + 0x1c) << 0x18) >> 31);
  puVar20 = param_1 + 4;
  pbVar21 = param_2;
  do {
    BOOL isOut = PokeathlonCourse_GetParticipantCount(param_3) <= (s32)uVar22;
    BOOL isZero = ov96_021E5F24(param_3) == 0;
    bVar4 = FALSE;
    if (isZero && isOut) {
      bVar4 = TRUE;
    }
    if (pbVar21 == NULL) {
      GF_AssertFail();
    }
    if (puVar20 == NULL) {
      GF_AssertFail();
    }
    uVar9 = (((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f6_2) + iStack_84;
    uVar8 = ((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f6_2;
    puVar20[0x39] = puVar20[0x39] & 0xffff3fff | (uVar8 << 30) >> 16;
    uVar10 = uVar9 & 0xff;
    piVar11 = (s32 *)ov96_021EAA04(*(s32 *)((u8 *)pvVar6 + 0x1c),uVar10);
    *puVar20 = (unsigned long)(piVar11);
    if (piVar11 == NULL) {
      GF_AssertFail();
    }
    uVar12 = ov96_021E5F24(param_3);
    if (uVar22 == uVar12) {
      bVar5 = pbVar21[4];
      uVar12 = *(u32 *)(param_2 + 0x1c);
      remainder = (s32)(((u32)(bVar5 << 24) >> 30) + 2) % (s32)(3);
      uStack_98 = (u32)(((uVar12 >> 8) >> ((u8)(remainder + iStack_84) << 1) & 3) == 1);
      remainder_00 = (s32)(((u32)(bVar5 << 24) >> 30) + 1) % (s32)(3);
      ov96_02219770(*param_1,uStack_98,
                    (u32)(((uVar12 >> 8) >> ((u8)(remainder_00 + iStack_84) << 1) & 3) == 1))
      ;
    }
    if (((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->f23_2 != 0 && (kind = ((BF_ov96_02216C38_4 *)(&pbVar21[2]))->f6_2) != 0) {
      uVar12 = ((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f4_2;
      bVar24 = FALSE;
      if ((s32)uVar12 <= (s32)uVar22) {
        uVar12 = uVar12 - 1;
      }
      if (kind == 1) {
        uVar13 = ov96_021E5F24(param_3);
        if (uVar12 == uVar13) {
          ov96_02215884((s32 *)((u8 *)pvVar6 + 0x7f8),4,10);
        }
        bVar24 = TRUE;
        PlaySE(0x8d0);
      }
      else if (kind == 2) {
        uVar13 = ov96_021E5F24(param_3);
        if (uVar12 == uVar13) {
          ov96_02215884((s32 *)((u8 *)pvVar6 + 0x7f8),10,0xf);
        }
        bVar24 = TRUE;
        PlaySE(0x8d2);
      }
      if (bVar24) {
        s32 x1 = param_2[uVar12 * 6];
        s32 y1 = param_2[uVar12 * 6 + 1];
        s32 x0 = pbVar21[0];
        s32 y0 = pbVar21[1];
        s32 dx, dy;
        s16 hx, hy;
        dx = x0 - x1;
        if (dx < 0) {
          dx = -dx;
        }
        hx = dx / 2;
        dy = y0 - y1;
        if (dy < 0) {
          dy = -dy;
        }
        hy = dy / 2;
        if (x0 > x1) {
          x0 = x1;
        }
        if (y0 > y1) {
          y0 = y1;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset((ManagedSprite *)puVar20[5], (s16)(hx + x0), (s16)(hy + y0), 0x1e0000);
        ManagedSprite_ResetSpriteAnimCtrlState((ManagedSprite *)puVar20[5]);
        ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[5],1);
      }
    }
    puVar20[0x39] = puVar20[0x39] & 0xfe7fffff | ((u32)(((BF_ov96_02216C38_4 *)(&pbVar21[2]))->f6_2) << 30) >> 7;
    puVar20[0x39] = puVar20[0x39] & 0xfffcffff | ((u32)(((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f4_2) << 30) >> 14;
    ov96_021EAC0C(piVar11,(((BF_ov96_02216C38_5 *)(&pbVar21[5]))->f3_2) + 1);
    if (((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 4) && (puVar20[6] != 4)) {
      ov96_021EAF60(*(s32 *)((u8 *)pvVar6 + 0x1c),uVar10,1,puVar20 + uVar8 * 0x10 + 0xf);
    }
    else if ((puVar20[6] == 4) && ((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) != 4)) {
      ov96_021EAF60(*(s32 *)((u8 *)pvVar6 + 0x1c),uVar10,1,puVar20 + uVar8 * 0x10 + 7);
    }
    if (((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 5) && (puVar20[6] != 5)) {
      uVar10 = ov96_021E5F24(param_3);
      if (uVar22 == uVar10) {
        PlaySE_SetPitch(0x8d1,0x100);
      }
      else {
        sub_02006118(0x8d5,0x40);
      }
    }
    else if (((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 0xc) &&
            ((puVar20[6] != 0xc &&
             (iVar16 = ov96_0221935C(param_3,uVar22 & 0xff,uVar8), *(u8 *)(iVar16 + 0x18) != 0)
             ))) {
      PlaySE(0x8d3);
    }
    bVar5 = ((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4;
    if ((bVar5 == 6) && (puVar20[6] != 6)) {
      ManagedSprite_SetAnimNoRestart((ManagedSprite *)puVar20[1],(((BF_ov96_02216C38_5 *)(&pbVar21[5]))->f3_2) + 0xb);
      ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[1],1);
    }
    else if (((bVar5 != 6) && (puVar20[6] == 6)) || ((bVar5 == 7 && (puVar20[6] == 5)))) {
      ManagedSprite_SetAnim((ManagedSprite *)puVar20[1],(((BF_ov96_02216C38_5 *)(&pbVar21[5]))->f3_2) + 0xf);
      ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[1],1);
    }
    BVar7 = ManagedSprite_IsAnimated((ManagedSprite *)puVar20[1]);
    if (BVar7 == 1) {
      ManagedSprite_SetPositionXYWithSubscreenOffset
                ((ManagedSprite *)puVar20[1],(u16)*pbVar21,(u16)pbVar21[1],0x1e0000);
    }
    if ((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 6) {
      ov96_021EAC5C(piVar11,0xc);
    }
    bVar5 = *pbVar21;
    if ((((bVar5 == 0) || (bVar5 == 0xff)) || (uVar19 = (u16)pbVar21[1], uVar19 == 0)) ||
       (uVar19 == 0xff)) {
      ov96_021EAB38(piVar11,0);
      ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[1],0);
      ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[2],0);
    }
    else {
      bVar2 = (((BF_ov96_02216C38_5 *)(&pbVar21[5]))->f5_2) != 0;
      bVar3 = (((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 3;
      bVar5 = (*(u32 *)(param_2 + 0x1c) >> 8) >> (uVar10 << 1) & 3;
      ManagedSprite_SetPositionXYWithSubscreenOffset((ManagedSprite *)puVar20[2], *pbVar21, uVar19 - (((BF_ov96_02216C38_4 *)(&pbVar21[2]))->f0_6) - 0x18, 0x1e0000);
      if (bVar3 || bVar2) {
        bVar5 = 0;
      }
      else if (bVar5 == 2) {
        ManagedSprite_SetAnimNoRestart((ManagedSprite *)puVar20[2],1);
        BVar7 = IsSEPlaying(0x890);
        if (BVar7 == 0) {
          PlaySE(0x890);
        }
      }
      else if (bVar5 == 1) {
        ManagedSprite_SetAnimNoRestart((ManagedSprite *)puVar20[2],9);
      }
      ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[2], bVar5 != 0);
    }
    ov96_02216AA4(pbVar21,puVar20,*param_1,param_3,uVar22 & 0xff);
    isThree = (((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 3;
    if (isThree == TRUE && ((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->b20 == 0) {
      puVar20[0x39] = puVar20[0x39] | 0x80000;
      *(u8 *)((u8 *)puVar20 + 0xe1) = 6;
      puVar20[0x37] = 0x1000;
      PlaySE(0x89e);
      ov96_0221996C(*param_1,uVar22 & 0xff,uVar8,1);
      uVar9 = ov96_021E5F24(param_3);
      if ((uVar22 == uVar9) || (bVar4)) {
        puVar20[0x39] = puVar20[0x39] | 0x200000;
      }
    }
    else if (isThree == FALSE && ((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->b20 == 1) {
      ManagedSprite_SetPositionXYWithSubscreenOffset
                ((ManagedSprite *)puVar20[4],(u16)*pbVar21,(u16)pbVar21[1],0x1e0000);
      ManagedSprite_SetDrawFlag((ManagedSprite *)puVar20[4],1);
      ManagedSprite_ResetSpriteAnimCtrlState((ManagedSprite *)puVar20[4]);
      uVar9 = ov96_021E5F24(param_3);
      ov96_022193CC(0x8b6,(u32)(uVar22 == uVar9));
      ov96_0221996C(*param_1,uVar22 & 0xff,uVar8,0);
      ov96_021EAB38(piVar11,1);
      puVar20[0x39] = puVar20[0x39] & 0xfff7ffff;
    }
    puVar20[0x39] = puVar20[0x39] & 0xffefffff | ((u32)isThree << 31) >> 11;
    ov96_021EB01C(piVar11,(u32)*pbVar21,(u32)pbVar21[1],1);
    ov96_021EB01C(piVar11,(u32)*pbVar21,(u32)pbVar21[1] - (((BF_ov96_02216C38_4 *)(&pbVar21[2]))->f0_6),0);
    uVar9 = puVar20[0x39];
    if ((uVar9 << 12) >> 31 != 0) {
      if ((uVar9 << 10) >> 31 != 0) {
        if (bVar4) {
          iVar16 = ov96_02219940(*(s32 *)((u8 *)pvVar6 + 0x180), uVar22, (uVar9 << 16) >> 30);
          if (iVar16 != 0) {
            puVar20[0x39] = puVar20[0x39] & 0xffdfffff;
          }
        }
        else {
          iVar16 = ov96_02219794(*(u32 **)((u8 *)pvVar6 + 0x180),uVar22, (uVar9 << 16) >> 30);
          if (iVar16 != 0) {
            puVar20[0x39] = puVar20[0x39] & 0xffdfffff;
          }
        }
      }
      *(s8 *)((u8 *)puVar20 + 0xe1) = *(s8 *)((u8 *)puVar20 + 0xe1) + -1;
      if (*(s8 *)((u8 *)puVar20 + 0xe1) < 1) {
        ov96_021EAB38(piVar11,0);
      }
      else {
        uStack_3c = (u32)*pbVar21;
        uStack_40 = (u32)pbVar21[1];
        puVar20[0x37] = puVar20[0x37] + -0x19a;
        fVar = (f32)(s32)puVar20[0x37] / 4096.0f;
        ov96_021EB10C(piVar11,fVar,fVar);
        ov96_021EB06C((s32)piVar11,uStack_3c,uStack_40,&uStack_3c,(s32 *)&uStack_40);
        if (0x9f < (s32)uStack_40) {
          ov96_021EB01C(piVar11,(u32)*pbVar21,
                        (u32)pbVar21[1] + (6 - *(s8 *)((u8 *)puVar20 + 0xe1)) * 4,1);
        }
      }
    }
    else {
      uStack_94 = 1.2f - (f32)(s32)((0xa0 - (u32)pbVar21[1]) * 0xc) / 4096.0f;
      uStack_74 = uStack_94;
      if ((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 5) {
        uVar9 = (u8)(((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->f0_8);
        uStack_74 = uStack_94 * (0.8 - 0.1 * uVar9);
        if (uVar9 < 4) {
          puVar20[0x39] = (puVar20[0x39] & ~0xff) | (u8)(uVar9 + 1);
        }
      }
      if ((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 8) {
        iVar16 = ov96_0221935C(param_3,uVar22 & 0xff,uVar8);
        if (((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->f0_8 == 0) {
          if (*(u8 *)(iVar16 + 0x18) == 0) {
            uVar8 = ov96_021E5F24(param_3);
            if (uVar22 == uVar8) {
              PlaySE(0x5f3);
            }
          }
          else {
            PlaySE(0x60a);
          }
        }
        uVar8 = (u8)(((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->f0_8);
        uStack_94 = uStack_94 * (0.8 - 0.1 * uVar8);
        if (uVar8 < 5) {
          puVar20[0x39] = (puVar20[0x39] & ~0xff) | (u8)(uVar8 + 1);
        }
      }
      if (((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 8) || ((((BF_ov96_02216C38_2 *)(&pbVar21[4]))->f0_4) == 5)) {
        ov96_02218510(piVar11,0);
      }
      else if (((BF_ov96_02216C38_3 *)(&puVar20[0x39]))->f0_8 != 0) {
        ov96_02218510(piVar11,1);
        puVar20[0x39] = puVar20[0x39] & ~0xff;
      }
      ov96_021EB10C(piVar11,uStack_74,uStack_94);
    }
    pbVar1 = pbVar21 + 4;
    uVar22 = uVar22 + 1;
    puVar20[6] = ((BF_ov96_02216C38_6 *)pbVar1)->f0_4;
    puVar20 = puVar20 + 0x3a;
    iStack_84 = iStack_84 + 3;
    pbVar21 = pbVar21 + 6;
  } while ((s32)uVar22 < 4);
  iVar17 = 0;
  iVar16 = 0;
  puVar20 = uStack_38;
  do {
    iVar15 = ov96_021EAA04(*(s32 *)((u8 *)pvVar6 + 0x1c),(u8)((((BF_ov96_02216C38_7 *)(&param_2[4]))->f6_2) + iVar16));
    uStack_44 = (u32)*param_2;
    uStack_48 = (u32)param_2[1];
    ov96_021EB06C(iVar15,uStack_44,uStack_48,&uStack_44,(s32 *)&uStack_48);
    iVar17 = iVar17 + 1;
    *(s8 *)puVar20 = (s8)uStack_48;
    puVar20[1] = iVar15;
    param_2 = param_2 + 6;
    iVar16 = iVar16 + 3;
    puVar20 = puVar20 + 2;
  } while (iVar17 < 4);
  MATH_QSort(uStack_38,4,8,(MATHCompareFunc)(ov96_02216C00),(void *)0x0);
  iVar16 = 0;
  puVar20 = uStack_38;
  do {
    ov96_021EABA8((s32 *)puVar20[1],iVar16 + 7);
    iVar16 = iVar16 + 1;
    puVar20 = puVar20 + 2;
  } while (iVar16 < 4);
  return;
}