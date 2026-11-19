#include "global.h"
#include "sprite_resources_header.h"

#include "overlay_02_fly_landing.h"

#include "constants/heap.h"

struct Sprite;
struct SpriteList;
struct SpriteResourceCollection;
struct SpriteResourceWithLocation;
typedef struct SpriteList SpriteList;
typedef struct Sprite Sprite;
struct Sprite {
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
};
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
typedef struct SpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} SpriteListTemplate;
Sprite *Sprite_Create(const SpriteListTemplate *template);
typedef struct SpriteResourceCollection SpriteResourceCollection;
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
typedef struct SpriteResourceWithLocation SpriteResourceWithLocation;
const VecFx32 *Sprite_GetMatrixPtr(const Sprite *sprite);

Sprite *ov02_02248C10(FlyResources *flyResources, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);
void ov02_02248C98(Sprite *param0, VecFx32 *param1);

Sprite *ov02_02248C10(FlyResources *flyResources, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority) {
    SpriteResourcesHeader resourceHeader;
    SpriteListTemplate spriteListTemplate;
    Sprite *sprite;

    if (animResourceID == flyResources->resource4ID) {
        animResourceID = -1;
    }

    CreateSpriteResourcesHeader(
        &resourceHeader,
        charResourceID,
        paletteResourceID,
        cellResourceID,
        animResourceID,
        -1,
        -1,
        0,
        headerPriority,
        flyResources->charLocation,
        flyResources->paletteLocation,
        flyResources->cellLocation,
        flyResources->animLocation,
        0L,
        0L);

    spriteListTemplate.list = flyResources->spriteList;
    spriteListTemplate.resourceData = &resourceHeader;
    spriteListTemplate.position = *position;
    spriteListTemplate.priority = listPriority;
    spriteListTemplate.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteListTemplate.heapID = HEAP_ID_FIELD1;

    sprite = Sprite_Create(&spriteListTemplate);
    ((sprite != 0L) ? (void)0 : GF_AssertFail());

    return sprite;
}

void ov02_02248C98(Sprite *param0, VecFx32 *param1) {
    const VecFx32 *v0 = Sprite_GetMatrixPtr(param0);
    *param1 = *v0;
}
