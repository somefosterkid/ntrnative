#include "global.h"

#include "overlay_74_wh_work.h"

#define WH_ASSERT(exp) (void)((exp) || (OS_Terminate(), 0))

BOOL ov74_02236258(void) {
    WMErrCode v0;
    u16 v1;

    WH_ASSERT(ov74_0223E2FC.work->state == 2);

    v1 = WM_GetAllowedChannel();

    if (v1 == 0x8000) {
        ov74_022361B8(WM_ERRCODE_ILLEGAL_STATE);
        return 0;
    }

    if (v1 == 0) {
        ov74_022361B8(22);
        return 0;
    }

    if (ov74_0223E2FC.work->channel >= 16) {
        int v2 = ov74_0223E2FC.work->channel - 16;
        int v3;

        for (v3 = 0; v3 < 13; v3++) {
            v2 = (v2 + 1) % 13;

            if (v1 & (1 << v2)) {
                break;
            }
        }

        ov74_0223E2FC.work->channel = (u16)(16 + v2);
        ov74_0223E2FC.work->scanParam.channel = (u16)(v2 + 1);
    } else {
        ov74_0223E2FC.work->scanParam.channel = ov74_0223E2FC.work->channel;
    }

    ov74_0223E2FC.work->scanParam.maxChannelTime = 220;
    ov74_0223E2FC.work->scanParam.scanBuf = &ov74_0223E2FC.work->bssDesc;

    if (ov74_0223E2FC.work->phase == 3) {
        int v4;

        for (v4 = 0; v4 < 6; v4++) {
            ov74_0223E2FC.work->scanParam.bssid[v4] = ov74_0223E2FC.work->parentBssid[v4];
        }
    }

    v0 = WM_StartScan(ov74_02236354, &ov74_0223E2FC.work->scanParam);

    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_022361B8(v0);
        return 0;
    }

    return 1;
}
