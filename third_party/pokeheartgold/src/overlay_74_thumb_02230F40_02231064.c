#include "global.h"

u8 *ov74_0223105C(void);

u8 ov74_02231064(void);

u8 ov74_02231064(void) {
    return *ov74_0223105C();
}

void ov74_02231070(s8 arg0);

void ov74_02231070(s8 arg0) {
    *ov74_0223105C() = arg0;
}

u8 ov74_0223107C(void);

u8 ov74_0223107C(void) {
    return *(u8 *)((u8 *)(ov74_0223105C()) + 1);
}

void ov74_02231088(s8 arg0);

void ov74_02231088(s8 arg0) {
    *(s8 *)((u8 *)(ov74_0223105C()) + 1) = arg0;
}

u8 ov74_02231094(void);

u8 ov74_02231094(void) {
    return *(u8 *)((u8 *)(ov74_0223105C()) + 2);
}

void ov74_022310A0(s8 arg0);

void ov74_022310A0(s8 arg0) {
    *(s8 *)((u8 *)(ov74_0223105C()) + 2) = arg0;
}

u8 ov74_022310AC(void);

u8 ov74_022310AC(void) {
    return *(u8 *)((u8 *)(ov74_0223105C()) + 3);
}
