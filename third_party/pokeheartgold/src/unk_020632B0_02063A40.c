#include "global.h"

#include "map_object.h"

extern void (*const _020FE104[])(LocalMapObject *);

void sub_02063A40(LocalMapObject *mapObj);

extern s32 (*const _020FE134[])(LocalMapObject *);
s32 sub_02063A5C(LocalMapObject *mapObj);

void sub_02063A40(LocalMapObject *mapObj) {
    int v0 = MapObject_GetType(mapObj);

    _020FE104[v0](mapObj);
}

s32 sub_02063A5C(LocalMapObject *mapObj) {
    int v0 = MapObject_GetType(mapObj);

    return _020FE134[v0](mapObj);
}
