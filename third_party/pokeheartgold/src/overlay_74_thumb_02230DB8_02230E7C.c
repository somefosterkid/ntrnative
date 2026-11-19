#include "global.h"

void *ov74_0223105C(void);

enum WMLinkLevel ov74_02230E7C(void);

enum WMLinkLevel ov74_02230E7C(void) {
    if (*(s32 *)((u8 *)(ov74_0223105C()) + 0x20) != 0) {
        return WM_LINK_LEVEL_0;
    }
    return WM_GetLinkLevel();
}

s32 ov74_02230D80(void);

s32 ov74_02230E44(void);

u8 ov74_02231094(void);

s32 ov74_02230E94(void);

s32 ov74_02230E94(void) {
    s32 v0;

    v0 = ov74_02231094();
    switch (v0) {
    case 1:
        return ov74_02230D80();
    case 2:
        return ov74_02230E44();
    default:
        return 0;
    }
}
