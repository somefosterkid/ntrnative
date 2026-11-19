/* PORTOK hk42 */
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
s32 ov96_021E6104(void);

s32 ov96_021E6108(void *arg0);

s32 ov96_021E6138(void *arg0);

void ov96_021E64B8(void *arg0);

u8 ov96_021E90FC(void *arg0);

void ov96_021EB3A4(void *arg0);

void ov96_021F0D60(void *arg0);

void ov96_021F2EC8(void *arg0);

void ov96_021F424C(void *arg0);

s32 _s32_div_f(s32, u32);

u32 ov96_021E60C0(PokeathlonCourseData *, u32, u32);

void ov96_021E6168(PokeathlonCourseData *, u32, u32, u8 *);

u32 ov96_021E6290(PokeathlonCourseData *, u32, u32, u32);

void ov96_021E634C(PokeathlonCourseData *, u32, u32, u32, u32, u32, s16 (*)[2]);

u32 ov96_021E64F8(PokeathlonCourseData *, u32, u32, u32, u32);

void ov96_021E6670(PokeathlonCourseData *, u32);

s32 ov96_021E8BB0(void *);

void ov96_021E8BB4(u8 *, u32, u32 *);

void ov96_021E92B0(u32 *, u32, u32, u32, u32);

u32 ov96_021E9A78(u32, u32, u32);

u32 ov96_021EA854(u32, u32, u32, u32, u32);

void ov96_021EA8A8(u32, u32, u8 *, u32 *, u32, u32);

s32 ov96_021EAA00(u32);

u32 ov96_021EAA04(u32, u32);

u32 ov96_021EAA20(u32);

void ov96_021EAB38(u32, u32);

void ov96_021EABA8(u32, u32);

void ov96_021EABF4(u32, VecFx32 *);

void ov96_021EAC0C(u32, u32);

void ov96_021EAF6C(u32, s32);

void ov96_021EAF70(u32, s32, u32);

void ov96_021EAF94(u32, u16, u16);

void ov96_021EB0A4(u32, u16, u16, s32 *, s32 *);

u32 ov96_021EB180(u32, VecFx32 *);

void ov96_021EB29C(u32, u32, u32);

void ov96_021EB2BC(u32, u32, u32, u32, u32);

void ov96_021EB2F4(u32, u32, u32, u32, u32, u32);

void ov96_021EB334(u32, u32, u32, u32);

void ov96_021EB36C(u32, u32, u32, u32);

void ov96_021EB408(u32, u32, u32, u32, u32);

u32 ov96_021EB4F4(u32, u32, u32);

u32 ov96_021EB5B8(u32);

void ov96_021EB5C8(u32, u32, u32, u32, u32);

u32 ov96_021EB5E8(u32);

void ov96_021F0A5C(void);

void ov96_021F0BD4(u32);

void ov96_021F0F04(u32 *, u8 *);

void ov96_021F2B24(PokeathlonCourseData *, u8 *, u32, u32, u32 *);

u32 ov96_021F30A4(u32);

void ov96_021F30C4(u32, u32);

void ov96_021F31F0(u32, u16);

u32 ov96_021F3390(u32, u32, u32);

void ov96_021F33E0(u32, u32, u16, s32, u32 *, u32 *, u32 *, u32);

u32 ov96_021F3BF0(u32, u32, PokeathlonCourseData *);

void ov96_021F3E58(u32);

void ov96_021F3E60(u32, u32);

void ov96_021F3EC0(u32, u32);

void ov96_021F3F80(u32, u32);

extern VecFx32 ov96_0221BC70;

extern u16 ov96_0221BDD4[][3][2];


u32 ov96_021F010C(PokeathlonCourseData *param_1) {
  u32 uStack_12c[4];
  u16 uVar1;
  u16 uVar2;
  u8 uVar3;
  u8 bVar4;
  BgConfig *pBVar6;
  u32 uVar7;
  u32 *puVar8;
  s32 iVar9;
  BOOL BVar10;
  u32 uVar11;
  void *pvVar13;
  s32 sVar14;
  s32 iVar15;
  Sprite *pSVar18;
  s32 remainder;
  s32 remainder_00;
  s32 remainder_01;
  s32 remainder_03;
  u32 uVar19;
  u32 *puVar5;
  u8 *puStack_1f0;
  u32 *puStack_1ec;
  u8 *puStack_1e8;
  s32 iStack_1e4;
  u32 *puStack_1e0;
  u32 uStack_1dc;
  s32 uStack_1cc;
  u32 sprite;
  u8 *entry;
  s32 w;
  u32 *data;
  s32 iStack_1a8;
  s32 iStack_1ac;
  VecFx32 vec;
  u32 narcData[6];
  s16 pos[3][2];
  VecFx32 uStack_138;
  u8 auStack_1a4[60];
  u8 auStack_d8[192];
  u32 uStack_11c[17];
  
  puVar5 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
  uVar3 = PokeathlonCourse_GetField1ED(param_1);
  switch(uVar3) {
  case 0:
    Heap_Create(HEAP_ID_POKEATHLON,HEAP_ID_140,0x50000);
    Main_SetVBlankIntrCB(NULL,NULL);
    Main_SetHBlankIntrCB(NULL,NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(u32 *)0x04000000 = *(u32 *)0x04000000 & 0xffffe0ff;
    *(u32 *)0x04001000 = *(u32 *)0x04001000 & 0xffffe0ff;
    ov96_021F0A5C();
    data = PokeathlonCourse_AllocPtr4FromHeap(param_1,0x7f4);
    MI_CpuFill8(data,0,0x7f4);
    pvVar13 = Heap_Alloc(HEAP_ID_140,0x28);
    data[0x1fc] = (unsigned long)(pvVar13);
    MI_CpuFill8((void *)data[0x1fc],0,0x28);
    pBVar6 = BgConfig_Alloc(HEAP_ID_140);
    *data = (unsigned long)(pBVar6);
    ov96_021E6670(param_1,8);
    uStack_12c[0] = 0x68;
    uStack_12c[1] = 0x40000;
    uStack_12c[2] = 0x4000;
    uStack_12c[3] = 0x8c;
    ov96_021E92B0(uStack_12c,0x16,0x8c,0x300010,0x10);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0,0x7e,0,0x20,0,0x7e,0,0x20,HEAP_ID_140);
    data[5] = 0x8c;
    FontID_Alloc(FONT_UNK_4,HEAP_ID_140);
    ov96_021F0BD4(*data);
    ov96_021F2EC8(data);
    ((u8 *)&gSystem)[0x69] = 1;
    GfGfx_SwapDisplay();
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 1:
    uVar11 = ov96_021F3BF0(puVar5[5],*puVar5,param_1);
    puVar5[0x1dd] = uVar11;
    bVar4 = PokeathlonCourse_GetParticipantCount(param_1);
    uVar7 = PokeathlonCourse_GetMode(param_1);
    uVar11 = ov96_021F3390(puVar5[5],4 - (u32)bVar4,uVar7);
    puVar5[0x1dc] = uVar11;
    uVar11 = ov96_021E9A78(puVar5[5],0xaa7,1);
    puVar5[0x1da] = uVar11;
    uVar11 = ov96_021F30A4(puVar5[5]);
    puVar5[0x1cb] = uVar11;
    puVar5[499] = puVar5[0x1cb];
    ov96_021F3E58(puVar5[0x1dd]);
    ov96_021E64B8(param_1);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 2:
    uStack_138 = ov96_0221BC70;
    uVar11 = ov96_021EB180(puVar5[5], &uStack_138);
    puVar5[6] = uVar11;
    ov96_021EB5C8(puVar5[6],0,0,0,0x200000);
    uVar11 = ov96_021EB5E8(puVar5[6]);
    uVar11 = ov96_021EA854(puVar5[5],0xc,5,puVar5[0x1da],uVar11);
    puVar5[0x1db] = uVar11;
    ov96_021EB29C(puVar5[6],0,0x65);
    ov96_021EB29C(puVar5[6],1,0x68);
    ov96_021EB29C(puVar5[6],2,0x66);
    ov96_021EB29C(puVar5[6],3,0x67);
    ov96_021EB29C(puVar5[6],4,0x69);
    ov96_021EB2BC(puVar5[6],0xa7,3,0x65,1);
    ov96_021EB2F4(puVar5[6],0xa7,0,0x65,1,1);
    ov96_021EB334(puVar5[6],0xa7,2,0x65);
    ov96_021EB36C(puVar5[6],0xa7,1,0x65);
    ov96_021EB2BC(puVar5[6],0xa7,0x1c,0x68,1);
    ov96_021EB2F4(puVar5[6],0xa7,0x19,0x68,1,1);
    ov96_021EB334(puVar5[6],0xa7,0x1b,0x68);
    ov96_021EB36C(puVar5[6],0xa7,0x1a,0x68);
    ov96_021EB2BC(puVar5[6],0xa7,0x20,0x69,2);
    ov96_021EB2F4(puVar5[6],0xa7,0x1d,0x69,2,5);
    ov96_021EB334(puVar5[6],0xa7,0x1f,0x69);
    ov96_021EB36C(puVar5[6],0xa7,0x1e,0x69);
    ov96_021F3E60(puVar5[6],puVar5[0x1dd]);
    ov96_021EB3A4((void *)puVar5[6]);
    iVar9 = 0;
    do {
      ov96_021EB408(puVar5[6],2,1,0x65,2);
      iVar9 = iVar9 + 1;
    } while (iVar9 < 8);
    iVar9 = 0;
    do {
      ov96_021EB408(puVar5[6],1,1,0x68,4);
      ov96_021EB408(puVar5[6],1,1,0x68,7);
      iVar9 = iVar9 + 1;
    } while (iVar9 < 0xc);
    ov96_021F30C4(puVar5[0x1cb],puVar5[6]);
    iVar9 = 0;
    do {
      iVar15 = iVar9 / 3;
      remainder = iVar9 % 3;
      uVar11 = ov96_021EB4F4(puVar5[6],0x68,4);
      puVar5[iVar15 * 0x6c + remainder * 0x24 + 9] = uVar11;
      pSVar18 = (Sprite *)ov96_021EB5B8(uVar11);
      Sprite_SetDrawPriority(pSVar18,2);
      uVar11 = ov96_021EB4F4(puVar5[6],0x68,7);
      puVar5[iVar15 * 0x6c + remainder * 0x24 + 10] = uVar11;
      pSVar18 = (Sprite *)ov96_021EB5B8(uVar11);
      Sprite_SetDrawPriority(pSVar18,4);
      iVar9 = iVar9 + 1;
    } while (iVar9 < 0xc);
    puVar8 = (u32 *)ov96_021E6290(param_1,0,puVar5[0x1da],puVar5[6]);
    pSVar18 = (Sprite *)*puVar8;
    puVar5[7] = (unsigned long)(pSVar18);
    Sprite_SetDrawPriority(pSVar18,1);
    ov96_021F3EC0(puVar5[6],puVar5[0x1dd]);
    ov96_021F3F80(puVar5[6],puVar5[0x1dd]);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 3:
    iVar9 = 0;
    puStack_1f0 = auStack_d8;
    puStack_1ec = uStack_11c;
    puStack_1e8 = puStack_1f0;
    do {
      remainder_00 = iVar9 % 3;
      uVar11 = iVar9 / 3;
      ov96_021E6168(param_1,uVar11,remainder_00,puStack_1e8);
      pvVar13 = (void *)ov96_021E60C0(param_1,uVar11,remainder_00);
      sVar14 = ov96_021E6108(pvVar13);
      puStack_1ec[5] = sVar14;
      iVar15 = iVar9 / 3;
      remainder_01 = iVar9 % 3;
      ov96_021E8BB4(puStack_1f0,puVar5[5],(u32 *)((u8 *)(puVar5 + 8) + iVar15 * 0x1b0 + remainder_01 * 0x90 + 0x48));
      MI_CpuCopy8((u8 *)(puVar5 + 8) + iVar15 * 0x1b0 + remainder_01 * 0x90 + 0x48, (u8 *)(puVar5 + 8) + iVar15 * 0x1b0 + remainder_01 * 0x90 + 0x68, 0x20);
      TintPalette_GrayScale((u16 *)((u8 *)(puVar5 + 8) + iVar15 * 0x1b0 + remainder_01 * 0x90 + 0x68),0x10);
      iVar9 = iVar9 + 1;
      puStack_1e8 = puStack_1e8 + 0x10;
      puStack_1ec = puStack_1ec + 1;
      puStack_1f0 = puStack_1f0 + 0x10;
    } while (iVar9 < 0xc);
    uStack_11c[0] = 0;
    uStack_11c[1] = 1;
    uStack_11c[2] = 0;
    uStack_11c[3] = 1;
    uStack_11c[4] = 1;
    ov96_021EA8A8(puVar5[0x1db],0xc,auStack_d8,uStack_11c,0,0);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 4:
    iVar9 = ov96_021EAA00(puVar5[0x1db]);
    if (iVar9 != 0) {
      BVar10 = ov96_021E5F24(param_1);
      PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)*puVar5);
      PokeathlonCourse_SetField1F4(param_1,1);
      ReadWholeNarcMemberByIdPair(narcData,NARC_a_1_7_0,0xb);
      for (uStack_1cc = 0; uStack_1cc < 12; uStack_1cc++) {
          if (!(uStack_1cc < 12)) {
              break;
          }
        sprite = ov96_021EAA04(puVar5[0x1db],uStack_1cc & 0xff);
        ov96_021EAB38(sprite,1);
        sVar14 = ov96_021E6138((void *)ov96_021E60C0(param_1,uStack_1cc / 3,uStack_1cc % 3));
        ov96_021EAF70(sprite,narcData[sVar14 * 2 - 2],narcData[sVar14 * 2 - 1]);
        sprite = ov96_021EAA04(puVar5[0x1db],uStack_1cc & 0xff);
        pvVar13 = (void *)ov96_021EAA20(sprite);
        vec.x = 0;
        vec.y = 0;
        vec.z = 0;
        iVar9 = uStack_1cc / 3;
        remainder_03 = uStack_1cc % 3;
        entry = (u8 *)(puVar5 + 8) + iVar9 * 0x1b0 + remainder_03 * 0x90;
        *(u32 *)entry = sprite;
        *(u32 *)(entry + 0x18) = 1;
        w = ov96_021E90FC(pvVar13);
        if (*(u16 *)(ov96_021E8BB0(pvVar13) + 4) != 0) {
          vec.x = 0x20000;
          vec.y = (w + (0x40 - w) / 2) << 12;
        } else {
          vec.x = 0x10000;
          vec.y = (w + (0x20 - w) / 2) << 12;
        }
        uVar2 = ov96_0221BDD4[iVar9][remainder_03][0];
        uVar1 = ov96_0221BDD4[iVar9][remainder_03][1];
        entry[0x40] = 2;
        ov96_021EAC0C(sprite,2);
        ov96_021EAF94(sprite,uVar2,uVar1);
        ov96_021EAF6C(sprite,ov96_021E6104());
        ov96_021EB0A4(sprite,uVar2,uVar1,&iStack_1a8,&iStack_1ac);
        *(s32 *)(entry + 0xc) = iStack_1a8 << 12;
        *(s32 *)(entry + 0x10) = iStack_1ac << 12;
        *(s32 *)(entry + 0x28) = iStack_1a8 << 12;
        *(s32 *)(entry + 0x2c) = iStack_1ac << 12;
        *(s32 *)(entry + 0x1c) = iStack_1a8 << 12;
        *(s32 *)(entry + 0x20) = iStack_1ac << 12;
        if (iVar9 == ov96_021E5F24(param_1)) {
          ov96_021EABA8(sprite,5);
        } else {
          ov96_021EABA8(sprite,6);
        }
        ov96_021EABF4(sprite,&vec);
        if (ov96_021E5F24(param_1) == 0) {
          ov96_021F0F04(puVar5,ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(param_1) + 0x28));
        }
        if (iVar9 == BVar10) {
          pos[remainder_03][0] = iStack_1a8;
          pos[remainder_03][1] = iStack_1ac;
          Sprite_SetDrawPriority((Sprite *)ov96_021E64F8(param_1,sprite,puVar5[0x1da],ov96_021EB5E8(puVar5[6]),1),3);
        }
      }
      ov96_021E634C(param_1,0,puVar5[0x1da],puVar5[6],1,3,pos);
      ReadWholeNarcMemberByIdPair(auStack_1a4,NARC_a_1_7_0,1);
      uStack_1dc = 0;
      puStack_1e0 = puVar5 + 8;
      iStack_1e4 = 0;
      do {
        *(u8 *)((s32)puVar5 + uStack_1dc + 0x720) = 0xc;
        uVar19 = 0;
        puVar8 = puStack_1e0;
        do {
          ov96_021F2B24(param_1,auStack_1a4,uStack_1dc & 0xff,uVar19 & 0xff,puVar8);
          ov96_021F33E0(puVar5[0x1dc],uVar19 + iStack_1e4 & 0xff,*(u16 *)((s32)puVar8 + 0x8a)
                        ,(s32)puVar8 + 0x8e,puVar8 + 10,puVar8 + 6,puVar8 + 7,*puVar8);
          uVar19 = uVar19 + 1;
          puVar8 = puVar8 + 0x24;
        } while ((s32)uVar19 < 3);
        puStack_1e0 = puStack_1e0 + 0x6c;
        iStack_1e4 = iStack_1e4 + 3;
        uStack_1dc = uStack_1dc + 1;
      } while ((s32)uStack_1dc < 4);
      *(u16 *)(puVar5 + 0x1cc) = 0;
      *(u16 *)((s32)puVar5 + 0x732) = 0;
      puVar5[0x1f5] = 0xc;
      ov96_021F31F0(puVar5[0x1cb],*(u16 *)(puVar5 + 0x1cc) & 0xff);
      G2x_SetBlendAlpha_(0x4000050,0,1,0xc,4);
      *(u8 *)((s32)puVar5 + 0x74b) = 0xc;
      ov96_021F0D60(puVar5);
      ov96_021F424C((void *)puVar5[0x1dd]);
      GfGfx_EngineATogglePlanes(0x10,1);
      GfGfx_EngineBTogglePlanes(0x10,1);
      sub_0203A994(1);
      PokeathlonCourse_IncrementField1ED(param_1);
    }
    break;
  case 5:
    return 1;
  }
  return 0;
}