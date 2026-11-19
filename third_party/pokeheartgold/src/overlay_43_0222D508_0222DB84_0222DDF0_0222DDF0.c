#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"

void ov43_0222DDF0(PalPadDetailsSlot *slot, PalPadGfx *gfx, const PalPadPanelLayout *layout, u32 baseTile, enum HeapID heapID);
void ov43_0222DE10(void);

void ov43_0222DDF0(PalPadDetailsSlot *slot, PalPadGfx *gfx, const PalPadPanelLayout *layout, u32 baseTile, enum HeapID heapID) {
    int v0;

    slot->layout = *layout;
    slot->baseTile = baseTile;

    for (v0 = 0; v0 < 8; v0++) {
        slot->windows[v0] = NULL;
        slot->windowCounts[v0] = 0;
    }
}

void ov43_0222DE10(void) {
}
