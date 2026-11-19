#include "global.h"

#include "field_types_def.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"

void ov01_021F2EDC(PlayerAvatar *);

s32 sub_0205CBEC(PlayerAvatar *, s32);

void sub_0205CC4C(PlayerAvatar *, s32, u16, u16);

void sub_0205CC74(PlayerAvatar *);

void sub_0205CC94(PlayerAvatar *);

s32 sub_0205D004(PlayerAvatar *, s32);

void sub_0205D340(PlayerAvatar *, MapLoadManager *, s32, u32, u16);

s32 sub_0205D40C(PlayerAvatar *, s32);

s32 sub_0205DDD4(PlayerAvatar *, u16, u16);

void PlayerAvatar_MoveControl(PlayerAvatar *playerAvatar, MapLoadManager *mapLoadManager, s32 a2, u16 a3, u16 a4, s32 a5);

void PlayerAvatar_MoveControl(PlayerAvatar *playerAvatar, MapLoadManager *mapLoadManager, s32 a2, u16 a3, u16 a4, s32 a5) {
    s32 iVar1;
    s32 sVar2;

    if (a2 == -1) {
        a2 = sub_0205DDD4(playerAvatar, a3, a4);
    }
    iVar1 = sub_0205CBEC(playerAvatar, a2);
    if (iVar1 != 0) {
        sub_0205CC4C(playerAvatar, a2, a3, a4);
        Field_PlayerAvatar_ApplyTransitionFlags(playerAvatar);
        iVar1 = sub_0205D004(playerAvatar, a2);
        if (iVar1 == 1) {
            ov01_021F2F24(playerAvatar);
            return;
        }
        sVar2 = PlayerAvatar_GetState(playerAvatar);
        if (sVar2 == 0) {
            iVar1 = sub_0205D40C(playerAvatar, a2);
            if (iVar1 != 0) {
                ov01_021F2F24(playerAvatar);
            } else {
                if (a5 == 1) {
                    ov01_021F2EDC(playerAvatar);
                }
            }
        }
        sub_0205D340(playerAvatar, mapLoadManager, a2, a3, a4);
        sub_0205CC74(playerAvatar);
        sub_0205CC94(playerAvatar);
    }
    return;
}
