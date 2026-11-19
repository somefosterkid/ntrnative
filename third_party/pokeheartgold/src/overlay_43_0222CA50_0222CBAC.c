#include "global.h"

#include "overlay_43_palpad.h"

void ov43_0222CD3C(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

void ov43_0222CBAC(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

void ov43_0222CBAC(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    ov43_0222CD3C(naming, shared, gfx, heapId);
}
