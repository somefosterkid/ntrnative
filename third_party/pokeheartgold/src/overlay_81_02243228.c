#include "global.h"

#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_81.h"

void ov81_02243228(GridInputHandler *arg0, s32 arg1, s32 *arg2, s32 *arg3);

extern GridCallbacks ov81_02243658;
extern TouchscreenHitbox ov81_022436F0;
extern DpadMenuBox ov81_02243808;
GridInputHandler *ov81_02243240(void *arg0, u8 arg1);
void ov81_02243270(GridInputHandler *arg0, s32 arg1);

void ov81_02243228(GridInputHandler *inputHandler, s32 target, s32 *arg2, s32 *arg3) {
    DpadMenuBox *dpadMenuBox;

    dpadMenuBox = (struct DpadMenuBox *)(GridInputHandler_GetDpadBox(inputHandler, target));
    *arg2 = (s32)dpadMenuBox->left;
    *arg3 = (s32)dpadMenuBox->top;
}

void ov81_0224323C(void) {
}

GridInputHandler *ov81_02243240(void *callbackParam, u8 initialPos) {
    return GridInputHandler_Create(&ov81_022436F0, &ov81_02243808, &ov81_02243658, callbackParam, 1, (u8)(s32)initialPos, HEAP_ID_100);
}

void ov81_02243270(GridInputHandler *inputHandler, s32 arg1) {
    if (arg1 == 1) {
        GridInputHandler_SetAllEnabled(inputHandler);
        return;
    }
    GridInputHandler_ClearEnabledFlag(inputHandler, 6U);
    GridInputHandler_ClearEnabledFlag(inputHandler, 7U);
    GridInputHandler_ClearEnabledFlag(inputHandler, 8U);
}
