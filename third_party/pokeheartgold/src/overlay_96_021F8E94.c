#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "unk_02013534.h"

void ov96_021F8E94(u32 *param_1);

void ov96_021F8E94(u32 *param_1) {
    if ((TextOBJ *)*param_1 != NULL) {
        TextOBJ_Destroy((TextOBJ *)*param_1);
        sub_02021B5C((UnkStruct_02021AC8 *)(param_1 + 1));
        *param_1 = 0;
    }
    return;
}

void *ov96_021F8EB0(enum HeapID arg0, BgConfig *arg1, void *arg2);

void *ov96_021F8EB0(enum HeapID arg0, BgConfig *bgConfig, void *arg2) {
    UnkStruct_02013910 *a0;
    Window *window;
    void *dest;

    dest = Heap_Alloc(arg0, 0xC4U);
    MI_CpuFill8(dest, 0U, 0xC4U);
    *(enum HeapID *)dest = arg0;
    *(UnkStruct_02013534 **)((u8 *)dest + 8) = FontSystem_NewInit(4, arg0);
    *(void **)((u8 *)dest + 4) = arg2;
    window = dest + 0xC;
    InitWindow(window);
    AddTextWindowTopLeftCorner(bgConfig, window, 8U, 2U, 0U, 0U);
    a0 = sub_02013910(window, *(enum HeapID *)dest);
    *(UnkStruct_02013910 **)((u8 *)window + 0x10) = a0;
    *(u32 *)((u8 *)window + 0x14) = sub_02013948(a0, NNS_G2D_VRAM_TYPE_2DSUB);
    return dest;
}
