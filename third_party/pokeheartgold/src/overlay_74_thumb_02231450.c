#include "global.h"

void ov74_02231450(void);

void ov74_02230018(void *param0);
/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov74_022310AC(void);
#else
u32 ov74_022310AC(void);
#endif
void ov74_02231448(WMErrCode param0);
BOOL ov74_02231460(const void *param0, u32 param1, u16 param2);

void ov74_02231450(void) {
}

void ov74_02231454(void);

void ov74_02231454(void) {
}

void ov74_02231458(void);

void ov74_02231458(void) {
}

void ov74_0223145C(void);

void ov74_0223145C(void) {
}

BOOL ov74_02231460(const void *param0, u32 param1, u16 param2) {
    WMErrCode v0;

    v0 = WM_SetMPDataToPort(ov74_02230018, param0, (u16)param1, param2, (u16)ov74_022310AC(), 2);

    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_02231448(v0);
        return 0;
    } else {
        return 1;
    }
}
