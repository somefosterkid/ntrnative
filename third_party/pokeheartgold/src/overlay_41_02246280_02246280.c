#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "overlay_41_sprite_resource_collection.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"

struct Background;

struct BgConfig;

struct NARC;

struct Pokepic;

struct PokepicManager;

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct SoftwareSpriteChars;

struct SoftwareSpriteCharsTemplate;

struct SoftwareSpriteManager;

struct SoftwareSpriteManagerTemplate;

struct SoftwareSpritePalette;

struct SoftwareSpritePaletteTemplate;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct SoftwareSpriteManagerTemplate {
    int numSprites;
    int numChars;
    int numPalettes;
    enum HeapID heapID;
} SoftwareSpriteManagerTemplate;

typedef struct SoftwareSpriteCharsTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dCharacterData *charsData;
} SoftwareSpriteCharsTemplate;

typedef struct SoftwareSpritePaletteTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dPaletteData *paletteData;
    u32 paletteSlot;
} SoftwareSpritePaletteTemplate;

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

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

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

struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
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

typedef struct PokepicManager {
    Pokepic sprites[4];
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
} PokepicManager;

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

typedef struct SpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} SpriteListTemplate;

Sprite *Sprite_Create(const SpriteListTemplate *template);

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

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

void ov22_02255410(UnkStruct_ov22_02255CB8 *param0, enum HeapID heapID);

void ov22_0225547C(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1, enum HeapID heapID);

void ov22_02255BF4(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID);

void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3);

void ov22_02255CB8(UnkStruct_ov22_02255CB8 *param0, int param1, int param2, enum HeapID heapID);

Sprite *ov41_02246280(UnkStruct_ov22_0225A0E4 *param0, int param1, int param2, int param3, int param4, int param5);

Sprite *ov41_02246280(UnkStruct_ov22_0225A0E4 *param0, int param1, int param2, int param3, int param4, int param5) {
    SpriteResourcesHeader v0;
    SpriteListTemplate v1;

    CreateSpriteResourcesHeader(&v0, param1, param1, param1, param1, 0xffffffff, 0xffffffff, 0, 0, param0->unk_48[0], param0->unk_48[1], param0->unk_48[2], param0->unk_48[3], 0L, 0L);

    v1.list = param0->unk_44;
    v1.resourceData = &v0;
    v1.position.x = param2 << 12;
    v1.position.y = param3 << 12;
    v1.position.z = 0;
    v1.priority = param4;
    v1.vramType = (NNS_G2D_VRAM_TYPE)(param5);
    v1.heapID = HEAP_ID_14;

    return Sprite_Create(&v1);
}
