#include "global.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "unk_0200B150.h"
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
BOOL SpriteList_Delete(SpriteList *collection);
typedef struct SpriteResource SpriteResource;
typedef struct SpriteResourceCollection SpriteResourceCollection;
void Destroy2DGfxResObjMan(SpriteResourceCollection *spriteResources);
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct BattleHallAppSpriteManager {
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *resources[2][4];
} BattleHallAppSpriteManager;
void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);
void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void ov82_0223FBBC(BattleHallAppSpriteManager *spriteMan);

void ov82_0223FBBC(BattleHallAppSpriteManager *spriteMan) {
    for (u8 i = 0; i < 2; i++) {
        SpriteTransfer_DeleteCharTransferTask(spriteMan->resources[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(spriteMan->resources[i][GF_GFX_RES_TYPE_PLTT]);
    }

    for (u8 i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(spriteMan->resourceCollection[i]);
    }

    SpriteList_Delete(spriteMan->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
