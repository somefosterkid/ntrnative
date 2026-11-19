#include "global.h"

#include "player_avatar.h"

u32 sub_0205D778();

s32 sub_0205D7AC();

s32 sub_0205D75C(PlayerAvatar *arg0, int dir);

s32 sub_0205D75C(PlayerAvatar *avatar, int dir) {
    s32 v0;

    v0 = sub_0205D7AC();
    PlayerAvatar_SetMoveState(avatar, sub_0205D778());
    return v0;
}
