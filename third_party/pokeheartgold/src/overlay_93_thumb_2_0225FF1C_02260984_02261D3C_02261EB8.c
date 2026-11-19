#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "font_types_def.h"

struct Background;

struct BgConfig;

struct CharTransferAllocation;

struct FontOAM;

struct Resource;

struct ResourceCollection;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourceList;

struct SpriteResourceTable;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct TextPrinterTemplate;

struct UnkStruct_02012744_t;

struct Window;


typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct Resource {
    int id;
    void *data;
} Resource;

typedef struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
} ResourceCollection;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

typedef struct SpriteResourceTable {
    void *entries;
    int count;
    enum SpriteResourceType type;
    u8 loadFromNARC;
} SpriteResourceTable;

typedef struct SpriteResourceList {
    SpriteResource **resources;
    int capacity;
    int count;
} SpriteResourceList;


SpriteList *SpriteManager_GetSpriteList(SpriteManager *spriteMan);

void InitWindow(Window *window);

void AddTextWindowTopLeftCorner(BgConfig *bgConfig, Window *window, u8 width, u8 height, u16 baseTile, u8 fillVal);

void RemoveWindow(Window *window);

typedef u16 charcode_t;

union StringToPrint {
    const charcode_t *raw;
    const String *string;
};

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

enum Font {
    FONT_SYSTEM = 0,
    FONT_MESSAGE,
    FONT_SUBSCREEN,
    FONT_UNOWN,

    FONT_MAX,
};

u32 FontID_String_GetWidthMultiline(enum Font font, const String *string, u32 letterSpacing);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColorAndSpacing(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, u32 letterSpacing, u32 lineSpacing, TextPrinterCallback callback);

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

FontOAM *sub_020135D8(const UnkStruct_020127E8 *param0);

int sub_02013688(const Window *param0, int param1, enum HeapID heapID);

void sub_020136B4(FontOAM *fontOAM, int x, int y);

void sub_020138B0(FontOAM *param0, u32 param1);

void ov93_02261EB8(BgConfig *param0, SpriteManager *param1, UnkStruct_02012744 *param2, UnkStruct_ov117_02265EB0 *param3, const String *param4, enum Font param5, TextColor param6, int param7, int param8, int param9, int param10, int param11, int param12, int param13, int param14);

void ov93_02261EB8(BgConfig *param0, SpriteManager *param1, UnkStruct_02012744 *param2, UnkStruct_ov117_02265EB0 *param3, const String *param4, enum Font param5, TextColor param6, int param7, int param8, int param9, int param10, int param11, int param12, int param13, int param14) {
    UnkStruct_020127E8 v0;
    Window v1;
    UnkStruct_02021AC8 v2;
    int v3;
    FontOAM *v4;
    int v5, v6;
    int v7 = 0;

    {
        v5 = FontID_String_GetWidthMultiline(param5, param4, v7);
        v6 = v5 / 8;

        if (FX_ModS32(v5, 8) != 0) {
            v6++;
        }
    }

    {
        InitWindow(&v1);
        AddTextWindowTopLeftCorner(param0, &v1, v6, param14, 0, 0);
        AddTextPrinterParameterizedWithColorAndSpacing(&v1, param5, param4, 0, 0, 0xFF, param6, v7, 0, 0L);
    }

    v3 = sub_02013688(&v1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_117);
    sub_02021AC8(v3, 1, NNS_G2D_VRAM_TYPE_2DMAIN, &v2);

    if (param11 == 1) {
        param9 -= v5 / 2;
    }

    v0.unk_00 = param2;
    v0.unk_04 = &v1;
    v0.unk_08 = SpriteManager_GetSpriteList(param1);
    v0.unk_0C = SpriteManager_FindPlttResourceProxy(param1, param8);
    v0.unk_10 = 0L;
    v0.unk_14 = v2.offset;
    v0.unk_18 = param9;
    v0.unk_1C = param10;
    v0.unk_20 = param12;
    v0.unk_24 = param13;
    v0.unk_28 = NNS_G2D_VRAM_TYPE_2DMAIN;
    v0.heapID = HEAP_ID_117;

    v4 = sub_020135D8(&v0);

    if (param7 != 0) {
        sub_020138B0(v4, param7);
    }

    sub_020136B4(v4, param9, param10);
    RemoveWindow(&v1);

    param3->unk_00 = v4;
    param3->unk_04 = v2;
    param3->unk_10 = v5;
}
