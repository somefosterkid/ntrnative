#include "global.h"

#include "overworld_anim_manager.h"
#include "sys_task.h"

struct MapObject;

struct SysTask;

struct SysTaskQueue;

typedef struct MapObject MapObject;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void *sub_0205F394(MapObject *mapObj);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    OverworldAnimManager *unk_04;
} UnkStruct_0206A0BC;

extern int (*const _020FE3CC[])(MapObject *, UnkStruct_0206A0BC *);

int sub_020663B4(MapObject *mapObj, UnkStruct_0206A0BC *param1);

int sub_020663E4(MapObject *mapObj, UnkStruct_0206A0BC *param1);

void sub_0206633C(MapObject *mapObj);

void sub_0206633C(MapObject *mapObj) {
    UnkStruct_0206A0BC *v0 = sub_0205F394(mapObj);

    while (_020FE3CC[v0->unk_00](mapObj, v0) == 1) {
        (void)0;
    }
}
