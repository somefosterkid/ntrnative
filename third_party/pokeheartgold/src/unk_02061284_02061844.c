#include "global.h"

#include "field_types_def.h"
#include "unk_02061284.h"

void sub_020616C0(LocalMapObject *param_1, u8 param_2);

struct MapObject;
typedef struct MapObject MapObject;
void *sub_0205F394(MapObject *mapObj);
typedef struct {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    s8 unk_03;
    int unk_04;
} UnkStruct_0206496C;
extern int (*const _020FD5A0[])(MapObject *, UnkStruct_0206496C *);
void sub_02061850(MapObject *mapObj);

void sub_02061844(LocalMapObject *param_1) {
    sub_020616C0(param_1, 3);
    return;
}

void sub_02061850(MapObject *mapObj) {
    UnkStruct_0206496C *v0 = sub_0205F394(mapObj);

    while (_020FD5A0[v0->unk_02](mapObj, v0) == 1) {
        (void)0;
    }
}
