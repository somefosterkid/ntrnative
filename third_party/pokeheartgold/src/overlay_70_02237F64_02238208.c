#include "global.h"

#include "overlay_70_gts_networking_state.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov70_02238360(s32 *, s32 *, s32, void *, s32);

extern s32 ov70_022462B4;

extern s32 ov70_02246814;

void ov70_02238208(s32 arg0, void *arg1, void *arg2);

void ov70_02238208(s32 arg0, void *arg1, void *arg2) {
    ov70_02246900.responseData = (u8 *)arg2;
    ov38_0221BE84();
    memcpy(&ov70_02246814, arg1, 0x124U);
    ov70_02246900.requestDataEnd = arg0;
    if (ov70_02238360(&ov70_022462B4, &ov70_02246814, 0x128, arg2, 0x124) != 0) {
        _02246800.status = NETWORK_STATUS_EXCHANGE_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}
