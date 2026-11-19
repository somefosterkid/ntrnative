#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FE780.h"
#include "player_avatar.h"

s32 ov01_021F3084(PlayerAvatar *, u32);

void ov01_021F1BC0(PlayerAvatar *arg0);

void ov01_021F1BC0(PlayerAvatar *avatar) {
    LocalMapObject *localMapObject;
    OverworldAnimManager *a0;
    u32 dir;

    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(2, PlayerAvatar_GetGender(avatar)));
    PlayerAvatar_SetState(avatar, 2);
    PlayerAvatar_ClearUnk24ClearFlag2(avatar);
    a0 = PlayerAvatar_GetUnk34(avatar);
    if (a0 != 0) {
        ov01_021F1640(a0);
    }
    PlayerAvatar_SetUnk34(avatar, NULL);
    dir = PlayerAvatar_GetFacingDirection(avatar);
    localMapObject = PlayerAvatar_GetMapObject(avatar);
    MapObject_GetFieldSystem(localMapObject);
    PlayerAvatar_SetUnk34(avatar, ov01_021FE7DC(localMapObject, 0, 0, dir, 1));
}

void ov01_021F1C30(PlayerAvatar *arg0);

void ov01_021F1C30(PlayerAvatar *avatar) {
    OverworldAnimManager *a0;

    ov01_021F3084(avatar, PlayerAvatar_GetSpriteByStateAndGender(3, PlayerAvatar_GetGender(avatar)));
    PlayerAvatar_SetState(avatar, 3);
    PlayerAvatar_ClearUnk24ClearFlag2(avatar);
    a0 = PlayerAvatar_GetUnk34(avatar);
    if (a0 != 0) {
        ov01_021F1640(a0);
    }
    PlayerAvatar_SetUnk34(avatar, NULL);
}
