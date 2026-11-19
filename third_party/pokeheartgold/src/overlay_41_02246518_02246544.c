#include "global.h"

#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_3d_loader.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "bg_window.h"
#include "filesystem.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

typedef enum HeapID HeapID;

struct BgConfig;

struct BgTemplate;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SoftwareSpriteChars;

struct SoftwareSpriteCharsTemplate;

struct SoftwareSpriteManager;

struct SoftwareSpriteManagerTemplate;

struct SoftwareSpritePalette;

struct SoftwareSpritePaletteTemplate;

struct SpriteAnimFrame;

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

enum BgScreenSize {
    BG_SCREEN_SIZE_128x128 = 0,
    BG_SCREEN_SIZE_256x256,
    BG_SCREEN_SIZE_256x512,
    BG_SCREEN_SIZE_512x256,
    BG_SCREEN_SIZE_512x512,
    BG_SCREEN_SIZE_1024x1024,
};

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void FreeBgTilemapBuffer(BgConfig *bgConfig, u8 bgLayer);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov22_02255410(UnkStruct_ov22_02255CB8 *param0, enum HeapID heapID);

void ov22_0225547C(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1, enum HeapID heapID);

void ov22_02255BF4(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID);

void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3);

void ov22_02255CB8(UnkStruct_ov22_02255CB8 *param0, int param1, int param2, enum HeapID heapID);

void ov41_02246544(UnkStruct_ov22_0225A0E4 *param0, BgConfig *param1, int param2);

void ov41_02246544(UnkStruct_ov22_0225A0E4 *param0, BgConfig *param1, int param2) {
    param0->unk_40 = param1;

    {
        BgTemplate v0 = {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = BG_SCREEN_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0xf000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = 0,
            .mosaic = 0,
        };

        FreeBgTilemapBuffer(param0->unk_40, GF_BG_LYR_MAIN_2);
        InitBgFromTemplate(param0->unk_40, GF_BG_LYR_MAIN_2, &v0, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, (HeapID)(0), (HeapID)(param2));
        BgClearTilemapBufferAndCommit(param0->unk_40, GF_BG_LYR_MAIN_2);
    }
}
