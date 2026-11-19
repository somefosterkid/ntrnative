#include "global.h"
struct TradeRoom;

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_65_trade_room.h"

void ov65_0221CCB0(struct TradeRoom *arg0);

void ov65_0221CCB0(TradeRoom *arg0) {
    s32 v0;

    SpriteTransfer_DeleteCharTransferTask(arg0->spriteResources[0][0]);
    SpriteTransfer_DeleteCharTransferTask(arg0->spriteResources[2][0]);
    SpriteTransfer_DeletePlttTransferTask(arg0->spriteResources[0][1]);
    SpriteTransfer_DeletePlttTransferTask(arg0->spriteResources[2][1]);
    v0 = 0;
    do {
        Destroy2DGfxResObjMan(((GF_2DGfxResMan *)arg0->spriteResourceCollections[v0]));
        v0 += 1;
    } while (v0 < 4);
    SpriteList_Delete(arg0->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
