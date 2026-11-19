#include "global.h"

s32 ov74_0222FE4C(void);

s32 ov74_0222FE78(void);

s32 ov74_02231070(s8);

u8 ov74_02231094(void);

void *ov74_02231154(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02231448(WMErrCode param0);
#define ov74_02231448() ov74_02231448((WMErrCode)0)
#else
void ov74_02231448();
#endif

s32 ov74_02231638(void);

void ov74_02230590(void *arg0);

void ov74_02230590(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 2) == 0) {
        ov74_02231070(7);
        if ((ov74_0222FE78() == 0) && (ov74_02231094() == 1) && (((u32)(*(u8 *)((u8 *)(ov74_02231154()) + 0x1C3) << 0x18) >> 0x1C) == 2)) {
            if (ov74_02231638() == 0) {
                ov74_0222FE4C();
                return;
            }
            ov74_02231070(3);
        }
    } else {
        ov74_02231448();
        ov74_0222FE4C();
    }
}
