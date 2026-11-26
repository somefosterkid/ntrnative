#include "global.h"

#include "player_avatar.h"

int sub_0205D778(int bikeState);

s32 sub_0205D7AC(PlayerAvatar *playerAvatar, int dir);

s32 sub_0205D75C(PlayerAvatar *arg0, int dir);

s32 sub_0205D75C(PlayerAvatar *avatar, int dir) {
    s32 v0;

    v0 = sub_0205D7AC(avatar, dir);
    PlayerAvatar_SetMoveState(avatar, sub_0205D778(v0));
    return v0;
}
