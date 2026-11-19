#include "global.h"

#include "field_types_def.h"
#include "overlay_01_map_load_manager.h"

u16 *ov01_021F65E4(MapLoadManager *mapLoadManager, u8 arg1);

u16 *ov01_021F65E4(MapLoadManager *mapLoadManager, u8 arg1) {
    return mapLoadManager->maps[arg1]->permissions;
}

u8 *ov01_021F65F0(MapLoadManager *mapLoadManager, u8 arg1);

u8 *ov01_021F65F0(MapLoadManager *mapLoadManager, u8 arg1) {
    return mapLoadManager->maps[arg1]->permissionBytes;
}
