#include "global.h"
struct GTSApplicationState;

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238D8C(Sprite *, int, int);

s32 ov70_02238E64(void *);

u16 ov70_02239730(void *);

s32 ov70_02239740(GTSApplicationState *, s32);

s32 ov70_02239BDC(void *, s32, s32, s32, s32);

extern s32 ov70_02245240;

extern s32 ov70_02245242;

s32 ov70_02239834(struct GTSApplicationState *arg0);

s32 ov70_02239834(GTSApplicationState *arg0) {
    s32 temp_r2;
    s32 temp_r3;
    s32 temp_r3_2;
    u16 temp_r0;

    temp_r0 = ov70_02239730(arg0);
    if (temp_r0 != -1U) {
        arg0->mainMenuSelectedOption = temp_r0;
        PlaySE(0x5DCU);
        temp_r2 = arg0->mainMenuSelectedOption;
        ov70_02238D8C(arg0->cursorSprite, *(u16 *)(&ov70_02245240 + temp_r2), *(u16 *)(&ov70_02245242 + temp_r2));
        ov70_02239740(arg0, temp_r0);
        return 3;
    }
    if (2 & gSystem.newKeys) {
        ov70_02239BDC(arg0, 7, ov70_02238E64(arg0), 0, 0xF0F);
        ov70_02238D84(arg0, 0xA, 0xC);
        Sprite_SetAnimActiveFlag(arg0->cursorSprite, 0);
    } else if (1 & gSystem.newKeys) {
        ov70_02239740(arg0, arg0->mainMenuSelectedOption);
    } else if (gSystem.newKeys & 0x40) {
        if (arg0->mainMenuSelectedOption != 0) {
            arg0->mainMenuSelectedOption -= 1;
            PlaySE(0x5DCU);
            temp_r3 = arg0->mainMenuSelectedOption;
            ov70_02238D8C(arg0->cursorSprite, *(u16 *)(&ov70_02245240 + temp_r3), *(u16 *)(&ov70_02245242 + temp_r3));
        }
    } else if ((gSystem.newKeys & 0x80) && (arg0->mainMenuSelectedOption < 2U)) {
        arg0->mainMenuSelectedOption += 1;
        PlaySE(0x5DCU);
        temp_r3_2 = arg0->mainMenuSelectedOption;
        ov70_02238D8C(arg0->cursorSprite, *(u16 *)(&ov70_02245240 + temp_r3_2), *(u16 *)(&ov70_02245242 + temp_r3_2));
    }
    return 3;
}
