#include "global.h"
#include "sprite_resources_header.h"
#include "unk_0200A090.h"
#include "unk_02009D48.h"
#include "sprite_transfer.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "field/field_3d_model.h"
#include "unk_02013534.h"
#include "camera.h"

struct Background;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspective;

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


struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_02012744_t;


struct Window;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

String *String_New(u32 size, u32 heapID);

typedef struct UnkStruct_02012744_t UnkStruct_02012744;


void AddTextWindowTopLeftCorner(BgConfig *bgConfig, Window *window, u8 width, u8 height, u16 baseTile, u8 fillVal);

void RemoveWindow(Window *window);

Sprite *Sprite_Create(const SimpleSpriteTemplate *template);

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

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

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

BOOL sub_02021AC8(int size, BOOL atEnd, NNS_G2D_VRAM_TYPE vramType, CharTransferAllocation *allocation);

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
    UnkStruct_02013910 *unk_4C;
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
        SimpleSpriteTemplate v1;

        v1.spriteList = param1->unk_1C;
        v1.header = &param0->unk_10;
        v1.position.x = (-((fx32)(((64) > 0) ? ((64) * (1 << 12) + 0.5f) : ((64) * (1 << 12) - 0.5f))));
        v1.position.y = ((fx32)(((48) > 0) ? ((48) * (1 << 12) + 0.5f) : ((48) * (1 << 12) - 0.5f)));
        v1.priority = (0 + 1);
        v1.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        v1.heapID = (enum HeapID)(heapID);

        param0->unk_34 = Sprite_Create(&v1);
        Sprite_SetDrawFlag(param0->unk_34, 0);
    }

    {
        int v2;
        BOOL v3;
        TextOBJTemplate v4;

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

        v4.fontSystem = param1->unk_1F0;
        v4.window = &param0->unk_38;
        v4.spriteList = param1->unk_1C;
        v4.plttResourceProxy = SpriteTransfer_GetPaletteProxy(param0->unk_60, 0L);
        v4.sprite = param0->unk_34;
        v4.offset = param0->unk_50.offset;
        v4.x = -7;
        v4.y = -5;
        v4.unk_20 = 0;
        v4.unk_24 = 0;
        v4.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
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
