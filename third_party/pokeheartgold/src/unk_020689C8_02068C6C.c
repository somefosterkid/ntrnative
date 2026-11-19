#include "global.h"

#include "overworld_anim_manager.h"

void sub_02068C6C(OverworldAnimManager *manager) {
    memset(manager, 0, sizeof(OverworldAnimManager));
    sub_02068CF4(manager, sub_02068DC8);
    sub_02068D48(manager, sub_02068DD4);
    sub_02068D10(manager, sub_02068DCC);
    sub_02068D2C(manager, sub_02068DD0);
    sub_02068D64(manager, sub_02068DD8);
    sub_02068D6C(manager, sub_02068DDC);
}

int sub_02068CC4(const OverworldAnimManagerList *managerList) {
    return managerList->count;
}

OverworldAnimManager *sub_02068CC8(const OverworldAnimManagerList *managerList) {
    return managerList->managers;
}

BOOL sub_02068CCC(const OverworldAnimManager *manager) {
    return manager->flags & OVERWORLD_ANIM_MANAGER_ACTIVE_FLAG ? TRUE : FALSE;
}
