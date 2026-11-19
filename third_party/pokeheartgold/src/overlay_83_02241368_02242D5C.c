#include "global.h"

#include "list_menu_items.h"
#include "overlay_83_battle_castle_self_app.h"
#include "overlay_80_02237A70.h"


s32 ov83_0224773C(void *, s32, s32);

SysTask *ov83_02247CE8(void *);

void ov83_02242D5C(BattleCastleSelfApp *arg0);

void ov83_02242D5C(BattleCastleSelfApp *arg0) {
    s32 v0;

    v0 = ov80_02237B24(arg0->challengeType, 1);
    ov83_0224773C(arg0->monIconSprites, v0, 0);
    ov83_0224773C(arg0->itemIconSprites, v0, 0);
    ov83_02247CE8(arg0->touchMenu);
    ListMenuItems_Delete(arg0->listMenuItems);
    arg0->yesNoMenuIsOpen = 0;
}
