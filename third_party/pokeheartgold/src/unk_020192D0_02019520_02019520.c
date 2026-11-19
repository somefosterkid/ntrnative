#include "global.h"

#include <dwc.h>

extern NNSFndHeapHandle _021D1108;

void *sub_02019520(DWCAllocType param_1, u32 param_2, int param_3);

void *sub_02019520(DWCAllocType param_1, u32 param_2, int param_3) {
    OSIntrMode state;
    void *pvVar1;

    state = OS_DisableInterrupts();
    pvVar1 = NNS_FndAllocFromExpHeapEx(_021D1108, param_2, param_3);
    OS_RestoreInterrupts(state);
    return pvVar1;
}

void sub_02019548(DWCAllocType param_1, void *param_2, u32 param_3);

void sub_02019548(DWCAllocType param_1, void *param_2, u32 param_3) {
    OSIntrMode state;

    if (param_2 != NULL) {
        state = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(_021D1108, param_2);
        OS_RestoreInterrupts(state);
    }
    return;
}
