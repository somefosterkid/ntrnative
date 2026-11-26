#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "system.h"
#include "unk_0203A3B0.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_021E9510(void *arg0);

void ov96_021E952C(void *arg0);

void ov96_021E93B4(void *, u32);

void ov96_021E9570(void *, u8);

void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);

void ov96_0221A56C(void *, u32);

BOOL ov96_02215CE8(PokeathlonCourseData *param_1, int unused);

void ov96_02216390(Ov96RdWork *work);
void ov96_022165FC(Ov96RdWork *work);
void ov96_022177D8(Ov96RdSprites *sprites);
void ov96_02217DBC(Ov96RdWork *work);
void ov96_022195E8(struct Ov96DigitWork *arg0);
void ov96_02219FDC(void *ptr);
BOOL ov96_02215D4C(PokeathlonCourseData *arg0, int unused);
u32 ov96_02215DBC(PokeathlonCourseData *param_1, s32 param_2);

BOOL ov96_02215CE8(PokeathlonCourseData *param_1, int unused) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 0x12a);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 7);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 7);
    return 1;
}

BOOL ov96_02215D4C(PokeathlonCourseData *data, int unused) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_02219FDC(work->unk_184);
    ov96_022195E8(work->unk_180);
    ov96_022177D8(&work->sprites);
    ov96_02217DBC(work);
    ov96_02216390(work);
    ov96_022165FC(work);
    sub_0203A914();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_145);
    return 1;
}

u32 ov96_02215DBC(PokeathlonCourseData *data, s32 idx) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    return work->course.players[idx].obj.state;
}
