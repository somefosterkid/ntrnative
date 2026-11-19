#include "global.h"

#include "player_avatar.h"

s32 sub_0205DFFC(PlayerAvatar *, u32, s32 *, s32 *);

void PlayerAvatar_GetCoordsInFront(PlayerAvatar *playerAvatar, s32 *x, s32 *z);

void PlayerAvatar_GetCoordsInFront(PlayerAvatar *playerAvatar, s32 *x, s32 *z) {
    sub_0205DFFC(playerAvatar, PlayerAvatar_GetFacingDirection(playerAvatar), x, z);
}
