#include "global.h"

WMParentParam *ov74_02231054(void);

WMScanParam *ov74_022310C4(void);

WMBssDesc *ov74_02231100(void);

void ov74_0222FF80(void);

void ov74_0222FFAC(void);

s32 ov74_0222FE4C(void);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02231448(WMErrCode param0);
#define ov74_02231448() ov74_02231448((WMErrCode)0)
#else
void ov74_02231448();
#endif
s32 ov74_0223144C(u16);
void ov74_0222FFFC(void *arg0);
void ov74_02230018(void *arg0);
s32 ov74_02231070(s8);
u8 ov74_0223107C(void);
s32 ov74_022314BC(void);
s32 ov74_022316E8(void);
void ov74_02230030(void *arg0);
s32 ov74_0223113C(s32);
void ov74_02230070(void *arg0);
s32 ov74_0222FEC8(void);
s32 ov74_0222FF68(void);
void *ov74_0223115C(void);
s32 ov74_022314DC();
s32 ov74_02231508(void);
void ov74_022300A8(void *arg0);
s32 ov74_0222FE78(void);
s32 ov74_02231544(void);
void ov74_02230110(void *arg0);

void ov74_0222FF80(void) {
    WMScanParam *v0 = ov74_022310C4();

    v0->scanBuf = ov74_02231100();
    v0->channel = ov74_02231054()->channel;
    v0->maxChannelTime = WM_GetDispersionScanPeriod();
    v0->bssid[0] = 0xff;
    v0->bssid[1] = 0xff;
    v0->bssid[2] = 0xff;
    v0->bssid[3] = 0xff;
    v0->bssid[4] = 0xff;
    v0->bssid[5] = 0xff;
}

void ov74_0222FFAC(void) {
    WMScanParam *v0 = ov74_022310C4();
    u16 v1;
    s32 v2;

    v1 = WM_GetAllowedChannel();

    if (v1 == 0) {
        return;
    }

    for (v2 = 0; v2 < 16; v2++) {
        if (v1 & (0x1 << ((v0->channel + v2) % 16))) {
            v0->channel = (u16)(((v0->channel + v2) % 16) + 1);
            break;
        }
    }
}

void ov74_0222FFFC(void *arg0) {
    ov74_0223144C(*(u16 *)((u8 *)arg0 + 4));
    if (*(u16 *)((u8 *)arg0 + 2) == 8) {
        ov74_02231448();
        ov74_0222FE4C();
    }
}

void ov74_02230018(void *arg0) {
    ov74_0223144C(*(u16 *)((u8 *)arg0 + 8));
    if (*(u16 *)((u8 *)arg0 + 2) != 0) {
        ov74_02231448();
    }
}

void ov74_02230030(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        ov74_02231070(2);
        if (ov74_0223107C() == 0xC) {
            ov74_022314BC();
            ov74_02231070(2);
            return;
        }
        if (ov74_022316E8() == 0) {
            ov74_022314BC();
            ov74_02231070(2);
        }
    } else {
        ov74_02231448();
        ov74_022314BC();
    }
}

void ov74_02230070(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        if (ov74_0223107C() == 0xC) {
            WM_Finish();
            ov74_02231070(0xC);
            ov74_0223113C(1);
            return;
        }
        ov74_02231070(1);
        return;
    }
    ov74_02231448();
    ov74_02231070(4);
}

void ov74_022300A8(void *arg0) {
    u16 v1;
    void *v0;

    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        v0 = ov74_0223115C();
        v1 = *(u16 *)((u8 *)arg0 + 0xA);
        if ((s32) * (u8 *)((u8 *)v0 + 0x61) > (s32)v1) {
            *(u8 *)((u8 *)v0 + 0x61) = (u8)v1;
            *(s8 *)((u8 *)v0 + 0x64) = (s8) * (u16 *)((u8 *)arg0 + 8);
        }
        if (ov74_0222FEC8() != 0) {
            if (ov74_022314DC() == 0) {
                ov74_0222FE4C();
            }
        } else if ((u32) * (u8 *)((u8 *)v0 + 0x61) < 0x66U) {
            ov74_0222FF68();
            if (ov74_02231508() == 0) {
                ov74_0222FE4C();
            }
        } else {
            ov74_0222FE4C();
        }
    } else {
        ov74_02231448();
        ov74_0222FE4C();
    }
}

void ov74_02230110(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        if ((ov74_0222FE78() == 0) && (ov74_02231544() == 0)) {
            ov74_0222FE4C();
        }
    } else {
        ov74_02231448();
        ov74_0222FE4C();
    }
}
