#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "unk_02005D10.h"

void sub_0205CD70(LocalMapObject *param_1, PlayerAvatar *param_2);

s32 sub_0205DE64(u32);

s32 sub_02060FA8(LocalMapObject *, s32);

s32 sub_02062390(u32 movementAction);

void sub_0205CC94(PlayerAvatar *param_1);

void sub_0205CC94(PlayerAvatar *param_1) {
    s32 arg1;
    u8 tile_00;
    u32 uVar2;
    LocalMapObject *object;
    BOOL BVar4;
    u16 uVar1;
    uVar2 = PlayerAvatar_GetMoveState(param_1);
    if (uVar2 == 1) {
        u8 tile;
        object = PlayerAvatar_GetMapObject(param_1);
        uVar1 = sub_0205F504(object);
        tile = (u8)uVar1;
        arg1 = sub_02062390(MapObject_GetMovementCommand(object));
        if (arg1 == (-1)) {
            tile_00 = tile;
        } else {
            tile_00 = sub_02060FA8(object, arg1);
        }
        BVar4 = MetatileBehavior_IsPuddle(tile);
        if (BVar4 == 1) {
            PlaySE(0x640);
        }
        BVar4 = MetatileBehavior_IsShallowWater(tile);
        if (BVar4 == 1) {
            PlaySE(0x641);
        }
        MetatileBehavior_IsSand(tile);
        BVar4 = MetatileBehavior_IsMud(tile);
        if (BVar4 == 1) {
            PlaySE(0x654);
        }
        uVar2 = MapObject_GetMovementCommand(object);
        PlayerAvatar_GetState(param_1);
        arg1 = sub_0205DE64(uVar2);
        if (arg1 == 0) {
            BVar4 = MetatileBehavior_IsVeryTallGrass(tile);
            if ((BVar4 == 1) || (BVar4 = MetatileBehavior_IsVeryTallGrass(tile_00), BVar4 == 1)) {
                PlaySE(0x652);
            }
            BVar4 = MetatileBehavior_IsTallGrass(tile);
            if ((BVar4 == 1) || (BVar4 = MetatileBehavior_IsTallGrass(tile_00), BVar4 == 1)) {
                PlaySE(0x877);
            }
        }
        sub_0205CD70(object, param_1);
    }
    return;
}
