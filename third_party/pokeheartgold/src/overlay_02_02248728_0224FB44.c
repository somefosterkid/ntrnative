#include "global.h"

#include "metatile_behavior.h"
#include "overlay_01.h"
#include "dialog_box.h"
#include "player_data.h"
#include "pm_string.h"
#include "overlay_02_follow_app.h"

s32 ov02_0224FB44(void *arg0, u16 *arg1);

s32 FollowMon_ExpandInteractionMessage(void *, String *, s32, s32);
s32 FollowMon_TryPrintInteractionMessage(void *arg0, Window *arg1, void *arg2);
s32 ov02_0224FCE0(void *, String *, s32, s32, u8);
void ov02_0224FC08(void *arg0, Window *arg1, s32 arg2);

s32 ov02_0224FB44(void *arg0, u16 *arg1) {
    if (*arg1 != 0) {
        return 1;
    }
    return 0;
}

s32 ov02_0224FB54(FieldSystem *arg0, void *arg1, void *arg2);

s32 ov02_0224FB54(FieldSystem *fieldSystem, void *arg1, void *arg2) {
    u8 v0;

    if (sub_0205BB04((u8)fieldSystem->unk120->unk882) != 0) {
        return 0;
    }
    v0 = *(u8 *)((u8 *)arg2 + 6);
    if (v0 != 0) {
        if ((u32)v0 > 0xEU) {
            return 0;
        }
        ov01_02203AB4(fieldSystem, fieldSystem->followMon.mapObject, v0 - 1);
        return 1;
    }
    return 0;
}

s32 FollowMon_TryPrintInteractionMessage(void *arg0, Window *window, void *arg2) {
    Options *options;

    if (*(u16 *)((u8 *)arg2 + 2) != 0) {
        ((Ov02FollowApp *)window)->unk_10 = String_New(0x400U, HEAP_ID_FIELD2);
        DialogBox_AddWindowToLayer3(((FieldSystem *)arg0)->bgConfig, window, GF_BG_LYR_MAIN_3);
        FollowMon_ExpandInteractionMessage(arg0, ((Ov02FollowApp *)window)->unk_10, 0xB, *(u16 *)((u8 *)arg2 + 2) - 1);
        options = Save_PlayerData_GetOptionsAddr(((FieldSystem *)arg0)->saveData);
        DialogBox_LoadFrame(window, options);
        *((s16 *)&((Ov02FollowApp *)window)->unk_86E) = (s16)DialogBox_PrintMessage(window, ((Ov02FollowApp *)window)->unk_10, options, 1);
        *(u8 *)((u8 *)arg0 + 0xD2) = (u8)(0x40 | *(u8 *)((u8 *)arg0 + 0xD2));
        return 1;
    }
    return 0;
}

void ov02_0224FC08(void *arg0, Window *window, s32 arg2) {
    Options *options;

    ((Ov02FollowApp *)window)->unk_10 = String_New(0x400U, HEAP_ID_FIELD2);
    DialogBox_AddWindowToLayer3(((FieldSystem *)arg0)->bgConfig, window, GF_BG_LYR_MAIN_3);
    ov02_0224FCE0(arg0, ((Ov02FollowApp *)window)->unk_10, 0xB, arg2, (s32) *((u8 *)&((Ov02FollowApp *)window)->unk_810[6]));
    options = Save_PlayerData_GetOptionsAddr(((FieldSystem *)arg0)->saveData);
    DialogBox_LoadFrame(window, options);
    *((s16 *)&((Ov02FollowApp *)window)->unk_86E) = (s16)DialogBox_PrintMessage(window, ((Ov02FollowApp *)window)->unk_10, options, 1);
    *(u8 *)((u8 *)arg0 + 0xD2) = (u8)(0x40 | *(u8 *)((u8 *)arg0 + 0xD2));
}
