#include "global.h"

#include "player_avatar.h"

u32 sub_0205D428(s32 moveState);

s32 sub_0205D450(PlayerAvatar *avatar, s32 dir);

s32 sub_0205D40C(PlayerAvatar *arg0, s32 dir);

s32 sub_0205D40C(PlayerAvatar *avatar, s32 dir) {
    s32 v0;

    v0 = sub_0205D450(avatar, dir);
    PlayerAvatar_SetMoveState(avatar, sub_0205D428(v0));
    return v0;
}
