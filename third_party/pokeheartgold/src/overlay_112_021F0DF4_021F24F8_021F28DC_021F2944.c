#include "global.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(void *, s32, s32);

s32 ov112_021F2204(void *, s32, s32);

s32 ov112_021F22B0(void *, s32, s32);

void ov112_021F2944(Ov112SpriteApp *arg0);

void ov112_021F2944(Ov112SpriteApp *arg0) {
    s32 v0;

    if (arg0->records[arg0->curIdx]->speciesA == 0) {
        v0 = 0x46;
    } else {
        ov112_021F2204(arg0, 0, 1);
        v0 = 0x45;
    }
    ov112_021F22B0(arg0->sprites[5], (u8) arg0->playerGender, 3);
    ov112_021F1624(arg0, 0x44, v0);
}
