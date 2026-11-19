#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "unk_0205FD20.h"
#include "unk_0206D494.h"

s32 sub_0205D1FC(PlayerAvatar *);

s32 sub_0205D240(PlayerAvatar *, u32);

s32 sub_0205D2A0(PlayerAvatar *, s32);

s32 sub_0205D2D0(PlayerAvatar *, u32);

s32 sub_0205DA1C(PlayerAvatar *, LocalMapObject *, s32);

s32 sub_0205DA34(PlayerAvatar *, LocalMapObject *, u32);

s32 sub_0206234C(u32, s32);

s32 sub_0205D0A8(PlayerAvatar *arg0);

s32 sub_0205D0A8(PlayerAvatar *arg0) {
    s32 sp0;
    FieldSystem *temp_r4;
    LocalMapObject *temp_r0;
    s32 temp_r7;
    u32 temp_r0_2;
    u32 temp_r0_3;

    temp_r0 = PlayerAvatar_GetMapObject(arg0);
    temp_r0_2 = MapObject_GetNextFacingDirection(temp_r0);
    sp0 = sub_0205D240(arg0, temp_r0_2);
    temp_r7 = sub_0205DA34(arg0, temp_r0, temp_r0_2);
    PlayerAvatar_SetForcedMovement(arg0, 1);
    if (temp_r7 != 0) {
        temp_r4 = MapObject_GetFieldSystem(temp_r0);
        PlayerAvatar_SetFlag1(arg0, 1);
        PlayerAvatar_SetMoveState(arg0, 0U);
        if (sub_0206D494(temp_r4) == 0) {
            sub_0205D1FC(arg0);
            return 0;
        }
        return 1;
    }
    if (sub_0205D2A0(arg0, sp0) == 0) {
        sub_0205D1FC(arg0);
        temp_r0_3 = sub_020611F4(temp_r0_2);
        if (sub_0205DA34(arg0, temp_r0, temp_r0_3) != 0) {
            return 0;
        }
        MapObject_SetFlagsBits(temp_r0, (MapObjectFlagBits)(0x180));
        sub_0205DA1C(arg0, temp_r0, sub_0206234C(temp_r0_3, 8));
        PlayerAvatar_SetFlag1(arg0, 1);
        PlayerAvatar_SetForcedMovement(arg0, 1);
        PlayerAvatar_SetUnk28Unk2C(arg0, -1, -1);
    } else {
        MapObject_SetFlagsBits(temp_r0, (MapObjectFlagBits)(0x180));
        sub_0205D2D0(arg0, temp_r0_2);
    }
    PlayerAvatar_SetMoveState(arg0, 1U);
    return 1;
}
