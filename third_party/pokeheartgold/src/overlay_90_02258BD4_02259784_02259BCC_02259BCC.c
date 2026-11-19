#include "global.h"
#include "sprite_resources_header.h"
#include "g2d_renderer.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "unk_0200A090.h"

struct BgConfig;

struct BgTemplate;

struct G2dRenderer;

struct GraphicsModes;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourcesHeader;

struct Window;


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

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetAnimCtrlSeq(Sprite *sprite, u32 animID);

void Sprite_SetPalOffset(Sprite *sprite, u32 paletteOffset);

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

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





void SetBgPriority(u8 bgLayer, u8 priority);

void CopyToBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, u8 destX, u8 destY, u8 destWidth, u8 destHeight, const void *src, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight);

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef struct {
    u8 unk_00[4];
    u8 unk_04[4];
    u8 unk_08[4];
} UnkStruct_ov114_02260204;


Sprite *ov90_02258EB4(UnkStruct_ov114_0225CDB4 *param0, SpriteList *param1, s32 param2, s32 param3, u16 param4, u32 param5);

u32 ov90_0225A050(u32 param0, u32 param1, u32 param2);

extern const s16 ov90_0225C274[4][2];

extern const UnkStruct_ov114_02260204 ov90_0225C310[4];

void ov114_0225CEF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_02259BCC(UnkStruct_ov114_0225DAC0 *param0, u32 param1, u32 param2, NNSG2dScreenData *param3, UnkStruct_ov114_0225CEF0 *param4, u32 param5, u32 param6, UnkStruct_ov114_0225CFCC *param7, UnkStruct_ov114_0225CDB4 *param8, u32 heapID);

void ov90_02259BCC(UnkStruct_ov114_0225DAC0 *param0, u32 param1, u32 param2, NNSG2dScreenData *param3, UnkStruct_ov114_0225CEF0 *param4, u32 param5, u32 param6, UnkStruct_ov114_0225CFCC *param7, UnkStruct_ov114_0225CDB4 *param8, u32 heapID) {
    s32 v0;

    switch (param2) {
    case UnkEnum_ov66_022324D0_00:
    case UnkEnum_ov66_022324D0_02:
        v0 = ov90_0225A050(param1, param5, param6);
        break;
    case UnkEnum_ov66_022324D0_01:
        if (param6 > param5) {
            v0 = param5 + 1;
        } else if (param6 == param5) {
            v0 = 0;
        } else {
            v0 = param5;
        }
        break;
    default:
        v0 = param5;
        break;
    }

    SetBgPriority(BG_LAYER_MAIN_1, 2);
    SetBgPriority(BG_LAYER_MAIN_0, 1);
    AddWindowParameterized(param4->unk_00, &param0->unk_00, 0, 5, 1 + ov90_0225C310[param1].unk_00[v0], 26, 4, 12, 513 + ((26 * 4) * param5));
    FillWindowPixelBuffer(&param0->unk_00, 0);
    CopyToBgTilemapRect(param4->unk_00, 1, 0, ov90_0225C310[param1].unk_00[v0], 32, 6, param3->rawData, 0, ov90_0225C310[param1].unk_04[param5], param3->screenWidth / 8, param3->screenHeight / 8);

    {
        u8 v1;
        u8 v2, v3;

        v1 = ov90_0225C310[param1].unk_08[v0];
        v2 = ov90_0225C274[v1][0];
        v3 = (ov90_0225C310[param1].unk_00[v0] * 8) + ov90_0225C274[v1][1];

        param0->unk_10 = ov90_02258EB4(param8, param7->unk_00, v2, v3, 0, heapID);

        Sprite_SetAnimCtrlSeq(param0->unk_10, v1);
        Sprite_SetPalOffset(param0->unk_10, param5);
        Sprite_SetDrawFlag(param0->unk_10, 0);
        Sprite_SetAnimActiveFlag(param0->unk_10, 1);
    }

    {
        u8 v4, v5;

        v4 = 24;
        v5 = (ov90_0225C310[param1].unk_00[v0] * 8) + 21;

        param0->unk_14 = ov90_02258EB4(param8, param7->unk_00, v4, v5, 0, heapID);

        Sprite_SetAnimCtrlSeq(param0->unk_14, 0);
        Sprite_SetDrawFlag(param0->unk_14, 0);
    }

    param0->unk_1A = param5;
    param0->unk_1B = v0;
    param0->unk_18 = param1;
}
