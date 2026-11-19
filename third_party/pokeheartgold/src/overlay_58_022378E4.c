#include "global.h"

#include "heap.h"
#include "overlay_58.h"
#include "overlay_manager.h"

void ov58_02237B40(Ov58ApricornBoxWork *work);

s32 ov58_02237CCC(void);

BOOL ApricornBox_Init(OverlayManager *man, int *state);

BOOL ApricornBox_Init(OverlayManager *man, int *state) {
    Ov58ApricornBoxWork *dest;

    ov58_02237CCC();
    Heap_Create(HEAP_ID_3, HEAP_ID_133, 0x2000U);
    dest = OverlayManager_CreateAndGetData(man, sizeof(Ov58ApricornBoxWork), HEAP_ID_133);
    MI_CpuFill8(dest, 0U, sizeof(Ov58ApricornBoxWork));
    dest->heapId = HEAP_ID_133;
    dest->args = OverlayManager_GetArgs(man);
    ov58_02237B40(dest);
    return 1;
}
