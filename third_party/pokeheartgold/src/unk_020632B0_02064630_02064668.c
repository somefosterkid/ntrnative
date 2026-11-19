#include "global.h"

#include "approaching_trainer_data.h"
#include "error_handling.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_022001E4.h"
#include "player_avatar.h"
#include "unk_02062108.h"
#include "unk_020689C8.h"

u32 sub_0206234C(s32, s32);
s32 sub_02061200(u32, u32, u32, u32);

s32 sub_02064668(ApproachingTrainerData *data);
s32 sub_02064694(ApproachingTrainerData *data);
s32 sub_020646DC(ApproachingTrainerData *data);
s32 sub_02064714(ApproachingTrainerData *data);
s32 sub_02064730(ApproachingTrainerData *data);
s32 sub_02064748(ApproachingTrainerData *data);
s32 sub_02064764(ApproachingTrainerData *data);
s32 sub_02064778(ApproachingTrainerData *data);
s32 sub_02064790(ApproachingTrainerData *data);
s32 sub_020647A8(ApproachingTrainerData *data);
s32 sub_020647C0(ApproachingTrainerData *data);
s32 sub_020647E8(ApproachingTrainerData *data);
s32 sub_02064808(ApproachingTrainerData *data);
s32 sub_02064824(ApproachingTrainerData *data);
s32 sub_020648A0(ApproachingTrainerData *data);
s32 sub_020648C8(ApproachingTrainerData *data);
s32 sub_020648E4(ApproachingTrainerData *data);

s32 sub_02064668(ApproachingTrainerData *data) {
    LocalMapObject *object;
    object = data->mapObj;
    if (MapObject_CheckSingleMovement(object) == 1) {
        return 0;
    }
    ov01_021F9408(data->mapObj, data->direction);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_MOVEMENT_PAUSED);
    data->state = 2;
    return 1;
}

s32 sub_02064694(ApproachingTrainerData *data) {
    u32 v0;
    if (MapObject_IsMovementPaused(PlayerAvatar_GetMapObject(data->playerAvatar)) == 0) {
        return 0;
    }
    v0 = MapObject_GetMovement(data->mapObj) - 0x33;
    switch (v0) {
    case 0:
    case 1:
    case 2:
    case 3:
        data->state = 7;
        return 1;
    default:
        data->state = 3;
        return 1;
    }
}

s32 sub_020646DC(ApproachingTrainerData *data) {
    if (MapObject_AreBitsSetForMovementScriptInit(data->mapObj) == 0) {
        return 0;
    }
    if (data->direction == -1) {
        GF_AssertFail();
    }
    MapObject_SetHeldMovement(data->mapObj, sub_0206234C(data->direction, 0));
    data->state = 4;
    return 0;
}

s32 sub_02064714(ApproachingTrainerData *data) {
    if (MapObject_IsMovementPaused(data->mapObj) == 0) {
        return 0;
    }
    data->state = 5;
    return 1;
}

s32 sub_02064730(ApproachingTrainerData *data) {
    data->animMan = ov01_02200540(data->mapObj, 0, 0);
    data->state = 6;
    return 0;
}

s32 sub_02064748(ApproachingTrainerData *data) {
    if (ov01_022003F4(data->animMan) == 1) {
        sub_02068B48(data->animMan);
        data->state = 9;
    }
    return 0;
}

s32 sub_02064764(ApproachingTrainerData *data) {
    MapObject_SetHeldMovement(data->mapObj, 0x65U);
    data->state = 8;
    return 0;
}

s32 sub_02064778(ApproachingTrainerData *data) {
    if (MapObject_IsMovementPaused(data->mapObj) == 1) {
        data->state = 9;
    }
    return 0;
}

s32 sub_02064790(ApproachingTrainerData *data) {
    s32 v0;
    v0 = data->delay + 1;
    data->delay = v0;
    if (v0 >= 0x1E) {
        data->delay = 0;
        data->state = 0xA;
    }
    return 0;
}

s32 sub_020647A8(ApproachingTrainerData *data) {
    if (data->sightRange <= 1) {
        data->state = 0xD;
        return 1;
    }
    data->state = 0xB;
    return 1;
}

s32 sub_020647C0(ApproachingTrainerData *data) {
    if (MapObject_AreBitsSetForMovementScriptInit(data->mapObj) == 1) {
        MapObject_SetHeldMovement(data->mapObj, sub_0206234C(data->direction, 0xC));
        data->state = 0xC;
    }
    return 0;
}

s32 sub_020647E8(ApproachingTrainerData *data) {
    if (MapObject_IsMovementPaused(data->mapObj) == 0) {
        return 0;
    }
    data->sightRange = data->sightRange - 1;
    data->state = 0xA;
    return 1;
}

s32 sub_02064808(ApproachingTrainerData *data) {
    s32 v0;
    v0 = data->delay + 1;
    data->delay = v0;
    if (v0 < 8) {
        return 0;
    }
    data->delay = 0;
    data->state = 0xE;
    return 1;
}

s32 sub_02064824(ApproachingTrainerData *data) {
    u32 v0;
    LocalMapObject *object;
    s32 v2;
    u32 v1;
    u32 v3;

    object = PlayerAvatar_GetMapObject(data->playerAvatar);
    v1 = MapObject_GetXCoord(object);
    v3 = MapObject_GetZCoord(object);
    v0 = MapObject_GetXCoord(data->mapObj);
    v2 = sub_02061200(v1, v3, v0, MapObject_GetZCoord(data->mapObj));
    if ((v2 != PlayerAvatar_GetFacingDirection(data->playerAvatar)) && ((data->approachNum == 0) || (data->approachType == 2))) {
        if (MapObject_AreBitsSetForMovementScriptInit(object) == 1) {
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK7);
            MapObject_SetHeldMovement(object, sub_0206234C(v2, 0));
            data->state = 0xF;
        }
    } else {
        data->state = 0x10;
    }
    return 0;
}

s32 sub_020648A0(ApproachingTrainerData *data) {
    LocalMapObject *localMapObject;
    localMapObject = PlayerAvatar_GetMapObject(data->playerAvatar);
    if (MapObject_IsMovementPaused(localMapObject) == 0) {
        return 0;
    }
    MapObject_ClearHeldMovementIfActive(localMapObject);
    data->state = 0x10;
    return 1;
}

s32 sub_020648C8(ApproachingTrainerData *data) {
    MapObject_ClearHeldMovementIfActive(data->mapObj);
    sub_0205FC94(data->mapObj, 0U);
    data->state = 0x11;
    return 1;
}

s32 sub_020648E4(ApproachingTrainerData *data) {
    data->done = TRUE;
    return 0;
}
