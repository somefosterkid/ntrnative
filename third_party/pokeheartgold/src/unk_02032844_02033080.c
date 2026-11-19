#include "global.h"

#include "wireless_manager.h"

static void sub_020330A4(void *arg);

s32 sub_02033080(void);

s32 sub_02033080(void) {
    sub_02032844(3);
    WMErrCode errCode = WM_EndMP(sub_020330A4);
    if (errCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errCode);
        return 0;
    }
    return 1;
}

static s32 sub_020330C8();

s32 sub_02033858(void);

static void sub_020330A4(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        sub_02032858(((WMCallback *)arg0)->errcode);
        sub_02033858();
        return;
    }
    if (sub_020330C8() == 0) {
        sub_02032844(9);
    }
}

static void sub_020330F0(void *arg);

s32 sub_02033830(void);

static s32 sub_020330C8(void) {
    sub_02032844(3);
    WMErrCode errCode = WM_Disconnect(sub_020330F0, 0U);
    if (errCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errCode);
        sub_02033830();
        return 0;
    }
    return 1;
}

static void sub_020330F0(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        sub_02032858(((WMCallback *)arg0)->errcode);
        return;
    }
    sub_02032844(1);
}

static void sub_0203312C(void *arg);

s32 sub_02033108(void);

s32 sub_02033108(void) {
    sub_02032844(3);
    WMErrCode errCode = WM_Reset(sub_0203312C);
    if (errCode != WM_ERRCODE_OPERATING) {
        sub_02032858(errCode);
        return 0;
    }
    return 1;
}

static void sub_0203312C(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        sub_02032844(9);
        sub_02032858(((WMCallback *)arg0)->errcode);
        return;
    }
    sub_02032844(1);
}

void sub_020331A4(void *arg);

s32 sub_0203314C(u16 *arg0, u16 arg1, u16 arg2, void *arg3);

s32 sub_0203314C(u16 *sendData, u16 sendDataSize, u16 port, void *arg3) {
    DC_FlushRange(_021D4124.sWirelessManager->sendBuffer, _021D4124.sWirelessManager->sendBufferSize);
    if (WM_SetMPDataToPortEx(sub_020331A4, arg3, sendData, sendDataSize, 0xFFFFU, (u16)(s32)port, 2U) == WM_ERRCODE_OPERATING) {
        return 1;
    }
    return 0;
}
