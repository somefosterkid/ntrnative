#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "overlay_96_ov96_02203468work.h"

typedef enum HeapID HeapID;

extern u8 ov96_0221C7B0[];

extern u8 ov96_0221C7EC[];

void ov96_02203310(Ov96_02203468Work *param_1);

void ov96_02203310(Ov96_02203468Work *param_1) {
    WindowTemplate *template;
    Window *window;
    s32 iVar1;

    AddWindow(param_1->bgConfig, &param_1->windows[0], (WindowTemplate *)ov96_0221C7B0);
    BG_FillCharDataRange(param_1->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    FillWindowPixelBuffer(&param_1->windows[0], 0);
    template = (WindowTemplate *)ov96_0221C7EC;
    iVar1 = 0;
    window = &param_1->windows[1];
    do {
        AddWindow(param_1->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        iVar1 = iVar1 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar1 < 3);
    BG_FillCharDataRange(param_1->bgConfig, GF_BG_LYR_MAIN_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, param_1->heapID);
    return;
}
