#include "global.h"

#include "overworld_anim_manager.h"

void sub_02068CD8(OverworldAnimManager *manager) {
    manager->flags |= OVERWORLD_ANIM_MANAGER_ACTIVE_FLAG;
}

void sub_02068CE4(OverworldAnimManager *manager, u32 mask) {
    manager->flags |= mask;
}
