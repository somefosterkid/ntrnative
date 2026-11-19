#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"

s32 Field_PlayerCanSurfOnTile(PlayerAvatar *fieldPlayerAvatar, s32 standingTile, s32 facingTile);

s32 Field_PlayerCanSurfOnTile(PlayerAvatar *fieldPlayerAvatar, s32 standingTile, s32 facingTile) {
    LocalMapObject *object;

    object = PlayerAvatar_GetMapObject(fieldPlayerAvatar);
    if (MetatileBehavior_IsSurfableWater((u8)facingTile) == 1) {
        if (((sub_0205BA30((u8)standingTile) == 1) || (sub_0205BA24((u8)standingTile) == 1)) && (MapObject_CheckFlag28(object) == 1)) {
            return 0;
        }
        if (sub_0205B78C((u8)standingTile) == 1) {
            return 0;
        }
        return 1;
    }
    return 0;
}
