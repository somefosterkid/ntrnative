#include "global.h"
struct UnkStruct_02069CA8;

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "unk_struct_02069_ca8.h"

void sub_02065CFC(LocalMapObject *param_1, struct UnkStruct_02069CA8 *param_2);

void sub_02065CFC(LocalMapObject *param_1, UnkStruct_02069CA8 *param_2) {
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    u32 uVar1;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    avatar = FieldSystem_GetPlayerAvatar(fieldSystem);
    param_2->unk_01 = 1;
    uVar1 = PlayerAvatar_GetXCoord(avatar);
    param_2->unk_04 = (s16)uVar1;
    uVar1 = PlayerAvatar_GetZCoord(avatar);
    *((s16 *)&param_2->unk_06) = (s16)uVar1;
    param_2->unk_8 = 0xff;
    return;
}

u32 sub_02065D24(LocalMapObject *param_1, u8 *param_2);

u32 sub_02065D24(LocalMapObject *param_1, u8 *param_2) {
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    u32 uVar1;
    u32 uVar2;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    avatar = FieldSystem_GetPlayerAvatar(fieldSystem);
    if (avatar != NULL) {
        uVar1 = PlayerAvatar_GetXCoord(avatar);
        uVar2 = PlayerAvatar_GetZCoord(avatar);
        if ((uVar1 != (s32) * (s16 *)(param_2 + 4)) || (uVar2 != (s32) * (s16 *)(param_2 + 6))) {
            return 1;
        }
    }
    return 0;
}

void sub_02065D58(LocalMapObject *param_1, u8 *param_2);

void sub_02065D58(LocalMapObject *param_1, u8 *param_2) {
    FieldSystem *fieldSystem;
    PlayerAvatar *avatar;
    u32 uVar1;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    avatar = FieldSystem_GetPlayerAvatar(fieldSystem);
    uVar1 = PlayerAvatar_GetXCoord(avatar);
    *(s16 *)(param_2 + 4) = (s16)uVar1;
    uVar1 = PlayerAvatar_GetZCoord(avatar);
    *(s16 *)(param_2 + 6) = (s16)uVar1;
    return;
}
