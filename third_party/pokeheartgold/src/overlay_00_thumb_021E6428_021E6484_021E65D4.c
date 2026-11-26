#include "global.h"

#include "overlay_00_nintendo_wfcmanager.h"

#include "unk_02037C94.h"

extern NintendoWFCManager *_0221A680;

void *ov00_021E65D4(u32 param_1, u32 param_2, s32 param_3);

void *ov00_021E65D4(u32 param_1, u32 param_2, s32 param_3) {
    OSIntrMode state;
    void *pvVar1;

    state = OS_DisableInterrupts();
    pvVar1 = NNS_FndAllocFromExpHeapEx(_0221A680->heap1, param_2, param_3);
    if ((pvVar1 == NULL) && (_0221A680->heap2AllocPtr != NULL)) {
        pvVar1 = NNS_FndAllocFromExpHeapEx(_0221A680->heap2, param_2, param_3);
    }
    if (pvVar1 == NULL) {
        sub_02039AD8(1);
        OS_RestoreInterrupts(state);
        return NULL;
    }
    OS_RestoreInterrupts(state);
    return pvVar1;
}

void ov00_021E6638(u32 param_1, void *param_2);

void ov00_021E6638(u32 param_1, void *param_2) {
    u16 uVar1;
    OSIntrMode state;

    if (param_2 != NULL) {
        state = OS_DisableInterrupts();
        uVar1 = NNS_FndGetGroupIDForMBlockExpHeap(param_2);
        if (uVar1 == 0x10) {
            if (_0221A680->heap2AllocPtr == NULL) {
                sub_02039AD8(1);
                return;
            }
            NNS_FndFreeToExpHeap(_0221A680->heap2, param_2);
        } else {
            NNS_FndFreeToExpHeap(_0221A680->heap1, param_2);
        }
        OS_RestoreInterrupts(state);
    }
    return;
}
