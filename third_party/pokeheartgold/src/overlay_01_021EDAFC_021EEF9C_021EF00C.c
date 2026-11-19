#include "global.h"

u32 ov01_021EF00C(void *arg0);

u32 ov01_021EF00C(void *arg0) {
    return (u32)(*(u8 *)((u8 *)arg0 + 0x97) << 0x1F) >> 0x1F;
}
