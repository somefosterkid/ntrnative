#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "pm_string.h"
#include "msgdata.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct MainMapMatrixData;

struct TownMapAppData;

struct TownMapAppFlyLocationsManager;

struct TownMapBlock;

struct TownMapBlockList;

struct TownMapContext;

struct TownMapGraphicsManager;

struct TownMapLocationHistoryEntry;

typedef struct TownMapLocationHistoryEntry {
    int x;
    int z;
    u16 faceDirection;
    u16 isSet;
} TownMapLocationHistoryEntry;

typedef struct TownMapContext TownMapContext;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct MainMapMatrixData MainMapMatrixData;

typedef struct TownMapBlock TownMapBlock;

typedef struct TownMapBlockList TownMapBlockList;

typedef struct TownMapAppFlyLocationsManager TownMapAppFlyLocationsManager;

typedef struct TownMapGraphicsManager TownMapGraphicsManager;

void ov108_021E6C68(SafariAreaCustomizer *appData);

void ov108_021E6C48(void);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

extern const GraphicsModes ov108_021EA738;

typedef struct {
    BgTemplate a[GF_BG_LYR_MAX];
} ov108_021EA898_Array;

extern const ov108_021EA898_Array ov108_021EA898;

void ov108_021E6C68(SafariAreaCustomizer *appData) {
    ov108_021E6C48();
    reg_GX_POWCNT &= ~0x8000;
    appData->bgConfig = BgConfig_Alloc(appData->heapID);

    GraphicsModes graphicsModes = ov108_021EA738;
    SetBothScreensModesAndDisable(&graphicsModes);

    ov108_021EA898_Array bgTemplates = ov108_021EA898;

    int layer = GF_BG_LYR_MAIN_0;
    for (int i = GF_BG_LYR_MAIN_0; i < GF_BG_LYR_MAX; i++) {
        InitBgFromTemplate(appData->bgConfig, layer, &(bgTemplates.a[i]), 0);
        BgClearTilemapBufferAndCommit(appData->bgConfig, layer);
        BG_ClearCharDataRange(layer, 0x40, 0, appData->heapID);
        layer++;
    }

    G2_SetBlendAlpha(2, 0x1c, 0x1c, 4);
    G2S_SetBlendAlpha(1, 0x1e, 0x1c, 4);
}
