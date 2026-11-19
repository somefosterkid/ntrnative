#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "gf_3d_loader.h"
#include "heap.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "error_handling.h"

struct SpriteList;

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

BOOL SpriteList_Delete(SpriteList *collection);

BOOL SpriteList_DeleteAllSprites(SpriteList *collection);

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct SpriteResourceWithLocation {
    s16 resourceId;
    s16 memberIdx;
    SpriteResource *spriteResource;
} SpriteResourceWithLocation;

typedef struct FlyResources {
    u8 resource1Count;
    u8 resource2Count;
    u8 resource3Count;
    u8 resource4Count;
    s8 resource1ID;
    s8 resource2ID;
    s8 resource3ID;
    s8 resource4ID;
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    GF_2DGfxResMan *charLocation;
    GF_2DGfxResMan *paletteLocation;
    GF_2DGfxResMan *cellLocation;
    GF_2DGfxResMan *animLocation;
    SpriteResourceWithLocation *charSourceWrapper;
    SpriteResourceWithLocation *paletteSourceWrapper;
    SpriteResourceWithLocation *cellSourceWrapper;
    SpriteResourceWithLocation *animSourceWrapper;
} FlyResources;

void ov02_0224886C(FlyResources *flyResources);

void ov02_02248980(FlyResources *flyResources, NARC *narc, int memberIdx, int resourceId);

void ov02_022489F0(FlyResources *flyResources, int resourceId);
void ov02_02248A24(FlyResources *flyResources, int resourceId);

void ov02_0224886C(FlyResources *flyResources) {
    int i;

    for (i = 0; i < flyResources->resource1Count; i++) {
        if (flyResources->charSourceWrapper[i].resourceId != flyResources->resource1ID) {
            SpriteTransfer_DeleteCharTransferTask(flyResources->charSourceWrapper[i].spriteResource);
        }
    }

    for (i = 0; i < flyResources->resource2Count; i++) {
        if (flyResources->paletteSourceWrapper[i].resourceId != flyResources->resource2ID) {
            SpriteTransfer_DeletePlttTransferTask(flyResources->paletteSourceWrapper[i].spriteResource);
        }
    }

    for (i = 0; i < flyResources->resource3Count; i++) {
        if (flyResources->cellSourceWrapper[i].resourceId != flyResources->resource3ID) {
            sub_0200A740(flyResources->cellSourceWrapper[i].spriteResource);
        }
    }

    for (i = 0; i < flyResources->resource4Count; i++) {
        if (flyResources->animSourceWrapper[i].resourceId != flyResources->resource4ID) {
            sub_0200A740(flyResources->animSourceWrapper[i].spriteResource);
        }
    }

    Destroy2DGfxResObjMan(flyResources->charLocation);
    Destroy2DGfxResObjMan(flyResources->paletteLocation);
    Destroy2DGfxResObjMan(flyResources->cellLocation);
    Destroy2DGfxResObjMan(flyResources->animLocation);
    Heap_Free(flyResources->charSourceWrapper);
    Heap_Free(flyResources->paletteSourceWrapper);
    Heap_Free(flyResources->cellSourceWrapper);
    Heap_Free(flyResources->animSourceWrapper);
    SpriteList_DeleteAllSprites(flyResources->spriteList);
    SpriteList_Delete(flyResources->spriteList);
}

void ov02_02248980(FlyResources *flyResources, NARC *narc, int memberIdx, int resourceId) {
    for (int i = 0; i < flyResources->resource1Count; i++) {
        if (flyResources->charSourceWrapper[i].resourceId == flyResources->resource1ID) {
            flyResources->charSourceWrapper[i].resourceId = resourceId;
            flyResources->charSourceWrapper[i].memberIdx = 0;
            flyResources->charSourceWrapper[i].spriteResource = AddCharResObjFromOpenNarc(flyResources->charLocation, narc, memberIdx, FALSE, resourceId, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_FIELD1);
            return;
        }
    }

    GF_ASSERT(FALSE);
}

void ov02_022489F0(FlyResources *flyResources, int resourceId) {
    for (int i = 0; i < flyResources->resource1Count; i++) {
        if (flyResources->charSourceWrapper[i].resourceId == resourceId) {
            SpriteTransfer_CreateCharTransferTask_AllocAtEnd(flyResources->charSourceWrapper[i].spriteResource);
            return;
        }
    }

    GF_ASSERT(FALSE);
}

void ov02_02248A24(FlyResources *flyResources, int resourceId) {
    for (int i = 0; i < flyResources->resource1Count; i++) {
        if (flyResources->charSourceWrapper[i].resourceId == resourceId) {
            sub_0200A740(flyResources->charSourceWrapper[i].spriteResource);
            return;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
}
