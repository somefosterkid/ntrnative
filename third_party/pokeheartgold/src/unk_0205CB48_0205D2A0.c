#include "global.h"

#include "unk_0205CB48.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01_022053EC.h"

u32 sub_0205D2A0(PlayerAvatar *param_1, s32 param_2);

s32 sub_0206234C(s32, s32);
void sub_0205D2D0(PlayerAvatar *arg0, s32 arg1);

u32 sub_0205D2A0(PlayerAvatar *param_1, s32 param_2) {
    s32 param1;

    param1 = PlayerAvatar_GetUnk24(param_1);
    if (param_2 == 1) {
        param1 = param1 + -1;
        if (param1 < 0) {
            return 0;
        }
    } else if ((param_2 == 2) && (param1 = param1 + 1, 3 < param1)) {
        param1 = 3;
    }
    PlayerAvatar_SetUnk24(param_1, param1);
    return 1;
}

void sub_0205D2D0(PlayerAvatar *avatar, s32 arg1) {
    FieldSystem *fieldSystem;
    LocalMapObject *object;
    s32 v0;
    s32 a0;
    s32 v1;
    u32 x;

    object = PlayerAvatar_GetMapObject(avatar);
    v1 = 0x10;
    v0 = PlayerAvatar_GetUnk24(avatar);
    switch (v0) {
    case 1:
        v1 = 0x50;
        break;
    case 2:
        v1 = 0x14;
        break;
    case 3:
        v1 = 0x14;
        break;
    }
    a0 = sub_0206234C(arg1, v1);
    sub_0205DA1C(avatar, object, a0);
    fieldSystem = MapObject_GetFieldSystem(object);
    if (FollowMon_IsActive(fieldSystem) != 0) {
        x = MapObject_GetPreviousXCoord(object);
        ov01_02205990(a0, (s32)x, (s32)MapObject_GetPreviousZCoord(object), &fieldSystem->followMon);
    }
}
