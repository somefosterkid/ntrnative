#include "global.h"

void ov74_02231070(s8 param0);

extern u8 ov74_02231094(void);

void ov74_0223144C(WMStateCode param0);

void ov74_0222FE4C(void);

BOOL ov74_0222FE78(void);

void ov74_022306C8(void *param0);

void ov74_022306C8(void *param0) {
    WMstartMPCallback *v0 = (WMstartMPCallback *)param0;

    ov74_0223144C((WMStateCode)v0->state);

    if (v0->errcode == WM_ERRCODE_SUCCESS) {
        if (v0->apiid == WM_APIID_START_MP) {
            if (ov74_0222FE78()) {
                return;
            }
        }

        switch (ov74_02231094()) {
        case 2:
            ov74_02231070(10);
            break;
        case 1:
            ov74_02231070(11);
            break;
        }

        switch (v0->state) {
        case WM_STATECODE_MP_START:
            break;
        case WM_STATECODE_MPEND_IND:
            break;
        case WM_STATECODE_MP_IND:
            break;
        }
    } else {
        switch (v0->errcode) {
        case WM_ERRCODE_SEND_FAILED:
        case WM_ERRCODE_TIMEOUT:
            break;
        case WM_ERRCODE_INVALID_POLLBITMAP:
            break;
        default:
            ov74_0222FE4C();
        }
    }
}
