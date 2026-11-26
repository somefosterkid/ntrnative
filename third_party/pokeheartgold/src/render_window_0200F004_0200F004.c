#include "global.h"

#include "bg_window.h"
#include "render_text.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0200E6B4(BgConfig *, u8, u8, u8, u8, u8, u8, u16);
#else
void sub_0200E6B4(BgConfig *, u8, u8, u8, u8, u8, u8, u32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0200ECBC(BgConfig *, u8, u8, u8, u8, u8, u8, u16);
#else
void sub_0200ECBC(BgConfig *, u8, u8, u8, u8, u8, u8, u32);
#endif

void sub_0200EF84(Window *, u16, u8);

void DrawFrameAndWindow3(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num, u8 type);

void DrawFrameAndWindow3(Window *window, BOOL dont_copy_to_vram, u16 baseTile, u8 palette_num, u8 type) {
    u8 uVar1;
    u8 uVar2;
    u8 uVar3;
    u8 uVar4;
    u8 uVar5;
    u32 tile;

    tile = (u32)baseTile;
    uVar1 = GetWindowBgId(window);
    if (type <= 1) {
        uVar2 = GetWindowX(window);
        uVar3 = GetWindowY(window);
        uVar4 = GetWindowWidth(window);
        uVar5 = GetWindowHeight(window);
        sub_0200ECBC(window->bgConfig, uVar1, uVar2, uVar3, uVar4, uVar5, palette_num, tile);
        sub_0200EF84(window, tile + 0x1e & 0xffff, palette_num);
    } else {
        uVar2 = GetWindowX(window);
        uVar3 = GetWindowY(window);
        uVar4 = GetWindowWidth(window);
        uVar5 = GetWindowHeight(window);
        sub_0200E6B4(window->bgConfig, uVar1, uVar2, uVar3, uVar4, uVar5, palette_num, tile);
    }
    if (dont_copy_to_vram == 0) {
        CopyWindowToVram(window);
    }
    TextPrinter_SetDownArrowBaseTile(tile);
    return;
}
