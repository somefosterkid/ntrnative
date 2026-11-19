#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "sprite.h"
#include "unk_02005D10.h"

s32 ov59_022385AC(Ov59ApricornApp *arg0);

void ov59_02238768(Ov59ApricornApp *arg0);

s32 ov59_0223892C(Ov59ApricornApp *, s32);

s32 ov59_02238C40(Ov59ApricornApp *, s32);

s32 ov59_02238CFC(Ov59ApricornApp *, s32, s32);

s32 ov59_02238D74(Ov59ApricornApp *);

s32 ov59_02238FF4(Ov59ApricornApp *, s32);

s32 ov59_022390A8(Ov59ApricornApp *, s32);

s32 ov59_0223919C(Ov59ApricornApp *, s32, s32);

s32 sub_02031C30(SaveApricornBox *, s32, s32);

s32 ov59_022383B4(Ov59ApricornApp *arg0);

s32 ov59_022383B4(Ov59ApricornApp *arg0) {
    u16 temp_r1;

    temp_r1 = arg0->state;
    switch ((u32)temp_r1) {
    case 0:
        if ((u32) arg0->unk_18 >= 5U) {
            ov59_02238C40(arg0, 7);
            ov59_02238CFC(arg0, 0x1A, 0xFF);
            arg0->state = 1U;
        } else {
            arg0->unk_49 = 2;
            arg0->unk_4A = 0;
            arg0->state = 2U;
        }
        break;
    case 1:
        if (ov59_022385AC(arg0) != 0) {
            ov59_02238D74(arg0);
            arg0->state = 0U;
            return 9;
        }
        break;
    case 2:
        if (ov59_022390A8(arg0, 0) != 0) {
            Sprite_SetDrawFlag(arg0->sprites[arg0->selected + 2], 0);
            Sprite_SetAnimCtrlSeq(arg0->sprites[10], 0x16);
            Sprite_ResetAnimCtrlState(arg0->sprites[10]);
            arg0->state = (u16)(arg0->state + 1);
        }
        break;
    case 3:
        if (Sprite_IsAnimated(arg0->sprites[10]) == 0) {
            PlaySE(0x8E6U);
            Sprite_SetAnimCtrlSeq(arg0->sprites[10], 0x14);
            sub_02031C30(arg0->apricornBox, arg0->selected, arg0->unk_38);
            ov59_02238768(arg0);
            ov59_0223919C(arg0, 1, 0);
            arg0->state = (u16)(arg0->state + 1);
        }
        break;
    case 4:
        if (arg0->animCount == 0) {
            if ((u32) arg0->unk_18 >= 5U) {
                ov59_02238C40(arg0, 7);
            } else {
                ov59_02238C40(arg0, 6);
            }
            ov59_0223892C(arg0, arg0->selected);
            ov59_02238FF4(arg0, 0);
            arg0->state = 0U;
            return 4;
        }
        break;
    }
    return 0xA;
}
