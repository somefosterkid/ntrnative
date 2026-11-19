#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

s32 sub_0205FD98(LocalMapObject *arg0);

s32 sub_0205FD98(LocalMapObject *object) {
    u32 v0;

    if (MapObject_CheckSingleMovement(object) == 1) {
        return 1;
    }
    if (MapObject_GetFlagsBitsMask(object, (MapObjectFlagBits)(0x1800)) == 0) {
        return 1;
    }
    if (MapObject_GetMovement(object) == 0x32) {
        return 1;
    }
    if (MapObject_GetMovement(object) == 0x30) {
        return 1;
    }
    v0 = MapObject_GetFlags(object);
    if ((v0 & 0x1000) && !(0x800000 & v0)) {
        return 0;
    }
    if ((v0 & 0x800) && (sub_0205F8D0(object) == 0)) {
        return 0;
    }
    return 1;
}
