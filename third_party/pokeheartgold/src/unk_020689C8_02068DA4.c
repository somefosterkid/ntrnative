#include "global.h"

#include "overworld_anim_manager.h"

void sub_02068DA4(OverworldAnimManager *manager, OverworldAnimManagerList *managerList) {
    manager->list = managerList;
}

void sub_02068DA8(OverworldAnimManager *manager, const VecFx32 *position) {
    manager->position = *position;
}

void sub_02068DB8(const OverworldAnimManager *manager, VecFx32 *position) {
    *position = manager->position;
}

BOOL sub_02068DC8(OverworldAnimManager *manager, void *ctx) {
    return TRUE;
}
