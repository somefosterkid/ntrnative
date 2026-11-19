#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"

void sub_0205D1FC(PlayerAvatar *param_1);

void sub_0205D1FC(PlayerAvatar *param_1) {
    BOOL BVar1;
    LocalMapObject *object;

    BVar1 = PlayerAvatar_CheckForcedMovement(param_1);
    if (BVar1 == 1) {
        object = PlayerAvatar_GetMapObject(param_1);
        MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK8 | MAPOBJECTFLAG_UNK7));
        BVar1 = PlayerAvatar_CheckFlag7(param_1);
        if (BVar1 == 0) {
            PlayerAvatar_ClearUnk24ClearFlag2(param_1);
        }
        PlayerAvatar_SetForcedMovement(param_1, 0);
        PlayerAvatar_SetFlag7(param_1, 0);
        PlayerAvatar_SetFlag5(param_1, 0);
    }
    return;
}
