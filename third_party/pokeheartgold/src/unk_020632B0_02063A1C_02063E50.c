#include "global.h"

#include "error_handling.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "unk_02055418.h"

void sub_02063E50(LocalMapObject *arg0);

void sub_02063E50(LocalMapObject *object) {
    MapObjectBillboardWorkD *v0;

    v0 = (MapObjectBillboardWorkD *)sub_0205F3E8(object, sizeof(MapObjectBillboardWorkD));
    v0->spriteId = 0x106;
    v0->unk_04 = 0;
    sub_0205F484(object);
}

s32 ov01_021F9344(LocalMapObject *);

void ov01_021F95CC(const LocalMapObject *, Billboard **, int);

s32 ov01_021FA40C(LocalMapObject *, Billboard *);

void *ov01_021FE66C(LocalMapObject *);

s32 sub_02023EE0(Billboard *, s32);

s32 sub_02023EF4(Billboard *);

s32 sub_02023F04(Billboard *, s32);

s32 sub_02023F40(Billboard *, s32);

u32 sub_020640A4(u32, s32);

void sub_02063E70(LocalMapObject *arg0);

void sub_02063E70(LocalMapObject *object) {
    Billboard *sprite;
    s32 v2;
    u32 v0;
    MapObjectBillboardWorkD *v1;
    u8 *v3;

    v3 = sub_0205F394(object);
    v1 = (MapObjectBillboardWorkD *)sub_0205F40C(object);
    v2 = sub_02055780(MapObject_GetFieldSystem(object), object);
    if (ov01_021FA2D4(object) == 1) {
        return;
    }
    if (v1->billboard == NULL) {
        v1->spriteId = sub_020640A4(FieldSystem_ApricornTree_TryGetApricorn(MapObject_GetFieldSystem(object), object), v2);
        MapObject_SetSpriteID(object, v1->spriteId);
        ov01_021F94C0(object, &v1->billboard, v1->spriteId);
    } else if (v2 != v1->unk_04) {
        ov01_021F95CC(object, &v1->billboard, v1->spriteId);
        v1->spriteId = sub_020640A4(FieldSystem_ApricornTree_TryGetApricorn(MapObject_GetFieldSystem(object), object), v2);
        MapObject_SetSpriteID(object, v1->spriteId);
        if (v1->spriteId != 0xFFFF) {
            if (v2 == 1) {
                ov01_021FE66C(object);
            }
            ov01_021F94C0(object, &v1->billboard, v1->spriteId);
        } else {
            GF_AssertFail();
        }
        *(s16 *)((u8 *)v3 + 2) = 0;
    }
    v1->unk_04 = v2;
    if (ov01_021FA2D4(object) != 1) {
        sprite = v1->billboard;
        if (sprite != NULL) {
            ov01_021FA3E8(object, sprite);
            if (ov01_021F9344(object) == 0) {
                v0 = sub_0205F330(object);
                switch (v0) {
                case 0:
                    if (sub_02023EF4(v1->billboard) != 0) {
                        sub_02023EE0(v1->billboard, 0);
                        sub_02023F40(v1->billboard, 0);
                    }
                    break;
                case 1:
                    if (sub_02023EF4(v1->billboard) != 1) {
                        sub_02023EE0(v1->billboard, 1);
                        sub_02023F40(v1->billboard, 0);
                    }
                    break;
                case 2:
                    if (sub_02023EF4(v1->billboard) != 2) {
                        sub_02023EE0(v1->billboard, 2);
                        sub_02023F40(v1->billboard, 0);
                    }
                    break;
                default:
                    GF_AssertFail();
                    if (sub_02023EF4(v1->billboard) != 0) {
                        sub_02023EE0(v1->billboard, 0);
                        sub_02023F40(v1->billboard, 0);
                    }
                    break;
                }
                sub_02023F04(v1->billboard, 0x1000);
            }
            ov01_021FA40C(object, v1->billboard);
        }
    }
}
