#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222DE14(PalPadDetailsSlot *param0, PalPadGfx *param1, u32 param2, u32 param3, const PalPadWindowSpec *param4, enum HeapID heapID);

void ov43_0222DE14(PalPadDetailsSlot *param0, PalPadGfx *param1, u32 param2, u32 param3, const PalPadWindowSpec *param4, enum HeapID heapID) {
    u32 new_var;
    u8 new_var2;
    const PalPadWindowSpec *new_var3;
    u32 i;
    u16 v1;
    int abs0;
    int abs2;
    param0->windows[param2] = AllocWindows(heapID, (u8)param3);
    param0->windowCounts[param2] = param3;
    v1 = (u16)param0->baseTile;
    for (i = 0; i < param3; i++) {
        InitWindow(&param0->windows[param2][i]);
        new_var3 = &param4[i];
        abs2 = param0->layout.y;
        if (0 > abs2) {
            abs2 = -abs2;
        }
        abs0 = param0->layout.x;
        if (0 > abs0) {
            abs0 = -abs0;
        }
        new_var2 = (*new_var3).y;
        AddWindowParameterized(param1->bgConfig, &param0->windows[param2][i], 3, (*new_var3).x + abs0, new_var2 + abs2, (*new_var3).width, (*new_var3).height, 0xb, v1);
        new_var = i;
        FillWindowPixelBuffer(&param0->windows[param2][new_var], 0);
        v1 = (u16)(v1 + ((*new_var3).width * (*new_var3).height));
    }
}
