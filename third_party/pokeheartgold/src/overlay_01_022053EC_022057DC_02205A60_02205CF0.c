#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "player_avatar.h"

u32 ov01_02205CF0(FieldSystem *param_1, void *param_2);

u32 ov01_02205CF0(FieldSystem *param_1, void *param_2) {
    LocalMapObject *pLVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    u32 uVar5;

    pLVar1 = PlayerAvatar_GetMapObject(param_1->playerAvatar);
    uVar2 = MapObject_GetXCoord(pLVar1);
    pLVar1 = PlayerAvatar_GetMapObject(param_1->playerAvatar);
    uVar3 = MapObject_GetZCoord(pLVar1);
    pLVar1 = FollowMon_GetMapObject(param_1);
    uVar4 = MapObject_GetXCoord(pLVar1);
    pLVar1 = FollowMon_GetMapObject(param_1);
    uVar5 = MapObject_GetZCoord(pLVar1);
    if ((uVar4 == uVar2) && (uVar5 == uVar3 + 1)) {
        *(u32 *)(param_2 + 0x44) = 1;
        return 2;
    }
    if ((uVar4 == uVar2 + 1) && (uVar5 == uVar3)) {
        *(u32 *)(param_2 + 0x44) = 0;
        return 3;
    }
    if ((uVar4 + 1 == uVar2) && (uVar5 == uVar3)) {
        *(u32 *)(param_2 + 0x44) = 1;
        return 3;
    }
    GF_AssertFail();
    return 2;
}
