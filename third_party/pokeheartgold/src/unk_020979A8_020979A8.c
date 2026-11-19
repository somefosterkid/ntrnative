#include "global.h"

#include "app_launch_warp_task.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"

s32 LoadAreaOrDungeonLightTxt(s32, ModelAttributes *);

BOOL sub_02097AA0(TaskManager *param_1);

void sub_020979A8(FieldSystem *fieldSystem, u8 a1, u8 a2, u32 a3, u32 a4, u32 a5);

void sub_020979A8(FieldSystem *fieldSystem, u8 a1, u8 a2, u32 a3, u32 a4, u32 a5) {
    s32 sp0;
    AppLaunchWarpTask *env;

    sp0 = (s32)a3;
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(AppLaunchWarpTask));
    env->mode = a1;
    env->direction = a2;
    env->mapId = sp0;
    env->x = (s32)a4;
    env->y = (s32)a5;
    LoadAreaOrDungeonLightTxt(0, &env->lights);
    env->state = 0;
    TaskManager_Call(fieldSystem->taskman, sub_02097AA0, env);
}
