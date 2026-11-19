#include "global.h"

#include "error_handling.h"

void ov96_021F4504(u8 *param_1, u32 param_2, u32 param_3);

void ov96_021F4504(u8 *param_1, u32 param_2, u32 param_3) {
    s32 iVar1;
    if ((1 + 1) <= param_2) {
        GF_AssertFail();
    }
    if ((0xb + 1) <= param_3) {
        GF_AssertFail();
    }
    iVar1 = param_3 * 8;
    DC_FlushRange(*((void **)((param_1 + 0x30) + iVar1)), 0xc80);
    GXS_LoadOBJ(*((void **)((param_1 + 0x30) + iVar1)), *((u32 *)(param_1 + ((param_2 * 4) + 0x20))), 0xc80);
    DC_FlushRange(*((void **)((param_1 + 0x34) + iVar1)), 0x20);
    GXS_LoadOBJPltt(*((void **)((param_1 + 0x34) + iVar1)), *((u32 *)((param_1 + (param_2 * 4)) + 0x28)), 0x20);
    return;
}
