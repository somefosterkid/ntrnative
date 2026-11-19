#include "global.h"

#include "map_object.h"
#include "player_avatar.h"

s32 sub_0205DA1C(PlayerAvatar *, LocalMapObject *, s32);

s32 sub_0206234C(u32, s32);

void sub_0205D610(PlayerAvatar *arg0, LocalMapObject *arg1, u32 arg2, u16 keyPad, u16 keyPress);

void sub_0205D610(PlayerAvatar *avatar, LocalMapObject *object, u32 direction, u16 keyPad, u16 keyPress) {
    sub_0205DA1C(avatar, object, sub_0206234C(direction, 0x28));
    MapObject_SetNextFacingDirection(object, direction);
    PlayerAvatar_ResetUnkC(avatar);
    PlayerAvatar_ToggleUnkC(avatar);
}
