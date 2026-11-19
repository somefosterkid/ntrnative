#include "global.h"

void ov74_02230590(void *arg);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02231448(WMErrCode param0);
#define ov74_02231448() ov74_02231448((WMErrCode)0)
#else
void ov74_02231448();
#endif

s32 ov74_0223161C(void);

s32 ov74_0223161C(void) {
    if (WM_EndScan(ov74_02230590) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        return 0;
    }
    return 1;
}

void ov74_022305E4(void *arg);

WMbssDesc *ov74_02231100(void);

u8 *ov74_022311E8(void);

s32 ov74_02231638(void);

s32 ov74_02231638(void) {
    u8 *ssid;

    ssid = ov74_022311E8();
    if (WM_StartConnectEx(ov74_022305E4, ov74_02231100(), ssid, 1, 0U) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        return 0;
    }
    return 1;
}
