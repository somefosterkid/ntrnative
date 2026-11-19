#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "sprite.h"
#include "sprite_system.h"

u32 ov59_022390A8(Ov59ApricornApp *param_1, s32 param_2);

u32 ov59_022390A8(Ov59ApricornApp *param_1, s32 param_2) {
    u8 bVar1;
    u16 sVar2;

    if (param_2 != 0) {
        sVar2 = param_1->subState;
        switch (sVar2) {
        case 0:
            Sprite_SetAnimCtrlSeq(param_1->sprites[1], 4);
            param_1->unk_51 = 0;
            param_1->subState = param_1->subState + 1;
            break;
        case 1:
            Sprite_OffsetPositionXY(param_1->sprites[1], 0, 2);
            Sprite_OffsetPositionXY(param_1->sprites[param_1->selected + 2], 0, -1);
            bVar1 = param_1->unk_51;
            param_1->unk_51 += 1;
            if (bVar1 >= 2) {
                Sprite_SetAnimCtrlSeq(param_1->sprites[1], 5);
                param_1->unk_51 = 0;
                param_1->subState = 0;
                return 1;
            }
            break;
        }
    } else {
        sVar2 = param_1->subState;
        switch (sVar2) {
        case 0:
            Sprite_SetAnimCtrlSeq(param_1->sprites[1], 4);
            param_1->unk_51 = 0;
            param_1->subState = param_1->subState + 1;
            break;
        case 1:
            Sprite_OffsetPositionXY(param_1->sprites[1], 0, -2);
            bVar1 = param_1->unk_51;
            param_1->unk_51 += 1;
            if (bVar1 >= 3) {
                Sprite_SetAnimCtrlSeq(param_1->sprites[1], 3);
                param_1->unk_51 = 0;
                param_1->subState = 0;
                return 1;
            }
            break;
        }
    }
    return 0;
}
