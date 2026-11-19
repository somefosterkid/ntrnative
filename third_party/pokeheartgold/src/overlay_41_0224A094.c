#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "gf_gfx_planes.h"
#include "pokemon_types_def.h"
#include "pokepic.h"

struct AffineSpriteListTemplate;
struct TextOBJ;
struct Pokemon;
struct Pokepic;
struct PokemonSpriteTransforms;
struct Sprite;
struct SpriteList;
struct UnkStruct_02012744_t;
typedef struct LoadCharContext {
    BgConfig *bgConfig;
    NNSG2dCharacterData *charData;
    void *ncgrBuffer;
    enum GFBgLayer bgLayer;
    int baseTile;
} LoadCharContext;
typedef struct LoadPlttContext {
    NNSG2dPaletteData *paletteData;
    void *nclrBuffer;
    int screen;
    int offset;
    int size;
} LoadPlttContext;
typedef struct Sprite Sprite;
typedef struct AffineSpriteListTemplate AffineSpriteListTemplate;
Sprite *Sprite_CreateAffine(const AffineSpriteListTemplate *template);
typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;
typedef void (*UnkFuncPtr_ov22_02259D78)(UnkStruct_ov22_02259C58 *param0, void *param1);
typedef struct {
    AffineSpriteListTemplate *unk_00;
    UnkFuncPtr_ov22_02259D78 unk_04;
    void *unk_08;
    int unk_0C;
} UnkStruct_ov22_02259C58_1;
void ov41_0224A254(UnkStruct_ov22_02259C58 *param0, void *param1);
typedef struct SpriteList SpriteList;
struct Sprite {
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
};
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
struct AffineSpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    VecFx32 affineScale;
    u16 affineZRotation;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
};
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct {
    UnkStruct_ov22_02259C58_1 unk_00;
    const Window *unk_10;
    const UnkStruct_02012744 *unk_14;
    const NNSG2dImagePaletteProxy *unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
} UnkStruct_ov22_02259C9C;
typedef struct TextOBJ TextOBJ;
typedef struct {
    UnkStruct_ov22_02259C58 unk_00;
    TextOBJ *unk_10;
    UnkStruct_02021AC8 unk_14;
} UnkStruct_ov22_0225A154;
void ov41_0224A118(UnkStruct_ov22_02259C58 *param0, UnkStruct_ov22_02259C58_1 *param1);
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
TextOBJ *sub_020135D8(const UnkStruct_020127E8 *param0);
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);
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
void Sprite_Delete(Pokepic *monSprite);
typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;

void ov41_0224A094(SysTask *sysTask, void *data);
void ov41_0224A0D0(SysTask *sysTask, void *data);
void ov41_0224A118(UnkStruct_ov22_02259C58 *param0, UnkStruct_ov22_02259C58_1 *param1);
void ov41_0224A15C(UnkStruct_ov22_0225A154 *param0, UnkStruct_ov22_02259C9C *param1);
void ov41_0224A1A8(UnkStruct_020298D8 *param0);

void ov41_0224A094(SysTask *sysTask, void *data) {
    LoadCharContext *ctx = data;

    DC_FlushRange(ctx->charData->pRawData, ctx->charData->szByte);

    BG_LoadCharTilesData(ctx->bgConfig, ctx->bgLayer, ctx->charData->pRawData, ctx->charData->szByte, ctx->baseTile);
    SysTask_Destroy(sysTask);
    Heap_Free(ctx->ncgrBuffer);
    Heap_Free(ctx);
}

void ov41_0224A0D0(SysTask *sysTask, void *data) {
    LoadPlttContext *ctx = data;

    DC_FlushRange(ctx->paletteData->pRawData, ctx->size);

    if (ctx->screen == 0) {
        GX_LoadBGPltt(ctx->paletteData->pRawData, ctx->offset, ctx->size);
    } else if (ctx->screen == 4) {
        GXS_LoadBGPltt(ctx->paletteData->pRawData, ctx->offset, ctx->size);
    }

    SysTask_Destroy(sysTask);
    Heap_Free(ctx->nclrBuffer);
    Heap_Free(ctx);
}

void ov41_0224A118(UnkStruct_ov22_02259C58 *param0, UnkStruct_ov22_02259C58_1 *param1) {
    ((param0) ? (void)0 : GF_AssertFail());
    ((param1) ? (void)0 : GF_AssertFail());

    param0->unk_00 = Sprite_CreateAffine(param1->unk_00);
    ((param0->unk_00) ? (void)0 : GF_AssertFail());

    param0->unk_04 = param1->unk_0C;
    param0->unk_08 = param1->unk_08;

    if (param1->unk_04) {
        param0->unk_0C = param1->unk_04;
    } else {
        param0->unk_0C = ov41_0224A254;
    }
}

void ov41_0224A15C(UnkStruct_ov22_0225A154 *param0, UnkStruct_ov22_02259C9C *param1) {
    UnkStruct_020127E8 v0;

    ov41_0224A118(&param0->unk_00, &param1->unk_00);

    v0.unk_00 = param1->unk_14;
    v0.unk_04 = param1->unk_10;
    v0.unk_08 = param1->unk_00.unk_00->list;
    v0.unk_0C = param1->unk_18;
    v0.unk_10 = param0->unk_00.unk_00;
    v0.unk_14 = param1->unk_24;
    v0.unk_18 = param1->unk_1C;
    v0.unk_1C = param1->unk_20;
    v0.unk_20 = 0;
    v0.unk_24 = 0;
    v0.unk_28 = param1->unk_00.unk_00->vramType;
    v0.heapID = param1->unk_00.unk_00->heapID;

    param0->unk_10 = sub_020135D8(&v0);
}

void ov41_0224A1A8(UnkStruct_020298D8 *param0) {
    Sprite_Delete(param0->unk_00);
    memset(param0, 0, sizeof(UnkStruct_020298D8));
}
