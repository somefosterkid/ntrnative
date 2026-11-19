#include "global.h"

#include "overlay_74_wh_work.h"

void ov74_022364C0(void *arg0) {
    WHCallback v0;

    if (((WMCallback *)arg0)->errcode != 0) {
        ov74_02236168(0xA);
        return;
    }
    ov74_02236168(0);
    v0 = ov74_0223E2FC.work->callback;
    if (v0 != NULL) {
        v0(3);
    }
}

BOOL ov74_022364F0(void) {
    ov74_0223E2FC.work->errCode = 0;
    if (ov74_0223653C() != 0) {
        return 1;
    }
    return 0;
}

void ov74_02236514(void *arg0) {
    WHCallback v0;

    if (((WMCallback *)arg0)->errcode == 8) {
        ov74_02236168(9);
        v0 = ov74_0223E2FC.work->callback;
        if (v0 != NULL) {
            v0(4);
        }
    }
}

BOOL ov74_0223653C(void) {
    WMErrCode v0;

    ov74_02236168(3);
    v0 = WM_Initialize(ov74_0223E2FC.work->sysBuf, ov74_02236570, 2U);
    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_022361B8(v0);
        ov74_02236168(0xA);
        return 0;
    }
    return 1;
}

void ov74_02236570(void *arg0) {
    WMErrCode v0;

    if (((WMCallback *)arg0)->errcode != 0) {
        ov74_022361B8(((WMCallback *)arg0)->errcode);
        ov74_02236168(0xA);
        return;
    }
    v0 = WM_SetIndCallback(ov74_02236514);
    if (v0 != WM_ERRCODE_SUCCESS) {
        ov74_022361B8(v0);
        ov74_02236168(0xA);
        return;
    }
    ov74_02236168(1);
    ov74_022361D4();
}

BOOL ov74_022365AC(void) {
    WHCallback v0;

    if (ov74_0223E2FC.work->state != 1) {
        OS_Terminate();
    }
    ov74_02236168(3);
    if (WM_End(ov74_022364C0) != WM_ERRCODE_OPERATING) {
        ov74_02236168(9);
        v0 = ov74_0223E2FC.work->callback;
        if (v0 != NULL) {
            v0(4);
        }
        return 0;
    }
    return 1;
}

BOOL ov74_022365FC(void) {
    if (ov74_0223E2FC.work->state == 1) {
        return ov74_022365AC();
    }
    return ov74_02236620();
}

BOOL ov74_02236620(void) {
    WMErrCode v0;

    ov74_02236168(3);
    v0 = WM_Reset(ov74_02236644);
    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_022361B8(v0);
        return 0;
    }
    return 1;
}

void ov74_02236644(void *arg0) {
    WHCallback v0;

    if (((WMCallback *)arg0)->errcode != 0) {
        ov74_02236168(9);
        ov74_022361B8(((WMCallback *)arg0)->errcode);
        v0 = ov74_0223E2FC.work->callback;
        if (v0 != NULL) {
            v0(4);
        }
    } else {
        ov74_02236168(1);
        ov74_022365AC();
    }
}
