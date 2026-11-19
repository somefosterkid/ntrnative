#include "global.h"

#include "wireless_manager.h"

s32 sub_02033A44(void);

s32 sub_02033A44(void) {
    if (_021D4124.sWirelessManager != NULL) {
        if ((u32) * (u8 *)&_021D4124.sWirelessManager->sentBeaconCount >= 6U) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void sub_02033A68(void);

void sub_02033A68(void) {
    _021D4124.sWirelessManager->sentBeaconCount = 0;
}

void sub_02033A7C(WirelessManagerGGIDScanFunc callback);

void sub_02033A7C(WirelessManagerGGIDScanFunc callback) {
    _021D4124.sWirelessManager->ggidScanCallback = callback;
}

void sub_02033A90(WirelessManagerConnectFunc callback);

void sub_02033A90(WirelessManagerConnectFunc callback) {
    _021D4124.sWirelessManager->connectCallback = callback;
}

void sub_02033AA4(s32 arg0);

void sub_02033AA4(s32 arg0) {
    _021D4124.sWirelessManager->pauseConnection = (s8)arg0;
}

u8 sub_02033AB8(void);

u8 sub_02033AB8(void) {
    return _021D4124.sWirelessManager->pauseConnection;
}

void sub_02033ACC(s32 arg0);

void sub_02033ACC(s32 arg0) {
    _021D4124.sWirelessManager->pauseConnectSystem = (s8)arg0;
}
