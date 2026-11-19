#include "global.h"

#include "player_avatar.h"

s32 sub_0205DA1C(PlayerAvatar *, LocalMapObject *, s32);

void sub_0205DFD4(PlayerAvatar *arg0, s32 arg1);

void sub_0205DFD4(PlayerAvatar *avatar, s32 arg1) {
    sub_0205DA1C(avatar, PlayerAvatar_GetMapObject(avatar), arg1);
}
