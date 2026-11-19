#include "global.h"

void ov74_02231070(s8 param0);

void ov74_0223110C(u16 param0);

BOOL ov74_02231118(void);

void ov74_02231124(BOOL param0);

void ov74_02231130(BOOL param0);

BOOL ov74_02231670(void);

void ov74_0223144C(WMStateCode param0);

s32 ov74_0222FE78(void);

void ov74_0222FE4C(void);

void ov74_02231448(WMErrCode param0);

void ov74_022305E4(void *param0);

void ov74_022305E4(void *param0) {
    WMstartConnectCallback *v0 = (WMstartConnectCallback *)param0;

    ov74_0223144C((WMStateCode)v0->state);

    if (v0->errcode == WM_ERRCODE_SUCCESS) {
        ov74_02231070(9);

        if (ov74_0222FE78()) {
            return;
        }

        switch (v0->state) {
        case WM_STATECODE_CONNECT_START:
            break;
        case WM_STATECODE_CONNECTED:
            ov74_0223110C(v0->aid);
            ov74_02231124(1);
            ov74_02231130(0);

            if (ov74_02231670() == 0) {
                ov74_0222FE4C();
            }
            break;
        case WM_STATECODE_BEACON_LOST:
            break;
        case WM_STATECODE_DISCONNECTED:
            if (ov74_02231118()) {
                ov74_02231130(1);
            }

            ov74_02231124(0);
            break;
        default:
            ov74_02231448((WMErrCode)v0->errcode);
            ov74_0222FE4C();
        }
    } else {
        if (v0->errcode == WM_ERRCODE_NO_ENTRY) {
            (void)0;
        }

        if (v0->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            (void)0;
        }

        ov74_02231448((WMErrCode)v0->errcode);
        ov74_0222FE4C();
    }
}
