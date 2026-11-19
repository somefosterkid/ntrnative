#include "global.h"

#include "field_types_def.h"
#include "unk_struct_02069_ca8.h"

struct MapObject;

typedef struct MapObject MapObject;

MapObject *MapObjectManager_GetFirstActiveObjectWithMovement1(const MapObjectManager *param0);

const MapObjectManager *MapObject_GetManager(const MapObject *mapObj);

void sub_02065CFC(MapObject *mapObj, UnkStruct_02069CA8 *param1);

int sub_02065CD0(MapObject *mapObj, UnkStruct_02069CA8 *param1);

int sub_02065CD0(MapObject *mapObj, UnkStruct_02069CA8 *param1) {
    const MapObjectManager *v0 = MapObject_GetManager(mapObj);
    MapObject *v1 = MapObjectManager_GetFirstActiveObjectWithMovement1(v0);

    if (v1 == NULL) {
        param1->unk_01 = 0;
        return 0;
    }

    if (param1->unk_01 == 0) {
        sub_02065CFC(mapObj, param1);
    }

    return 1;
}
