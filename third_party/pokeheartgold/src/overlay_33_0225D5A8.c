#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_33.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_manager.h"

void ov33_0225D7B8(void *);

void ov33_0225D820(void *param_1);

struct OverlayManager;
BOOL ov33_0225D5CC(OverlayManager *appMan, int *state);

void ov33_0225D5A8(u32 param_1, SysTask *param_2) {
    void **puVar1;

    puVar1 = SysTask_GetData(param_2);
    ov33_0225D820(puVar1);
    ov33_0225D7B8(*puVar1);
    DestroySysTaskAndEnvironment(param_2);
    Heap_Destroy(HEAP_ID_8);
    return;
}

BOOL ov33_0225D5CC(OverlayManager *appMan, int *state) {
    return 1;
}
