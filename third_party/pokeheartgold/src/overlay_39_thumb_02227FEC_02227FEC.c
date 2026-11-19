#include "global.h"

#include "overlay_39_thumb.h"

s32 ov39_0222A2B4(void);

u64 ov39_02227FEC(void) {
    s32 iVar1;

    iVar1 = ov39_0222A2B4();
    return *(u64 *)(iVar1 + 4);
}
