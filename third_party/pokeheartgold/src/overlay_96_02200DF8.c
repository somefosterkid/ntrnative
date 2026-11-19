#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "overlay_96_ov96_mon_icon_work.h"

extern WindowTemplate ov96_0221C760;

extern WindowTemplate ov96_0221C768;

void ov96_02200DF8(Ov96MonIconWork *work) {
    AddWindow(work->bgConfig, &work->windows[0], &ov96_0221C760);
    AddWindow(work->bgConfig, &work->windows[1], &ov96_0221C768);
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, work->heapID);
}

void *ov96_02200E3C(enum HeapID param_1, s8 param_2);

void *ov96_02200E3C(enum HeapID param_1, s8 param_2) {
    void *dest;

    dest = Heap_Alloc(param_1, 1000);
    MI_CpuFill8(dest, 0, 1000);
    *(s8 *)((u8 *)dest + 0x3e0) = param_2;
    *(s8 *)((u8 *)dest + 0x3e5) = 4 - param_2;
    *(u8 *)((u8 *)dest + 0x3e4) = *(u8 *)((u8 *)dest + 0x3e5);
    *(u8 *)((u8 *)dest + 0x3e3) = 10;
    *(u8 *)((u8 *)dest + 0x3e2) = *(u8 *)((u8 *)dest + 0x3e3);
    return dest;
}

void ov96_02200E78(void *param_1, s32 param_2, void *param_3);

void ov96_02200E78(void *param_1, s32 param_2, void *param_3) {
    *(void **)(param_1 + param_2 * 0x10) = param_3;
    return;
}
