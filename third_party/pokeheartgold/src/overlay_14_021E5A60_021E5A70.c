#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_14_box_funcs.h"
struct UnkStruct_ov14_boxapp;

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void Bg_ScheduleAffineRotationCenter(BgConfig *bgConfig, u8 bgLayer, u8 bgAffineUpdateOp, int val);

BgConfig *BgConfig_Alloc(enum HeapID heapID);

extern const GraphicsModes ov14_021F7BC8;

extern const BgTemplate ov14_021F7C20;

extern const BgTemplate ov14_021F7C3C;

extern const BgTemplate ov14_021F7C74;

extern const BgTemplate ov14_021F7C90;

extern const BgTemplate ov14_021F7C58;

extern const BgTemplate ov14_021F7CAC;

extern const BgTemplate ov14_021F7CC8;

void ov14_021E5A70(UnkStruct_ov14_boxapp *param_1) {
    param_1->sys->unk_14 = BgConfig_Alloc((enum HeapID)10);
    GraphicsModes graphicsModes = ov14_021F7BC8;
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate layerMain0 = ov14_021F7C20;
    InitBgFromTemplate(param_1->sys->unk_14, GF_BG_LYR_MAIN_0, &layerMain0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(param_1->sys->unk_14, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, (enum HeapID)10);

    BgTemplate layerMain1 = ov14_021F7C3C;
    InitBgFromTemplate(param_1->sys->unk_14, GF_BG_LYR_MAIN_1, &layerMain1, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(param_1->sys->unk_14, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, (enum HeapID)10);

    BgTemplate layerMain2 = ov14_021F7C74;
    InitBgFromTemplate(param_1->sys->unk_14, GF_BG_LYR_MAIN_2, &layerMain2, BG_TYPE_STATIC);

    BgTemplate layerMain3 = ov14_021F7C90;
    InitBgFromTemplate(param_1->sys->unk_14, GF_BG_LYR_MAIN_3, &layerMain3, BG_TYPE_STATIC);

    BgTemplate layerSub0 = ov14_021F7C58;
    InitBgFromTemplate(param_1->sys->unk_14, GF_BG_LYR_SUB_0, &layerSub0, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(param_1->sys->unk_14, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, (enum HeapID)10);

    BgTemplate layerSub1 = ov14_021F7CAC;
    InitBgFromTemplate(param_1->sys->unk_14, GF_BG_LYR_SUB_1, &layerSub1, BG_TYPE_STATIC);

    BgTemplate layerSub3 = ov14_021F7CC8;
    InitBgFromTemplate(param_1->sys->unk_14, (enum GFBgLayer)6, &layerSub3, BG_TYPE_STATIC);
    BgClearTilemapBufferAndCommit(param_1->sys->unk_14, (enum GFBgLayer)6);
    BG_ClearCharDataRange((enum GFBgLayer)6, 32, 0, (enum HeapID)10);
}
