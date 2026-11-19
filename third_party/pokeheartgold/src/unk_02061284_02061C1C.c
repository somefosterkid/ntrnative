#include "global.h"

#include "overworld_anim_manager.h"
#include "sys_task.h"

struct MapObject;

typedef struct MapObject MapObject;

void *sub_0205F394(MapObject *mapObj);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    OverworldAnimManager *unk_04;
} UnkStruct_0206A0BC;

extern int (*const _020FD4EC[])(MapObject *, UnkStruct_0206A0BC *);

void sub_02061C1C(MapObject *mapObj);

void sub_02061C1C(MapObject *mapObj) {
    UnkStruct_0206A0BC *v0 = sub_0205F394(mapObj);

    while (_020FD4EC[v0->unk_00](mapObj, v0) == 1) {
        (void)0;
    }
}
