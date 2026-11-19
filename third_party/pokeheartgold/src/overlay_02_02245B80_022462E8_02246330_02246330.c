#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"
#include "task.h"
#include "unk_02005D10.h"

BOOL ov02_02246330(TaskManager *param_1);

BOOL ov02_02246330(TaskManager *param_1) {
    u8 cVar1;
    LegendFadeTask *ptr;
    s32 iVar2;
    BOOL BVar3;
    u8 bVar4;
    ptr = TaskManager_GetEnvironment(param_1);
    cVar1 = ptr->state;
    switch (cVar1) {
    case 0:
        ov02_02246398(ptr);
        PlaySE(0x87d);
        ptr->state = ptr->state + 1;
        break;
    case 1:
        iVar2 = ov02_02246490(ptr);
        if (iVar2 != 0) {
            bVar4 = ptr->count;
            ptr->count = bVar4 + 1;
            if (bVar4 >= 1) {
                ptr->state = ptr->state + 1;
            }
        }
        break;
    case 2:
        BVar3 = IsSEPlaying(0x87d);
        if (BVar3 == 0) {
            ov02_02246444(ptr);
            Heap_Free(ptr);
            return 1;
        }
    }
    return 0;
}
