#include "global.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "overlay_41_sprite_resource_collection.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct BgConfig;
struct NARC;
struct PokemonSprite;
struct PokemonSpriteManager;
struct PokemonSpriteTransforms;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteCharsTemplate;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SoftwareSpritePaletteTemplate;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct ResourceCollection ResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct SoftwareSpriteCharsTemplate SoftwareSpriteCharsTemplate;
typedef struct SoftwareSpritePaletteTemplate SoftwareSpritePaletteTemplate;
typedef struct BgConfig BgConfig;
typedef struct NARC NARC;
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
BOOL SpriteList_Delete(SpriteList *collection);
void Destroy2DGfxResObjMan(SpriteResourceCollection *spriteResources);
void ObjCharTransfer_Destroy(void);
void ObjPlttTransfer_Destroy(void);
void OamManager_Free(void);
void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3);
void SpriteList_RenderAndAnimateSprites(const SpriteList *collection);
typedef struct Resource {
    int id;
    void *data;
} Resource;
struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
};
Resource *GF2dGfxRawResMan_AllocObj(ResourceCollection *collection, void *data, int id);
struct SoftwareSpriteCharsTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dCharacterData *charsData;
};
struct SoftwareSpritePaletteTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dPaletteData *paletteData;
    u32 paletteSlot;
};
typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;
struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
};
struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
};
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef struct PokemonSprite PokemonSprite;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);
typedef struct PokemonSpriteTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokemonSpriteTemplate;
struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};
typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;
struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};
struct PokemonSpriteManager {
    PokemonSprite sprites[4];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    enum HeapID heapID;
    u32 charBaseAddr;
    u32 charSize;
    u32 plttBaseAddr;
    u32 plttSize;
    u8 *charRawData;
    u16 *plttRawData;
    u16 *plttRawDataUnfaded;
    NNSG2dCharacterData charData;
    NNSG2dPaletteData plttData;
    u8 dummy330;
    u8 needLoadChar;
    u8 needLoadPltt;
    u8 excludeIdentity;
    u32 hideShadows;
};
enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};
typedef struct SpriteResource {
    Resource *rawResource;
    enum SpriteResourceType type;
    void *data;
} SpriteResource;
NNSG2dCharacterData *ov41_022463DC(UnkStruct_ov22_0225A0E4 *param0, void *param1, int param2);
void *GfGfxLoader_LoadFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd);
void Heap_Free(void *ptr);
void *PokepicManager_Create(enum HeapID heapID);
void PokepicManager_SetCharBaseAddrAndSize(PokemonSpriteManager *monSpriteMan, u32 addr, u32 size);
void PokepicManager_SetPlttBaseAddrAndSize(PokemonSpriteManager *monSpriteMan, u32 addr, u32 size);
void PokepicManager_Delete(PokemonSpriteManager *monSpriteMan);
ResourceCollection *GF2dGfxRawResMan_Create(s32 maxResources, enum HeapID heapID);
void *Heap_Alloc(u32 heapID, u32 size);
void GF2dGfxRawResObj_Destroy(ResourceCollection *collection);

void ov41_02246B34(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02246B5C(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02246B68(UnkStruct_ov22_0225A0E4 *param0, UnkStruct_ov22_02255CB8 *param1);
void ov41_02246BEC(UnkStruct_ov22_0225A0E4 *param0, UnkStruct_ov22_02255CB8 *param1);
void ov41_02246C90(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID);
void ov41_02246CB0(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3);
void ov41_02246D2C(UnkStruct_ov22_0225A0E4 *param0);
void ov41_02246D54(UnkStruct_ov22_02255CB8 *param0, int param1, int param2, enum HeapID heapID);
void ov41_02246DA8(UnkStruct_ov22_02255CB8 *param0);

void ov41_02246B34(UnkStruct_ov22_0225A0E4 *param0) {
    SpriteList_Delete(param0->unk_44);

    {
        int v0;

        for (v0 = 0; v0 < 4; v0++) {
            Destroy2DGfxResObjMan(param0->unk_48[v0]);
        }
    }

    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void ov41_02246B5C(UnkStruct_ov22_0225A0E4 *param0) {
    SpriteList_RenderAndAnimateSprites(param0->unk_44);
}

void ov41_02246B68(UnkStruct_ov22_0225A0E4 *param0, UnkStruct_ov22_02255CB8 *param1) {
    int v0;
    int v1;
    void *v2;

    for (v0 = 0; v0 < 100; v0++) {
        v2 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, v0 + 1, 0, HEAP_ID_14, 1);

        ((v2) ? (void)0 : GF_AssertFail());

        v1 = v0;

        param1->unk_00[v1].charsData = ov41_022463DC(param0, v2, v0);
        param1->unk_00[v1].softSpriteMan = param0->unk_00;
    }

    v2 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, 0, 0, HEAP_ID_14, 1);

    GF2dGfxRawResMan_AllocObj(param1->unk_14, v2, 0);
    NNS_G2dGetUnpackedPaletteData(v2, &param1->unk_08[0].paletteData);

    param1->unk_08[0].softSpriteMan = param0->unk_00;
    param1->unk_08[0].paletteSlot = 3;
}

void ov41_02246BEC(UnkStruct_ov22_0225A0E4 *param0, UnkStruct_ov22_02255CB8 *param1) {
    int v0;
    void *v1;
    int v2;

    for (v0 = 0; v0 < 18; v0++) {
        v1 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, v0 + 206, 0, HEAP_ID_14, 1);
        v2 = v0 + 100;

        param1->unk_00[v2].charsData = ov41_022463DC(param0, v1, v2);
        param1->unk_00[v2].softSpriteMan = param0->unk_00;

        v2 = v0 + 1;
        v1 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, (v0) * 4 + 134 + 1, 0, HEAP_ID_14, 1);

        GF2dGfxRawResMan_AllocObj(param1->unk_14, v1, v2);
        NNS_G2dGetUnpackedPaletteData(v1, &param1->unk_08[v2].paletteData);
        ((param1->unk_08[v2].paletteData) ? (void)0 : GF_AssertFail());

        param1->unk_08[v2].softSpriteMan = param0->unk_00;
        param1->unk_08[v2].paletteSlot = 1;
    }
}

void ov41_02246C90(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID) {
    param0->unk_30 = GfGfxLoader_LoadFromOpenNarc(param0->unk_5C, 235, 0, heapID, 0);
}

void ov41_02246CB0(UnkStruct_ov22_0225A0E4 *param0) {
    Heap_Free(param0->unk_30);
    param0->unk_30 = 0L;
}

void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3) {
    param0->unk_20 = PokepicManager_Create(heapID);
    param0->unk_24 = NNS_GfdAllocTexVram(param2, 0, 0);
    param0->unk_28 = NNS_GfdAllocPlttVram(param3, 0, NNS_GFD_ALLOC_FROM_LOW);

    PokepicManager_SetCharBaseAddrAndSize(param0->unk_20, NNS_GfdGetTexKeyAddr(param0->unk_24), NNS_GfdGetTexKeySize(param0->unk_24));
    PokepicManager_SetPlttBaseAddrAndSize(param0->unk_20, NNS_GfdGetPlttKeyAddr(param0->unk_28), NNS_GfdGetPlttKeySize(param0->unk_28));

    param0->unk_2C = 1;
}

void ov41_02246D2C(UnkStruct_ov22_0225A0E4 *param0) {
    PokepicManager_Delete(param0->unk_20);

    NNS_GfdFreeTexVram(param0->unk_24);
    NNS_GfdFreePlttVram(param0->unk_28);

    param0->unk_2C = 0;
}

void ov41_02246D54(UnkStruct_ov22_02255CB8 *param0, int param1, int param2, enum HeapID heapID) {
    param0->unk_00 = Heap_Alloc(heapID, sizeof(SoftwareSpriteCharsTemplate) * param1);
    memset(param0->unk_00, 0, sizeof(SoftwareSpriteCharsTemplate) * param1);
    param0->unk_10 = GF2dGfxRawResMan_Create(param1, heapID);
    param0->unk_04 = param1;

    param0->unk_08 = Heap_Alloc(heapID, sizeof(SoftwareSpritePaletteTemplate) * param2);
    memset(param0->unk_08, 0, sizeof(SoftwareSpritePaletteTemplate) * param2);
    param0->unk_14 = GF2dGfxRawResMan_Create(param2, heapID);
    param0->unk_0C = param2;
}

void ov41_02246DA8(UnkStruct_ov22_02255CB8 *param0) {
    if (param0->unk_10) {
        GF2dGfxRawResObj_Destroy(param0->unk_10);
        param0->unk_10 = 0L;
    }

    if (param0->unk_14) {
        GF2dGfxRawResObj_Destroy(param0->unk_14);
        param0->unk_14 = 0L;
    }

    Heap_Free(param0->unk_00);
    param0->unk_00 = 0L;
    Heap_Free(param0->unk_08);
    param0->unk_08 = 0L;
}
