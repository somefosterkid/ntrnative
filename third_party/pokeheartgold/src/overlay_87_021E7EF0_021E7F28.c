#include "global.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_87_scratch_off_card_app.h"

void SpriteTransfer_ReplacePlttData(const SpriteResource *resource);

extern u8 ov87_021E84C0[8];

void ov87_021E7F28(ScratchOffCardsAppSpriteManager *spriteMan, int index);

void ov87_021E7F28(ScratchOffCardsAppSpriteManager *spriteMan, int index) {
    if (index >= 8) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    SpriteResource *resource = SpriteResourceCollection_Find(spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], 3);

    ReplacePlttResObjFromNarc(spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], resource, (enum NarcId)207, ov87_021E84C0[index], 0, (enum HeapID)122);
    SpriteTransfer_ReplacePlttData(resource);
}
