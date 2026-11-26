#include "global.h"

#include "unk_0205CB48.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "unk_02005D10.h"
#include "pokemon_types_def.h"

u32 sub_0205D658(PlayerAvatar *, u32);

s32 sub_0205D6B4(PlayerAvatar *);


u32 sub_0205DA34(PlayerAvatar *, LocalMapObject *, u32);

u32 sub_0206234C(u32, u32);

void sub_0205D978(PlayerAvatar *param_1, LocalMapObject *param_2, s32 arg2, u16 arg3, u16 keyPress);

void sub_0205D978(PlayerAvatar *param_1, LocalMapObject *param_2, s32 arg2, u16 arg3, u16 keyPress) {
    u32 direction;
    u32 uVar1;
    u32 uStack_1c;
    u32 uStack_20;

    uStack_20 = sub_0205D6B4(param_1);
    direction = PlayerAvatar_GetNextFacingDirection(param_1);
    uVar1 = sub_0205DA34(param_1, param_2, direction);
    if ((uVar1 & 4) != 0) {
        uStack_1c = sub_0206234C(direction, 0x38);
    } else if ((uVar1 & 0x40) != 0) {
        uStack_1c = sub_0206234C(direction, 0);
        PlayerAvatar_ClearUnk24ClearFlag2(param_1);
    } else if (uVar1 != 0) {
        uStack_20 = 0;
        uStack_1c = sub_0206234C(direction, 0x1c);
        if ((uVar1 & 8) == 0) {
            PlaySE(0x600);
        }
        MapObject_SetNextFacingDirection(param_2, direction);
        PlayerAvatar_ClearUnk24ClearFlag2(param_1);
    } else {
        uStack_1c = sub_0205D658(param_1, direction);
    }
    if (uStack_20 == 0) {
        PlayerAvatar_SetFlag2(param_1, 0);
    }
    sub_0205DA1C(param_1, param_2, uStack_1c);
    return;
}
