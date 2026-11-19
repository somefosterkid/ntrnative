#include "global.h"

#include "overlay_70_gts_networking_state.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov70_02238360(s32 *, s32 *, s32, s32 *, s32);

extern s32 ov70_02246358;

extern s32 ov70_02246814;

extern s32 ov70_0224693C;

void ov70_022382C0(void);

void ov70_022382C0(void) {
    ov38_0221BE84();
    if (ov70_02238360(&ov70_02246358, &ov70_02246814, 0, &ov70_0224693C, 2) != 0) {
        _02246800.status = NETWORK_STATUS_INIT_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}
