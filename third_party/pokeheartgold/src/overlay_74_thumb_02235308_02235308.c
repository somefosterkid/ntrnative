#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "overlay_manager.h"

struct OverlayManager;

struct BgConfig;

struct BgTemplate;

enum HeapSize {
    HEAP_SIZE_SYSTEM = 0xD200,
    HEAP_SIZE_SAVE = 0x20E00,
    HEAP_SIZE_DEBUG = 0x10,
    HEAP_SIZE_APPLICATION = 0x10D800,

    HEAP_SIZE_BATTLE_HALL_APP = 0x20000,
    HEAP_SIZE_BATTLE_FACTORY_APP = 0x20000,
    HEAP_SIZE_BATTLE_ARCADE_APP = 0x20000,
    HEAP_SIZE_FRONTIER_RECORDS_APP = 0x20000,
    HEAP_SIZE_BAG = 0x30000,

    HEAP_SIZE_PRE_POKETCH_SUBSCREEN = 0x18000,
    HEAP_SIZE_POKETCH_MAIN = 0xC000,
    HEAP_SIZE_POKETCH_APP = 0xC000,
    HEAP_SIZE_BOX_DATA = 0x4000,
    HEAP_SIZE_BOX_GRAPHICS = 0x3C000,

    HEAP_SIZE_FIELD1 = 0x20000,
    HEAP_SIZE_FIELD2 = 0x1C000,
    HEAP_SIZE_FIELD3 = 0x4000,

    HEAP_SIZE_TOWN_MAP = 0x20000,

    HEAP_SIZE_EGG_HATCH = 0x40000,
    HEAP_SIZE_CHOOSE_STARTER_APP = 0x40000,
    HEAP_SIZE_LIBRARY_TV = 0x40000,
    HEAP_SIZE_TRAINER_CASE = 0x28000,
    HEAP_SIZE_MINING = 0x50000,
    HEAP_SIZE_DISTORTION_WORLD_WARP = 0x50000,
    HEAP_SIZE_UNDERGROUND = 0xE800,
    HEAP_SIZE_DRAWING = 0x40000,
    HEAP_SIZE_MAIL = 0x1000,
    HEAP_SIZE_MAIL_VIEWER = 0x20000,
    HEAP_SIZE_BOAT_CUTSCENE = 0x20000,
    HEAP_SIZE_EVOLUTION = 0x30000,
    HEAP_SIZE_GAME_START = 0x20000,
    HEAP_SIZE_RECORD_LIST = 0x6000,
    HEAP_SIZE_MAIN_MENU = 0x40000,
    HEAP_SIZE_MIGRATE_FROM_GBA = 0x38000,
    HEAP_SIZE_RANGER_LINK = 0x20000,
    HEAP_SIZE_MYSTERY_GIFT_APP = 0x30000,
    HEAP_SIZE_WONDER_CARDS_APP = 0x30000,
    HEAP_SIZE_NETWORK_ICON = 0x300,
};

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

enum BgScreenSize {
    BG_SCREEN_SIZE_128x128 = 0,
    BG_SCREEN_SIZE_256x256,
    BG_SCREEN_SIZE_256x512,
    BG_SCREEN_SIZE_512x256,
    BG_SCREEN_SIZE_512x512,
    BG_SCREEN_SIZE_1024x1024,
};

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void *ov74_022352D0(OverlayManager *appMan, enum HeapID heapID, int appDataSize, enum HeapSize heapSize);

void ov74_02235308(BgConfig *bgConfig, enum GFBgLayer bgLayer, u8 screenSize, u32 screenBase, u32 charBase);

void ov74_02235308(BgConfig *bgConfig, enum GFBgLayer bgLayer, u8 screenSize, u32 screenBase, u32 charBase) {
    BgTemplate bgTemplate = {
        .x = 0,
        .y = 0,
        .bufferSize = 0x800,
        .baseTile = 0,
        .size = BG_SCREEN_SIZE_256x256,
        .colorMode = GX_BG_COLORMODE_16,
        .screenBase = GX_BG_SCRBASE_0x0000,
        .charBase = GX_BG_CHARBASE_0x00000,
        .bgExtPltt = GX_BG_EXTPLTT_01,
        .priority = 0,
        .areaOver = 0,
        .mosaic = FALSE
    };

    bgTemplate.size = screenSize;

    switch (screenSize) {
    case BG_SCREEN_SIZE_256x256:
        bgTemplate.bufferSize = 0x800;
        break;
    case BG_SCREEN_SIZE_256x512:
        bgTemplate.bufferSize = 0x1000;
        break;
    case BG_SCREEN_SIZE_512x256:
        bgTemplate.bufferSize = 0x1000;
        break;
    case BG_SCREEN_SIZE_512x512:
        bgTemplate.bufferSize = 0x2000;
        break;
    }

    bgTemplate.screenBase = screenBase / 0x800;
    bgTemplate.charBase = charBase / 0x4000;

    InitBgFromTemplate(bgConfig, bgLayer, &bgTemplate, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, bgLayer);
}
