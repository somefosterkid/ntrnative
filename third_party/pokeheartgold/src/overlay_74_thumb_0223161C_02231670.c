#include "global.h"

void ov74_022306C8(void *param0);

void ov74_022308E0(void *param0);

WMParentParam *ov74_02231054(void);

u32 ov74_022310D0(void);

u32 ov74_022310DC(void);

void *ov74_022310E8(void);

void *ov74_022310F4(void);

void ov74_02231448(WMErrCode param0);

BOOL ov74_02231670(void);

BOOL ov74_02231670(void) {
    WMErrCode v0;

    v0 = WM_SetPortCallback(4, ov74_022308E0, 0L);

    if (v0 == WM_ERRCODE_SUCCESS) {
        (void)0;
    } else {
        ov74_02231448(v0);
        return 0;
    }

    v0 = WM_StartMP(ov74_022306C8, ov74_022310F4(), (u16)ov74_022310DC(), ov74_022310E8(), (u16)ov74_022310D0(), (u16)(ov74_02231054()->CS_Flag ? 0 : 1));

    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_02231448(v0);
        return 0;
    } else {
        return 1;
    }
}
