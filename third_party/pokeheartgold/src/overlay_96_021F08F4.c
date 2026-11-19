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
#include "overlay_96_arena_cpu.h"
#include "overlay_96_ov96_arena_work.h"
#include "overlay_96_ov96_sprite_mgr.h"

void ov96_021E9510(void *arg0);

void ov96_021E952C(void *arg0);

void ov96_021E93B4(void *, u32);

void ov96_021E9570(void *, u8);

void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);

void ov96_0221A56C(void *, u32);

u32 ov96_021F08F4(PokeathlonCourseData *param_1);

s32 ov96_021E6550(PokeathlonCourseData *);
s32 ov96_021E9C0C(void *);
SysTask *ov96_021F30BC(void *);
s32 ov96_021F3C38(void *);
s32 ov96_021F095C(PokeathlonCourseData *arg0);

u32 ov96_021F08F4(PokeathlonCourseData *param_1) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    PokeathlonCourse_GetHeapAllocPtr4(param_1);
    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 0x125);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 2);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 2);
    return 1;
}

s32 ov96_021F095C(PokeathlonCourseData *data) {
    Ov96ArenaWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    sub_0203A914();
    ov96_021E6550(data);
    ov96_021F30BC(work->unk_72C);
    Heap_Free(work->unk_734);
    FreeBgTilemapBuffer(work->bgConfig, 0U);
    FreeBgTilemapBuffer(work->bgConfig, 1U);
    FreeBgTilemapBuffer(work->bgConfig, 2U);
    FreeBgTilemapBuffer(work->bgConfig, 3U);
    FreeBgTilemapBuffer(work->bgConfig, 4U);
    FreeBgTilemapBuffer(work->bgConfig, 5U);
    FreeBgTilemapBuffer(work->bgConfig, 6U);
    FreeBgTilemapBuffer(work->bgConfig, 7U);
    RemoveWindow(&work->window);
    Heap_Free(work->bgConfig);
    ov96_021EB21C(work->unk_18);
    ov96_021EA894(work->spriteSet);
    ov96_021E9C0C(work->resSrc);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov96_021F3424(work->cpu);
    ov96_021F3C38(work->unk_774);
    FontID_Release(FONT_UNK_4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    Heap_Free(work->unk_7F0);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_140);
    return 1;
}
