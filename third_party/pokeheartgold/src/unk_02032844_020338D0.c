#include "global.h"

#include "wireless_manager.h"

void sub_02033214(void *arg);

s32 sub_020338D0(void);

u16 sub_020338F4(void);
#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02033908(int numConnectionsMax);
#else
void sub_02033908(u8 numConnectionsMax);
#endif
BOOL sub_02033920(void);
BOOL sub_0203393C(void);
BOOL sub_02033958(void);

s32 sub_020338D0(void) {
    sub_02032844(3);
    if (WM_End(sub_02033214) != WM_ERRCODE_OPERATING) {
        sub_02032844(9);
        return 0;
    }
    return 1;
}

u16 sub_020338F4(void) {
    return _021D4124.sWirelessManager->aid;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02033908(int numConnectionsMax) {
#else
void sub_02033908(u8 numConnectionsMax) {
#endif
    if (_021D4124.sWirelessManager != NULL) {
        _021D4124.sWirelessManager->numConnectionsMax = numConnectionsMax;
    }
}

BOOL sub_02033920(void) {
    return _021D4124.sWirelessManager->state == WIRELESS_STATE_IDLE;
}

BOOL sub_0203393C(void) {
    return _021D4124.sWirelessManager->state == WIRELESS_STATE_BUSY;
}

BOOL sub_02033958(void) {
    return _021D4124.sWirelessManager->state == WIRELESS_STATE_ERROR;
}
