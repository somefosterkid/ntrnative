#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct AffineSpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    VecFx32 affineScale;
    u16 affineZRotation;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} AffineSpriteListTemplate;

Sprite *Sprite_CreateAffine(const AffineSpriteListTemplate *template);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetAnimSpeed(Sprite *sprite, fx32 speed);

void Sprite_SetAnimCtrlSeq(Sprite *sprite, u32 animID);

typedef struct SpriteResource SpriteResource;

void CreateSpriteResourcesHeader(SpriteResourcesHeader *resourceHeader,
    int charResourceID,
    int plttResourceID,
    int cellResourceID,
    int animResourceID,
    int mcellResourceID,
    int manimResourceID,
    BOOL vramTransfer,
    int priority,
    SpriteResourceCollection *charResources,
    SpriteResourceCollection *plttResources,
    SpriteResourceCollection *cellResources,
    SpriteResourceCollection *animResources,
    SpriteResourceCollection *mcellResources,
    SpriteResourceCollection *manimResources);

typedef struct BattleHallAppSpriteManager {
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *resources[2][4];
} BattleHallAppSpriteManager;

Sprite *ov82_0223FB08(BattleHallAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, u8 onSubScreen);

Sprite *ov82_0223FB08(BattleHallAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, u8 onSubScreen) {
    SpriteResourcesHeader resourceHeader;
    CreateSpriteResourcesHeader(&resourceHeader, resourceID, resourceID, resourceID, resourceID, -1, -1, FALSE, 1, spriteMan->resourceCollection[GF_GFX_RES_TYPE_CHAR], spriteMan->resourceCollection[GF_GFX_RES_TYPE_PLTT], spriteMan->resourceCollection[GF_GFX_RES_TYPE_CELL], spriteMan->resourceCollection[GF_GFX_RES_TYPE_ANIM], NULL, NULL);

    AffineSpriteListTemplate spriteTemplate;

    spriteTemplate.list = spriteMan->spriteList;
    spriteTemplate.resourceData = &resourceHeader;

    spriteTemplate.position.x = 0;
    spriteTemplate.position.y = 0;
    spriteTemplate.position.z = 0;
    spriteTemplate.affineScale.x = FX32_ONE;
    spriteTemplate.affineScale.y = FX32_ONE;
    spriteTemplate.affineScale.z = FX32_ONE;
    spriteTemplate.affineZRotation = 0;
    spriteTemplate.priority = priority;

    if (!onSubScreen) {
        spriteTemplate.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        spriteTemplate.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    }

    spriteTemplate.heapID = HEAP_ID_105;

    if (onSubScreen == TRUE) {
        spriteTemplate.position.y += FX32_CONST(192);
    }

    Sprite *sprite = Sprite_CreateAffine(&spriteTemplate);

    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animID);

    return sprite;
}
