#include "global.h"

#include "constants/heap.h"

#include "field/field_3d_object_task.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "math_util.h"
#include "overlay_02_0224D0C8.h"
#include "player_avatar.h"
#include "unk_02005D10.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void Field3dObject_SetXRotation(Field3dObject *, s16, u32);
#else
void Field3dObject_SetXRotation(Field3dObject *, u16, u32);
#endif

void ov02_0224D0AC(PlayerAvatar *, Field3dObject *);

extern u8 ov02_022538EC;

void ov02_0224D5B4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *taskData) {
    Field3dEffectData16 *data = taskData;
    u16 uVar3;
    u32 uVar4;
    u16 auStack_1a[5];

    ((u8 *)auStack_1a)[1] = ((u8 *)&ov02_022538EC)[0];
    ((u8 *)auStack_1a)[0] = ((u8 *)&ov02_022538EC)[1];
    auStack_1a[1] = ((u16 *)&ov02_022538EC)[3];
    auStack_1a[2] = ((u16 *)&ov02_022538EC)[4];
    auStack_1a[3] = ((u16 *)&ov02_022538EC)[5];
    auStack_1a[4] = ((u16 *)&ov02_022538EC)[6];
    memset(data, 0, sizeof(Field3dEffectData16));
    HeapExp_FndInitAllocator(&data->allocator, HEAP_ID_FIELD1, 0x20);
    uVar4 = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    ov02_0224D0C8(&data->model, ((u8 *)auStack_1a)[1], ((u8 *)auStack_1a)[0], 1, &data->allocator);
    ov02_0224D1AC(&data->model);
    ov02_0224D0AC(fieldSystem->playerAvatar, &data->model.object);
    uVar3 = GF_DegreeToSinCosIdxNoWrap(auStack_1a[uVar4 + 1]);
    Field3dObject_SetXRotation(&data->model.object, uVar3, 1);
    PlaySE(0x64c);
    data->state = 0;
    return;
}
