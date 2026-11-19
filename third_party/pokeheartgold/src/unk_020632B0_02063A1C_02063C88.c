#include "global.h"

#include "map_object.h"

void sub_02063C88(LocalMapObject *arg0);

void sub_02063C88(LocalMapObject *object) {
    s8 v1;
    u8 *v0;

    v0 = sub_0205F398(object, 9);
    *(s8 *)((u8 *)v0 + 3) = (s8)MapObject_GetParam(object, 1U);
    if (MapObject_GetType(object) != 5) {
        v1 = 1;
    } else {
        v1 = 0;
    }
    *(s8 *)((u8 *)v0 + 5) = v1;
}
