#include "global.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "field_system.h"

u32 _GetCoordsOfFacingTile(FieldSystem *param_1, u32 *param_2, u32 *param_3);

u32 _GetCoordsOfFacingTile(FieldSystem *param_1, u32 *param_2, u32 *param_3) {
    u32 uVar1;
    u32 uVar2;

    uVar1 = PlayerAvatar_GetFacingDirection(param_1->playerAvatar);
    uVar2 = PlayerAvatar_GetXCoord(param_1->playerAvatar);
    *param_2 = uVar2;
    uVar2 = PlayerAvatar_GetZCoord(param_1->playerAvatar);
    *param_3 = uVar2;
    switch (uVar1) {
    case 0:
        *param_3 = *param_3 - 1;
        break;
    case 1:
        *param_3 = *param_3 + 1;
        break;
    case 2:
        *param_2 = *param_2 - 1;
        break;
    case 3:
        *param_2 = *param_2 + 1;
    }
    return uVar1;
}
