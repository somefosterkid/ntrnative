#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_01_021FB04C.h"

Bdhc *ov01_021FB254(void) {
    Bdhc *bdhc = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Bdhc));

    bdhc->points = 0L;
    bdhc->normals = 0L;
    bdhc->plates = 0L;
    bdhc->strips = 0L;
    bdhc->accessList = 0L;
    bdhc->accessList = 0L;
    bdhc->loaded = 0;
    bdhc->stripsCount = 0;

    return bdhc;
}
