#include "global.h"

#include "heap.h"
#include "overworld_anim_manager.h"

OverworldAnimManagerList *sub_02068C08(enum HeapID heapID) {
    OverworldAnimManagerList *managerList = Heap_Alloc(heapID, sizeof(OverworldAnimManagerList));
    GF_ASSERT(managerList != NULL);

    memset(managerList, 0, sizeof(OverworldAnimManagerList));
    return managerList;
}

void sub_02068C2C(OverworldAnimManagerList *managerList) {
    Heap_FreeExplicit(managerList->heapID, managerList);
}
