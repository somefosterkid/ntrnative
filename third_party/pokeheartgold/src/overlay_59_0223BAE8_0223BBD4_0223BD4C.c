#include "global.h"

#include "overlay_59_juice_feed_app.h"
#include "sprite.h"

/* DS form: the typed form below compiles to different register use / address folding on mwcc */
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov59_0223BD4C(Ov59JuiceFeedApp *arg0, Ov59JuiceMon *arg1, s32 arg2) {
    s8 *state = (s8 *)&arg1->modifiers + arg2;
    Sprite **sprites = arg0->sprites;
    s32 i;
    s32 j;
    u8 lim;

    if (((s8 *)&arg1->modifiers)[arg2] == 0) {
        Sprite_SetDrawFlag(sprites[arg2 + 3], 0);
    } else {
        Sprite_SetDrawFlag(sprites[arg2 + 3], 1);
        if (*state > 0) {
            Sprite_SetAnimCtrlSeq(sprites[arg2 + 3], 8);
        } else {
            Sprite_SetAnimCtrlSeq(sprites[arg2 + 3], 9);
        }
    }
    j = arg2 * 5;
    lim = (arg1->stars.stars >> (arg2 * 3)) & 7;
    for (i = 0; i < 5; i++, j++) {
        if (i > (s32)arg1->today.stats[arg2].hi) {
            ov59_0223BE44(arg0, j, 0xFF, 0);
        } else if (i > lim) {
            ov59_0223BE44(arg0, j, 7, 1);
        } else {
            ov59_0223BE44(arg0, j, (u8)(arg1->stars.color[arg2] + 4), 1);
        }
    }
}
#else
void ov59_0223BD4C(Ov59JuiceFeedApp *app, Ov59JuiceMon *mon, s32 arg2) {
    u8 *arg0 = (u8 *)app;
    u8 *arg1 = (u8 *)mon;
    s8 *state = (s8 *)(arg1 + 0x10) + arg2;
    s32 i;
    s32 j;
    u8 lim;

    if (((s8 *)(arg1 + 0x10))[arg2] == 0) {
        Sprite_SetDrawFlag(*(Sprite **)(arg0 + (arg2 + 3) * 4 + 0x260), 0);
    } else {
        Sprite_SetDrawFlag(*(Sprite **)(arg0 + 0x260 + (arg2 + 3) * 4), 1);
        if (*state > 0) {
            Sprite_SetAnimCtrlSeq(*(Sprite **)(arg0 + 0x260 + (arg2 + 3) * 4), 8);
        } else {
            Sprite_SetAnimCtrlSeq(*(Sprite **)(arg0 + 0x260 + (arg2 + 3) * 4), 9);
        }
    }
    j = arg2 * 5;
    lim = (*(u16 *)(arg1 + 0x16) >> (arg2 * 3)) & 7;
    for (i = 0; i < 5; i++, j++) {
        if (i > (s32)((u32)(*(u16 *)(arg1 + 0x1E + arg2 * 4) << 26) >> 29)) {
            ov59_0223BE44(app, j, 0xFF, 0);
        } else if (i > lim) {
            ov59_0223BE44(app, j, 7, 1);
        } else {
            ov59_0223BE44(app, j, (u8)(arg1[arg2 + 0x18] + 4), 1);
        }
    }
}
#endif
