#include "global.h"
#include "constants/heap.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "overlay_28.h"
#include "player_avatar.h"
#include "sprite.h"
#include "unk_02009D48.h"

extern const DowsingMchnSpriteDef ov28_0225EA9A;
extern const DowsingMchnSpriteDef ov28_0225EA9E;
extern const DowsingMchnSpriteDef ov28_0225EAA2;
extern const DowsingMchnSpriteDef ov28_0225EAA6;

void ov28_0225DA74(DowsingMchnApp *app) {
    u16 uVar1;
    NARC *narc;
    PlayerAvatar *avatar;
    SpriteResourcesHeader SStack_30;

    narc = NARC_New(NARC_a_0_1_4, HEAP_ID_8);
    ov28_0225D92C(app->buttonResObjs, app->resMans, narc, 0x46, 7, 0x44, 0x45, 4, 0x1f2, 0x1f2, 0x1f2, 0x1f2);
    NARC_Delete(narc);
    ov28_0225D9BC(app->buttonResObjs, app->resMans, &SStack_30, 1);
    ov28_0225DA1C(app, 0, &SStack_30, &ov28_0225EA9A);
    ov28_0225DA1C(app, 1, &SStack_30, &ov28_0225EA9E);
    ov28_0225DA1C(app, 2, &SStack_30, &ov28_0225EAA2);
    ov28_0225DA1C(app, 3, &SStack_30, &ov28_0225EAA6);
    avatar = FieldSystem_GetPlayerAvatar(app->fieldSystem);
    uVar1 = PlayerAvatar_CheckRunningShoesLock(avatar);
    if (uVar1 == 0) {
        Sprite_SetAnimCtrlSeq(app->sprites[1], 3);
        Sprite_SetAnimCtrlSeq(app->sprites[2], 0xb);
        return;
    }
    Sprite_SetAnimCtrlSeq(app->sprites[1], 4);
    Sprite_SetAnimCtrlSeq(app->sprites[2], 7);
    return;
}
