#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "overlay_96_app_0221040c.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "system.h"
#include "unk_0203A3B0.h"

void ov96_021E9510(void *arg0);

void ov96_021E952C(void *arg0);

void ov96_021E93B4(void *, u32);

void ov96_021E9570(void *, u8);

void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);

void ov96_0221A56C(void *, u32);

BOOL ov96_0220E60C(PokeathlonCourseData *param_1, int unused);

s32 ov96_0220EAA4(UnkStruct_ov96_0220E3F0 *);
s32 ov96_0220EE4C(UnkStruct_ov96_0220E3F0 *);
s32 ov96_022101D0(UnkStruct_ov96_0220E3F0_Hud *);
SysTask *ov96_02210850(void *);
BOOL ov96_0220E670(PokeathlonCourseData *arg0, int unused);

BOOL ov96_0220E60C(PokeathlonCourseData *param_1, int unused) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 0x127);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 4);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 4);
    return 1;
}

BOOL ov96_0220E670(PokeathlonCourseData *data, int unused) {
    UnkStruct_ov96_0220E3F0 *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021E6550(data);
    ov96_02210850(v0->unk_C8);
    ov96_022102D4(v0->unk_C4);
    ov96_022101D0(&v0->hud);
    ov96_0220EAA4(v0);
    ov96_0220EE4C(v0);
    sub_0203A914();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_142);
    return 1;
}
