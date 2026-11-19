#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "launch_application.h"
#include "overlay_104_105_args.h"
#include "task.h"

BOOL sub_02097A48(TaskManager *param_1);

BOOL sub_02097A48(TaskManager *param_1) {
    FieldSystem *fieldSystem;
    Ov105AppArgs *args;
    u32 *puVar1;
    u32 uVar2;
    BOOL BVar3;

    fieldSystem = TaskManager_GetFieldSystem(param_1);
    args = TaskManager_GetEnvironment(param_1);
    puVar1 = TaskManager_GetStatePtr(param_1);
    uVar2 = *puVar1;
    switch (uVar2) {
    case 0:
        sub_0203FC90(fieldSystem, args);
        break;
    case 1:
        BVar3 = FieldSystem_ApplicationIsRunning(fieldSystem);
        if (BVar3 != 0) {
            return 0;
        }
        break;
    case 2:
        Heap_Free(args);
        return 1;
    }
    *puVar1 = *puVar1 + 1;
    return 0;
}
