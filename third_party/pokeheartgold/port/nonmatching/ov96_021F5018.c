/* PORTOK hk42 */
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
s32 ov96_021E6104(void);
void ov96_021F5630(void);
void *ov96_021E60C0(PokeathlonCourseData *, u32, s32);
u32 ov96_021E6108(void *);
s32 ov96_021E6138(void *);
void ov96_021E6168(PokeathlonCourseData *, u32, s32, u8 *);
u32 ov96_021E6290(PokeathlonCourseData *, u32, u32, u32);
void ov96_021E634C(PokeathlonCourseData *, u32, u32, u32, u32, u32, u16 *);
void ov96_021E6670(PokeathlonCourseData *, u32);
void ov96_021E92B0(u32 *, u32, u32, u32, u32);
u32 ov96_021E9A78(u32, u32, u32);
u32 ov96_021EA854(u32, u32, u32, u32, u32);
void ov96_021EA8A8(u32, u32, u8 *, u32 *, u32, u32);
s32 ov96_021EAA00(u32);
u32 ov96_021EAA04(u32, u32);
void ov96_021EAB38(u32, u32);
void ov96_021EAC0C(u32, u32);
void ov96_021EAF6C(u32, s32);
void ov96_021EAF70(u32, u32, u32);
void ov96_021EAF94(u32, s32, u32);
void ov96_021EB0A4(u32, s32, u32, s32 *, s32 *);
u32 ov96_021EB180(u32, u32 *);
void ov96_021EB29C(u32, u32, u32);
void ov96_021EB3A4(u32);
void ov96_021EB5C8(u32, u32, u32, u32, u32);
u32 ov96_021EB5E8(u32);
void ov96_021F584C(u32);
void ov96_021F5980(u32 *);
void ov96_021F6138(u32 *);
void ov96_021F6C18(u32);
void ov96_021F6C5C(u32 *, u32);
void ov96_021F6DA4(u32 *, u32);
void ov96_021F6E38(u32 *);
void ov96_021F6F3C(u32 *, u32);
void ov96_021F6F80(PokeathlonCourseData *, u32 *);
void ov96_021F7050(u32 *);
void ov96_021F715C(PokeathlonCourseData *, u8 *, u32, u32 *);
u32 ov96_021F74A4(u32);
u32 ov96_021F7684(u32, u32, u32, u8 *, PokeathlonCourseData *);
extern VecFx32 ov96_0221C0B8;

u32 ov96_021F5018(PokeathlonCourseData *param_1) {
  u8 uVar1;
  u8 bVar2;
  u32 *puVar3;
  BgConfig *pBVar4;
  NNSG2dCellTransferState *pNVar5;
  u32 uVar6;
  u8 *puVar7;
  u32 *puVar8;
  s32 sVar9;
  u32 uVar10;
  s32 iVar11;
  u32 uVar12;
  s32 iVar13;
  u8 *puVar14;
  u16 *puVar15;
  u32 sArr [17];
  u32 sHeapDesc [4];
  VecFx32 uStack_78;
  u8 auStack_a8 [48];
  u32 auStack_c0 [6];
  u16 auStack_cc [6];
  u8 auStack_108 [60];
  s32 auStack_10c [1];
  s32 auStack_110 [1];
  u32 *puStack_11c;
  u32 *puStack_118;
  u32 uStack_114;

  
  puVar3 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
  uVar1 = PokeathlonCourse_GetField1ED(param_1);
  switch(uVar1) {
  case 0:
    Heap_Create(HEAP_ID_POKEATHLON,HEAP_ID_143,0x50000);
    Main_SetVBlankIntrCB((GFIntrCB)0x0,(void *)0x0);
    Main_SetHBlankIntrCB((GFIntrCB)0x0,(void *)0x0);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(u32 *)0x04000000 = *(u32 *)0x04000000 & 0xffffe0ff;
    *(u32 *)0x04001000 = *(u32 *)0x04001000 & 0xffffe0ff;
    ov96_021F5630();
    puVar3 = PokeathlonCourse_AllocPtr4FromHeap(param_1,0x1004);
    MI_CpuFill8(puVar3,0,0x1004);
    pBVar4 = BgConfig_Alloc(HEAP_ID_143);
    *puVar3 = (u32)pBVar4;
    ov96_021E6670(param_1,8);
    sHeapDesc[0] = 0xd5;
    sHeapDesc[1] = 0x40000;
    sHeapDesc[2] = 0x4000;
    sHeapDesc[3] = PokeathlonCourse_GetHeapID(param_1);
    ov96_021E92B0(sHeapDesc,0xf,0x8f,0x300010,0x300010);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0,0x7e,0,0x20,0,0x7e,0,0x20,HEAP_ID_143);
    puVar3[0x15] = 0x8f;
    FontID_Alloc(FONT_UNK_4,(enum HeapID)puVar3[0x15]);
    ov96_021F584C(*puVar3);
    ov96_021F6138(puVar3);
    ((u8 *)&gSystem)[0x69] = 1;
    GfGfx_SwapDisplay();
    GF_CreateVramTransferManager(0xc,(enum HeapID)puVar3[0x15]);
    pNVar5 = sub_02020654(0xc,(enum HeapID)puVar3[0x15]);
    puVar3[0x400] = (u32)pNVar5;
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 1:
    uVar10 = ov96_021F74A4(puVar3[0x15]);
    puVar3[0x23] = uVar10;
    bVar2 = PokeathlonCourse_GetParticipantCount(param_1);
    uVar6 = PokeathlonCourse_GetMode(param_1);
    puVar7 = PokeathlonCourse_GetDataCopyArea(param_1);
    uVar10 = ov96_021F7684(puVar3[0x15],4 - (u32)bVar2,uVar6,puVar7,param_1);
    puVar3[0x4e] = uVar10;
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 2:
    uStack_78 = ov96_0221C0B8;
    uVar10 = ov96_021EB180(puVar3[0x15],(u32 *)&uStack_78);
    puVar3[0x18] = uVar10;
    ov96_021EB5C8(puVar3[0x18],0,0xc0000,0,0);
    uVar10 = ov96_021EB5E8(puVar3[0x18]);
    uVar10 = ov96_021EA854(puVar3[0x15],3,10,0,uVar10);
    puVar3[99] = uVar10;
    ov96_021EB29C(puVar3[0x18],0,0x65);
    ov96_021F6C18(puVar3[0x18]);
    ov96_021EB3A4(puVar3[0x18]);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 3:
    uVar12 = ov96_021E5F24(param_1);
    iVar11 = 0;
    puVar14 = auStack_a8;
    puVar8 = sArr;
    do {
      ov96_021E6168(param_1,uVar12 & 0xff,iVar11,puVar14);

      uVar10 = ov96_021E6108(ov96_021E60C0(param_1,uVar12 & 0xff,iVar11));
      puVar8[5] = uVar10;
      iVar11 = iVar11 + 1;
      puVar14 = puVar14 + 0x10;
      puVar8 = puVar8 + 1;
    } while (iVar11 < 3);
    sArr[0] = 0;
    sArr[1] = 1;
    sArr[2] = 1;
    sArr[3] = 3;
    sArr[4] = 3;
    ov96_021EA8A8(puVar3[99],3,auStack_a8,sArr,0,0);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 4:
    iVar11 = ov96_021EAA00(puVar3[99]);
    if (iVar11 != 0) {
      uVar10 = ov96_021E9A78(puVar3[0x15],0x3a1,1);
      puVar3[0x62] = uVar10;
      puVar8 = (u32 *)ov96_021E6290(param_1,0xc0,puVar3[0x62],puVar3[0x18]);
      Sprite_SetDrawPriority((Sprite *)*puVar8,1);
      PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)*puVar3);
      PokeathlonCourse_SetField1F4(param_1,1);
      ReadWholeNarcMemberByIdPair(auStack_c0,NARC_a_1_7_0,0x10);
      uStack_114 = 0;
      puStack_11c = puVar3 + 0x24;
      iVar11 = 0x40;
      puVar15 = auStack_cc;
      do {
        uVar10 = ov96_021EAA04(puVar3[99],uStack_114 & 0xff);
        ov96_021EAB38(uVar10,1);
        *puStack_11c = uVar10;

        iVar13 = ov96_021E6138(ov96_021E60C0(param_1,0,uStack_114));
        ov96_021EAF70(uVar10,auStack_c0[iVar13 * 2 - 2],auStack_c0[iVar13 * 2 - 1]);
        ov96_021EAC0C(uVar10,2);
        ov96_021EAF94(uVar10,iVar11,0x120);
        sVar9 = ov96_021E6104();
        ov96_021EAF6C(uVar10,sVar9);
        puStack_11c[2] = iVar11 << 0xc;
        puStack_11c[3] = 0x120000;
        puStack_11c[7] = iVar11 << 0xc;
        ov96_021EB0A4(uVar10,iVar11,0x120,auStack_10c,auStack_110);
        iVar11 = iVar11 + 0x40;
        *puVar15 = (u16)auStack_10c[0];
        puVar15[1] = (u16)auStack_110[0];
        puVar15 = puVar15 + 2;
        puStack_11c = puStack_11c + 0xe;
        uStack_114 = uStack_114 + 1;
      } while ((s32)uStack_114 < 3);
      ov96_021E634C(param_1,0,puVar3[0x62],puVar3[0x18],1,3,auStack_cc);
      *(u8 *)(puVar3 + 0x50) = 3;
      ov96_021F5980(puVar3);
      ov96_021F6C5C(puVar3,puVar3[0x18]);
      ov96_021F7050(puVar3);
      uVar12 = ov96_021E5F24(param_1);
      ov96_021F6DA4(puVar3,uVar12 & 0xff);
      PokeathlonCourse_IncrementField1ED(param_1);
    }
    break;
  case 5:
    GfGfx_EngineATogglePlanes(1,1);
    GfGfx_EngineATogglePlanes(0x10,1);
    GfGfx_EngineBTogglePlanes(0x10,1);
    ReadWholeNarcMemberByIdPair(auStack_108,NARC_a_1_7_0,6);
    uVar12 = 0;
    puVar8 = puVar3 + 0x24;
    do {
      ov96_021F715C(param_1,auStack_108,uVar12 & 0xff,puVar8);
      uVar12 = uVar12 + 1;
      puVar8 = puVar8 + 0xe;
    } while ((s32)uVar12 < 3);
    ov96_021E5F24(param_1);
    puVar3[0x4f] = 0x708;
    ov96_021F6F3C(puVar3,puVar3[0x4f] & 0xffff);
    ov96_021F6F80(param_1,puVar3);
    iVar11 = 0;
    iVar13 = 0;
    puVar8 = puVar3;
    do {
      puVar8[0x68] = 0;
      puVar8[0xe8] = 0;
      puVar8[0x168] = 0;
      puVar8[0x1e8] = 0;
      puVar8[0x268] = 0;
      puVar8[0x2e8] = 0;
      puVar8[0x368] = 0;
      iVar11 = iVar11 + 1;
      puVar8 = puVar8 + 1;
    } while (iVar11 < 0x80);
    iVar11 = 0x40;
    puVar8 = puVar3;
    puStack_118 = puVar3;
    do {
      puVar3[0x3ed] = 0;
      puVar8[1000] = iVar11 / 2;
      puVar3[0x3ec] = (s32)((((double)(float)iVar11) / 64.0 - 2.0) * 4096.0);
      *(u8 *)(puStack_118 + 0x2e) = 1;
      iVar13 = iVar13 + 1;
      puStack_118 = puStack_118 + 0xe;
      puVar3 = puVar3 + 7;
      iVar11 = iVar11 + 0x40;
      puVar8 = puVar8 + 1;
    } while (iVar13 < 3);
    sub_0203A994(1);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 6:
    ov96_021F6E38(puVar3);
    return 1;
  }
  return 0;
}
