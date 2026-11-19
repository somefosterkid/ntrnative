#include "global.h"

#include "sprite.h"
#include "bg_window.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_0223AD64(NintendoWfcConnect *param_1);

void ov72_0223AD94(NintendoWfcConnect *arg0);

void ov72_0223AE20(NintendoWfcConnect *arg0, u8 arg1);

void ov72_0223AD94(NintendoWfcConnect *arg0) {
    s32 v0;

    ov72_0223AD64(arg0);
    Sprite_SetDrawFlag(arg0->sprites[0], 0);
    Sprite_SetDrawFlag(arg0->sprites[1], 0);
    Sprite_SetDrawFlag(arg0->sprites[2], 0);
    Sprite_SetDrawFlag(arg0->sprites[3], 0);
    Sprite_SetDrawFlag(arg0->sprites[4], 0);
    Sprite_SetDrawFlag(arg0->sprites[5], 0);
    Sprite_SetDrawFlag(arg0->sprites[6], 0);
    Sprite_SetDrawFlag(arg0->sprites[7], 0);
    v0 = 0;
    do {
        Sprite_SetAnimCtrlSeq(arg0->sprites[v0], 0);
        v0 += 1;
    } while (v0 < 3);
}

void ov72_0223AE20(NintendoWfcConnect *arg0, u8 arg1) {
    NintendoWfcButtonFlash *v0;

    v0 = &arg0->buttonFlash;
    if (arg0->buttonFlash.isBgRect == 0) {
        Sprite_SetAnimCtrlSeq(arg0->sprites[v0->target], arg1);
        return;
    }
    BgTilemapRectChangePalette(arg0->bgConfig, v0->target, v0->x, v0->y, (u8)(s32)v0->width, (u8)(s32)v0->height, (u8)arg1);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, v0->target);
}
