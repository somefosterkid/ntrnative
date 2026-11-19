#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "field/field_3d_model.h"

struct Background;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct TextOBJ;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct Window;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

String *String_New(u32 size, u32 heapID);

typedef struct TextOBJ TextOBJ;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

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

void AddTextWindowTopLeftCorner(BgConfig *bgConfig, Window *window, u8 width, u8 height, u16 baseTile, u8 fillVal);

void RemoveWindow(Window *window);

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

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

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

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct CameraLookAt {
    VecFx32 position;
    VecFx32 target;
    VecFx32 up;
} CameraLookAt;

typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct Camera {
    CameraPerspectiveProjection perspective;
    CameraLookAt lookAt;
    fx32 distance;
    CameraAngle angle;
    u8 projection;
    u16 fovY;
    VecFx32 prevTargetPos;
    const VecFx32 *targetPos;
    BOOL trackTargetX;
    BOOL trackTargetY;
    BOOL trackTargetZ;
    CameraPositionHistory *history;
} Camera;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

BOOL sub_02021AC8(int size, BOOL atEnd, NNS_G2D_VRAM_TYPE vramType, CharTransferAllocation *allocation);

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

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

SpriteResource *AddPlttResObjFromNarc(SpriteResourceCollection *spriteResources, enum NarcId narcID, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, int paletteIdx, enum HeapID heapID);

SpriteResource *AddCharResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID);

SpriteResource *AddPlttResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, NNS_G2D_VRAM_TYPE vramType, int paletteIdx, enum HeapID heapID);

SpriteResource *AddCellOrAnimResObjFromOpenNarc(SpriteResourceCollection *spriteResources, NARC *narc, int memberIdx, BOOL compressed, int id, GfGfxResType type, enum HeapID heapID);

void sub_0200A740(SpriteResource *spriteRes);

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

BOOL SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(const SpriteResource *resource);

BOOL SpriteTransfer_CreatePlttTransferTask(const SpriteResource *resource);

const NNSG2dImagePaletteProxy *SpriteTransfer_GetPaletteProxy(const SpriteResource *resource, NNSG2dImageProxy *imageProxy);

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

void TextOBJ_SetSpritesDrawFlag(TextOBJ *param0, BOOL param1);

UnkStruct_02012B20 *sub_02013910(const Window *param0, enum HeapID heapID);

int sub_02013948(const UnkStruct_02012B20 *param0, int param1);

TextOBJ *TextOBJ_Create(const UnkStruct_020127E8 *param0, const UnkStruct_02012B20 *param1);

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov115_02261304;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    Window unk_38;
    TextOBJ *unk_48;
    UnkStruct_02012B20 *unk_4C;
    CharTransferAllocation unk_50;
    String *unk_5C;
    SpriteResource *unk_60;
    UnkStruct_ov115_02261304 unk_64;
    UnkStruct_ov115_02261304 unk_7C;
    s16 unk_94;
    s16 unk_96;
    u16 unk_98;
    s16 unk_9A;
    enum HeapID heapID;
} UnkStruct_ov115_02265788;

void ov91_0225D40C(UnkStruct_ov115_02261304 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

void ov91_02261580(UnkStruct_ov115_02265788 *param0, UnkStruct_ov115_02261ADC *param1, NARC *param2, u32 heapID);

void ov91_02261580(UnkStruct_ov115_02265788 *param0, UnkStruct_ov115_02261ADC *param1, NARC *param2, u32 heapID) {
    param0->heapID = (enum HeapID)(heapID);

    {
        BOOL v0;

        param0->unk_00[0] = AddCharResObjFromOpenNarc(param1->unk_1AC[0], param2, 10, 0, 140, NNS_G2D_VRAM_TYPE_2DMAIN, (enum HeapID)(heapID));
        param0->unk_00[1] = AddPlttResObjFromOpenNarc(param1->unk_1AC[1], param2, 11, 0, 140, NNS_G2D_VRAM_TYPE_2DMAIN, 1, (enum HeapID)(heapID));
        param0->unk_00[2] = AddCellOrAnimResObjFromOpenNarc(param1->unk_1AC[2], param2, 9, 0, 140, (GfGfxResType)(2), (enum HeapID)(heapID));
        param0->unk_00[3] = AddCellOrAnimResObjFromOpenNarc(param1->unk_1AC[3], param2, 8, 0, 140, (GfGfxResType)(3), (enum HeapID)(heapID));

        v0 = SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(param0->unk_00[0]);
        ((v0) ? (void)0 : GF_AssertFail());
        v0 = SpriteTransfer_CreatePlttTransferTask(param0->unk_00[1]);
        ((v0) ? (void)0 : GF_AssertFail());

        sub_0200A740(param0->unk_00[0]);
        sub_0200A740(param0->unk_00[1]);
        CreateSpriteResourcesHeader(&param0->unk_10, 140, 140, 140, 140, 0xffffffff, 0xffffffff, 0, 0, param1->unk_1AC[0], param1->unk_1AC[1], param1->unk_1AC[2], param1->unk_1AC[3], 0L, 0L);
    }

    {
        SpriteListTemplate v1;

        v1.list = param1->unk_1C;
        v1.resourceData = &param0->unk_10;
        v1.position.x = (-((fx32)(((64) > 0) ? ((64) * (1 << 12) + 0.5f) : ((64) * (1 << 12) - 0.5f))));
        v1.position.y = ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f)));
        v1.priority = (0 + 1);
        v1.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        v1.heapID = (enum HeapID)(heapID);

        param0->unk_34 = Sprite_Create(&v1);
        Sprite_SetDrawFlag(param0->unk_34, 0);
    }

    {
        int v2;
        BOOL v3;
        UnkStruct_020127E8 v4;

        param0->unk_5C = String_New(16, heapID);
        AddTextWindowTopLeftCorner(param1->unk_00, &param0->unk_38, 3, 2, 0, 0);
        param0->unk_4C = sub_02013910(&param0->unk_38, (enum HeapID)(heapID));

        v2 = sub_02013948(param0->unk_4C, NNS_G2D_VRAM_TYPE_2DMAIN);
        v3 = sub_02021AC8(v2, 1, NNS_G2D_VRAM_TYPE_2DMAIN, &param0->unk_50);
        ((v3 == 1) ? (void)0 : GF_AssertFail());

        param0->unk_60 = AddPlttResObjFromNarc(param1->unk_1AC[1], (enum NarcId)(16), 7, 0, 150, NNS_G2D_VRAM_TYPE_2DMAIN, 1, (enum HeapID)(heapID));
        v3 = SpriteTransfer_CreatePlttTransferTask(param0->unk_60);
        ((v3) ? (void)0 : GF_AssertFail());

        sub_0200A740(param0->unk_60);

        v4.unk_00 = param1->unk_1F0;
        v4.unk_04 = &param0->unk_38;
        v4.unk_08 = param1->unk_1C;
        v4.unk_0C = SpriteTransfer_GetPaletteProxy(param0->unk_60, 0L);
        v4.unk_10 = param0->unk_34;
        v4.unk_14 = param0->unk_50.offset;
        v4.unk_18 = -7;
        v4.unk_1C = -5;
        v4.unk_20 = 0;
        v4.unk_24 = 0;
        v4.unk_28 = NNS_G2D_VRAM_TYPE_2DMAIN;
        v4.heapID = (enum HeapID)(heapID);

        param0->unk_48 = TextOBJ_Create(&v4, param0->unk_4C);

        TextOBJ_SetSpritesDrawFlag(param0->unk_48, 0);
        RemoveWindow(&param0->unk_38);
    }

    ov91_0225D40C(&param0->unk_64, -((fx32)(((64) > 0) ? ((64) * (1 << 12) + 0.5f) : ((64) * (1 << 12) - 0.5f))), ((fx32)(((32) > 0) ? ((32) * (1 << 12) + 0.5f) : ((32) * (1 << 12) - 0.5f))), ((fx32)(((36) > 0) ? ((36) * (1 << 12) + 0.5f) : ((36) * (1 << 12) - 0.5f))), 8);
    ov91_0225D40C(&param0->unk_7C, ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f))), ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f))), ((fx32)(((12) > 0) ? ((12) * (1 << 12) + 0.5f) : ((12) * (1 << 12) - 0.5f))), 4);

    param0->unk_94 = 0;
    param0->unk_96 = 4;
}
