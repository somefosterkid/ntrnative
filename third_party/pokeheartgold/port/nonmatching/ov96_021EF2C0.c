/* PORTOK hk42 */
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "poke_overlay.h"
#include "pokeathlon/pokeathlon.h"
#include "system.h"
void ov96_021EF23C(void *);
void ov96_021EF260(void);
s8 ov96_021EF2A0(PokeathlonCourseData *);
s32 ov96_021EF2AC(PokeathlonCourseData *);
s32 sub_0200FC20(s32);
extern s32 SDK_OVERLAY_OVY_98_ID;

void ov96_021EF2C0(PokeathlonCourseData *arg0)
{
  s32 var_r0;
  void *temp_r0;
  enum HeapID heapId;
  PokeathlonCourse_GetHeapAllocPtr4(arg0);
  Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_136, 0x40000U);
  heapId = HEAP_ID_136;
  HandleLoadOverlay((u32) (&SDK_OVERLAY_OVY_98_ID), OVY_LOAD_ASYNC);
  Main_SetVBlankIntrCB(0L, 0L);
  Main_SetHBlankIntrCB(0L, 0L);
  GfGfx_DisableEngineAPlanes();
  GfGfx_DisableEngineBPlanes();
  GX_SetVisiblePlane(0);
  GXS_SetVisiblePlane(0);
  ov96_021EF260();
  temp_r0 = PokeathlonCourse_AllocPtr4FromHeap(arg0, 0x48U);
  MI_CpuFill8(temp_r0, 0U, 0x48U);
  *((NARC **) (((u8 *) temp_r0) + 8)) = NARC_New(NARC_a_2_1_9, heapId);
  var_r0 = 4;
  *((BgConfig **) (((u8 *) temp_r0) + var_r0)) = BgConfig_Alloc(heapId);
  *((s8 *) (((u8 *) temp_r0) + 0x20)) = ov96_021EF2A0(arg0);
  *((s8 *) (((u8 *) temp_r0) + 0x22)) = (s8) PokeathlonCourse_GetCurrentParticipantIndex(arg0);
  if (PokeathlonCourse_GetMode(arg0) == 1)
  {
    var_r0 = 1;
  }
  else
  {
    var_r0 = 0;
  }
  *((s32 *) (((u8 *) temp_r0) + 0x24)) = var_r0;
  *((PokeathlonCourseData **) temp_r0) = arg0;
  *((s32 *) (((u8 *) temp_r0) + 0xC)) = heapId;
  Main_SetVBlankIntrCB(ov96_021EF23C, temp_r0);
  gSystem.screensFlipped = 1;
  GfGfx_SwapDisplay();
  sub_0200FC20(0x7FFF);
  if ((*((s32 *) (((u8 *) temp_r0) + 0x24))) != 0)
  {
    *((s32 *) (((u8 *) temp_r0) + 0x28)) = 0;
    return;
  }
  *((s32 *) (((u8 *) temp_r0) + 0x28)) = ov96_021EF2AC(arg0);
}

