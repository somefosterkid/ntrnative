#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

extern void *ov48_0225B1B0;

void ov48_02259650(void **param_1);

void ov48_02259650(void **param_1) {
    s32 iVar1;
    u32 *puVar2;

    puVar2 = (u32 *)&ov48_0225B1B0;
    iVar1 = 0;
    do {
        FreeBgTilemapBuffer((BgConfig *)*param_1, (u8)*puVar2);
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 5);
    Heap_Free((void *)*param_1);
    ((u8 *)&gSystem)[0x69] = 0;
    GfGfx_SwapDisplay();
    return;
}
