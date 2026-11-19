#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "player_avatar.h"

s32 ov02_0224E31C(u32, u32);

s32 ov02_0224E340(FieldSystem *fieldSystem);

s32 ov02_0224E340(FieldSystem *fieldSystem) {
    u32 v0;

    v0 = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    return ov02_0224E31C(v0, PlayerAvatar_GetZCoord(fieldSystem->playerAvatar));
}
