#include "global.h"

#include "error_handling.h"
#include "overlay_39_thumb.h"

s32 ov39_02227DB8(void *arg0);

s32 ov40_02244B3C(void *);

u32 sub_0202FBCC(void);

void *sub_0202FC74(void);

void *sub_020307DC(void);

s32 ov39_02227534(void *arg0, void *src) {
    if (ov39_02227DB8(arg0) == 0) {
        return 0;
    }
    if ((sub_0202FBCC() - 4) != 0x1D4C) {
        GF_AssertFail();
    }
    *(void **)((u8 *)arg0 + 0x190) = sub_0202FC74();
    ov40_02244B3C(*(void **)arg0);
    MI_CpuCopy8(src, sub_020307DC(), 0x80U);
    *(s8 *)((u8 *)arg0 + 0x411) = 0x3C;
    *(s32 *)((u8 *)arg0 + 0x3E8) = 0x59D8;
    return 1;
}
