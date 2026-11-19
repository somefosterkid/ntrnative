#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

struct PokemonSpriteManager;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
typedef struct ResourceCollection ResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct PokemonSpriteManager PokemonSpriteManager;
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
typedef struct SpriteResourceCollection SpriteResourceCollection;
void DestroySingle2DGfxResObj(SpriteResourceCollection *spriteResources, SpriteResource *resource);
SpriteResource *SpriteResourceCollection_Find(const SpriteResourceCollection *spriteResources, int id);
void ov41_02246360(UnkStruct_ov22_0225A0E4 *param0, int param1);
void ov41_02246374(UnkStruct_ov22_0225A0E4 *param0, int param1);
void ov41_02246388(UnkStruct_ov22_0225A0E4 *param0, int param1);
void ov41_0224639C(UnkStruct_ov22_0225A0E4 *param0, int param1);

void ov41_02246360(UnkStruct_ov22_0225A0E4 *param0, int param1) {
    SpriteResource *v0 = SpriteResourceCollection_Find(param0->unk_48[0], param1);
    DestroySingle2DGfxResObj(param0->unk_48[0], v0);
}

void ov41_02246374(UnkStruct_ov22_0225A0E4 *param0, int param1) {
    SpriteResource *v0 = SpriteResourceCollection_Find(param0->unk_48[1], param1);
    DestroySingle2DGfxResObj(param0->unk_48[1], v0);
}

void ov41_02246388(UnkStruct_ov22_0225A0E4 *param0, int param1) {
    SpriteResource *v0 = SpriteResourceCollection_Find(param0->unk_48[2], param1);
    DestroySingle2DGfxResObj(param0->unk_48[2], v0);
}

void ov41_0224639C(UnkStruct_ov22_0225A0E4 *param0, int param1) {
    SpriteResource *v0 = SpriteResourceCollection_Find(param0->unk_48[3], param1);
    DestroySingle2DGfxResObj(param0->unk_48[3], v0);
}
