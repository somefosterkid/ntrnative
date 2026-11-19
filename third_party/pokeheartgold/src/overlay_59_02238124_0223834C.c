#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "sprite.h"

s32 ov59_02238FF4(Ov59ApricornApp *, s32);

s32 ov59_022390A8(Ov59ApricornApp *, s32);

s32 ov59_0223919C(Ov59ApricornApp *, s32, s32);

s32 ov59_0223834C(Ov59ApricornApp *arg0);

s32 ov59_0223834C(Ov59ApricornApp *arg0) {
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch (temp_r1) {
    case 0:
        ov59_0223919C(arg0, 0, 1);
        ov59_0223919C(arg0, 1, 1);
        Sprite_SetAnimCtrlSeq(arg0->sprites[10], 0x14);
        arg0->state = (u16) (arg0->state + 1);
        break;
    case 1:
        if (arg0->animCount == 0) {
            arg0->state = (u16) (temp_r1 + 1);
        }
        break;
    case 2:
        if (ov59_022390A8(arg0, 0) != 0) {
        default:
            ov59_02238FF4(arg0, 0);
            arg0->state = 0U;
            return 4;
        }
        break;
    }
    return 9;
}
