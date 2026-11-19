#include "global.h"

#include "field/field_control.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "overlay_28.h"
#include "player_avatar.h"
#include "unk_0203DB6C.h"

s32 ov01_021F6BD0(s32 arg0);

s32 ov01_021F6BB0(u32 param_1);

extern u8 ov28_0225EB7C[];

void ov28_0225E810(DowsingMchnApp *app) {
    BOOL BVar1;
    int new_var;
    PlayerAvatar *avatar;
    LocalMapObject *object;
    u32 arg0;
    s32 sVar2;
    s32 iVar3;
    u32 uVar4;
    LocalMapObject *pLStack_10;
    if (app->fieldSystem->textbox_open == 1) {
        uVar4 = 4;
    } else {
        BVar1 = FieldSystem_IsPlayerMovementAllowed(app->fieldSystem);
        if (BVar1 == 0) {
            avatar = FieldSystem_GetPlayerAvatar(app->fieldSystem);
            object = PlayerAvatar_GetMapObject(avatar);
            uVar4 = MapObject_GetSpriteID(object);
            if (((uVar4 - (new_var = 0xBD)) + 1) <= 1) {
                uVar4 = sub_0205F330(object);
                if (uVar4 == 1) {
                    uVar4 = 3;
                } else {
                    uVar4 = 4;
                }
            } else {
                uVar4 = app->windowSel;
            }
        } else {
            uVar4 = ov01_021E7F54(app->fieldSystem);
            if (uVar4 == 1) {
                FieldSystem_GetFacingObject(app->fieldSystem, &pLStack_10);
                arg0 = MapObject_GetScriptID(pLStack_10);
                sVar2 = ov01_021F6BD0(arg0);
                if (sVar2 != 1) {
                    iVar3 = ov01_021F6BB0(MapObject_GetSpriteID(pLStack_10));
                    if (iVar3 != 1) {
                        goto code_r0x02000080;
                    }
                }
                uVar4 = 0;
            }
        }
    }
code_r0x02000080:
    if (uVar4 != app->windowSel) {
        app->windowSel = uVar4;
        ScheduleWindowCopyToVram(&app->windows[ov28_0225EB7C[app->windowSel]]);
    }

    return;
}
