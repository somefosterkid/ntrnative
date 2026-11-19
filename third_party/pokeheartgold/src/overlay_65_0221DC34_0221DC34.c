#include "global.h"

#include "sav_chatot.h"
#include "sound_chatot.h"
#include "sprite.h"
#include "touchscreen.h"
#include "overlay_65_trade_room.h"

extern TouchscreenHitbox ov65_0221FE6C;

void ov65_0221DC34(TradeRoom *arg0);

void ov65_0221DC34(TradeRoom *arg0) {
    s32 temp_r0;
    u8 new_var;
    unsigned short temp_r0_2;
    s32 slot;

    if (arg0->selectedSlot[0] < 6) {
        temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov65_0221FE6C);
        if (((temp_r0 != (-1)) && (temp_r0 < 6)) && (arg0->monDisplayData[temp_r0].species != 0)) {
            slot = arg0->selectedSlot[0];
            if (arg0->monDisplayData[slot].isEgg == 0) {
                temp_r0_2 = arg0->monDisplayData[slot].species;
                new_var = arg0->monDisplayData[slot].form;
                if (temp_r0_2 == 0x1B9) {
                    sub_02006E4C(Save_Chatot_Get(arg0->saveData), 0U, 0x64U, 0);
                } else {
                    PlayCry(temp_r0_2, (u16)new_var);
                }
                Sprite_SetAnimActiveFlag(arg0->detailCardSprites[0], 1);
                Sprite_SetAnimCtrlSeq(arg0->detailCardSprites[0], 4);
            }
        }
    }
}
