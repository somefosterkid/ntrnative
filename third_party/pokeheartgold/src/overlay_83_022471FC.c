#include "global.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "item.h"
#include "battle_castle_app_sprite_manager.h"

void ov83_022471FC(BattleCastleAppSpriteManager *spriteMan);

void ov83_02247264(BattleCastleAppSpriteManager *arg0, s32 arg1, u16 arg2);
void ov83_022472A0(BattleCastleAppSpriteManager *arg0, s32 arg1, u16 arg2);

void ov83_022471FC(BattleCastleAppSpriteManager *spriteMan) {
    u8 i;

    i = 0;
    do {
        SpriteTransfer_DeleteCharTransferTask(spriteMan->resources[i][0]);
        i++;
    } while (i < BATTLE_CASTLE_APP_SPRITE_RESOURCE_COUNT);
    i = 0;
    do {
        SpriteTransfer_DeletePlttTransferTask(spriteMan->resources[i][1]);
        i++;
    } while (i < 11);
    i = 0;
    do {
        Destroy2DGfxResObjMan(spriteMan->resourceCollection[i]);
        i++;
    } while (i < 4);
    SpriteList_Delete(spriteMan->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    return;
}

void ov83_02247264(BattleCastleAppSpriteManager *arg0, s32 id, u16 itemId) {
    SpriteResource *spriteResource;

    spriteResource = SpriteResourceCollection_Find(arg0->resourceCollection[0], id);
    ReplaceCharResObjFromNarc(arg0->resourceCollection[0], spriteResource, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 1), 0, HEAP_ID_107);
    SpriteTransfer_ReplaceCharData(spriteResource);
}

void ov83_022472A0(BattleCastleAppSpriteManager *arg0, s32 id, u16 itemId) {
    SpriteResource *spriteResource;

    spriteResource = SpriteResourceCollection_Find(arg0->resourceCollection[1], id);
    ReplacePlttResObjFromNarc(arg0->resourceCollection[1], spriteResource, NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 2), 0, HEAP_ID_107);
    SpriteTransfer_ReplacePlttData(spriteResource);
}
