#include "global.h"

#include "overworld_anim_manager.h"

OverworldAnimManagerList *sub_020689C8(enum HeapID heapID, int managerCount) {
    OverworldAnimManagerList *managerList = sub_02068C08(heapID);
    managerList->managers = sub_02068C38(heapID, managerCount);
    managerList->count = managerCount;
    managerList->heapID = heapID;
    return managerList;
}

static void sub_020689E8(OverworldAnimManagerList *managerList) {
    sub_02068C5C(managerList);
    sub_02068C2C(managerList);
}

void sub_020689F8(OverworldAnimManagerList *managerList) {
    sub_02068B80(managerList);
    sub_020689E8(managerList);
}
