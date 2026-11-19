#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_021E9510(void *arg0);

void ov96_021E952C(void *arg0);

void ov96_021E93B4(void *, u32);

void ov96_021E9570(void *, u8);

void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);

void ov96_0221A56C(void *, u32);

u32 ov96_02209748(PokeathlonCourseData *param_1);

void ov96_022098E8(Ov96_0220Data *);
void ov96_022099EC(Ov96_0220Data *);
void ov96_02209CA8(Ov96_0220Data *);
void ov96_0220A0E0(Ov96_0220Data *);
s32 ov96_0220A7F8(void *);
s32 ov96_0220B500(void *);
s32 ov96_0220B8A0(void *);
SysTask *ov96_0220C998(void *);
s32 ov96_022097AC(PokeathlonCourseData *arg0);

u32 ov96_02209748(PokeathlonCourseData *param_1) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 0x126);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 3);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 3);
    return 1;
}

s32 ov96_022097AC(PokeathlonCourseData *data) {
    Ov96_0220Data *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_0220A0E0(v0);
    if (ov96_021E5F24(data) == 0) {
        ov96_0220C998(v0->unk_48);
    }
    ov96_0220A7F8(v0->unk_40);
    ov96_0220B8A0(v0->unk_44);
    ov96_0220B500(v0->unk_4C);
    ov96_022099EC(v0);
    ov96_022098E8(v0);
    ov96_02209CA8(v0);
    sub_0203A914();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_141);
    return 1;
}
