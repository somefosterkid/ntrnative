#include "global.h"

s32 sub_02014A78(void *, u8);

s32 sub_02014A8C();

void ov90_0225A198(void *arg0);

void ov90_0225A198(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x304) != 0) {
        sub_02014A8C();
        sub_02014A78(*(void **)((u8 *)arg0 + 0x304), 0);
    }
}
