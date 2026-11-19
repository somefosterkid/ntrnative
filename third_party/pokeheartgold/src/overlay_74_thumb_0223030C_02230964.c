#include "global.h"

s32 ov74_0222FE78(void);

s32 ov74_02230674(void);

s32 ov74_02231070(s8);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02231448(WMErrCode param0);
#define ov74_02231448() ov74_02231448((WMErrCode)0)
#else
void ov74_02231448();
#endif

void ov74_02230964(void *arg0);

void ov74_02230964(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        if (ov74_0222FE78() == 0) {
            ov74_02230674();
        }
    } else {
        ov74_02231448();
        ov74_02231070(4);
    }
}

s32 ov74_0222FE4C(void);

s32 ov74_022314BC(void);

void ov74_02230988(void *arg0);

void ov74_02230988(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        if (ov74_022314BC() != 0) {
            ov74_02231070(2);
        }
    } else {
        ov74_02231448();
        ov74_0222FE4C();
    }
}

u8 ov74_0223107C(void);

s32 ov74_0223110C(s16);

s32 ov74_02231704(void);

s32 ov74_02231744(void);

void ov74_022309AC(void *arg0);

void ov74_022309AC(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        ov74_0223110C(0);
        if (ov74_0223107C() == 0xC) {
            if (ov74_02231744() != 0) {
                ov74_02231070(3);
            }
        } else if (ov74_02231704() != 0) {
            ov74_02231070(3);
        }
    } else {
        ov74_02231448();
        ov74_02231070(4);
    }
}

s32 ov74_0223113C(s32);

void ov74_022309F0(void *arg0);

void ov74_022309F0(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        ov74_02231070(0xC);
        ov74_0223113C(1);
        return;
    }
    ov74_02231448();
    ov74_02231070(4);
}

void ov74_0222FFFC(void *arg);

s32 ov74_02230A14(void);

s32 ov74_02230A14(void) {
    if (WM_SetIndCallback(ov74_0222FFFC) != WM_ERRCODE_SUCCESS) {
        ov74_02231070(4);
        return 0;
    }
    return 1;
}
