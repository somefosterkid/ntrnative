#include "global.h"

void *ov74_02231054(void);

void ov74_022310B8(s16 arg0);

s32 ov74_02231100(void);
void ov74_0223110C(s16 arg0);
s32 ov74_02231118(void);

void ov74_02231124(s32 arg0);
void ov74_02231130(s32 arg0);

void ov74_0223113C(s32 arg0);

void ov74_022310B8(s16 arg0) {
    *(s16 *)((u8 *)(ov74_02231054()) + 0x32) = arg0;
}

void *ov74_0223105C(void);

s32 ov74_022310C4(void);

s32 ov74_022310C4(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x24);
}

s32 ov74_022310D0(void);

s32 ov74_022310D0(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x2C);
}

s32 ov74_022310DC(void);

s32 ov74_022310DC(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x30);
}

s32 ov74_022310E8(void);

s32 ov74_022310E8(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x38);
}

s32 ov74_022310F4(void);

s32 ov74_022310F4(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x3C);
}

s32 ov74_02231100(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x28);
}

void ov74_0223110C(s16 arg0) {
    *(s16 *)((u8 *)(ov74_0223105C()) + 0xC) = arg0;
}

s32 ov74_02231118(void) {
    return *(s32 *)((u8 *)(ov74_0223105C()) + 0x14);
}

void ov74_02231124(s32 arg0) {
    *(s32 *)((u8 *)(ov74_0223105C()) + 0x14) = arg0;
}

void ov74_02231130(s32 arg0) {
    *(s32 *)((u8 *)(ov74_0223105C()) + 0x18) = arg0;
}

void ov74_0223113C(s32 arg0) {
    *(s32 *)((u8 *)(ov74_0223105C()) + 0x20) = arg0;
}
