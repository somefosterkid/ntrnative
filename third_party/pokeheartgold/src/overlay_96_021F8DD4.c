#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"

void ov96_021F8D98(enum HeapID *param_1, Window *param_2, u16 param_3);

void ov96_021F8DD4(enum HeapID *param_1);

void ov96_021F8DD4(enum HeapID *param_1) {
    ov96_021F8D98(param_1, (Window *)(param_1 + 0xbf), 8);
    ov96_021F8D98(param_1, (Window *)(param_1 + 0xc5), 0x18);
    return;
}
