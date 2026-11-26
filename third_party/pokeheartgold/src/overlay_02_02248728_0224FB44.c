#include "global.h"

#include "metatile_behavior.h"
#include "overlay_01.h"
#include "dialog_box.h"
#include "player_data.h"
#include "pm_string.h"

s32 ov02_0224FB44(FieldSystemUnkSub120 *arg0, FieldSystemUnkSub120_Sub7E4 *arg1);

s32 FollowMon_ExpandInteractionMessage(void *, String *, s32, s32);
s32 FollowMon_TryPrintInteractionMessage(FieldSystem *arg0, FieldSystemUnkSub120 *arg1, FieldSystemUnkSub120_Sub7E4 *arg2);
s32 ov02_0224FCE0(void *, String *, s32, s32, u8);
void ov02_0224FC08(FieldSystem *arg0, FieldSystemUnkSub120 *arg1, s32 arg2);

s32 ov02_0224FB44(FieldSystemUnkSub120 *arg0, FieldSystemUnkSub120_Sub7E4 *arg1) {
    if (arg1->unk0 != 0) {
        return 1;
    }
    return 0;
}

s32 ov02_0224FB54(FieldSystem *arg0, FieldSystemUnkSub120 *arg1, FieldSystemUnkSub120_Sub7E4 *arg2);

s32 ov02_0224FB54(FieldSystem *fieldSystem, FieldSystemUnkSub120 *arg1, FieldSystemUnkSub120_Sub7E4 *arg2) {
    u8 v0;

    if (sub_0205BB04((u8)fieldSystem->unk120->unk882) != 0) {
        return 0;
    }
    v0 = arg2->unk6;
    if (v0 != 0) {
        if ((u32)v0 > 0xEU) {
            return 0;
        }
        ov01_02203AB4(fieldSystem, fieldSystem->followMon.mapObject, v0 - 1);
        return 1;
    }
    return 0;
}

s32 FollowMon_TryPrintInteractionMessage(FieldSystem *fieldSystem, FieldSystemUnkSub120 *app, FieldSystemUnkSub120_Sub7E4 *arg2) {
    Options *options;

    if (arg2->unk2 != 0) {
        app->string = String_New(0x400U, HEAP_ID_FIELD2);
        DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &app->window, GF_BG_LYR_MAIN_3);
        FollowMon_ExpandInteractionMessage(fieldSystem, app->string, 0xB, arg2->unk2 - 1);
        options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        DialogBox_LoadFrame(&app->window, options);
        app->unk86E = (s16)DialogBox_PrintMessage(&app->window, app->string, options, 1);
        fieldSystem->textbox_open = 1;
        return 1;
    }
    return 0;
}

void ov02_0224FC08(FieldSystem *fieldSystem, FieldSystemUnkSub120 *app, s32 arg2) {
    Options *options;

    app->string = String_New(0x400U, HEAP_ID_FIELD2);
    DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &app->window, GF_BG_LYR_MAIN_3);
    ov02_0224FCE0(fieldSystem, app->string, 0xB, arg2, app->unk816);
    options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    DialogBox_LoadFrame(&app->window, options);
    app->unk86E = (s16)DialogBox_PrintMessage(&app->window, app->string, options, 1);
    fieldSystem->textbox_open = 1;
}
