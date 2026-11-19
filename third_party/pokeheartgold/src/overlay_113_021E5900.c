#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "overlay_113.h"

s32 ov113_021E59F8(OverlayManager *);

s32 ov113_021E5A7C(UnownReportData *);

s32 ov113_021E5B70(UnownReportData *);

BOOL UnownReport_Init(OverlayManager *man, int *state);

BOOL UnownReport_Init(OverlayManager *man, int *state) {
    s32 temp_r1;
    UnownReportData *temp_r0;

    temp_r1 = *state;
    switch (temp_r1) {
    case 0:
        ov113_021E59F8(man);
        Heap_Create(HEAP_ID_3, HEAP_ID_152, 0x20000U);
        temp_r0 = OverlayManager_CreateAndGetData(man, sizeof(UnownReportData), HEAP_ID_152);
        MI_CpuFill8(temp_r0, 0U, sizeof(UnownReportData));
        temp_r0->heapID = HEAP_ID_152;
        temp_r0->args = OverlayManager_GetArgs(man);
        ov113_021E5A7C(temp_r0);
        *state += 1;
    default:
        break;
    case 1:
        if (ov113_021E5B70(OverlayManager_GetData(man)) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
