#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"
#include "task.h"
#include "unk_02005D10.h"
#include "constants/heap.h"

s32 ov02_02246818(TaskManager *arg0);

s32 ov02_02246818(TaskManager *arg0) {
    AlphUnownTextTask *env;
    env = TaskManager_GetEnvironment(arg0);
    int st = env->state;
    switch (st) {
    case 0:
        ov02_0224686C(env);
        PlaySE(0x5DCU);
        env->state = (u8)(env->state + 1);
        break;

    case 1:
        if (ov02_022469A0() != 0) {
            PlaySE(0x5DCU);
            env->state = (u8)(env->state + 1);
            ov02_02246964(env);
            Heap_Free(env);
            return 1;
        }
        break;
    }

    return 0;
}
