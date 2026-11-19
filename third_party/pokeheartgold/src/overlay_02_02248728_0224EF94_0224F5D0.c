#include "global.h"
struct FieldSystem;

#include "error_handling.h"
#include "follow_mon.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "save_local_field_data.h"
#include "unk_02054648.h"
#include "unk_02055418.h"
#include "follow_mon_interact_work.h"

void ov02_0224F5D0(struct FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F5D0(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    u16 v0;

    v0 = LocalFieldData_GetWeatherType(Save_LocalFieldData_Get(arg0->saveData));
    switch (v0) {
    case 0:
        arg1->unk_11 = 1;
        return;
    case 1:
        arg1->unk_11 = 3;
        return;
    default:
        arg1->unk_11 = 0;
        return;
    }
}

void ov02_0224F5FC(FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F5FC(FieldSystem *fieldSystem, FollowMonInteractWork *arg1) {
    u32 x;
    u8 tile;

    x = MapObject_GetXCoord(FollowMon_GetMapObject(fieldSystem));
    tile = GetMetatileBehavior(fieldSystem, (s32)x, (s32)MapObject_GetZCoord(FollowMon_GetMapObject(fieldSystem)));
    arg1->unk_12 = tile;
    fieldSystem->unk120->unk882 = (u16)tile;
    if (MetatileBehavior_CanGenerateWalkingEncounters(tile) != 0) {
        arg1->unk_13 = 1;
        return;
    }
    arg1->unk_13 = 2;
}

void ov02_0224F644(struct FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F644(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    arg1->mapId = (s16) * *((s32 * *)&arg0->location);
}

void ov02_0224F64C(FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F64C(FieldSystem *fieldSystem, FollowMonInteractWork *arg1) {
    u32 v0;

    v0 = (u32)Field_GetTimeOfDay(fieldSystem);
    switch (v0) {
    case 0:
        arg1->unk_14 = 1;
        return;
    case 1:
        arg1->unk_14 = 2;
        return;
    case 2:
        arg1->unk_14 = 3;
        return;
    case 3:
        arg1->unk_14 = 4;
        return;
    case 4:
        arg1->unk_14 = 5;
        return;
    default:
        GF_AssertFail();
        arg1->unk_14 = 0;
        return;
    }
}

void ov02_0224F698(struct FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F698(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    arg1->mood = FieldSystem_UnkSub108_GetMonMood(arg0->unk108);
}
