#include "global.h"

#include "overlay_70_gts_networking_state.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov70_02238360(s32 *, s32 *, s32, s32 *, s32);

extern s32 ov70_022460A0;

extern s32 ov70_02246814;

extern s32 ov70_0224693C;

void ov70_02237F64(void *arg0);

void ov70_02237F64(void *arg0) {
    memcpy(&ov70_02246814, arg0, 0x124U);
    ov38_0221BE84();
    if (ov70_02238360(&ov70_022460A0, &ov70_02246814, 0x124, &ov70_0224693C, 2) != 0) {
        _02246800.status = NETWORK_STATUS_POST_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}
