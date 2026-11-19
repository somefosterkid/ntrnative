#include "global.h"
#include "battle_castle_app_sprite_manager.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"
#include "constants/heap.h"
#include "sprite.h"
#include "struct_defs/resdat_narc_structs.h"

Sprite *Sprite_CreateAffine(const SpriteTemplate *template);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetAnimSpeed(Sprite *sprite, fx32 speed);

void CreateSpriteResourcesHeader(SpriteResourcesHeader *resourceHeader,
    int charResourceID,
    int plttResourceID,
    int cellResourceID,
    int animResourceID,
    int mcellResourceID,
    int manimResourceID,
    BOOL vramTransfer,
    int priority,
    struct GF_2DGfxResMan *charResources,
    struct GF_2DGfxResMan *plttResources,
    struct GF_2DGfxResMan *cellResources,
    struct GF_2DGfxResMan *animResources,
    struct GF_2DGfxResMan *mcellResources,
    struct GF_2DGfxResMan *manimResources);

Sprite *ov83_0224714C(BattleCastleAppSpriteManager *spriteMan, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen);

Sprite *ov83_0224714C(BattleCastleAppSpriteManager *spriteMan, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen) {
    SpriteResourcesHeader resourceHeader;
    CreateSpriteResourcesHeader(&resourceHeader, charResourceID, plttResourceID, cellResourceID, cellResourceID, -1, -1, FALSE, resourcePriority, spriteMan->resourceCollection[0], spriteMan->resourceCollection[1], spriteMan->resourceCollection[2], spriteMan->resourceCollection[3], NULL, NULL);

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

    if (!onSubScreen) {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }

    spriteTemplate.heapID = HEAP_ID_107;

    if (onSubScreen == TRUE) {
        spriteTemplate.position.y += FX32_CONST(192);
    }

    Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);

    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animID);

    return sprite;
}
