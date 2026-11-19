#include "global.h"

#include "bg_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "touchscreen.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_0223A5B4(NintendoWfcConnect *arg0);

s32 ov72_0223A5B4(NintendoWfcConnect *arg0) {
    BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 4, 1, HEAP_ID_67);
    arg0->numberInput.state = 1;
    return 0;
}

void ov72_02238730(Sprite *, s32, u32);

s32 ov72_02238B18(NintendoWfcConnect *, s32);

static void ov72_0223A680(NintendoWfcConnect *arg0);

void ov72_0223ACA8(NintendoWfcConnect *);

void ov72_0223AD20(Window *, MsgData *, s32);

s32 ov72_0223A5E0(NintendoWfcConnect *arg0);

s32 ov72_0223A5E0(NintendoWfcConnect *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        ov72_0223A680(arg0);
        ov72_02238730(arg0->sprites[4], arg0->numberInput.digitPos, arg0->twoDigits);
        ov72_02238B18(arg0, 1);
        ov72_0223ACA8(arg0);
        ov72_0223AD20(&arg0->buttonWindows[0], arg0->msgDataTower, 0x14);
        ov72_0223AD20(&arg0->buttonWindows[1], arg0->msgDataTower, 0x15);
        ov72_0223AD20(&arg0->buttonWindows[2], arg0->msgDataTower, 0x16);
        ToggleBgLayer(4U, 1U);
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 4, 1, HEAP_ID_67);
        arg0->numberInput.state = 2;
    }
    return 0;
}

void ov72_02238750(Sprite *, s32, u32);

static void ov72_0223A680(NintendoWfcConnect *arg0) {
    s32 v0;

    if (arg0->numberInput.maxValue == 0x64) {
        Sprite_SetDrawFlag(arg0->sprites[0], 1);
    }
    Sprite_SetDrawFlag(arg0->sprites[1], 1);
    Sprite_SetDrawFlag(arg0->sprites[2], 1);
    v0 = 0;
    do {
        ov72_02238750(arg0->sprites[v0], v0, arg0->twoDigits);
        v0 += 1;
    } while (v0 < 3);
    Sprite_SetDrawFlag(arg0->sprites[3], 1);
    Sprite_SetDrawFlag(arg0->sprites[4], 1);
    Sprite_SetDrawFlag(arg0->sprites[5], 1);
    Sprite_SetDrawFlag(arg0->sprites[6], 1);
    Sprite_SetDrawFlag(arg0->sprites[7], 1);
}

s32 ov72_0223A71C(NintendoWfcConnect *arg0);

s32 ov72_0223A71C(NintendoWfcConnect *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        arg0->numberInput.state = 3;
    }
    return 0;
}

extern TouchscreenHitbox ov72_0223B774;

extern TouchscreenHitbox ov72_0223B7B8;

void ov72_0223A738(NintendoWfcConnect *arg0);

void ov72_0223A738(NintendoWfcConnect *arg0) {
    if (arg0->numberInput.maxValue == 0x64) {
        TouchscreenHitbox_FindRectAtTouchNew(&ov72_0223B774);
        return;
    }
    TouchscreenHitbox_FindRectAtTouchNew(&ov72_0223B7B8);
}
