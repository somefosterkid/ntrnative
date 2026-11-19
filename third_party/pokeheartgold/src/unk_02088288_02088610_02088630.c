#include "global.h"
#include "constants/heap.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

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

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

enum BgScreenSize {
    BG_SCREEN_SIZE_128x128 = 0,
    BG_SCREEN_SIZE_256x256,
    BG_SCREEN_SIZE_256x512,
    BG_SCREEN_SIZE_512x256,
    BG_SCREEN_SIZE_512x512,
    BG_SCREEN_SIZE_1024x1024,
};

typedef struct BgTemplate {
    u32 x;
    u32 y;
    u32 bufferSize;
    u32 baseTile;
    u8 size;
    u8 colorMode;
    u8 screenBase;
    u8 charBase;
    u8 bgExtPltt;
    u8 priority;
    u8 areaOver;
    u8 dummy;
    u32 mosaic;
} BgTemplate;

typedef struct BgConfig BgConfig;

typedef struct GraphicsModes {
    GXDispMode displayMode;
    GXBGMode mainBgMode;
    GXBGMode subBgMode;
    GXBG0As bg0As2DOr3D;
} GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ScheduleSetBgPosText(BgConfig *bgConfig, u8 bgLayer, u8 bgOffsetUpdateOp, int val);

void sub_02088630(BgConfig *bgConfig);

extern const GraphicsModes _021038C4;

extern const BgTemplate _021038E8;

extern const BgTemplate _02103904;

extern const BgTemplate _02103920;

extern const BgTemplate _0210393C;

extern const BgTemplate _02103958;

extern const BgTemplate _02103974;

void sub_02088630(BgConfig *bgConfig) {
    GraphicsModes graphicsModes = _021038C4;

    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate bgMain1 = _021038E8;

    InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_1, &bgMain1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_1);

    BgTemplate bgMain2 = _02103904;

    InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_2, &bgMain2, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_2);
    ScheduleSetBgPosText(bgConfig, BG_LAYER_MAIN_2, BG_OFFSET_UPDATE_SET_X, 0);
    ScheduleSetBgPosText(bgConfig, BG_LAYER_MAIN_2, BG_LAYER_MAIN_3, BG_TYPE_STATIC);

    BgTemplate bgMain3 = _02103920;

    InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_3, &bgMain3, BG_TYPE_STATIC);

    BgTemplate bgSub0 = _0210393C;

    InitBgFromTemplate(bgConfig, (enum BgLayer)4, &bgSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum BgLayer)4);

    BgTemplate bgSub1 = _02103958;

    InitBgFromTemplate(bgConfig, (enum BgLayer)5, &bgSub1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(bgConfig, (enum BgLayer)5);

    BgTemplate bgSub2 = _02103974;

    InitBgFromTemplate(bgConfig, (enum BgLayer)6, &bgSub2, BG_TYPE_STATIC);

    BG_ClearCharDataRange(BG_LAYER_MAIN_1, 32, 0, HEAP_ID_19);
    BG_ClearCharDataRange((enum BgLayer)4, 32, 0, HEAP_ID_19);
}
