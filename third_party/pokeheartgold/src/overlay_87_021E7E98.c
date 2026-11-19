#include "global.h"

#include "unk_0200A090.h"
#include "overlay_87_scratch_off_card_app.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "unk_0200B150.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "sprite.h"
#include "struct_defs/resdat_narc_structs.h"

struct SpriteList;
struct SpriteResource;
BOOL SpriteList_Delete(SpriteList *collection);
void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);
void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void ov87_021E7E98(ScratchOffCardsAppSpriteManager *spriteMan);

void ov87_021E7E98(ScratchOffCardsAppSpriteManager *spriteMan) {
    for (u8 i = 0; i < 5; i++) {
        SpriteTransfer_DeleteCharTransferTask(spriteMan->resources[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(spriteMan->resources[i][GF_GFX_RES_TYPE_PLTT]);
    }

    for (u8 i = 0; i < (GF_GFX_RES_TYPE_ANIM + 1); i++) {
        Destroy2DGfxResObjMan(spriteMan->resourceCollections[i]);
    }

    SpriteList_Delete(spriteMan->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
