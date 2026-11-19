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

void sub_02066420(MapObject *param0, OverworldAnimManager *param1);

void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);

void *sub_0205F394(MapObject *mapObj);

void MapObject_SetFacingVector(MapObject *mapObj, const VecFx32 *vec);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    OverworldAnimManager *unk_04;
} UnkStruct_0206A0BC;

void sub_02066370(MapObject *mapObj);

void sub_02066370(MapObject *mapObj) {
    UnkStruct_0206A0BC *v0 = sub_0205F394(mapObj);

    v0->unk_00 = 0;
    sub_02066420(mapObj, 0L);

    if (v0->unk_02 == 0) {
        VecFx32 v1 = { 0, (((fx32)0x0000000000001000L) * -32), 0 };

        MapObject_SetFacingVector(mapObj, &v1);
        MapObject_SetFlagsBits(mapObj, (1 << 20));
    }
}
