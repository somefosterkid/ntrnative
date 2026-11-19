#include "global.h"

#include "overlay_74_wh_work.h"

static BOOL ov74_0223648C(void);

void ov74_02236354(void *arg0) {
    WHCallback v1;
    WHCallback v2;
    WHCallback v3;
    WHCallback v4;
    s32 v7;
    u16 v0;
    u8 v6;
    void *v5;

    if (((WMStartScanCallback *)arg0)->errcode != 0) {
        ov74_022361B8(((WMStartScanCallback *)arg0)->errcode);
        ov74_02236168(9);
        v1 = ov74_0223E2FC.work->callback;
        if (v1 != NULL) {
            v1(4);
        }
    } else if (ov74_0223E2FC.work->state != 2) {
        if (ov74_0223648C() == 0) {
            ov74_02236168(9);
            v2 = ov74_0223E2FC.work->callback;
            if (v2 != NULL) {
                v2(4);
            }
        }
    } else {
        v0 = ((WMStartScanCallback *)arg0)->state;
        switch (v0) {
        case 5:
            DC_InvalidateRange(&ov74_0223E2FC.work->bssDesc, sizeof(WMbssDesc));
            if ((((WMStartScanCallback *)arg0)->gameInfoLength < 8U) || (((WMStartScanCallback *)arg0)->gameInfo.ggid != 0x400318)) {
                if (ov74_0223E2FC.print != NULL) {
                    ov74_0223E2FC.print(ov74_0223D018, ((WMStartScanCallback *)arg0)->gameInfo.ggid, 0x400318);
                }
                goto block_21;
            }
            ov74_0223E2FC.work->linkLevel = ((WMStartScanCallback *)arg0)->linkLevel;
            if (ov74_0223E2FC.work->phase == 2) {
                ov74_022366E8((WHBlock *)((WMStartScanCallback *)arg0)->gameInfo.userGameInfo);
                v7 = 0;
                // void * arithmetic: mwcc strength-reduces the typed `macAddress[v7]` form and the code no longer matches
                do {
                    v6 = ((WMStartScanCallback *)(arg0 + v7))->macAddress[0];
                    v5 = (void *)ov74_0223E2FC.work + v7;
                    v7 += 1;
                    ((WHWork *)v5)->parentBssid[0] = v6;
                } while (v7 < 6);
                ov74_0223E2FC.work->channel = (u16)(ov74_0223E2FC.work->channel - 0xF);
                v3 = ov74_0223E2FC.work->callback;
                if (v3 != NULL) {
                    v3(1);
                }
            }
            if (ov74_02236768((WHBlock *)((WMStartScanCallback *)arg0)->gameInfo.userGameInfo) != 0) {
                ov74_0223E2FC.work->phase = 4U;
                ov74_022365FC();
                return;
            }
        default:
        case 4:
block_21:
            if (ov74_02236258() == 0) {
                ov74_02236168(9);
                v4 = ov74_0223E2FC.work->callback;
                if (v4 != NULL) {
                    v4(4);
                }
            }
        case 3:
            return;
        }
    }
}

static void ov74_022364A8(void *arg0);

static BOOL ov74_0223648C(void) {
    WMErrCode v0 = WM_EndScan(ov74_022364A8);

    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_022361B8(v0);
        return 0;
    }
    return 1;
}

static void ov74_022364A8(void *arg0) {
    if (((WMCallback *)arg0)->errcode != 0) {
        ov74_022361B8(((WMCallback *)arg0)->errcode);
        return;
    }
    ov74_02236168(1);
}
