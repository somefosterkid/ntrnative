#include "global.h"

void ov74_022300A8(void *param0);
void ov74_02231448(WMErrCode param0);

BOOL ov74_022314DC(u32 param0);

BOOL ov74_022314DC(u32 param0) {
    WMErrCode v0;

    v0 = WM_MeasureChannel(ov74_022300A8, 3, 17, (u16)param0, 30);

    if (v0 != WM_ERRCODE_OPERATING) {
        ov74_02231448(v0);
        return 0;
    } else {
        return 1;
    }
}
