#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "overlay_01_021FC66C.h"
#include "unk_02062108.h"
#include "unk_02005D10.h"


s32 ov01_021FC7DC(void *arg0, PlayerAvatar *playerAvatar, LocalMapObject *obj) {
    if (MapObject_AreBitsSetForMovementScriptInit(obj) == 1) {
        MapObject_ClearHeldMovementIfActive(obj);
        Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, 0x20);
        Field_PlayerAvatar_ApplyTransitionFlags(playerAvatar);
        sub_0205F328(obj, 1U);
        *(s32 *)((u8 *)arg0 + 0xC) = 2;
    }
    return 0;
}

u32 ov01_021FC814(s32 *param_1, void *param_2, void *param_3) {
    s32 iVar1;

    iVar1 = param_1[4];
    param_1[4] = iVar1 + 1;
    if (iVar1 + 1 == 10) {
        PlaySE(0x64f);
    }
    if (param_1[4] < 0x22) {
        return 0;
    }
    if (*param_1 == 1) {
        iVar1 = 3;
    } else {
        iVar1 = 0xc;
    }
    param_1[3] = iVar1;
    param_1[4] = 0;
    return 1;
}
