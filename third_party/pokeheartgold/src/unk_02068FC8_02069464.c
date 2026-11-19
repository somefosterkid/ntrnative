#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"
#include "unk_02068FC8.h"
#include "launch_application.h"
#include "union_room_card_task.h"
#include "union_room_manager.h"

s32 sub_02069498(TaskManager *arg0);

void sub_02069464(FieldSystem *fieldSystem) {
    UnionRoomCardViewTask *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnionRoomCardViewTask));
    env->state = 0;
    env->card = sub_0205ABD8(fieldSystem->unk80);
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(sub_02069498), env);
    return;
}

s32 sub_02069498(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    s32 v0;
    UnionRoomCardViewTask *env;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    env = TaskManager_GetEnvironment(taskManager);
    v0 = env->state;
    switch (v0) {
    case 0:
        sub_0205AC70(fieldSystem->unk80);
        env->state = 1;
    case 1:
        if (env->card->unk_66A != 0) {
            env->state = 0xA;
        }
    default:
        break;
    case 10:
        MI_CpuCopy8(env->card, &env->args, sizeof(TrainerCase));
        TrainerCard_LaunchApp(fieldSystem, &env->args);
        env->state = 0xB;
        break;
    case 11:
        if (FieldSystem_ApplicationIsRunning(fieldSystem) == 0) {
            sub_0205AC4C(fieldSystem->unk80);
            Heap_Free(env);
            return 1;
        }
        break;
    }
    return 0;
}
