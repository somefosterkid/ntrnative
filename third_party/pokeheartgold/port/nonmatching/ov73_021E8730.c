/* PORTOK hk42 */
#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "palette.h"
#include "render_window.h"
#include "sys_task_api.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "palette.h"
#include "render_window.h"
#include "sys_task.h"
#include "sys_task_api.h"
void ov73_021E8960(SysTask *task, void *data);

void ov73_021E8730(void *arg0)
{
  NARC *temp_r0;
  s32 sp34;
  void *sp30;
  NARC *sp2C;
  NNSG2dPaletteData *sp38;
  s32 sp28;
  s32 sp24;
  s32 sp20;
  s32 sp1C;
  void *sp18;
  void *sp10;
  void *var_r4;
  u16 *var_r6;
  s32 var_r7;
  BgConfig *temp_r4;
  void *temp_r4_2;
  s32 temp_r1;
  void *sp14;
  s32 temp_r0_2;
  sp10 = arg0;
  temp_r4 = *((BgConfig **) (((u8 *) arg0) + 4));
  temp_r0 = NARC_New(NARC_a_0_8_8, HEAP_ID_150);
  sp2C = temp_r0;
  GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_150);
  GfGfxLoader_GXLoadPalFromOpenNarc(temp_r0, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_150);
  LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_150);
  LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_150);
  LoadUserFrameGfx2(temp_r4, GF_BG_LYR_MAIN_FIRST, 1U, 0xEU, (s32) Options_GetFrame(*((Options **) (((u8 *) (*((void **) arg0))) + 0x10))), HEAP_ID_150);
  LoadUserFrameGfx1(temp_r4, GF_BG_LYR_MAIN_FIRST, 0x1FU, 0xBU, 0U, HEAP_ID_150);
  GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 2, temp_r4, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, HEAP_ID_150);
  GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 6, temp_r4, GF_BG_LYR_SUB_0_F, 0U, 0x600U, 0, HEAP_ID_150);
  GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 0xB, temp_r4, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_150);
  GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r0, 0xC, temp_r4, GF_BG_LYR_SUB_1, 0U, 0x600U, 0, HEAP_ID_150);
  BG_SetMaskColor(0U, 0U);
  BG_SetMaskColor(4U, 0U);
  MI_CpuFill8(arg0 + 0xF98, 0U, 0x330U);
  temp_r4_2 = GfGfxLoader_GetPlttDataFromOpenNarc(temp_r0, 5, &sp38, HEAP_ID_150);
  MIi_CpuCopy16(sp38->pRawData, sp10 + 0xFA0, 0x80U);
  MIi_CpuCopy16(sp38->pRawData, sp10 + 0x1020, 0x80U);

  Heap_Free(temp_r4_2);
  sp24 = 0;
  sp28 = 0;
  sp18 = sp10 + 0x1020;
  sp14 = sp10 + 0xFA0;
  do
  {
    sp20 = 0;
    sp1C = 0;
    sp30 = sp10 + ((sp28 + 1) << 5);
    loop_2:
    if (sp24 >= 0x15)
    {
      GF_AssertFail();
    }

    var_r7 = 1;
    var_r4 = sp30 + 2;
    var_r6 = sp18 + 2;
    temp_r4_2 = sp14 + 2;
    sp34 = (s32) (sp20 >> 8);
    loop_5:
    BlendPalette(temp_r4_2, var_r6, 1U, sp34, (s32) (*((u16 *) (((u8 *) var_r4) + 0xFA0))));

    var_r7 += 1;
    var_r4 += 2;
    var_r6 += 1;
    temp_r4_2 += 2;
    if (var_r7 < 0x10)
    {
      goto loop_5;
    }
    sp18 += 0x20;
    sp24 += 1;
    if (sp1C != 1)
    {
      temp_r1 = sp20 + 0x300;
      sp20 = temp_r1;
      if (temp_r1 >= 0x1000)
      {
        sp20 = 0x1000;
        sp1C = 1;
      }
      goto loop_2;
    }
    sp14 += 0x20;
    temp_r0_2 = sp28 + 1;
    sp28 = temp_r0_2;
  }
  while (temp_r0_2 < 3);
  DC_FlushRange(sp10 + 0x1020, 0x2A0U);
  *((s32 *) (((u8 *) sp10) + 0xF9C)) = 1;
  *((s32 *) (((u8 *) sp10) + 0x12C4)) = 0;
  *((SysTask **) (((u8 *) sp10) + 0xF98)) = SysTask_CreateOnVBlankQueue(ov73_021E8960, sp10 + 0xF98, 0x14U);
  NARC_Delete(sp2C);
}

