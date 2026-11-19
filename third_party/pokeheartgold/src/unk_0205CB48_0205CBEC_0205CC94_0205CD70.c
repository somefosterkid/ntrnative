#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "unk_02005D10.h"
#include "unk_0205CB48.h"

void sub_02005BA8(u16);

u32 sub_02060FE0(LocalMapObject *, s32);

s32 sub_02062390(u32 movementAction);

extern u8 _020FCB98;

void sub_0205CD70(LocalMapObject *param_1, PlayerAvatar *param_2);

void sub_0205CD70(LocalMapObject *param_1, PlayerAvatar *param_2) {
    u8 tile;
    u16 uVar1;
    s32 iVar2;
    u32 uVar4;
    s32 sVar5;
    u8 uVar6;
    BOOL BVar3;

    iVar2 = sub_02062390(MapObject_GetMovementCommand(param_1));
    uVar1 = sub_0205F504(param_1);
    uVar6 = (u8)uVar1;
    uVar1 = sub_0205F504(param_1);
    tile = (u8)uVar1;
    BVar3 = MetatileBehavior_IsVeryTallGrass(tile);
    if ((((BVar3 == 0) && (BVar3 = MetatileBehavior_IsPuddle(tile), BVar3 != 1)) && (BVar3 = MetatileBehavior_IsShallowWater(tile), BVar3 != 1)) && (BVar3 = MetatileBehavior_IsTallGrass(tile), BVar3 == 0)) {
        if (iVar2 != -1) {
            uVar6 = (u8)sub_02060FE0(param_1, iVar2);
        }
        if (*(u16 *)(&_020FCB98 + uVar6 * 4) == 0x876) {
            PlayerAvatar_ResetUnkC(param_2);
        } else {
            uVar4 = PlayerAvatar_GetPlayerMoveState(param_2);
            if (uVar4 == 0) {
                PlayerAvatar_ResetUnkC(param_2);
            } else {
                PlayerAvatar_ToggleUnkC(param_2);
            }
        }
        BVar3 = PlayerAvatar_GetUnkC(param_2);
        if (((BVar3 == 0) && (sVar5 = PlayerAvatar_GetState(param_2), sVar5 != 1)) && (sVar5 != 2)) {
            if (uVar6 < 0x10) {
                if ((uVar6 == 0) && (BVar3 = sub_0205DE98(param_2), BVar3 == 1)) {
                    uVar6 = 1;
                }
                iVar2 = uVar6 * 4;
                if (*(u16 *)(iVar2 + (&_020FCB98 + 0x2)) == 1) {
                    uVar1 = *(u16 *)(&_020FCB98 + iVar2);
                    sub_02006088(uVar1);
                } else {
                    uVar1 = *(u16 *)(&_020FCB98 + iVar2);
                    PlaySE(uVar1);
                }
                sub_02005BA8(uVar1);
            } else {
                if (uVar6 >= 0x10) {
                    GF_AssertFail();
                }
            }
        }
    }
    return;
}
