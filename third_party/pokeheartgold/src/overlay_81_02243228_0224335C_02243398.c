#include "global.h"
#include "overlay_81_battle_factory_app.h"

#include "unk_02019BA4.h"

BOOL ov81_02242710(BattleFactoryApp *arg0);

void ov81_02243220(GridInputHandler *);

extern GridCallbacks ov81_02243638;

extern TouchscreenHitbox ov81_022436A8;

extern TouchscreenHitbox ov81_022436CC;

extern DpadMenuBox ov81_02243788;

extern DpadMenuBox ov81_022437C8;

GridInputHandler *ov81_02243398(void *arg0, GridInputHandler *arg1);

GridInputHandler *ov81_02243398(void *callbackParam, GridInputHandler *arg1) {
    ov81_02243220(arg1);
    if (ov81_02242710(callbackParam) == 1) {
        return GridInputHandler_Create(&ov81_022436CC, &ov81_02243788, &ov81_02243638, callbackParam, 1, 0U, HEAP_ID_100);
    }
    return GridInputHandler_Create(&ov81_022436A8, &ov81_022437C8, &ov81_02243638, callbackParam, 1, 0U, HEAP_ID_100);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov81_02242704(u16);
#else
s32 ov81_02242704(s32);
#endif

void ov81_022433FC(GridInputHandler *arg0, s32 arg1, s32 arg2);

void ov81_022433FC(GridInputHandler *inputHandler, s32 arg1, s32 arg2) {
    if (arg1 == 1) {
        GridInputHandler_SetAllEnabled(inputHandler);
        GridInputHandler_ClearEnabledFlag(inputHandler, 4U);
        GridInputHandler_ClearEnabledFlag(inputHandler, 5U);
    } else {
        GridInputHandler_SetAllEnabled(inputHandler);
        GridInputHandler_ClearEnabledFlag(inputHandler, 6U);
        GridInputHandler_ClearEnabledFlag(inputHandler, 7U);
    }
    if (ov81_02242704(arg2) == 0) {
        GridInputHandler_ClearEnabledFlag(inputHandler, 3U);
    }
}
