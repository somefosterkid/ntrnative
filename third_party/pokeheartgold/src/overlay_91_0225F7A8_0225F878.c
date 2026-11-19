#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "pm_string.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

struct BgConfig;

struct BgTemplate;

struct CameraPerspective;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct G3DPipelineBuffers;

struct GraphicsModes;

struct MessageBank;

struct MessageBankEntry;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

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

void LoadFontPal1(enum GFPalLoadLocation palLocation, u32 palSlotOffset, enum HeapID heapID);

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

void LoadUserFrameGfx1(BgConfig *bgConfig, GFBgLayer bgLayer, u16 tileOffset, u8 palOffset, u8 standardWindowType, u32 heapID);

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

void ov91_0225F878(UnkStruct_ov115_02261ADC *param0, u32 heapID);

extern const GraphicsModes ov91_02261C74;

extern const BgTemplate ov91_02261C9C;

extern const BgTemplate ov91_02261CB8;

extern const BgTemplate ov91_02261CD4;

extern const BgTemplate ov91_02261CF0;

extern const BgTemplate ov91_02261D0C;

extern const BgTemplate ov91_02261D28;

void ov91_0225F878(UnkStruct_ov115_02261ADC *param0, u32 heapID) {
    param0->unk_00 = BgConfig_Alloc((enum HeapID)(heapID));

    {
        GraphicsModes v0 = ov91_02261C74;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov91_02261C9C;

        InitBgFromTemplate(param0->unk_00, GF_BG_LYR_MAIN_1, &v1, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, (enum HeapID)(heapID));
        BgClearTilemapBufferAndCommit(param0->unk_00, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v2 = ov91_02261CB8;

        InitBgFromTemplate(param0->unk_00, GF_BG_LYR_MAIN_2, &v2, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, (enum HeapID)(heapID));
        BgClearTilemapBufferAndCommit(param0->unk_00, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v3 = ov91_02261CD4;

        InitBgFromTemplate(param0->unk_00, GF_BG_LYR_SUB_0, &v3, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, (enum HeapID)(heapID));
        BgClearTilemapBufferAndCommit(param0->unk_00, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate v4 = ov91_02261CF0;

        InitBgFromTemplate(param0->unk_00, GF_BG_LYR_SUB_1, &v4, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, (enum HeapID)(heapID));
        BgClearTilemapBufferAndCommit(param0->unk_00, GF_BG_LYR_SUB_1);
    }

    {
        BgTemplate v5 = ov91_02261D0C;

        InitBgFromTemplate(param0->unk_00, GF_BG_LYR_SUB_2, &v5, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, 0, (enum HeapID)(heapID));
        BgClearTilemapBufferAndCommit(param0->unk_00, GF_BG_LYR_SUB_2);
    }

    {
        BgTemplate v6 = ov91_02261D28;

        InitBgFromTemplate(param0->unk_00, GF_BG_LYR_SUB_3, &v6, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, (enum HeapID)(heapID));
        BgClearTilemapBufferAndCommit(param0->unk_00, GF_BG_LYR_SUB_3);
    }

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, ((5) * ((16) * sizeof(u16))), (enum HeapID)(heapID));
    LoadUserFrameGfx1(param0->unk_00, GF_BG_LYR_MAIN_1, 1, 4, 0, heapID);
}
