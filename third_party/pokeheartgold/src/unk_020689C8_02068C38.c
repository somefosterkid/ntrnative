#include "global.h"

#include "heap.h"
#include "overworld_anim_manager.h"

OverworldAnimManager *sub_02068C38(enum HeapID heapID, int managerCount) {
    OverworldAnimManager *managers = Heap_Alloc(heapID, managerCount * sizeof(OverworldAnimManager));
    GF_ASSERT(managers != NULL);

    memset(managers, 0, managerCount * sizeof(OverworldAnimManager));
    return managers;
}

void sub_02068C5C(OverworldAnimManagerList *managerList) {
    Heap_FreeExplicit(managerList->heapID, managerList->managers);
}
