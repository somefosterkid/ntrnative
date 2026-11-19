#include "global.h"
#include "system.h"
#include "overlay_59_apricorn_app.h"

#include "sprite.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

void ov59_02238FF4(Ov59ApricornApp *, u32);

s32 ov59_022390A8(Ov59ApricornApp *, u32);

void ov59_0223919C(Ov59ApricornApp *, u32, u32);


u32 ov59_02238218(Ov59ApricornApp *param_1);

u32 ov59_02238218(Ov59ApricornApp *param_1) {
    s32 iVar1;
    BOOL BVar2;

    switch (param_1->state) {
    case 0:
        ov59_02238FF4(param_1, 2);
        param_1->state = param_1->state + 1;
        break;
    case 1:
        iVar1 = ov59_022390A8(param_1, 1);
        if (iVar1 != 0) {
            ov59_0223919C(param_1, 0, 0);
            ov59_0223919C(param_1, 1, 0);
            param_1->state = param_1->state + 1;
        }
        break;
    case 2:
        if (param_1->animCount != 0) {
            break;
        }
        PlaySE(0x8e6);
        Sprite_SetAnimCtrlSeq(param_1->sprites[10], 0x15);
        param_1->state = param_1->state + 1;
    case 3:
        if ((gSystem.newKeys & 1) != 0) {
            param_1->state = 0;
            PlaySE(0x5ea);
            return 10;
        }
        if ((gSystem.newKeys & 2) != 0) {
            param_1->state = 0;
            PlaySE(0x682);
            return 9;
        }
        BVar2 = System_GetTouchNew();
        if (BVar2 != 0) {
            param_1->state = 0;
            PlaySE(0x682);
            if (param_1->menuInputState != 1) {
                param_1->menuInputState = MENU_INPUT_STATE_BUTTONS;
            }
            return 9;
        }
        break;
    default:
        param_1->state = 0;
        return 4;
    }
    return 7;
}
