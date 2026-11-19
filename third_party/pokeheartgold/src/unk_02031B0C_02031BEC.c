#include "global.h"

void sub_02031BEC(void *arg0, s32 arg1);

u32 sub_02031C00(void *arg0);

void sub_02031BEC(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x1C) = (s32)((0x3FFFFFFF & *(s32 *)((u8 *)arg0 + 0x1C)) | (arg1 << 0x1E));
}

u32 sub_02031C00(void *arg0) {
    return (u32) * (u32 *)((u8 *)arg0 + 0x1C) >> 0x1E;
}
