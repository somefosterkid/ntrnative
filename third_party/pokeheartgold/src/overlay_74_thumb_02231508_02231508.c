#include "global.h"

void ov74_02230110(void *arg);

WMpparam *ov74_02231054(void);

void *ov74_0223115C(void);

u32 ov74_022311D8(void);

u16 *ov74_022311DC(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02231448(WMErrCode param0);
#define ov74_02231448() ov74_02231448((WMErrCode)0)
#else
void ov74_02231448();
#endif

s32 ov74_02231508(void);

s32 ov74_02231508(void) {
    WMpparam *pparaBuf;
    u16 *v0;

    pparaBuf = ov74_02231054();
    v0 = ov74_022311DC();
    pparaBuf->channel = (u16) * (u8 *)((u8 *)(ov74_0223115C()) + 0x64);
    pparaBuf->userGameInfo = v0;
    pparaBuf->userGameInfoLength = ov74_022311D8();
    if (WM_SetParentParameter(ov74_02230110, pparaBuf) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        return 0;
    }
    return 1;
}

void ov74_02230138(void *arg);

s32 ov74_02231544(void);

s32 ov74_02231544(void) {
    if (WM_StartParent(ov74_02230138) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        return 0;
    }
    return 1;
}

void ov74_02230404(void *arg);

WMscanParam *ov74_022310C4(void);

s32 ov74_02231560(void);

s32 ov74_02231560(void) {
    if (WM_StartScan(ov74_02230404, ov74_022310C4()) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        return 0;
    }
    return 1;
}
