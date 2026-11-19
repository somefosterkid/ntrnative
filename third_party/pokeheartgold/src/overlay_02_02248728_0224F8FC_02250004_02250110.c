#include "global.h"

#include "bg_window.h"
#include "dialog_box.h"
#include "fashion_case.h"
#include "field_system.h"
#include "map_object.h"
#include "overlay_01.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "render_window.h"
#include "save_vars_flags.h"
#include "script_pokemon_util.h"
#include "sys_flags.h"
#include "system.h"
#include "task.h"
#include "unk_02005D10.h"

void ov02_0224F880(void *arg0, s32 arg1);
void ov02_0224FC08(void *arg0, Window *window, s32 arg2);
u32 ov02_0224EF94(FieldSystem *arg0);
s32 ov02_0224F8FC(void *arg0, void *arg1);

BOOL Task_FollowMonInteract(TaskManager *task);

BOOL Task_FollowMonInteract(TaskManager *task) {
    int a;
    int b;
    int result;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(task);
    u32 *state = TaskManager_GetStatePtr(task);

    switch (*state) {
    case 0:
        ov02_0224F880(fieldSystem->unk120, ov02_0224EF94(fieldSystem));
        (*state)++;
        break;
    case 1:
        result = ov02_0224F8FC(fieldSystem, fieldSystem->unk120);
        if (result == 1) {
            MapObject_PauseMovement(fieldSystem->followMon.mapObject);
            return 1;
        } else if (result == 2) {
            fieldSystem->unk120->unk869 = 10;
            *state = 2;
        } else if (result == 3) {
            *state = 3;
        } else if (result == 4) {
            *state = 4;
        }
        break;
    case 2:
        switch (fieldSystem->unk120->unk869) {
        case 10:
            ov01_021F6A9C(fieldSystem, 3, 0);
            fieldSystem->unk120->unk869 = 11;
            break;
        case 11:
            a = ov01_021F6B00(fieldSystem);
            b = ov01_021F6B10(fieldSystem);
            if (a == 3 && b == 1) {
                ov01_021F6ABC(fieldSystem, 3, 3, &fieldSystem->unk120->unk880);
                fieldSystem->unk120->unk869 = 12;
            }
            break;
        case 12:
            a = ov01_021F6B00(fieldSystem);
            b = ov01_021F6AEC(fieldSystem);
            if (a == 3 && b == 6) {
                ov01_021F6A9C(fieldSystem, 0, 0);
                fieldSystem->unk120->unk869 = 13;
            }
            break;
        case 13:
            a = ov01_021F6B00(fieldSystem);
            b = ov01_021F6B10(fieldSystem);
            if (a == 0 && b == 1) {
                switch (fieldSystem->unk120->unk880) {
                case 0:
                    ClearFrameAndWindow2(&fieldSystem->unk120->window, 0);
                    RemoveWindow(&fieldSystem->unk120->window);
                    String_Delete(fieldSystem->unk120->string);
                    fieldSystem->textbox_open = FALSE;
                    if (fieldSystem->unk120->unk810 == 0) {
                        MapObject_PauseMovement(fieldSystem->followMon.mapObject);
                        return 1;
                    }
                    ov02_0224F880(fieldSystem->unk120, fieldSystem->unk120->unk810);
                    *state = 1;
                    break;
                case 1:
                    ClearFrameAndWindow2(&fieldSystem->unk120->window, 0);
                    RemoveWindow(&fieldSystem->unk120->window);
                    String_Delete(fieldSystem->unk120->string);
                    fieldSystem->textbox_open = FALSE;
                    if (fieldSystem->unk120->unk812 == 0) {
                        MapObject_PauseMovement(fieldSystem->followMon.mapObject);
                        return 1;
                    }
                    ov02_0224F880(fieldSystem->unk120, fieldSystem->unk120->unk812);
                    *state = 1;
                    break;
                }
            }
            break;
        }
        break;
    case 3: {
        FashionCase *fashionCase = Save_FashionData_GetFashionCase(Save_FashionData_Get(fieldSystem->saveData));
        int idx = fieldSystem->unk120->unk816 - 1;

        if (idx < 0 || idx >= 100) {
            GF_AssertFail();
        }
        if (sub_0202BA2C(fashionCase, idx, 1)) {
            FashionCase_GiveFashionItem(fashionCase, idx, 1);
            ov02_0224FC08(fieldSystem, &fieldSystem->unk120->window, 3);
            PlayFanfare(0x4A7);
        } else {
            ov02_0224FC08(fieldSystem, &fieldSystem->unk120->window, 2);
        }
        *state = 5;
        break;
    }
    case 4: {
        int monDataId;
        Pokemon *mon;
        u8 flag;

        switch (fieldSystem->unk120->unk817) {
        case 1:
            monDataId = 181;
            break;
        case 2:
            monDataId = 182;
            break;
        case 3:
            monDataId = 183;
            break;
        case 4:
            monDataId = 184;
            break;
        case 5:
            monDataId = 185;
            break;
        default:
            GF_AssertFail();
            return 1;
        }
        mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        if ((u8)GetMonData(mon, monDataId, 0) == 0) {
            SetFlag99C(Save_VarsFlags_Get(fieldSystem->saveData));
            flag = 1;
            SetMonData(mon, monDataId, &flag);
            ov02_0224FC08(fieldSystem, &fieldSystem->unk120->window, 1);
            PlayFanfare(0x4A7);
        } else {
            ov02_0224FC08(fieldSystem, &fieldSystem->unk120->window, 0);
        }
        *state = 5;
        break;
    }
    case 5:
        if (DialogBox_IsPrintFinished((u8)fieldSystem->unk120->unk86E) == 1
            && IsFanfarePlaying() == 0
            && (gSystem.newKeys & 3)) {
            ClearFrameAndWindow2(&fieldSystem->unk120->window, 0);
            RemoveWindow(&fieldSystem->unk120->window);
            String_Delete(fieldSystem->unk120->string);
            fieldSystem->textbox_open = FALSE;
            MapObject_PauseMovement(fieldSystem->followMon.mapObject);
            return 1;
        }
        break;
    }

    return 0;
}
