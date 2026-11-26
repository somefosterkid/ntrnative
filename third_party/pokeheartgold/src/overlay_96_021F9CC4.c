#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "system.h"
#include "unk_0203A3B0.h"

#include "overlay_96_ov96_icon_work.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"
#include "overlay_96_ov96_sprite_mgr.h"

s32 ov96_021E93B4(void *, s32);

s32 ov96_021E9510(void *arg0);

s32 ov96_021E952C(void *);

s32 ov96_021E9570(void *, u8);

s32 ov96_021E966C(PokeathlonCourseData *, void *, u32, s32, s32, s32, u8, u8, u8, u8, s32);

s32 ov96_0221A56C(void *, s32);

BOOL ov96_021F9CC4(PokeathlonCourseData *arg0, int unused);

s32 ov96_021E92D0();
s32 ov96_021E9C0C(void *);
s32 ov96_021EA894(void *);
s32 ov96_021FC05C(s32 *);
SysTask *ov96_021FC690(void *);
BOOL ov96_021F9D58(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021F9CC4(PokeathlonCourseData *data, int unused) {
    void *v0;
    s32 v1;
    u32 v2;
    PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = PokeathlonCourse_GetGraphicsSystem(data);
    ov96_021E9510(v0);
    v2 = 0;
    do {
        v1 = (s32)PokeathlonCourse_GetField5F0_AtIndex(data, (s32)(v2 & 0xFFu));
        ov96_021E966C(data, v0, v2, (u16)v1, v1 / 30, ((s32)(0xA * (v1 % 30))) / 30, 0, 1, 3, 1, 0x123);
        v2 += 1;
    } while (((s32)v2) < 4);
    ov96_021E93B4(v0, 2);
    ov96_0221A56C(v0, (s32)0);
    ov96_021E952C(v0);
    ov96_021E9570(v0, 0);
    return 1;
}

BOOL ov96_021F9D58(PokeathlonCourseData *data, int unused) {
    UnkStruct_ov96_021F94A8 *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    sub_0203A914();
    ov96_021E87B0(PokeathlonCourse_GetSystem(data), 0);
    ov96_021FC1A8(v0->unk_228);
    PokeathlonCourse_ResetField3A4(data);
    ov96_021FC690(v0->unk_DC);
    ov96_021FC05C(v0->unk_D8);
    FreeBgTilemapBuffer(v0->bgConfig, 0U);
    FreeBgTilemapBuffer(v0->bgConfig, 1U);
    FreeBgTilemapBuffer(v0->bgConfig, 2U);
    FreeBgTilemapBuffer(v0->bgConfig, 3U);
    FreeBgTilemapBuffer(v0->bgConfig, 4U);
    FreeBgTilemapBuffer(v0->bgConfig, 5U);
    FreeBgTilemapBuffer(v0->bgConfig, 6U);
    FreeBgTilemapBuffer(v0->bgConfig, 7U);
    RemoveWindow(&v0->window);
    Heap_Free(v0->bgConfig);
    ov96_021EA894(v0->unk_3A4);
    ov96_021E9C0C(v0->unk_3A0);
    ov96_021EB21C(v0->unk_224);
    ov96_021E92D0();
    FontID_Release(FONT_UNK_4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_138);
    return 1;
}
