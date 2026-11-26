#include "global.h"

#include "unk_0205CB48.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "map_object.h"
#include "pokemon_types_def.h"

u32 sub_0205D450(PlayerAvatar *param_1, u32 param_2);

s32 sub_0206234C(u32, s32);
void sub_0205D494(void *arg0, LocalMapObject *arg1, int dir, u16 keyPad, u16 keyPress);

u32 sub_0205D450(PlayerAvatar *param_1, u32 param_2) {
    u32 uVar1;

    if (param_2 == 0xffffffff) {
        PlayerAvatar_SetMoveState(param_1, 0);
        return 0;
    }
    uVar1 = PlayerAvatar_GetFacingDirection(param_1);
    if ((uVar1 != param_2) && (uVar1 = PlayerAvatar_GetMoveState(param_1), uVar1 != 1)) {
        PlayerAvatar_SetMoveState(param_1, 2);
        return 2;
    }
    PlayerAvatar_SetMoveState(param_1, 1);
    return 1;
}

void sub_0205D494(void *arg0, LocalMapObject *object, int dir, u16 keyPad, u16 keyPress) {
    sub_0205DA1C(arg0, object, sub_0206234C(MapObject_GetFacingDirection(object), 0));
}
