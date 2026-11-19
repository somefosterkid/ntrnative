#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

typedef enum HeapID HeapID;

extern u8 ov96_0221C1CC[];

void ov96_021F7050(UnkStruct_ov96_021F5018 *param_1);

void ov96_021F7050(UnkStruct_ov96_021F5018 *param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        AddWindow(param_1->bgConfig, &param_1->windows[uVar1], (WindowTemplate *)(uVar1 * 8 + ((u8 *)ov96_0221C1CC)));
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 5);
    BG_FillCharDataRange(param_1->bgConfig, GF_BG_LYR_SUB_2, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, param_1->heapID);
    BG_FillCharDataRange(param_1->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, param_1->heapID);
    return;
}
