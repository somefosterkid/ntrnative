#include "global.h"

#include "unk_02009D48.h"
#include "overlay_87_scratch_off_card_app.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "sprite.h"
#include "struct_defs/resdat_narc_structs.h"

Sprite *Sprite_CreateAffine(const SpriteTemplate *template);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetAnimSpeed(Sprite *sprite, fx32 speed);

Sprite *ov87_021E7DF8(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 resourcePriority, u32 priority, u8 onSubScreen);

Sprite *ov87_021E7DF8(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 resourcePriority, u32 priority, u8 onSubScreen) {
    SpriteResourcesHeader resourceHeader;
    CreateSpriteResourcesHeader(&resourceHeader, resourceID, resourceID, resourceID, resourceID, -1, -1, FALSE, resourcePriority, spriteMan->resourceCollections[GF_GFX_RES_TYPE_CHAR], spriteMan->resourceCollections[GF_GFX_RES_TYPE_PLTT], spriteMan->resourceCollections[GF_GFX_RES_TYPE_CELL], spriteMan->resourceCollections[GF_GFX_RES_TYPE_ANIM], NULL, NULL);

    SpriteTemplate spriteTemplate;
    spriteTemplate.spriteList = spriteMan->spriteList;
    spriteTemplate.header = &resourceHeader;
    spriteTemplate.position.x = 0;
    spriteTemplate.position.y = 0;
    spriteTemplate.position.z = 0;
    spriteTemplate.scale.x = FX32_ONE;
    spriteTemplate.scale.y = FX32_ONE;
    spriteTemplate.scale.z = FX32_ONE;
    spriteTemplate.rotation = 0;
    spriteTemplate.drawPriority = priority;
    spriteTemplate.heapID = (enum HeapID)122;

    if (!onSubScreen) {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }

    Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);

    Sprite_SetAnimActiveFlag(sprite, FALSE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animID);

    return sprite;
}
