#include "global.h"
#include "sprite_resources_header.h"

#include "wild_encounter.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"

struct Background;

struct BgConfig;

struct CharTransferAllocation;

struct MailMessage;

struct TextOBJ;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct PaletteBuffer;

struct PaletteData;

struct PaletteFadeControl;

struct Pokepic;

struct PokepicManager;

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchHitboxController;

struct TouchScreenHitTableActionStateInternal;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct Window;

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;

typedef struct PaletteBuffer {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    PaletteFadeControl selected;
} PaletteBuffer;

typedef struct PaletteData {
    PaletteBuffer buffers[14];
    u16 selectedFlag : 2;
    u16 selectedBuffer : 14;
    u16 transparentBit : 14;
    u16 callbackFlag : 1;
    u16 autoTransparent : 1;
    u8 forceExit;
    u8 unused[3];
} PaletteData;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

BOOL sub_02021AC8(int size, BOOL atEnd, NNS_G2D_VRAM_TYPE vramType, CharTransferAllocation *allocation);

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

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct ManagedSprite {
    Sprite *sprite;
    SpriteResourcesHeader *resourceHeader;
    SpriteResourcesHeaderList *resourceHeaderList;
    BOOL vramTransfer;
} ManagedSprite;

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    GfGfxResType type;
} SpriteResourceCollection;

typedef struct GF_2DGfxResHeader {
    void *entries;
    int count;
    GfGfxResType type;
    u8 loadFromNARC;
} GF_2DGfxResHeader;

typedef struct GF_2DGfxResObjList {
    SpriteResource **resources;
    int capacity;
    int count;
} GF_2DGfxResObjList;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct SpriteSystem {
    enum HeapID heapID;
    int spriteManagerCount;
    BOOL inUse;
    NNSG2dCellTransferState *cellTransferStates;
    G2dRenderer renderer;
} SpriteSystem;

typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    GF_2DGfxResHeader *resourcePaths;
    SpriteResourceCollection *ownedResources[GF_GFX_RES_TYPE_MAX];
    GF_2DGfxResObjList *unownedResources[GF_GFX_RES_TYPE_MAX];
    int loadedResourceCount[GF_GFX_RES_TYPE_MAX];
    int numResourceTypes;
} SpriteManager;

const NNSG2dImagePaletteProxy *SpriteManager_FindPlttResourceProxy(SpriteManager *spriteMan, int resourceID);

void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);

SpriteList *SpriteManager_GetSpriteList(SpriteManager *spriteMan);

typedef struct TextOBJ TextOBJ;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct SaveData SaveData;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

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

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef struct MessageFormat {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} MessageFormat;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    BOOL unk_18;
    u8 padding_1C[4];
} UnkStruct_ov62_0223118C_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_02233310_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct {
    const UnkStruct_ov62_02249380 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    u32 unk_24;
    const TouchScreenRect *unk_28;
} UnkStruct_ov62_02248CDC;

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
    int xScale;
    int yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

void InitWindow(Window *window);

void AddTextWindowTopLeftCorner(BgConfig *bgConfig, Window *window, u8 width, u8 height, u16 baseTile, u8 fillVal);

void RemoveWindow(Window *window);

typedef struct {
    int unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[2];
    int unk_0C;
    int unk_10;
    int unk_14;
    Window unk_18;
    const UnkStruct_ov62_02248CDC *unk_28;
    ManagedSprite *unk_2C[2];
    struct MsgData *unk_34;
    UnkStruct_ov62_02233310_sub1 unk_38;
    BOOL unk_48;
    u8 padding_4C[4];
} UnkStruct_ov62_02233310;

typedef struct {
    int unk_00;
    BOOL unk_04;
    u32 unk_08;
    u32 unk_0C;
    s16 unk_10;
    u8 padding_12[2];
    BOOL unk_14;
    ManagedSprite *unk_18[2];
} UnkStruct_ov62_0223359C;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchHitboxController {
    const TouchscreenHitbox *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
} TouchHitboxController;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    u8 padding_0C[4];
    BgConfig *unk_10;
    PaletteData *unk_14;
    TouchHitboxController *unk_18;
    TouchScreenRect pad_1C[5];
    BOOL unk_30;
    struct MsgData *unk_34;
    struct MsgData *unk_38;
    UnkStruct_02012744 *unk_3C;
    int unk_40;
    int unk_44;
    u8 unk_48;
    u8 padding_49[3];
    G3DPipelineBuffers *unk_4C;
    PokepicManager *unk_50;
    struct { struct SaveData *saveData; void *unk_04; void *unk_08; struct { s32 unk_00; u8 unk_04; u8 language; s8 unk_06[301]; u8 padding[5]; } unk_0C; u32 heapID; int unk_148; int unk_14C; struct { void *unk_00; void *unk_04; void *unk_08; void *unk_0C; void *unk_10; void *unk_14; void *unk_18; void *unk_1C; void *unk_20; void *unk_24; void *unk_28; } unk_150; struct { BOOL unk_00; int unk_04; int unk_08; int unk_0C; } unk_17C; void *unk_18C; struct { u8 unk_0_anon[540]; u8 unk_21C[8]; } unk_190; void *unk_3B4; int unk_3B8; int unk_3BC; int unk_3C0; BOOL unk_3C4; BOOL unk_3C8; struct { int unk_00; int unk_04; int unk_08; } unk_3CC; int unk_3D8; int unk_3DC; int unk_3E0; int unk_3E4; int unk_3E8; int unk_3EC; int unk_3F0; struct MsgData *unk_3F4; struct MessageFormat *unk_3F8; struct String *unk_3FC; struct { void *unk_00; u16 unk_04; u16 unk_06; u8 unk_08; u8 unk_C_pad[3]; void *unk_0C; } unk_400; u8 unk_40D; u8 unk_40E; u8 unk_412_tail[2]; } unk_54;
    UnkStruct_ov62_0223359C unk_46C;
    UnkStruct_ov62_02233310 unk_48C;
    u8 padding_4D8[24];
    UnkStruct_ov62_0223118C_sub1 unk_4F0;
    BOOL unk_510;
    BOOL unk_514;
    u8 pad_518[4];
} UnkStruct_ov62_0223118C;

typedef struct {
    u8 padding_00[4];
    ManagedSprite *unk_08;
    TextOBJ *unk_0C;
    UnkStruct_02012B20 *unk_10;
    CharTransferAllocation unk_14;
} UnkStruct_ov62_022312B0;

typedef struct {
    const UnkStruct_02012744 *unk_00;
    const Window *unk_04;
    SpriteList *unk_08;
    const NNSG2dImagePaletteProxy *unk_0C;
    const Sprite *unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_020127E8;

int sub_02013688(const Window *param0, int param1, enum HeapID heapID);

void TextOBJ_SetSpritesDrawFlag(TextOBJ *param0, BOOL param1);

void sub_020138E0(TextOBJ *param0, u32 param1);

UnkStruct_02012B20 *sub_02013910(const Window *param0, enum HeapID heapID);

TextOBJ *TextOBJ_Create(const UnkStruct_020127E8 *param0, const UnkStruct_02012B20 *param1);

void ov40_0222D5AC(UnkStruct_ov62_022312B0 *param0, UnkStruct_ov62_0223118C *param1, int param2);

void ov40_0222D5AC(UnkStruct_ov62_022312B0 *param0, UnkStruct_ov62_0223118C *param1, int param2) {
    s16 v0, v1;
    UnkStruct_020127E8 v2;
    int v4;
    Window v6;
    int v7 = 9999;

    if (param2 == NNS_G2D_VRAM_TYPE_2DSUB) {
        v7 = 10000;
    }

    InitWindow(&v6);
    AddTextWindowTopLeftCorner(param1->unk_10, &v6, 20, 2, 0, 0);

    param0->unk_10 = sub_02013910(&v6, HEAP_ID_109);
    v4 = sub_02013688(&v6, param2, HEAP_ID_109);

    sub_02021AC8(v4, 1, (NNS_G2D_VRAM_TYPE)(param2), &param0->unk_14);
    ManagedSprite_GetPositionXY(param0->unk_08, &v0, &v1);

    v2.unk_00 = param1->unk_3C;
    v2.unk_04 = &v6;
    v2.unk_08 = SpriteManager_GetSpriteList(param1->unk_08);
    v2.unk_0C = SpriteManager_FindPlttResourceProxy(param1->unk_08, v7);
    v2.unk_10 = param0->unk_08->sprite;
    v2.unk_14 = param0->unk_14.offset;
    v2.unk_18 = v0 + 36;
    v2.unk_1C = v1 + -8;
    v2.unk_20 = 3;
    v2.unk_24 = 0;
    v2.unk_28 = param2;
    v2.heapID = HEAP_ID_109;

    param0->unk_0C = TextOBJ_Create(&v2, param0->unk_10);

    sub_020138E0(param0->unk_0C, (2 - 1));
    RemoveWindow(&v6);
    TextOBJ_SetSpritesDrawFlag(param0->unk_0C, 0);
}
