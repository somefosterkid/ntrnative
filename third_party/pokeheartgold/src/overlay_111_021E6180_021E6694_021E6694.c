#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "heap.h"
#include "overlay_111.h"
#include "sprite.h"
#include "sprite_system.h"

void ov111_021E6694(SwapMonPanel *panel) {
    ManagedSprite *sprite;
    s32 i;
    Window *window;

    if (panel == NULL) {
        GF_AssertFail();
    }
    for (i = 0; i < 2; i++) {
        sprite = panel->sprites[i];
        if (sprite != NULL) {
            Sprite_DeleteAndFreeResources(sprite);
            panel->sprites[i] = NULL;
        }
    }
    i = 0;
    window = panel->windows;
    for (; i < 6; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
    Heap_Free(panel);
}
