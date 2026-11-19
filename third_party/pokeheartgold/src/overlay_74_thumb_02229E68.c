#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "overlay_manager.h"

void ov74_022359BC(void);

void ov74_02229E68(OverlayManager *param_1);

s32 ov74_0223563C(void);
s32 ov74_02235690();
s32 ov74_02235728(s32, s32, s32, s32, s32, s32);
void ov74_02229F04(void);

void ov74_02229E68(OverlayManager *param_1) {
    u32 *puVar1;
    BOOL BVar2;
    u32 *puVar3;
    Window *window;
    s32 iVar4;

    puVar1 = OverlayManager_GetData(param_1);
    ov74_022359BC();
    if ((ListMenuItem *)puVar1[0x1f] != NULL) {
        ListMenuItems_Delete((ListMenuItem *)puVar1[0x1f]);
    }
    if ((ListMenu *)puVar1[0x1e] != NULL) {
        DestroyListMenu((ListMenu *)puVar1[0x1e], (u16 *)0x0, (u16 *)0x0);
    }
    ClearWindowTilemapAndCopyToVram((Window *)(puVar1 + 6));
    RemoveWindow((Window *)(puVar1 + 6));
    BVar2 = WindowIsInUse((Window *)(puVar1 + 0x16));
    if (BVar2 != 0) {
        ClearWindowTilemapAndCopyToVram((Window *)(puVar1 + 0x16));
        RemoveWindow((Window *)(puVar1 + 0x16));
    }
    iVar4 = 0;
    puVar3 = puVar1;
    window = (Window *)(puVar1 + 10);
    do {
        if (puVar3[10] != 0) {
            ClearWindowTilemapAndCopyToVram(window);
            RemoveWindow(window);
        }
        iVar4 = iVar4 + 1;
        puVar3 = puVar3 + 4;
        window = window + 1;
    } while (iVar4 < 3);
    FreeBgTilemapBuffer((BgConfig *)*puVar1, 0);
    FreeBgTilemapBuffer((BgConfig *)*puVar1, 1);
    FreeBgTilemapBuffer((BgConfig *)*puVar1, 4);
    FreeBgTilemapBuffer((BgConfig *)*puVar1, 5);
    Heap_Free((void *)*puVar1);
    return;
}

void ov74_02229F04(void) {
    ov74_0223563C();
    ov74_02235690();
    ov74_02235728(0x71, 0xE, 0xB, 0xD, 0xC, 0);
}
