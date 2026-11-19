#include "global.h"
#include "overlay_83_battle_castle_self_app.h"
#include "touchscreen_list_menu.h"

u8 ov83_0223FC48(BattleCastleSelfApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov83_0224777C(SaveData *saveData, u8 challengeType, u8 arg2);

void ov83_02242E88(BattleCastleSelfApp *arg0);

void ov83_02242E88(BattleCastleSelfApp *arg0) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r2;
    u8 temp_r2;

    temp_r2 = arg0->touchMenu->cursorPos;
    if (arg0->listSelection != temp_r2) {
        temp_r0 = arg0->listMenuItems[temp_r2].value;
        switch (temp_r0) {
        case 6:
            var_r2 = 0x30;
            break;
        case 7:
            var_r2 = 0x31;
            break;
        case 8:
            temp_r0_2 = ov83_0224777C(arg0->saveData, arg0->challengeType, 1);
            if (temp_r0_2 == 3) {
                var_r2 = 0x34;
            } else if (temp_r0_2 == 1) {
                var_r2 = 0x32;
            } else {
                var_r2 = 0x33;
            }
            break;
        default:
            var_r2 = 0x35;
            break;
        }
        ov83_0223FC48(arg0, &arg0->windows[SELF_APP_WINDOW_MSG_BOX], var_r2, 1, 1, 0xFF, 1, 2, 0xF, 1);
        arg0->listSelection = (s32)arg0->touchMenu->cursorPos;
    }
}
