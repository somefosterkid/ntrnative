#include "global.h"

#include "list_menu_items.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02242AEC(void *, u8, s32, s32, u8);

extern s32 ov83_02247D6C;

void ov83_02242BAC(BattleCastleSelfApp *arg0);

void ov83_02242BAC(BattleCastleSelfApp *arg0) {
    u32 v0;
    s32 *v1;

    arg0->listMenuItems = ListMenuItems_New(3U, HEAP_ID_107);
    v1 = &ov83_02247D6C;
    v0 = 0;
    do {
        ListMenuItems_AppendFromMsgData(arg0->listMenuItems, ((MsgData *)arg0->msgLoader), *(s32 *)v1, *(s32 *)((u8 *)v1 + 4));
        v0 += 1;
        v1 += 2;
    } while (v0 < 3U);
    ov83_02242AEC(arg0, 3, 0x11, 8, 0xD);
}
