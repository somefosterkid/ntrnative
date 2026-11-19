#include "global.h"

#include "overlay_70_gts_networking_state.h"

s32 ov38_0221BE84(void);

s32 ov38_0221BFEC(void);

s32 ov70_02238360(s32 *, s32 *, s32, void *, s32);

extern s32 ov70_0224613C;

extern s32 ov70_02246814;

void ov70_02238008(void *arg0);

void ov70_02238008(void *arg0) {
    ov70_02246900.responseData = arg0;
    ov38_0221BE84();
    if (ov70_02238360(&ov70_0224613C, &ov70_02246814, 0, arg0, 0x124) != 0) {
        _02246800.status = NETWORK_STATUS_GET_LISTED_POKEMON_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}

extern s32 ov70_02246184;

void ov70_02238058(void *arg0);

void ov70_02238058(void *arg0) {
    ov70_02246900.responseData = arg0;
    ov38_0221BE84();
    if (ov70_02238360(&ov70_02246184, &ov70_02246814, 0, arg0, 0x124) != 0) {
        _02246800.status = NETWORK_STATUS_GET_LISTING_STATUS_PENDING;
        return;
    }
    _02246800.status = NETWORK_STATUS_COMPLETE;
    _02246800.result = -0xD;
    ov38_0221BFEC();
}
