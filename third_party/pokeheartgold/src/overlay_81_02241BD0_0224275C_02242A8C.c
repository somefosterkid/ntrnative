#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "overlay_81_battle_factory_app.h"
#include "sprite.h"
#include "unk_02009D48.h"

Sprite *ov81_02242A8C(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen);

Sprite *ov81_02242A8C(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen) {
    SpriteResourcesHeader resourceHeader;
    CreateSpriteResourcesHeader(&resourceHeader, resourceID, resourceID, resourceID, resourceID, -1, -1, FALSE, resourcePriority, spriteMan->resourceCollection[GF_GFX_RES_TYPE_CHAR], spriteMan->resourceCollection[GF_GFX_RES_TYPE_PLTT], spriteMan->resourceCollection[GF_GFX_RES_TYPE_CELL], spriteMan->resourceCollection[GF_GFX_RES_TYPE_ANIM], NULL, NULL);

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

    spriteTemplate.heapID = HEAP_ID_100;

    if (onSubScreen == TRUE) {
        spriteTemplate.position.y += FX32_CONST(HW_LCD_HEIGHT * 2 + 128);
    }

    Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);

    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, animID);

    return sprite;
}
