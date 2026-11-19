#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "unk_02054648.h"
#include "unk_0205CB48.h"
#include "unk_0205FD20.h"
#include "sys_task.h"

u32 sub_0205DCFC(PlayerAvatar *param_1, LocalMapObject *param_2, s32 param_3);

enum FaceDirection {
    FACE_NONE = -1,
    FACE_UP = 0,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
};
enum FaceDirection sub_0205DDD4(PlayerAvatar *playerAvatar, u16 pressedKeys, u16 heldKeys);
enum FaceDirection sub_0205DD9C(u16 pad);
enum FaceDirection sub_0205DDB8(u16 pad);

u32 sub_0205DCFC(PlayerAvatar *param_1, LocalMapObject *param_2, s32 param_3) {
    u8 tile;
    s32 sVar1;
    FieldSystem *fieldSystem;
    u32 uVar2;
    s32 iVar3;
    u32 uVar4;
    s32 iVar5;
    BOOL BVar6;

    if ((param_3 != -1) && (sVar1 = PlayerAvatar_GetState(param_1), sVar1 == 1)) {
        fieldSystem = MapObject_GetFieldSystem(param_2);
        uVar2 = MapObject_GetXCoord(param_2);
        iVar3 = GetDeltaXByFacingDirection(param_3);
        uVar4 = MapObject_GetZCoord(param_2);
        iVar5 = GetDeltaYByFacingDirection(param_3);
        tile = GetMetatileBehavior(fieldSystem, uVar2 + iVar3, uVar4 + iVar5);
        BVar6 = sub_02060E54(param_2, (u32)tile);
        if (BVar6 != 0) {
            return 1;
        }
        BVar6 = sub_02060EBC(param_2, (u32)tile);
        if (BVar6 != 0) {
            return 1;
        }
        BVar6 = MetatileBehavior_IsVeryTallGrass(tile);
        if (BVar6 != 0) {
            return 1;
        }
        BVar6 = MetatileBehavior_IsMud(tile);
        if (BVar6 != 0) {
            return 1;
        }
    }
    return 0;
}

int sub_0205DD94(PlayerAvatar *avatar, u16 newKeys, u16 heldKeys) {
    return sub_0205DDD4(avatar, newKeys, heldKeys);
}

enum FaceDirection sub_0205DD9C(u16 pad) {
    if (pad & 0x0020) {
        return FACE_LEFT;
    }

    if (pad & 0x0010) {
        return FACE_RIGHT;
    }

    return FACE_NONE;
}

enum FaceDirection sub_0205DDB8(u16 pad) {
    if (pad & 0x0040) {
        return FACE_UP;
    }

    if (pad & 0x0080) {
        return FACE_DOWN;
    }

    return FACE_NONE;
}
