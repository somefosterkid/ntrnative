/* PORTOK hk42 */
#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
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
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
s32 ov96_021E6104(void);

s32 ov96_021E6108(void *arg0);

s32 ov96_021E6138(void *arg0);

void ov96_021EB3A4(void *arg0);

void ov96_021FD060(void *arg0);

void ov96_021FFD4C(void *arg0);

s32 _s32_div_f();

u32 ov96_021E60C0(PokeathlonCourseData *, u32, u32);

void ov96_021E6168(PokeathlonCourseData *, u32, u32, u16 *);

u32 ov96_021E6290(PokeathlonCourseData *, u32, u32, u32);

void ov96_021E634C(PokeathlonCourseData *, u32, u32, u32, u32, u32, s16 *);

void ov96_021E6670(PokeathlonCourseData *, u32);

void ov96_021E92B0(u32 *, u32, u32, u32, u32);

u32 ov96_021E9A78(u32, u32, u32);

u32 ov96_021EA854(u32, u32, u32, u32, u32);

void ov96_021EA8A8(u32, u32, u16 *, u32 *, u32, u32);

s32 ov96_021EAA00(u32);

u32 ov96_021EAA04(u32, u8);

void ov96_021EAB38(u32, u32);

void ov96_021EAC0C(u32, u32);

void ov96_021EAF6C(u32, s32);

void ov96_021EAF70(u32, u32, u32);

void ov96_021EAF94(u32, u32, s32);

void ov96_021EB0A4(u32, u32, s32, s32 *, s32 *);

u32 ov96_021EB180(u32, u32 *);

void ov96_021EB29C(u32, u32, u32);

void ov96_021EB5C8(u32, u32, u32, u32, u32);

u32 ov96_021EB5E8(u32);

void ov96_021FCEE0(void);

void ov96_021FCF00(u32);

void ov96_021FD0E4(u32);

void ov96_021FD128(u32 *, u32);

void ov96_021FDA30(u32 *, u8 *);

void ov96_021FEFE8(PokeathlonCourseData *, u8 *, u8, u8, u8 *);

void ov96_021FFB7C(u8 *, u8, u32);

u32 ov96_021FFF3C(u32, u32, PokeathlonCourseData *);

void ov96_022000E4(u32, u32);

void ov96_02200180(u32, u32, u32);

void ov96_022002F8(u32, u16 *);

void ov96_022003E8(u32, u16 *);

void ov96_02200454(u32, u32, u32);

void ov96_02200A64(u32, u16);

void ov96_02200B04(u32, BOOL);

u32 ov96_02200E3C(u32, u32, u32);

void ov96_02200E78(u32, u8, u8 *);

void ov96_02200E80(u32, u32 *);

typedef struct { u32 v[3]; } Ov96V3;
extern Ov96V3 ov96_0221C5E4;


u32 ov96_021FC768(PokeathlonCourseData *param_1) {
  u32 *data;
  s32 i;
  s32 j;
  s32 rem;
  s32 q;
  s32 y;
  u32 obj;
  u32 self;
  u8 *sub;
  u32 *pArr;
  u16 *pBuf;
  u16 *pOut;
  u16 buf[12][8];
  u32 arr[17];
  u8 buf160[160];
  u32 tmp4[4];
  Ov96V3 tmp3;
  u16 out[24];
  u32 tbl[3][2];
  s16 pos[2];
  s32 x;
  s32 yy;

  data = PokeathlonCourse_GetHeapAllocPtr4(param_1);
  switch (PokeathlonCourse_GetField1ED(param_1)) {
  case 0:
    Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_144, 0x68000);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    ov96_021FCEE0();
    data = PokeathlonCourse_AllocPtr4FromHeap(param_1, 0x648);
    MI_CpuFill8(data, 0, 0x648);
    data[0] = (u32)BgConfig_Alloc(HEAP_ID_144);
    PokeathlonCourse_SetField3A4(param_1, (u32)(data + 0xf9), (u32)(data + 0x117), 0x78);
    ov96_021E6670(param_1, 8);
    tmp4[0] = 0x97;
    tmp4[1] = 0x40000;
    tmp4[2] = 0x4000;
    tmp4[3] = 0x90;
    ov96_021E92B0(tmp4, 0x16, 0x90, 0x300010, 0x10);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7e, 0, 0x20, 0, 0x7e, 0, 0x20, HEAP_ID_144);
    data[5] = 0x90;
    FontID_Alloc(FONT_UNK_4, HEAP_ID_144);
    ov96_021FCF00(data[0]);
    ov96_021FFD4C(data);
    ((u8 *)&gSystem)[0x69] = 1;
    GfGfx_SwapDisplay();
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 1:
    data[0xf8] = ov96_021FFF3C(data[5], data[0], param_1);
    data[0xf7] = ov96_02200E3C(data[5], 4 - PokeathlonCourse_GetParticipantCount(param_1), PokeathlonCourse_GetMode(param_1));
    data[0xf1] = ov96_021E9A78(data[5], 0x2bf, 1);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 2:
    tmp3 = ov96_0221C5E4;
    data[6] = ov96_021EB180(data[5], tmp3.v);
    ov96_021EB5C8(data[6], 0, 0, 0, 0x200000);
    data[0xf2] = ov96_021EA854(data[5], 0xc, 4, data[0xf1], ov96_021EB5E8(data[6]));
    ov96_021EB29C(data[6], 0, 0x65);
    ov96_021EB29C(data[6], 1, 0x66);
    ov96_021EB29C(data[6], 2, 0x67);
    ov96_021EB29C(data[6], 3, 0x68);
    ov96_022000E4(data[0xf8], data[6]);
    ov96_021FD0E4(data[6]);
    ov96_021EB3A4((void *)data[6]);
    Sprite_SetDrawPriority(*(Sprite **)ov96_021E6290(param_1, 0, data[0xf1], data[6]), 1);
    ov96_021FD128(data, data[6]);
    ov96_02200180(data[0xf8], data[6], data[0xf1]);
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 3:
    pArr = arr;
    pBuf = buf[0];
    pOut = out;
    for (i = 0; i < 12; i++) {
      rem = i % 3;
      q = i / 3;
      ov96_021E6168(param_1, q, rem, pBuf);
      pArr[5] = ov96_021E6108((void *)ov96_021E60C0(param_1, q, rem));
      pOut[0] = pBuf[0];
      pOut[1] = pBuf[1];
      pBuf += 8;
      pOut += 2;
      pArr++;
    }
    ov96_022002F8(data[0xf8], out);
    ov96_022003E8(data[0xf8], buf[(u8)ov96_021E5F24(param_1) * 3]);
    ov96_02200454(data[0xf8], 0, 1);
    ov96_02200454(data[0xf8], 1, 2);
    arr[1] = 2;
    arr[0] = 0;
    arr[2] = 0;
    arr[3] = 1;
    arr[4] = 1;
    ov96_021EA8A8(data[0xf2], 0xc, buf[0], arr, 0, 0);
    ov96_02200B04(data[0xf8], ov96_021E5F24(param_1));
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 4:
    if (ov96_021EAA00(data[0xf2]) != 0) {
      self = ov96_021E5F24(param_1);
      PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)data[0]);
      PokeathlonCourse_SetField1F4(param_1, 1);
      ReadWholeNarcMemberByIdPair(tbl, NARC_a_1_7_0, 0xc);
      for (i = 0; i < 12; i++) {
        obj = ov96_021EAA04(data[0xf2], i);
        rem = i % 3;
        if (rem == 0) {
          ov96_021EAB38(obj, 1);
        }
        j = ov96_021E6138((void *)ov96_021E60C0(param_1, i / 3, i % 3));
        ov96_021EAF70(obj, tbl[j - 1][0], tbl[j - 1][1]);
        obj = ov96_021EAA04(data[0xf2], i);
        q = i / 3;
        sub = (u8 *)data + 0x30 + q * 0xd4;
        sub[0x88] = 4;
        ((u32 *)sub)[rem] = obj;
        y = (q + 1) * 0x1b + 0x28;
        ov96_021EAC0C(obj, 4);
        ov96_021EAF94(obj, 0x50, y);
        ov96_021EAF6C(obj, ov96_021E6104());
        *(u32 *)(sub + 0x7c) = 0;
        *(s32 *)(sub + 0x80) = y << 12;
        if (q == self && rem == 0) {
          ov96_021EB0A4(obj, 0x50, y, &x, &yy);
          pos[0] = x;
          pos[1] = yy;
        }
      }
      sub = (u8 *)data + 0x30;
      for (i = 0; i < 4; i++) {
        ov96_021FFB7C(sub, *(s32 *)(sub + 0x80) / 4096, i == ov96_021E5F24(param_1));
        sub += 0xd4;
      }
      ReadWholeNarcMemberByIdPair(buf160, NARC_a_1_7_0, 2);
      *(u16 *)((u8 *)data + 0x640) = 0xa8c;
      ov96_02200A64(data[0xf8], *(u16 *)((u8 *)data + 0x640));
      sub = (u8 *)data + 0x30;
      for (i = 0; i < 4; i++) {
        data[0xec + i] = 0;
        for (j = 0; j < 3; j++) {
          ov96_021FEFE8(param_1, buf160, i, j, sub);
        }
        if (ov96_021E5F24(param_1) == 0) {
          ov96_02200E78(data[0xf7], i, sub);
        }
        ((u8 *)data)[i * 0xd4 + 0x100] = i;
        sub += 0xd4;
      }
      if (ov96_021E5F24(param_1) == 0) {
        ov96_021FDA30(data, ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(param_1) + 0x28));
      }
      ov96_021E634C(param_1, 0, data[0xf1], data[6], 1, 1, pos);
      ov96_021FD060(data);
      GfGfx_EngineATogglePlanes(0x10, 1);
      GfGfx_EngineBTogglePlanes(0x10, 1);
      sub_0203A994(1);
      PokeathlonCourse_IncrementField1ED(param_1);
    }
    break;
  case 5:
    if (ov96_021E5F24(param_1) == 0) {
      ReadWholeNarcMemberByIdPair(data + 0xf9, NARC_a_2_2_7, LCRandom() % 10);
      ov96_02200E80(data[0xf7], data + 0xf9);
    }
    PokeathlonCourse_IncrementField1ED(param_1);
    break;
  case 6:
    GF_ASSERT(GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON));
    return 1;
  }
  return 0;
}
