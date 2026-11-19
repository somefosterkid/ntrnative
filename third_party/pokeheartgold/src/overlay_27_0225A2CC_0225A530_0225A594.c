#include "global.h"
struct Ov27Task;

#include "field/field_control.h"

#include "field_system.h"
#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "unk_0203DB6C.h"
#include "overlay_27_ov27task.h"

s32 ov01_021F6BD0(s32 arg0);

u32 ov27_0225BD44(void *arg0);

s32 ov01_021F6BB0(u32 param_1);

u32 ov27_0225A594(Ov27Task *arg0);

u32 ov27_0225A594(Ov27Task *arg0) {
    LocalMapObject *sp0;
    FieldSystem *temp_r4;
    LocalMapObject *temp_r0;
    u32 temp_r6;
    u32 var_r0;
    u32 var_r5;

    temp_r4 = arg0->fieldSystem;
    if (ov27_0225BD44(temp_r4) != 0) {
        return 4U;
    }
    if (FieldSystem_IsPlayerMovementAllowed(temp_r4) == 0) {
        temp_r0 = PlayerAvatar_GetMapObject(FieldSystem_GetPlayerAvatar(temp_r4));
        temp_r6 = sub_0205F330(temp_r0);
        if ((u32)(MapObject_GetSpriteID(temp_r0) - 0xBC) <= 1U) {
            if (temp_r6 == 1) {
                return 3U;
            }
            return 4U;
        }
        var_r0 = arg0->unk_510;
        if (var_r0 == 4) {
            goto block_8;
        }
        return var_r0;
    }
block_8:
    var_r5 = ov01_021E7F54(temp_r4);
    if (var_r5 == 1) {
        FieldSystem_GetFacingObject(temp_r4, &sp0);
        if (ov01_021F6BD0((s32)MapObject_GetScriptID(sp0)) == 0) {
            if (ov01_021F6BB0(MapObject_GetSpriteID(sp0)) != 0) {
                goto block_11;
            }
        } else {
block_11:
            var_r5 = 0;
        }
    }
    var_r0 = var_r5;
    return var_r0;
}
