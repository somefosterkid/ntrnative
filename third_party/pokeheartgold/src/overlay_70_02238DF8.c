#include "global.h"

#include <dwc.h>
#include "overlay_70_gtsapplication_state.h"
struct GTSApplicationState;

extern NNSFndHeapHandle ov70_02246944;

void *ov70_02238DF8(DWCAllocType param_1, u32 param_2, int param_3);

int ov70_02238E44(void);

void ov70_02238E50(struct GTSApplicationState *arg0, s32 arg1, s32 arg2);
void ov70_02238E58(struct GTSApplicationState *arg0);

void *ov70_02238DF8(DWCAllocType param_1, u32 param_2, int param_3) {
    OSIntrMode state;
    void *pvVar1;

    state = OS_DisableInterrupts();
    pvVar1 = NNS_FndAllocFromExpHeapEx(ov70_02246944, param_2, param_3);
    OS_RestoreInterrupts(state);
    return pvVar1;
}

void ov70_02238E20(DWCAllocType param_1, void *param_2, u32 param_3);

void ov70_02238E20(DWCAllocType param_1, void *param_2, u32 param_3) {
    OSIntrMode state;

    if (param_2 != NULL) {
        state = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(ov70_02246944, param_2);
        OS_RestoreInterrupts(state);
    }
    return;
}

int ov70_02238E44(void) {
    return WM_LINK_LEVEL_3 - DWC_GetLinkLevel();
}

void ov70_02238E50(GTSApplicationState *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&arg0->nextScreen) = arg1;
    *((s32 *)&arg0->screenArgument) = arg2;
}

void ov70_02238E58(GTSApplicationState *arg0) {
    *((s32 *)&arg0->previousScreen) = (s32) *((s32 *)&arg0->screenId);
    *((s32 *)&arg0->screenId) = (s32) *((s32 *)&arg0->nextScreen);
}
