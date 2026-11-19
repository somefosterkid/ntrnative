#include "global.h"

#include "unk_02037C94.h"
#include "wireless_manager.h"

BOOL sub_0203276C(void *param0);

BOOL sub_02032874(void);

void sub_02033664(void *wm_callback);

BOOL sub_02033668(int connectionType, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, BOOL entryFlag);

BOOL sub_020340C4(int param0);

BOOL sub_02033668(int connectionType, u16 tgid, u16 channel, u16 maxEntry, u16 beaconPeriod, BOOL entryFlag) {
    if (sub_020340C4(sub_0203993C())) {
        WM_SetLifeTime(sub_02033664, 0xFFFF, 100, 5, 100);
    }

    _021D4124.sWirelessManager->recvBufferSize = MATH_MAX(WM_SIZE_MP_PARENT_RECEIVE_BUFFER(12, 7, 0), MATH_MAX(WM_SIZE_MP_PARENT_RECEIVE_BUFFER(12, 7, 0), WM_SIZE_MP_PARENT_RECEIVE_BUFFER(38, (5 - 1), 0)));
    _021D4124.sWirelessManager->sendBufferSize = WM_SIZE_MP_PARENT_SEND_BUFFER(MATH_MAX((12 * (1 + 7) + 4), 192), 0);

    _021D4124.sWirelessManager->connectionType = connectionType;
    sub_02032844(WIRELESS_STATE_BUSY);

    _021D4124.sWirelessManager->parentParam.tgid = tgid;
    _021D4124.sWirelessManager->parentParam.channel = channel;
    _021D4124.sWirelessManager->parentParam.beaconPeriod = beaconPeriod;

    switch (connectionType) {
    case WIRELESS_CONNECTION_MP_SERVER:
        _021D4124.sWirelessManager->parentParam.parentMaxSize = 192;

        if (maxEntry >= 5) {
            _021D4124.sWirelessManager->parentParam.childMaxSize = 12;
        } else {
            _021D4124.sWirelessManager->parentParam.childMaxSize = 38;
        }

        break;
    case WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER:
        _021D4124.sWirelessManager->parentParam.parentMaxSize = (12 * (1 + 7) + 4);
        _021D4124.sWirelessManager->parentParam.childMaxSize = 12;
        break;
    }

    _021D4124.sWirelessManager->parentParam.maxEntry = maxEntry;
    _021D4124.sWirelessManager->parentParam.CS_Flag = 0;
    _021D4124.sWirelessManager->parentParam.multiBootFlag = 0;
    _021D4124.sWirelessManager->parentParam.entryFlag = entryFlag;
    _021D4124.sWirelessManager->parentParam.KS_Flag = (u16)((connectionType == WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER) ? 1 : 0);

    switch (connectionType) {
    case WIRELESS_CONNECTION_MP_SERVER:
    case WIRELESS_CONNECTION_TRANSMIT_KEY_SERVER:
    case WIRELESS_CONNECTION_TRANSMIT_DATA_SERVER:
        return sub_02032874();
    default:
        break;
    }

    return FALSE;
}
