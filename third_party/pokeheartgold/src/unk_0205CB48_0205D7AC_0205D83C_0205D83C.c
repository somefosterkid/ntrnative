#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "unk_02005D10.h"

u32 sub_0205D658(PlayerAvatar *, u32);

void sub_0205D684(PlayerAvatar *);

void sub_0205DA1C(PlayerAvatar *, LocalMapObject *, u32);

u32 sub_0205DA34(PlayerAvatar *, LocalMapObject *, u32);

void sub_0205E048(PlayerAvatar *);

u32 sub_0206234C(u32, u32);

void sub_0205D83C(PlayerAvatar *param_1, LocalMapObject *param_2, u32 param_3, u16 keyPad, u16 keyPress);

void sub_0205D83C(PlayerAvatar *param_1, LocalMapObject *param_2, u32 param_3, u16 keyPad, u16 keyPress) {
    u32 uVar1;
    s32 sVar2;
    u32 direction;
    u32 uStack_18;

    uVar1 = sub_0205DA34(param_1, param_2, param_3);
    if ((uVar1 & 4) != 0) {
        uStack_18 = sub_0206234C(param_3, 0x38);
        sub_0205D684(param_1);
        sub_0205E048(param_1);
        PlayerAvatar_SetFlag6(param_1);
    } else if ((uVar1 & 0x10) != 0) {
        sVar2 = PlayerAvatar_GetUnk24(param_1);
        if (sVar2 >= 3) {
            uStack_18 = sub_0205D658(param_1, param_3);
            PlayerAvatar_SetFlag6(param_1);
            PlayerAvatar_SetForcedMovement(param_1, 1);
        } else {
            uStack_18 = sub_0206234C(param_3, 0x1c);
            if ((uVar1 & 8) == 0) {
                PlaySE(0x600);
            }
            MapObject_SetNextFacingDirection(param_2, param_3);
            PlayerAvatar_ClearUnk24ClearFlag2(param_1);
        }
    } else if ((uVar1 & 0x40) != 0) {
        direction = MapObject_GetNextFacingDirection(param_2);
        uStack_18 = sub_0206234C(direction, 0);
        MapObject_SetNextFacingDirection(param_2, direction);
        PlayerAvatar_ClearUnk24ClearFlag2(param_1);
        PlayerAvatar_SetUnk28Unk2C(param_1, -1, -1);
    } else if (uVar1 != 0) {
        uStack_18 = sub_0206234C(param_3, 0x1c);
        if ((uVar1 & 8) == 0) {
            PlaySE(0x600);
        }
        MapObject_SetNextFacingDirection(param_2, param_3);
        PlayerAvatar_ClearUnk24ClearFlag2(param_1);
    } else {
        uStack_18 = sub_0205D658(param_1, param_3);
        sub_0205D684(param_1);
        sub_0205E048(param_1);
        PlayerAvatar_SetFlag6(param_1);
    }
    sub_0205DA1C(param_1, param_2, uStack_18);
    return;
}
