#include "global.h"

#include "overlay_70_gts_networking_state.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov70_02238360(s32 *, s32 *, s32, void *, s32);

extern s32 ov70_02246268;

extern s32 ov70_02246814;

void ov70_02238130(void *arg0, s8 arg1, void *arg2);

void ov70_02238130(void *arg0, s8 arg1, void *arg2) {
    ov70_02246900.responseData = (u8 *)arg2;
    ov38_0221BE84();
    *(u8 *)(&ov70_02246814) = (u8) * (u8 *)arg0;
    *(u8 *)((u8 *)(&ov70_02246814) + 1) = (u8) * (u8 *)((u8 *)arg0 + 1);
    *(u8 *)((u8 *)(&ov70_02246814) + 2) = (u8) * (u8 *)((u8 *)arg0 + 2);
    *(u8 *)((u8 *)(&ov70_02246814) + 3) = (u8) * (u8 *)((u8 *)arg0 + 3);
    *(u8 *)((u8 *)(&ov70_02246814) + 4) = (u8) * (u8 *)((u8 *)arg0 + 4);
    *(u8 *)((u8 *)(&ov70_02246814) + 5) = (u8) * (u8 *)((u8 *)arg0 + 5);
    _02246800.requestData[6] = arg1;
    if (ov70_02238360(&ov70_02246268, &ov70_02246814, 7, arg2, 0x124 * arg1) != 0) {
        _02246800.status = NETWORK_STATUS_SEARCH_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}
