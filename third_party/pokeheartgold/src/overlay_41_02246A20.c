#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

struct PokemonSpriteManager;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct ResourceCollection ResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};
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
typedef struct SpriteResourceCollection SpriteResourceCollection;
ResourceCollection *GF2dGfxRawResMan_Create(s32 maxResources, enum HeapID heapID);
void GF2dGfxRawResObj_Destroy(ResourceCollection *collection);

void ov41_02246A20(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02246A50(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02246A7C(UnkStruct_ov22_0225A0E4 *param0);

void ov41_02246A20(UnkStruct_ov22_0225A0E4 *param0) {
    FreeBgTilemapBuffer(param0->unk_40, BG_LAYER_MAIN_1);
    FreeBgTilemapBuffer(param0->unk_40, BG_LAYER_MAIN_2);
    FreeBgTilemapBuffer(param0->unk_40, BG_LAYER_MAIN_3);
    FreeBgTilemapBuffer(param0->unk_40, BG_LAYER_SUB_0);
    FreeBgTilemapBuffer(param0->unk_40, BG_LAYER_SUB_1);
}

void ov41_02246A50(UnkStruct_ov22_0225A0E4 *param0) {
    param0->unk_34 = GF2dGfxRawResMan_Create((100 + 18) + 1, HEAP_ID_14);
    param0->unk_38 = Heap_Alloc(HEAP_ID_14, sizeof(NNSG2dCharacterData *) * ((100 + 18) + 1));
    memset(param0->unk_38, 0, sizeof(NNSG2dCharacterData *) * ((100 + 18) + 1));
    param0->unk_3C = ((100 + 18) + 1);
}

void ov41_02246A7C(UnkStruct_ov22_0225A0E4 *param0) {
    Heap_Free(param0->unk_38);
    GF2dGfxRawResObj_Destroy(param0->unk_34);
    param0->unk_3C = 0;
}
