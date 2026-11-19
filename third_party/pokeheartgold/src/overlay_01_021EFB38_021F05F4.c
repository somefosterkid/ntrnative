#include "global.h"

struct SpriteList;
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
typedef struct SpriteResourceCollection SpriteResourceCollection;
void Destroy2DGfxResObjMan(SpriteResourceCollection *spriteResources);
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct {
    SpriteList *unk_00;
    G2dRenderer unk_04;
    SpriteResourceCollection *unk_190[4];
} UnkStruct_ov5_021DE47C;

void ov01_021F05F4(UnkStruct_ov5_021DE47C *param0);

void ov01_021F05F4(UnkStruct_ov5_021DE47C *param0) {
    int v0;

    SpriteList_Delete(param0->unk_00);

    for (v0 = 0; v0 < 4; v0++) {
        Destroy2DGfxResObjMan(param0->unk_190[v0]);
    }
}
