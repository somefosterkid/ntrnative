#include "global.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_42.h"
#include "sprite_system.h"

struct BgConfig;

struct BgTemplate;

struct FrontierMapBgTemplates;

struct FrontierMapRodata;

struct GraphicsBanks;

struct GraphicsModes;

struct OamCharTransferParam;

struct OamManagerParam;

struct UnkTemplate_ov42_022293B8;

void GfGfx_SetBanks(const GraphicsBanks *banks);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable);

void SetBothScreensModesAndDisable(const GraphicsModes *modes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgId, const BgTemplate *template, u8 bgType);

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgId, enum BgPosAdjustOp op, int val);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgId);

s32 ov80_0222ACA0(s32 index, s32 fieldSel);

typedef struct FrontierMapBgTemplates {
    BgTemplate t[3];
} FrontierMapBgTemplates;

typedef struct FrontierMapRodata {
    UnkTemplate_ov42_022293B8 ov80_0223D554;
    GraphicsModes ov80_0223D560;
    OamCharTransferParam ov80_0223D570;
    SpriteResourceCountsListUnion ov80_0223D584;
    BgTemplate ov80_0223D59C;
    OamManagerParam ov80_0223D5B8;
    GraphicsBanks ov80_0223D5D8;
    FrontierMapBgTemplates ov80_0223D600;
    u8 ov80_0223D654[0x400];
} FrontierMapRodata;

extern const FrontierMapRodata ov80_0223D554;

void FrontierMap_SetVramBank(BgConfig *bgConfig, u8 scene);

void FrontierMap_SetVramBank(BgConfig *bgConfig, u8 scene) {
    GraphicsBanks banks;
    GraphicsModes modes;
    FrontierMapBgTemplates templates;
    BgTemplate bgTemplate4;
    int bgMode;
    u16 size;

    bgMode = ov80_0222ACA0(scene, 0);
    GfGfx_DisableEngineAPlanes();
    banks = ov80_0223D554.ov80_0223D5D8;
    GfGfx_SetBanks(&banks);
    MI_CpuClear32((void *)HW_BG_VRAM, 0x80000);
    MI_CpuClear32((void *)HW_DB_BG_VRAM, 0x20000);
    MI_CpuClear32((void *)HW_OBJ_VRAM, 0x40000);
    MI_CpuClear32((void *)HW_DB_OBJ_VRAM, 0x20000);
    modes = ov80_0223D554.ov80_0223D560;
    modes.bgMode = (GXBGMode)bgMode;
    SetBothScreensModesAndDisable(&modes);
    templates = ov80_0223D554.ov80_0223D600;
    if (bgMode == 0) {
        templates.t[1].colorMode = 0;
        templates.t[2].colorMode = 0;
        templates.t[1].bgExtPltt = 0;
        templates.t[2].bgExtPltt = 0;
    }
    size = ov80_0222ACA0(scene, 4);
    templates.t[2].size = size;
    if (ov80_0222ACA0(scene, 9) != 0xFFFF) {
        templates.t[1].size = size;
    }
    if (bgMode == 0) {
        InitBgFromTemplate(bgConfig, 1, &templates.t[0], 0);
        BgClearTilemapBufferAndCommit(bgConfig, 1);
        BgSetPosTextAndCommit(bgConfig, 1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, 1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, 2, &templates.t[1], 0);
        BgClearTilemapBufferAndCommit(bgConfig, 2);
        BgSetPosTextAndCommit(bgConfig, 2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, 2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, 3, &templates.t[2], 0);
        BgClearTilemapBufferAndCommit(bgConfig, 3);
        BgSetPosTextAndCommit(bgConfig, 3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, 3, BG_POS_OP_SET_Y, 0);
    } else {
        InitBgFromTemplate(bgConfig, 1, &templates.t[0], 0);
        BgClearTilemapBufferAndCommit(bgConfig, 1);
        BgSetPosTextAndCommit(bgConfig, 1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, 1, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, 2, &templates.t[1], 2);
        BgClearTilemapBufferAndCommit(bgConfig, 2);
        BgSetPosTextAndCommit(bgConfig, 2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, 2, BG_POS_OP_SET_Y, 0);
        InitBgFromTemplate(bgConfig, 3, &templates.t[2], 2);
        BgClearTilemapBufferAndCommit(bgConfig, 3);
        BgSetPosTextAndCommit(bgConfig, 3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bgConfig, 3, BG_POS_OP_SET_Y, 0);
    }
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(1, 1);
    bgTemplate4 = ov80_0223D554.ov80_0223D59C;
    InitBgFromTemplate(bgConfig, 4, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);
    BgSetPosTextAndCommit(bgConfig, 4, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, 4, BG_POS_OP_SET_Y, 0);
}
