#include "global.h"

#include "comm_player_manager.h"
#include "field_system_def.h"
#include "player_avatar.h"
#include "unk_02035900.h"

s32 sub_02056EE0(void);

s32 sub_02056EE0(void) {
    if (_021D41C4 != 0) {
        return 1;
    }
    return 0;
}

void sub_02056EF4(void);

void sub_02056EF4(void) {
    u16 v0;
    u16 v1;
    u16 v2;
    u16 v3;
    u16 v4;
    u16 v5;

    _021D41C4->avatars[sub_0203769C()] = _021D41C4->fieldSystem->playerAvatar;
    _021D41C4->active[sub_0203769C()] = 1;
    v0 = sub_0203769C();
    _021D41C4->positions[v0].x = (s16)PlayerAvatar_GetXCoord(_021D41C4->fieldSystem->playerAvatar);
    v1 = sub_0203769C();
    _021D41C4->positions[v1].z = (s16)PlayerAvatar_GetZCoord(_021D41C4->fieldSystem->playerAvatar);
    v2 = sub_0203769C();
    _021D41C4->positions[v2].facing = (s8)PlayerAvatar_GetFacingDirection(_021D41C4->fieldSystem->playerAvatar);
    v3 = sub_0203769C();
    _021D41C4->hostPositions[v3].x = (s16)PlayerAvatar_GetXCoord(_021D41C4->fieldSystem->playerAvatar);
    v4 = sub_0203769C();
    _021D41C4->hostPositions[v4].z = (s16)PlayerAvatar_GetZCoord(_021D41C4->fieldSystem->playerAvatar);
    v5 = sub_0203769C();
    _021D41C4->hostPositions[v5].facing = (s8)PlayerAvatar_GetFacingDirection(_021D41C4->fieldSystem->playerAvatar);
}
