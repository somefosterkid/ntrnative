#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_01.h"

void ov01_021F434C(SaveInfoWindow *a0) {
    RemoveWindow(a0->window);
    Heap_Free(a0->window);
}
