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

void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);

void sub_0205F328(MapObject *mapObj, u32 param1);

void *sub_0205F370(MapObject *mapObj, int size);

void MapObject_ClearSingleMovement(MapObject *mapObj);

void MapObject_SetFacingVector(MapObject *mapObj, const VecFx32 *vec);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    OverworldAnimManager *unk_04;
} UnkStruct_0206A0BC;

void sub_020662C4(MapObject *mapObj, int param1);

void sub_020662C4(MapObject *mapObj, int param1) {
    UnkStruct_0206A0BC *v0 = sub_0205F370(mapObj, (sizeof(UnkStruct_0206A0BC)));

    v0->unk_01 = param1;

    sub_0205F328(mapObj, 0x0);
    MapObject_ClearSingleMovement(mapObj);
    MapObject_SetFlagsBits(mapObj, (1 << 20));

    VecFx32 v1 = { 0, ((fx32)0x0000000000001000L) * -32, 0 };

    MapObject_SetFacingVector(mapObj, &v1);
}
