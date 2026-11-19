#include "global.h"

#include "player_avatar.h"

s32 ov01_021F2378(s32, void *, PlayerAvatar *);

s32 sub_0205DA34(PlayerAvatar *, LocalMapObject *, s32);

s32 ov01_021F232C(void *arg0, PlayerAvatar *arg1, s32 arg2, s32 arg3);

s32 ov01_021F232C(void *arg0, PlayerAvatar *avatar, s32 arg2, s32 arg3) {
    s32 v0;

    if ((arg3 != 1) || (PlayerAvatar_GetState(avatar) != 2)) {
        return 0;
    }
    v0 = sub_0205DA34(avatar, PlayerAvatar_GetMapObject(avatar), arg2);
    if (v0 == 0x20) {
        return 0;
    }
    if (v0 & ~0x20) {
        return 0;
    }
    ov01_021F2378(arg2, arg0, avatar);
    return 1;
}
