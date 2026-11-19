#include "global.h"

#include "battle/battle_system.h"

#include "filesystem.h"
#include "item.h"
#include "sprite_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "battle/battle_bag_context.h"
#include "overlay_08_battle_bag.h"

void ov08_02223D80(void *arg0);

void ov08_02223E74(BattleBag *battleBag, u16 itemId, s16 palIndex);

void ov08_02223D80(void *arg0) {
    SpriteSystem *spriteSystem;
    NARC *narc;
    s32 resId;
    u32 v0;

    narc = NARC_New(NARC_itemtool_itemdata_item_icon, ((BattleBag *)arg0)->context->heapID);
    spriteSystem = BattleSystem_GetSpriteSystem(((BattleBag *)arg0)->context->battleSys);
    v0 = 0;
    do {
        resId = v0 + 0xB4B7;
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, ((BattleBag *)arg0)->spriteManager, narc, GetItemIndexMapping(1U, 1), 0, 2, resId);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(((BattleBag *)arg0)->palette, PLTTBUF_SUB_OBJ, spriteSystem, ((BattleBag *)arg0)->spriteManager, narc, GetItemIndexMapping(1U, 2), 0, 1, 2, resId);
        v0 += 1;
    } while (v0 < 6U);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, ((BattleBag *)arg0)->spriteManager, narc, GetItemIconCell(), 0, 0xB4B7);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, ((BattleBag *)arg0)->spriteManager, narc, GetItemIconAnim(), 0, 0xB4B7);
    NARC_Delete(narc);
}

void ov08_02223E3C(BattleBag *battleBag, u16 arg1, s32 arg2);

void ov08_02223E3C(BattleBag *battleBag, u16 itemId, s32 resId) {
    SpriteSystem *spriteSystem;

    spriteSystem = BattleSystem_GetSpriteSystem(battleBag->context->battleSys);
    SpriteSystem_ReplaceCharResObj(spriteSystem, battleBag->spriteManager, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 1), 0, resId);
}

void ov08_02223E74(BattleBag *battleBag, u16 itemId, s16 palIndex) {
    s32 memberNo;

    memberNo = GetItemIndexMapping(itemId, 2);
    PaletteData_LoadNarc(battleBag->palette, NARC_itemtool_itemdata_item_icon, memberNo, battleBag->context->heapID, PLTTBUF_SUB_OBJ, 0x20, palIndex << 4);
    return;
}
