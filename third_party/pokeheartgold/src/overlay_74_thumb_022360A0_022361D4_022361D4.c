#include "global.h"

#include "overlay_74_wh_work.h"

BOOL ov74_022361D4(void) {
    s32 iVar1;

    if (ov74_0223E2FC.work->state == 4) {
        OS_Terminate();
    }
    ov74_02236168(2);
    *(u16 *)&ov74_0223E2FC.work->scanParam.bssid[4] = 0xffff;
    *(u16 *)&ov74_0223E2FC.work->scanParam.bssid[2] = 0xffff;
    *(u16 *)&ov74_0223E2FC.work->scanParam.bssid[0] = 0xffff;
    iVar1 = ov74_02236258();
    if (iVar1 == 0) {
        ov74_02236168(9);
        if (ov74_0223E2FC.work->callback != NULL) {
            ov74_0223E2FC.work->callback(4);
        }
        return 0;
    }
    ov74_0223E2FC.work->phase = 2;
    if (ov74_0223E2FC.work->callback != NULL) {
        ov74_0223E2FC.work->callback(0);
    }
    return 1;
}
