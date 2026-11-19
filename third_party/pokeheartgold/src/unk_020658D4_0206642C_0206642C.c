#include "global.h"
#include "overworld_anim_manager.h"

#include "field_types_def.h"
#include "map_object.h"

OverworldAnimManager *sub_0206642C(LocalMapObject *param_1);

OverworldAnimManager *sub_0206642C(LocalMapObject *param_1) {
    u8 *puVar1;

    puVar1 = sub_0205F394(param_1);
    return *(OverworldAnimManager **)(puVar1 + 4);
}

void sub_02066438(LocalMapObject *param_1);

void sub_02066438(LocalMapObject *param_1) {
    u8 *puVar1;

    puVar1 = sub_0205F394(param_1);
    puVar1[2] = 1;
    return;
}
