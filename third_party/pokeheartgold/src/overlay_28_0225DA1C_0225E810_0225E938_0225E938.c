#include "global.h"
#include "field_types_def.h"
#include "overlay_28.h"
#include "player_avatar.h"
#include "sprite.h"
#include "touchscreen.h"

extern const TouchscreenHitbox ov28_0225EA84;

void ov28_0225E938(DowsingMchnApp *app) {
    u16 uVar1;
    s32 sVar2;
    s32 iVar3;
    BOOL BVar4;
    PlayerAvatar *avatar;

    sVar2 = PlayerAvatar_GetState(app->fieldSystem->playerAvatar);
    if ((sVar2 == 1) || (!(iVar3 = ov28_0225EA58(app), iVar3 != 1))) {
        Sprite_SetDrawFlag(app->sprites[1], 0);
        Sprite_SetDrawFlag(app->sprites[2], 0);
        return;
    }
    Sprite_SetDrawFlag(app->sprites[1], 1);
    Sprite_SetDrawFlag(app->sprites[2], 1);
    BVar4 = TouchscreenHitbox_TouchNewIsIn(&ov28_0225EA84);
    if (BVar4 == 1) {
        avatar = FieldSystem_GetPlayerAvatar(app->fieldSystem);
        uVar1 = PlayerAvatar_CheckRunningShoesLock(avatar);
        if (uVar1 == 0) {
            Sprite_SetAnimCtrlSeq(app->sprites[1], 4);
            Sprite_SetAnimCtrlSeq(app->sprites[2], 7);
        } else {
            Sprite_SetAnimCtrlSeq(app->sprites[1], 3);
            Sprite_SetAnimCtrlSeq(app->sprites[2], 0xb);
        }
        PlayerAvatar_SetRunningShoesLock(avatar, uVar1 ^ 1);
    }
    return;
}
