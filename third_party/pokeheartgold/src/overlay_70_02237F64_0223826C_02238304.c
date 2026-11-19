#include "global.h"

#include "overlay_70_gts_networking_state.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov70_02238360(s32 *, s32 *, s32, void *, s32);

extern s32 ov70_022463A4;

extern s32 ov70_02246814;

void ov70_02238304(void *arg0, void *arg1);

void ov70_02238304(void *arg0, void *arg1) {
    OS_GetMacAddress(arg0 + 0x1C);
    memcpy(&ov70_02246814, arg0, 0x64U);
    ov70_02246900.responseData = (u8 *)arg1;
    ov38_0221BE84();
    if (ov70_02238360(&ov70_022463A4, &ov70_02246814, 0x64, (u8 *)ov70_02246900.responseData, 8) != 0) {
        _02246800.status = NETWORK_STATUS_SET_PROFILE_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}
