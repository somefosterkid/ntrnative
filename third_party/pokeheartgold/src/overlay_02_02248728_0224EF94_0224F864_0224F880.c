#include "global.h"

#include "filesystem.h"

void ov02_0224F880(void *arg0, s32 arg1);

void ov02_0224F880(void *arg0, s32 arg1) {
    s32 v0;
    void *v1;

    ReadWholeNarcMemberByIdPair(arg0 + 0x7E4, NARC_a_2_2_1, arg1 - 1);
    v0 = 0;
    *(s8 *)((u8 *)arg0 + 0x868) = 0;
    *(s8 *)((u8 *)arg0 + 0x869) = 0;
    *(s8 *)((u8 *)arg0 + 0x86B) = 0;
    *(u8 *)((u8 *)arg0 + 0x86C) = (u8)(*(u8 *)((u8 *)arg0 + 0x86C) & ~0xF);
    *(s8 *)((u8 *)arg0 + 0x86A) = 0;
    *(s8 *)((u8 *)arg0 + 0x86D) = 0;
    v1 = arg0;
    *(s16 *)((u8 *)arg0 + 0x86E) = 0;
loop_1:
    if (*(u16 *)((u8 *)v1 + 0x7E4) != 0xFFFF) {
        v0 += 1;
        v1 += 8;
        if (v0 < 5) {
            goto loop_1;
        }
    }
    *(u8 *)((u8 *)arg0 + 0x86C) = (u8)((*(u8 *)((u8 *)arg0 + 0x86C) & ~0xF0) | (u8)((u8)v0 * 0x10));
}
