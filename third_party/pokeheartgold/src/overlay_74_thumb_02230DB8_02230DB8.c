#include "global.h"

u8 ov74_02231064(void);

u8 ov74_02231094(void);

void *ov74_02231154(void);

s32 ov74_02230DB8(s32 arg0);

s32 ov74_02230DB8(s32 arg0) {
    void *v0;

    if ((ov74_02231094() == 1) && (ov74_02231064() == 7)) {
        v0 = ov74_02231154();
        if ((((u32)(*(u8 *)((u8 *)v0 + 0x1C3) << 0x18) >> 0x1C) == 1) && (*(s32 *)(v0 + (0x38 * arg0)) != 0)) {
            return 1;
        }
    }
    return 0;
}
