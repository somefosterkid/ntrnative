#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "heap.h"
#include "overlay_01_021FCE98.h"
#include "task.h"

s32 ov01_021FD014(TaskManager *arg0);

s32 ov01_021FD014(TaskManager *arg0) {
    u32 *temp_r0;
    u32 temp_r0_2;
    SweetScentEffect *temp_r5;

    temp_r5 = TaskManager_GetEnvironment(arg0);
    temp_r0 = TaskManager_GetStatePtr(arg0);
    temp_r0_2 = *temp_r0;
    switch (temp_r0_2) {
    case 0:
        if (ov01_021FD154(temp_r5->anims, 3) != 0) {
            *temp_r0 += 1;
        }
        Field3dObject_Draw(&temp_r5->object);
        break;
    case 1:
        ov01_021FD128(temp_r5);
        Heap_Free(temp_r5);
        return 1;
    }
    return 0;
}
