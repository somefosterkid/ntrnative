#include "global.h"
#include "overlay_83_battle_castle_self_app.h"
#include "touchscreen_list_menu.h"

u8 ov83_0223FC48(BattleCastleSelfApp *app, Window *window, int messageID, u32 x, u32 y, u32 renderDelay, u8 textColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov83_0224777C(SaveData *saveData, u8 challengeType, u8 arg2);

void ov83_02242DFC(BattleCastleSelfApp *arg0);

void ov83_02242DFC(BattleCastleSelfApp *arg0) {
    s32 temp_r0;
    u32 temp_r1;
    s32 var_r2;
    u8 temp_r2;

    temp_r2 = arg0->touchMenu->cursorPos;
    if (arg0->listSelection != temp_r2) {
        temp_r1 = arg0->listMenuItems[temp_r2].value;
        switch (temp_r1) {
        case 4:
            temp_r0 = ov83_0224777C(arg0->saveData, arg0->challengeType, 0);
            if (temp_r0 == 3) {
                var_r2 = 0x1B;
            } else if (temp_r0 == 1) {
                var_r2 = 0x19;
            } else {
                var_r2 = 0x1A;
            }
            break;
        case -2:
            var_r2 = 0x1C;
            break;
        default:
            var_r2 = 0x18;
            break;
        }
        ov83_0223FC48(arg0, &arg0->windows[SELF_APP_WINDOW_MSG_BOX], var_r2, 1, 1, 0xFF, 1, 2, 0xF, 1);
        arg0->listSelection = (s32)arg0->touchMenu->cursorPos;
    }
}
