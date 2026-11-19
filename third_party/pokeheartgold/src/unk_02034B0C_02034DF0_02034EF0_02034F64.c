#include "global.h"

#include "comm_server_client.h"

void sub_02034E2C(void);

void sub_020367A8(u16 param0, u16 *param1, u16 param2);

typedef void (*WirelessManagerScanFunc)(WMBssDesc *);

typedef void (*WirelessManagerRecvFunc)(u16, u16 *, u16);

BOOL sub_02032C1C(WirelessManagerScanFunc scanCallback, const u8 *macAddress, u16 channel);

int sub_02033298(void);

void sub_020337D0(WirelessManagerRecvFunc recvFunction, int port);

void sub_02034E8C(BOOL);

void sub_02034C20(WMBssDesc *param0);

BOOL sub_02034F64(BOOL param0, BOOL param1);

BOOL sub_02034F64(BOOL param0, BOOL param1) {
    sub_02034E8C(param0);

    if (param1) {
        sub_02034E2C();
    }

    if (!_021D4134.sCommServerClient->unk_D93) {
        sub_020337D0(sub_020367A8, 14);
        _021D4134.sCommServerClient->unk_D93 = 1;
    }

    if (sub_02033298() == 1) {
        const u8 v0[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

        if (sub_02032C1C(sub_02034C20, v0, 0)) {
            return 1;
        }
    }

    return 0;
}
