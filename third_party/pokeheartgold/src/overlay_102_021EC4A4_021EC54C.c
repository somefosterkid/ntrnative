#include "global.h"

#include "error_handling.h"

extern s32 ov102_021EC820;

s32 ov102_021EC54C(u32 index);

s32 ov102_021EC54C(u32 index) {
    if (index >= 0x1BU) {
        GF_AssertFail();
    }
    return (s32) * (u8 *)(((u8 *)&ov102_021EC820) + 7 + (index * 0xA));
}
