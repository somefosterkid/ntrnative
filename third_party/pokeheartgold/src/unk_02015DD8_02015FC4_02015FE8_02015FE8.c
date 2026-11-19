#include "global.h"

s32 sub_02015FE8(void *arg0);

s32 sub_02015FE8(void *arg0) {
    return (*(u16 *)((u8 *)arg0 + 0xA) << 0x10) | *(u16 *)((u8 *)arg0 + 0x8);
}
