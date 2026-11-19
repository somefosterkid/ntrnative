#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "sprite.h"
#include "unk_02019BA4.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_app_sprite.h"

void ov83_022407FC(BattleCastleSelfApp *arg0);

void ov83_0224759C(BattleCastleAppSprite *sprite, int x, int y);

GridInputHandler *ov83_02247B7C(BattleCastleSelfApp *);

void ov83_02240528(BattleCastleSelfApp *param_1, s32 param_2);

void ov83_02240664(BattleCastleSelfApp *);

void ov83_02240748(BattleCastleSelfApp *);

void ov83_0224088C(BattleCastleSelfApp *);

void ov83_022408E0(BattleCastleSelfApp *, u32);

void ov83_02240984(BattleCastleSelfApp *app);

void ov83_0224755C(BattleCastleAppSprite *, u32);

u8 ov83_0224777C(SaveData *, u8, u8);

extern const u16 ov83_02247D12[];

extern const u16 ov83_02247D24[];

extern const u16 ov83_02247EE0[];

extern const u16 ov83_02247F88[];

void ov83_02240B54(BattleCastleSelfApp *app, s32 param_2);

void ov83_02240B54(BattleCastleSelfApp *app, s32 param_2) {
    MsgData *msgData;
    s32 iVar1;
    u16 i;
    const u16 *puVar4;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xde, HEAP_ID_107);
    iVar1 = ov83_0224777C(app->saveData, app->challengeType, 1);
    if (param_2 == 6) {
        s32 j = iVar1 - 1;
        app->numEntries = ov83_02247D12[j];
        puVar4 = ov83_02247F88;
    } else {
        s32 j = iVar1 - 1;
        app->numEntries = ov83_02247D24[j];
        puVar4 = ov83_02247EE0;
    }
    app->listItems = ListMenuItems_New(app->numEntries, HEAP_ID_107);
    for (i = 0; i < app->numEntries; i++) {
        ListMenuItems_AppendFromMsgData(app->listItems, msgData, puVar4[i], i);
    }
    DestroyMsgData(msgData);
    app->gridInput = ov83_02247B7C(app);
    ov83_0224755C(app->itemSelectCursorSprite, 1);
    ov83_02240528(app, param_2);
    ov83_02240664(app);
    ov83_02240748(app);
    ov83_022407FC(app);
    ov83_0224088C(app);
    ov83_022408E0(app, 0);
    ov83_02240984(app);
    ov83_0224759C(app->selectedMonSprite, 0x30, 0x48);
}
