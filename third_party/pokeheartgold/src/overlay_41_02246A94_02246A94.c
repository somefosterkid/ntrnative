#include "global.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"

struct Background;

struct BgConfig;

struct CharTransferTemplate;

struct G2dRenderer;

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

struct SpriteResource;

struct SpriteResourceCollection;

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
    Background bgs[8];
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

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

SpriteResourceCollection *Create2DGfxResObjMan(int capacity, GfGfxResType type, enum HeapID heapID);

SpriteList *G2dRenderer_Init(int maxElements, G2dRenderer *g2dRenderer, enum HeapID heapID);

void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *g2dRenderer, fx32 x, fx32 y);

typedef struct CharTransferTemplate {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    enum HeapID heapID;
} CharTransferTemplate;

void ObjCharTransfer_InitEx(const CharTransferTemplate *template, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub);

void ObjCharTransfer_ClearBuffers(void);

void ObjPlttTransfer_Init(int capacity, enum HeapID heapID);

void ObjPlttTransfer_Reset(void);

void OamManager_Create(int mainOamStart,
    int mainOamCount,
    int mainAffineOamStart,
    int mainAffineOamCount,
    int subOamStart,
    int subOamCount,
    int subAffineOamStart,
    int subAffineOamCount,
    enum HeapID heapID);

void ov22_02255410(UnkStruct_ov22_02255CB8 *param0, enum HeapID heapID);

void ov22_0225547C(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1, enum HeapID heapID);

void ov22_02255BF4(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID);

void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3);

void ov22_02255CB8(UnkStruct_ov22_02255CB8 *param0, int param1, int param2, enum HeapID heapID);

void ov41_02246A94(UnkStruct_ov22_0225A0E4 *param0);

extern const CharTransferTemplate ov41_0224BFA4;

void ov41_02246A94(UnkStruct_ov22_0225A0E4 *param0) {
    {
        CharTransferTemplate v0 = ov41_0224BFA4;

        ObjCharTransfer_InitEx(&v0, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    }

    ObjPlttTransfer_Init(5, HEAP_ID_14);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();

    NNS_G2dInitOamManagerModule();

    OamManager_Create(0, 124, 0, 31, 0, 124, 0, 31, (enum HeapID)(14));
    param0->unk_44 = G2dRenderer_Init(48, &param0->unk_58, HEAP_ID_14);
    G2dRenderer_SetSubSurfaceCoords(&param0->unk_58, 0, 512 * ((fx32)0x0000000000001000L));

    param0->unk_48[0] = Create2DGfxResObjMan(8, (GfGfxResType)(0), HEAP_ID_14);
    param0->unk_48[1] = Create2DGfxResObjMan(5, (GfGfxResType)(1), HEAP_ID_14);
    param0->unk_48[2] = Create2DGfxResObjMan(48, (GfGfxResType)(2), HEAP_ID_14);
    param0->unk_48[3] = Create2DGfxResObjMan(48, (GfGfxResType)(3), HEAP_ID_14);
}
