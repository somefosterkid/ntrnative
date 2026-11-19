#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "unk_02009D48.h"

void ov96_021F87D0(void *param_1);

void ov96_021F87D0(void *param_1) {
    SpriteList_RenderAndAnimateSprites(*(SpriteList **)(param_1 + 0x17c));
    return;
}

void ov96_021F8980(enum HeapID *param_1);

void ov96_021F8A50(void *param_1);

void ov96_021F8AFC(u32 *param_1, s32 param_2, u32 *param_3);

void ov96_021F8DD4(enum HeapID *param_1);

void ov96_021F87E0(void *param_1, s32 param_2);

void ov96_021F87E0(void *param_1, s32 param_2) {
    u32 uVar1;
    u32 *puVar2;

    ov96_021F8980((enum HeapID *)param_1);
    ov96_021F8A50(param_1);
    uVar1 = 0;
    puVar2 = (u32 *)(param_1 + 0x14);
    do {
        ov96_021F8AFC((u32 *)param_1, uVar1 & 0xff, puVar2);
        uVar1 = uVar1 + 1;
        puVar2 = puVar2 + 0x12;
    } while ((s32)uVar1 < 5);
    ov96_021F8DD4((enum HeapID *)param_1);
    G2dRenderer_SetSubSurfaceCoords((G2dRenderer *)(param_1 + 0x180), 0, 0x12c000);
    G2dRenderer_SetMainSurfaceCoords((G2dRenderer *)(param_1 + 0x180), 0, 0);
    return;
}
