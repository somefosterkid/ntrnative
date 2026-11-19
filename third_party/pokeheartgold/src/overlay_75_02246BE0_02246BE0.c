#include "global.h"

#include <dwc.h>

#include "overlay_75.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"

void ov75_02246BE0(UnkStruct_ov98_02246E88 *arg0, s8 arg1);

void ov75_02246BE0(UnkStruct_ov98_02246E88 *arg0, s8 arg1) {
    arg0->unk_7E = arg1;
}

u8 ov75_02246BE8(UnkStruct_ov98_02246E88 *arg0) {
    return arg0->unk_7E;
}

extern NNSFndHeapHandle _02249BE0;

void *ov75_02246BF0(DWCAllocType param_1, u32 param_2, int param_3);

void *ov75_02246BF0(DWCAllocType param_1, u32 param_2, int param_3) {
    OSIntrMode state;
    void *pvVar1;

    state = OS_DisableInterrupts();
    pvVar1 = NNS_FndAllocFromExpHeapEx(_02249BE0, param_2, param_3);
    OS_RestoreInterrupts(state);
    return pvVar1;
}

void ov75_02246C18(DWCAllocType param_1, void *param_2, u32 param_3);

void ov75_02246C18(DWCAllocType param_1, void *param_2, u32 param_3) {
    OSIntrMode state;

    if (param_2 != NULL) {
        state = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(_02249BE0, param_2);
        OS_RestoreInterrupts(state);
    }
    return;
}
