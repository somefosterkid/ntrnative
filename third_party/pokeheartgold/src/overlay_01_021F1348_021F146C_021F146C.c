#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"

FieldEffectManager *ov01_021F146C(LocalMapObject *mapObject) {
    return MapObject_GetFieldSystem(mapObject)->fieldEffectManager;
}
