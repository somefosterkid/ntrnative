#include "global.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "pm_string.h"
#include "render_window.h"
#include "system.h"

s32 FollowMon_TryPrintInteractionMessage(FieldSystem *, FieldSystemUnkSub120 *, FieldSystemUnkSub120_Sub7E4 *);

s32 ov02_0224FB44(FieldSystemUnkSub120 *, FieldSystemUnkSub120_Sub7E4 *);

s32 ov02_0224FB54(void *, void *, void *);

s32 ov02_0224FC74(void *, void *);

s32 ov02_02250004(FieldSystem *, FieldSystemUnkSub120 *, s32);

void ov02_02250504(FieldSystem *);

s32 ov02_0224F8FC(FieldSystem *arg0, FieldSystemUnkSub120 *arg1);

s32 ov02_0224F8FC(FieldSystem *arg0, FieldSystemUnkSub120 *arg1) {
    u16 temp_r7_2;
    u32 temp_r2;
    u8 temp_r2_3;
    FieldSystemUnkSub120_Sub7E4 *temp_r6;
    temp_r2 = arg1->unk86C_3;
    if (temp_r2 >= 5U) {
        ov02_02250504(arg0);
        if (arg1->unk80C != 0) {
            return 2;
        }
        if (arg1->unk816 != 0) {
            return 3;
        }
        if (arg1->unk817 != 0) {
            return 4;
        }
        return 1;
    }
    temp_r6 = &arg1->unk7E4[temp_r2];
    temp_r7_2 = temp_r6->unk0;
    if (temp_r7_2 == 0xFFFF) {
        ov02_02250504(arg0);
        if (arg1->unk80C != 0) {
            return 2;
        }
        if (arg1->unk816 != 0) {
            return 3;
        }
        if (arg1->unk817 != 0) {
            return 4;
        }
        return 1;
    }
    temp_r2_3 = arg1->unk869;
    switch ((u32)temp_r2_3) {
    case 0:
        if (ov02_0224FB44(arg1, temp_r6) != 0) {
            MapObject_UnpauseMovement(arg0->followMon.mapObject);
            arg1->unk868 = 0;
            arg1->unk869 = 5U;
            break;
        }
    case 1:
        if (ov02_0224FB54(arg0, arg1, temp_r6) != 0) {
            MapObject_UnpauseMovement(arg0->followMon.mapObject);
            arg1->unk869 = 2U;
            break;
        }
    case 2:
        if (FollowMon_TryPrintInteractionMessage(arg0, arg1, temp_r6) != 0) {
            MapObject_PauseMovement(arg0->followMon.mapObject);
            arg1->unk869 = 6U;
            break;
        }
    case 3:
        if (ov02_0224FC74(arg1, temp_r6) != 0) {
            arg1->unk869 = 7U;
            break;
        }
    case 4:
        arg1->unk86C_3 = arg1->unk86C_3 + 1;
        arg1->unk869 = 0U;
        break;

    case 5:
        if (ov02_02250004(arg0, arg1, temp_r7_2) != 0) {
            arg1->unk869 = 1U;
        }
        break;

    case 6:
        if (DialogBox_IsPrintFinished((u8)arg1->unk86E) == 1) {
            if (arg1->unk86C_4 > arg1->unk86C_3 + 1) {
                if (3 & gSystem.newKeys) {
                    ClearFrameAndWindow2(&arg1->window, 0);
                    RemoveWindow(&arg1->window);
                    String_Delete(arg1->string);
                    arg0->textbox_open = 0;
                    arg1->unk869 = 3U;
                }
            } else if (arg1->unk80C != 0) {
                arg1->unk869 = 3U;
            } else if (3 & gSystem.newKeys) {
                ClearFrameAndWindow2(&arg1->window, 0);
                RemoveWindow(&arg1->window);
                String_Delete(arg1->string);
                arg0->textbox_open = 0;
                arg1->unk869 = 3U;
            }
        }
        break;

    case 7:
        if (arg1->unk86D >= temp_r6->unk7) {
            arg1->unk86C_3 = temp_r2 + 1;
            arg1->unk869 = 0U;
        } else {
            arg1->unk86D += 1;
        }
        break;
    }

    return 0;
}
