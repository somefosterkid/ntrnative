#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system_priv.h"

#include "heap.h"

void HBlankSystem_Delete(HBlankSystem *hBlankSystem) {
    if (hBlankSystem == NULL) {
        return;
    }

    if (hBlankSystem->active == TRUE) {
        HBlankSystem_Stop(hBlankSystem);
    }

    ov01_021FB55C(hBlankSystem);
    Heap_Free(hBlankSystem);
}
