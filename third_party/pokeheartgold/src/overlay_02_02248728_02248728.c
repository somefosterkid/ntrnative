#include "global.h"

#include "overlay_02_fly_landing.h"
#include "overlay_41_sprite_resource_collection.h"
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

typedef struct SpriteResource SpriteResource;

SpriteResourceCollection *Create2DGfxResObjMan(int capacity, GfGfxResType type, enum HeapID heapID);

SpriteList *G2dRenderer_Init(int maxElements, G2dRenderer *g2dRenderer, enum HeapID heapID);

void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *g2dRenderer, fx32 x, fx32 y);

typedef struct SpriteResourceWithLocation {
    s16 resourceId;
    s16 memberIdx;
    SpriteResource *spriteResource;
} SpriteResourceWithLocation;

void *ov02_0224B690(enum HeapID heapID, int size);

void ov02_02248728(FlyResources *flyResources, int maxSpriteListElements, int resource1Count, int resource2Count, int resource3Count, int resource4Count, int resource1ID, int resource2ID, int resource3ID, int resource4ID);

void ov02_02248728(FlyResources *flyResources, int maxSpriteListElements, int resource1Count, int resource2Count, int resource3Count, int resource4Count, int resource1ID, int resource2ID, int resource3ID, int resource4ID) {
    int i;

    flyResources->resource1Count = resource1Count;
    flyResources->resource2Count = resource2Count;
    flyResources->resource3Count = resource3Count;
    flyResources->resource4Count = resource4Count;
    flyResources->resource1ID = resource1ID;
    flyResources->resource2ID = resource2ID;
    flyResources->resource3ID = resource3ID;
    flyResources->resource4ID = resource4ID;
    flyResources->spriteList = G2dRenderer_Init(maxSpriteListElements, &flyResources->g2dRenderer, HEAP_ID_FIELD1);

    G2dRenderer_SetSubSurfaceCoords(&flyResources->g2dRenderer, 0, ((fx32)(((512) > 0) ? ((512) * (1 << 12) + 0.5f) : ((512) * (1 << 12) - 0.5f))));

    flyResources->charLocation = Create2DGfxResObjMan(resource1Count, GF_GFX_RES_TYPE_CHAR, HEAP_ID_FIELD1);
    flyResources->paletteLocation = Create2DGfxResObjMan(resource2Count, GF_GFX_RES_TYPE_PLTT, HEAP_ID_FIELD1);
    flyResources->cellLocation = Create2DGfxResObjMan(resource3Count, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    flyResources->animLocation = Create2DGfxResObjMan(resource4Count, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    flyResources->charSourceWrapper = ov02_0224B690(HEAP_ID_FIELD1, (sizeof(SpriteResourceWithLocation)) * resource1Count);
    flyResources->paletteSourceWrapper = ov02_0224B690(HEAP_ID_FIELD1, (sizeof(SpriteResourceWithLocation)) * resource2Count);
    flyResources->cellSourceWrapper = ov02_0224B690(HEAP_ID_FIELD1, (sizeof(SpriteResourceWithLocation)) * resource3Count);
    flyResources->animSourceWrapper = ov02_0224B690(HEAP_ID_FIELD1, (sizeof(SpriteResourceWithLocation)) * resource4Count);

    for (i = 0; i < resource1Count; flyResources->charSourceWrapper[i].resourceId = resource1ID, i++) {}

    for (i = 0; i < resource2Count; flyResources->paletteSourceWrapper[i].resourceId = resource2ID, i++) {}

    for (i = 0; i < resource3Count; flyResources->cellSourceWrapper[i].resourceId = resource3ID, i++) {}

    for (i = 0; i < resource4Count; flyResources->animSourceWrapper[i].resourceId = resource4ID, i++) {}
}
