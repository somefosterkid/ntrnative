#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_icon_idx.h"
#include "overlay_113.h"

void ov113_021E6AB0(UnownReportData *arg0);

void ov113_021E6AE8(UnownReportData *arg0, s32 arg1, u32 arg2);
void ov113_021E6B1C(UnownReportData *arg0);

void ov113_021E6AB0(UnownReportData *arg0) {
    ManagedSprite *managedSprite;
    Sprite *sprite;
    s32 v1;
    s32 v3;

    v3 = 0;
    do {
        managedSprite = arg0->monIcons[v3];
        if (managedSprite != NULL) {
            Sprite_DeleteAndFreeResources(managedSprite);
        }
        v3 += 1;
    } while (v3 < 0xE);
    v1 = 0;
    do {
        sprite = arg0->sprites[v1];
        if (sprite != NULL) {
            Sprite_Delete(sprite);
        }
        v1 += 1;
    } while (v1 < 3);
}

void ov113_021E6AE8(UnownReportData *arg0, s32 arg1, u32 form) {
    sub_0200E2B8(arg0->spriteSystem, arg0->spriteManager2, arg0->narc, (s32)GetMonIconNaixEx(0xC9U, 0, form), 0, arg1 + 1);
}

void ov113_021E6B1C(UnownReportData *arg0) {
    u8 v0;

    if ((arg0->numPages == 1) || (arg0->page == 0)) {
        Sprite_SetAnimCtrlSeq(arg0->sprites[1], 4);
    } else {
        Sprite_SetAnimCtrlSeq(arg0->sprites[1], 2);
    }
    v0 = arg0->numPages;
    if ((v0 == 1) || (arg0->page == (v0 - 1))) {
        Sprite_SetAnimCtrlSeq(arg0->sprites[2], 7);
        return;
    }
    Sprite_SetAnimCtrlSeq(arg0->sprites[2], 5);
}
