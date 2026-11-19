#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "overlay_01.h"
#include "task.h"

BOOL ov01_02205DB4(TaskManager *param_1);

BOOL ov01_02205D68(FieldSystem *fieldSystem) {
    s32 *temp_r0;

    if (FollowMon_IsActive(fieldSystem) == 0) {
        return 0;
    }
    if (ov01_022057C4(fieldSystem) != 0) {
        sub_0206A054(fieldSystem);
        ov01_02205790(fieldSystem, 0U);
        return 0;
    }
    temp_r0 = Heap_AllocAtEnd(HEAP_ID_FIELD2, 0x44U);
    *temp_r0 = 0;
    TaskManager_Call(fieldSystem->taskman, ov01_02205DB4, temp_r0);
    return 1;
}
