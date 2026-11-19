#include "global.h"
#include "unk_struct_02069_ca8.h"

struct MapObject;

typedef struct MapObject MapObject;

void *sub_0205F394(MapObject *mapObj);

void MapObject_ClearFlag18(MapObject *mapObj, int param1);

int sub_02065CD0(MapObject *mapObj, UnkStruct_02069CA8 *param1);

extern int (*const _020FE404[])(MapObject *, UnkStruct_02069CA8 *);

int sub_02065BE8(MapObject *mapObj, UnkStruct_02069CA8 *param1);

int sub_02065C2C(MapObject *mapObj, UnkStruct_02069CA8 *param1);

void sub_02065900(MapObject *mapObj);

void sub_02065900(MapObject *mapObj) {
    UnkStruct_02069CA8 *v0 = sub_0205F394(mapObj);

    if (sub_02065CD0(mapObj, v0) == 0) {
        return;
    }

    MapObject_ClearFlag18(mapObj, 0);

    while (_020FE404[v0->unk_00](mapObj, v0) == 1) {
        (void)0;
    }
}
