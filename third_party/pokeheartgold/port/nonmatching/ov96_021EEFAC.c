#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokeathlon/pokeathlon.h"
#include "system.h"
#include "bg_window.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokeathlon/pokeathlon.h"
#include "save.h"
#include "system.h"
void ov96_021EEEEC(void *data);
s32 ov96_021EE740(s32);
s32 ov96_021EE75C(s32, BgConfig *, s32, u32, SaveData *);
s32 ov96_021EEECC();
s32 ov96_021EEF0C(BgConfig *, s32);

void ov96_021EEFAC(PokeathlonCourseData *arg0) {
    BgConfig *temp_r0_2;
    u32 temp_r6;
    void *temp_r0;

    Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_159, 0x40000U);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(s32 *)REG_DISPCNT_ADDR &= 0xFFFFE0FF;
    *(s32 *)REG_DB_DISPCNT_ADDR &= 0xFFFFE0FF;
    ov96_021EEECC();
    temp_r0 = PokeathlonCourse_AllocPtr4FromHeap(arg0, 0x10U);
    MI_CpuFill8(temp_r0, 0U, 0x10U);
    *(s32 *)temp_r0 = 0x9F;
    temp_r0_2 = BgConfig_Alloc(HEAP_ID_159);
    *(BgConfig **)((u8 *)temp_r0 + 4) = temp_r0_2;
    ov96_021EEF0C(temp_r0_2, *(s32 *)temp_r0);
    *(s32 *)((u8 *)temp_r0 + 8) = ov96_021EE740(*(s32 *)temp_r0);
    temp_r6 = PokeathlonCourse_GetMode(arg0);
    ov96_021EE75C(*(s32 *)((u8 *)temp_r0 + 8), *(BgConfig **)((u8 *)temp_r0 + 4), 0, temp_r6, PokeathlonCourse_GetSaveData(arg0));
    Main_SetVBlankIntrCB(ov96_021EEEEC, temp_r0);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
}
