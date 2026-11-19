#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "unk_02054648.h"
#include "unk_0205FD20.h"

u32 sub_0205DB68(void *param_1, LocalMapObject *param_2, s32 param_3);

u32 sub_0205DB68(void *param_1, LocalMapObject *param_2, s32 param_3) {
    u8 tile;
    FieldSystem *fieldSystem;
    u32 uVar1;
    s32 iVar2;
    u32 uVar3;
    s32 iVar4;
    BOOL BVar5;

    if (param_3 != -1) {
        fieldSystem = MapObject_GetFieldSystem(param_2);
        uVar1 = MapObject_GetXCoord(param_2);
        iVar2 = GetDeltaXByFacingDirection(param_3);
        uVar3 = MapObject_GetZCoord(param_2);
        iVar4 = GetDeltaYByFacingDirection(param_3);
        tile = GetMetatileBehavior(fieldSystem, uVar1 + iVar2, uVar3 + iVar4);
        switch (param_3) {
        case 0:
            BVar5 = MetatileBehavior_IsJumpNorth(tile);
            if (BVar5 == 1) {
                return 1;
            }
            break;
        case 1:
            BVar5 = MetatileBehavior_IsJumpSouth(tile);
            if (BVar5 == 1) {
                return 1;
            }
            break;
        case 2:
            BVar5 = MetatileBehavior_IsJumpWest(tile);
            if (BVar5 == 1) {
                return 1;
            }
            break;
        case 3:
            BVar5 = MetatileBehavior_IsJumpEast(tile);
            if (BVar5 == 1) {
                return 1;
            }
        }
    }
    return 0;
}

u32 sub_0205DBF4(void *param_1, LocalMapObject *param_2, s32 param_3);

u32 sub_0205DBF4(void *param_1, LocalMapObject *param_2, s32 param_3) {
    u8 uVar1;
    FieldSystem *fieldSystem;
    u32 x;
    u32 z;
    s32 iVar2;
    s32 iVar3;
    BOOL BVar4;

    if (param_3 != -1) {
        fieldSystem = MapObject_GetFieldSystem(param_2);
        x = MapObject_GetXCoord(param_2);
        z = MapObject_GetZCoord(param_2);
        uVar1 = GetMetatileBehavior(fieldSystem, x, z);
        switch (param_3) {
        case 0:
            BVar4 = MetatileBehavior_IsWarpEntranceNorth(uVar1);
            if (BVar4 == 1) {
                return 1;
            }
        case 1:
            BVar4 = MetatileBehavior_IsWarpEntranceSouth(uVar1);
            if (BVar4 == 1) {
                return 1;
            }
        case 2:
            BVar4 = MetatileBehavior_IsWarpEntranceWest(uVar1);
            if (BVar4 == 1) {
                return 1;
            }
        case 3:
            BVar4 = MetatileBehavior_IsWarpEntranceEast(uVar1);
            if (BVar4 == 1) {
                return 1;
            }
        }
        iVar2 = GetDeltaXByFacingDirection(param_3);
        iVar3 = GetDeltaYByFacingDirection(param_3);
        uVar1 = GetMetatileBehavior(fieldSystem, x + iVar2, z + iVar3);
        BVar4 = MetatileBehavior_IsDoor(uVar1);
        if (BVar4 == 1) {
            return 1;
        }
    }
    return 0;
}
