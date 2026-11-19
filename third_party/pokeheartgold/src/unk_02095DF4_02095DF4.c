#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"
#include "unk_02095DF4.h"

BOOL sub_02095E30(TaskManager *taskman);

void sub_02095DF4(FieldSystem *fieldSystem, u8 a1, u8 a2, u8 a3, u16 *a4, u16 *a5, u16 *a6, u16 *a7) {
    UnkStruct_02095DF4 *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_02095DF4));
    env->unk00 = 0;
    env->unk08 = a1;
    env->unk0E = a2;
    env->unk0C = a3;
    env->unk10 = a4;
    env->unk14 = a5;
    env->unk18 = a6;
    env->unk1C = a7;
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(sub_02095E30), env);
    return;
}
