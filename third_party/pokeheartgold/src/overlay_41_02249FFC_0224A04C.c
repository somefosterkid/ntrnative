#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task_api.h"

void ov41_0224A0D0(SysTask *task, void *data);

void ov41_0224A04C(enum NarcId arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, enum HeapID arg5);

void ov41_0224A04C(enum NarcId arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, enum HeapID arg5) {
    s32 sp0;
    NNSG2dPaletteData **temp_r0;
    u32 var_r1;
    u8 *var_r2;

    sp0 = arg1;
    temp_r0 = Heap_Alloc(arg5, 0x14U);
    var_r2 = (u8 *)temp_r0;
    var_r1 = 0x14;
    do {
        *var_r2 = 0;
        var_r2 += 1;
        var_r1 -= 1;
    } while (var_r1 != 0);
    *(void **)((u8 *)temp_r0 + 4) = GfGfxLoader_GetPlttData(arg0, sp0, temp_r0, arg5);
    *(s32 *)((u8 *)temp_r0 + 8) = arg2;
    *(s32 *)((u8 *)temp_r0 + 0xC) = arg3;
    *(s32 *)((u8 *)temp_r0 + 0x10) = arg4;
    SysTask_CreateOnVWaitQueue(ov41_0224A0D0, temp_r0, 0x80U);
}
