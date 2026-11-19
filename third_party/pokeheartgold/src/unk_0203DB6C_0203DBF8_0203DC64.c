#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "unk_0203DB6C.h"

u8 sub_0203DBD4(PlayerAvatar *, LocalMapObject *);

BOOL sub_0203DC64(FieldSystem *fieldSystem, LocalMapObject **localMapObject) {
    LocalMapObject *object;

    FieldSystem_GetFacingObject(fieldSystem, localMapObject);
    object = *localMapObject;
    if ((object != NULL) && (MapObject_CheckFlag19Disabled(object) == 1) && (sub_0203DBD4(fieldSystem->playerAvatar, *localMapObject) == 1)) {
        return 1;
    }
    return 0;
}
