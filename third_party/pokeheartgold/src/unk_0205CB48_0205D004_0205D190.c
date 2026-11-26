#include "global.h"

#include "unk_0205CB48.h"

#include "player_avatar.h"
#include "pokemon_types_def.h"


s32 sub_0205DA34(PlayerAvatar *, LocalMapObject *, s32);

s32 sub_0206234C(s32, s32);

s32 sub_0205D190(PlayerAvatar *arg0);

s32 sub_0205D190(PlayerAvatar *avatar) {
    LocalMapObject *localMapObject;

    localMapObject = PlayerAvatar_GetMapObject(avatar);
    if (sub_0205DA34(avatar, localMapObject, 1) == 0) {
        sub_0205DA1C(avatar, localMapObject, sub_0206234C(1, 0x14));
        PlayerAvatar_SetMoveState(avatar, 1U);
        PlayerAvatar_SetUnk24(avatar, 3);
        return 1;
    }
    sub_0205DA1C(avatar, localMapObject, sub_0206234C(1, 1));
    PlayerAvatar_ClearUnk24ClearFlag2(avatar);
    PlayerAvatar_SetMoveState(avatar, 0U);
    PlayerAvatar_SetFlag2(avatar, 0);
    return 1;
}
