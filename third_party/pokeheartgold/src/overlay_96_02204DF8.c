#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "overlay_96_hurdle_cpu.h"
#include "overlay_96_ov96hurdle_work.h"
#include "overlay_96_ov96_sprite_mgr.h"

void ov96_021E9510(void *arg0);

void ov96_021E952C(void *arg0);

void ov96_021E93B4(void *, u32);

void ov96_021E9570(void *, u8);

void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);

void ov96_0221A56C(void *, u32);

u32 ov96_02204DF8(PokeathlonCourseData *param_1);

s32 ov96_021E9C0C(void *);
s32 ov96_021EA894(void *);
s32 ov96_02207D64(void *);
s32 ov96_02204E58(PokeathlonCourseData *arg0);

u32 ov96_02204DF8(PokeathlonCourseData *param_1) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 0x124);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 1);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 1);
    return 1;
}

s32 ov96_02204E58(PokeathlonCourseData *data) {
    Ov96HurdleWork *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    sub_0203A914();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_ResetField3A4(data);
    Heap_Free(v0->unk_18);
    FreeBgTilemapBuffer(v0->bgConfig, 0U);
    FreeBgTilemapBuffer(v0->bgConfig, 3U);
    FreeBgTilemapBuffer(v0->bgConfig, 4U);
    FreeBgTilemapBuffer(v0->bgConfig, 5U);
    FreeBgTilemapBuffer(v0->bgConfig, 6U);
    RemoveWindow(&v0->window);
    Heap_Free(v0->bgConfig);
    ov96_021EB21C(v0->unk_20);
    ov96_021EA894(v0->unk_348);
    ov96_021E9C0C(v0->unk_344);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov96_02208B2C(v0->cpu);
    ov96_02207D64(v0->hud);
    FontID_Release(FONT_UNK_4);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    *(s16 *)REG_BLDCNT_ADDR = 0;
    Heap_Destroy(HEAP_ID_139);
    return 1;
}
