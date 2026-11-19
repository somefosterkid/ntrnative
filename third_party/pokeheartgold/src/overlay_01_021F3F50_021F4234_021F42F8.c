#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_01.h"

void ov01_021F426C(SaveInfoWindow *param_1);

void ov01_021F42F8(SaveInfoWindow *a0) {
    void *pvVar1;

    pvVar1 = Heap_Alloc(a0->heapID, sizeof(Window));
    a0->window = pvVar1;
    AddWindowParameterized(a0->bgConfig, a0->window, a0->bgLayer, 7, 2, (u8)a0->width, (u8)a0->height, 4, 0x10b);
    FillWindowPixelBuffer(a0->window, 0);
    ov01_021F426C(a0);
    CopyWindowToVram(a0->window);
    return;
}
