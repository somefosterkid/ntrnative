#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_111.h"
#include "sprite.h"
#include "sprite_system.h"

ManagedSprite *ov111_021E6330(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, s16 param_4, u8 param_5, u8 param_6);

ManagedSprite *ov111_021E6380(SpriteSystem *param_1, SpriteManager *param_2, s16 param_3, u32 param_4, u8 param_5);

extern const SwapMonPanelWindowTemplate ov111_021E6C68[6];

SwapMonPanel *ov111_021E63D0(SwapMonPanels *panels, u8 x, u8 y, s32 idx, Pokemon *mon) {
    SwapMonPanel *dest;
    u8 bgId;
    u8 yBase;
    u8 xBase;
    SpriteSystem *spriteSystem;
    const SwapMonPanelWindowTemplate *tmpl;
    Window *window;
    s32 i;

    bgId = (idx == 0) ? 2 : 3;
    dest = Heap_Alloc(panels->heapID, sizeof(SwapMonPanel));
    MI_CpuFill8(dest, 0, sizeof(SwapMonPanel));
    dest->mon = mon;
    dest->x = (s16)x;
    dest->y = (s16)y;
    dest->sprites[0] = ov111_021E6330(panels->spriteSystem, panels->spriteManager, dest->x, dest->y, 0, 1);
    spriteSystem = panels->spriteSystem;
    dest->sprites[1] = ov111_021E6380(spriteSystem, panels->spriteManager, dest->x - 0x2c, (s16)(dest->y - 8), (u8)idx);
    tmpl = ov111_021E6C68;
    yBase = (u8)(y >> 3);
    xBase = (u8)(x >> 3);
    i = 0;
    window = dest->windows;
    do {
        AddWindowParameterized(panels->bgConfig, window, bgId, xBase + tmpl->x, yBase + tmpl->y, tmpl->width, tmpl->height, tmpl->paletteNum, tmpl->baseTile);
        i = i + 1;
        tmpl = tmpl + 1;
        window = window + 1;
    } while (i < 6);
    return dest;
}
