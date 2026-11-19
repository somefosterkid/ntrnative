#include "global.h"

#include "gf_3d_loader.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

struct FieldSystem;

struct LocalMapObject;

struct MapObjectManager;

struct NARC;

struct SysTask;

struct SysTaskQueue;

int ov01_021F9FCC(MapObjectManager *manager, int spriteId, LocalMapObject *exclude);

int ov01_021F9FCC(MapObjectManager *manager, int spriteId, LocalMapObject *exclude) {
    s32 index = 0;
    u32 new_var;
    LocalMapObject *object = 0L;
    while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == 1) {
        if ((exclude != 0L) && (exclude == object)) {
            continue;
        }
        new_var = MapObject_GetSpriteID(object);
        if (new_var == spriteId) {
            return 1;
        }
    }

    return 0;
}
