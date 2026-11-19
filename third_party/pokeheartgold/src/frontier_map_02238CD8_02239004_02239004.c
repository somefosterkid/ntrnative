#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_map.h"

#include "assert.h"
#include "bg_window.h"
#include "field_bgm.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "overlay_42.h"
#include "overlay_80_02229EE0.h"
#include "overlay_80_0222ACA0.h"
#include "palette.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_020210A0.h"
#include "unk_02026E30.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"

struct BgConfig;

struct BgTemplate;

struct FrontierMapBgTemplates;

struct FrontierMapInternal;

struct FrontierMapRodata;

struct FrontierMapSpriteData;

struct G2dRenderer;

struct GF3DVramMan;

struct GraphicsBanks;

struct GraphicsModes;

struct ManagedSprite;

struct NARC;

struct OamCharTransferParam;

struct OamManagerParam;

struct PaletteData;

struct PlayerProfile;

struct PokedexData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov42_02227F68;

struct UnkStruct_ov42_022280A8;

struct UnkStruct_ov42_02228EDC;

struct UnkStruct_ov42_022293B8;

struct UnkStruct_ov42_022299C0;

struct UnkStruct_ov42_02229A40;

struct UnkStruct_ov44_02232031;

struct UnkStruct_ov44_02232914;

struct UnkTemplate_ov42_022293B8;

s32 ov80_0222ACA0(s32 index, s32 fieldSel);

typedef struct FrontierMapSpriteData {
    ManagedSprite *sprites[8];
    u16 ids[8];
    u32 flags;
    u16 resIds[8];
} FrontierMapSpriteData;

typedef struct FrontierMapInternal {
    BgConfig *bgConfig;
    PaletteData *paletteData;
    void *work;
    GF3DVramMan *vramMan;
    void *particles;
    UnkStruct_ov42_022280A8 *unk14;
    UnkStruct_ov42_02227F68 *unk18;
    UnkStruct_ov44_02232914 scroll;
    UnkStruct_ov42_02228EDC *unk20;
    UnkStruct_ov42_022293B8 *unk24;
    UnkStruct_ov42_022293B8 *unk28;
    UnkStruct_ov42_02229A40 *unk2C;
    UnkStruct_ov42_022299C0 *unk30;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    FrontierMapSpriteData spriteData;
    ManagedSprite *unk80[4];
    u8 unk90[4];
    SysTask *unk94;
    SysTask *unk98;
    SysTask *unk9C;
    SysTask *unkA0;
    u8 unkA4[4];
    s16 unkA8;
    s16 unkAA;
    u8 padAC[0xC1 - 0xAC];
    u8 scene;
    u8 padC2[2];
} FrontierMapInternal;

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

void ov80_02239004(FrontierMapInternal *map, u8 scene, PlayerProfile *profile);

void ov80_02239004(FrontierMapInternal *map, u8 scene, PlayerProfile *profile) {
    UnkTemplate_ov42_022293B8 tmpl;
    SpriteList *spriteList;
    s32 variant;
    s32 hasSub;
    NarcId narcId;
    NARC *narc;

    map->unk14 = ov42_02228010(0x20, HEAP_ID_101);
    map->unk18 = ov42_02227EE0(0x10, 0x10, HEAP_ID_101);
    ov42_02229394(&map->scroll);
    spriteList = SpriteManager_GetSpriteList(map->spriteManager);
    map->unk20 = ov42_02228F24(spriteList, map->paletteData, 0x20, ov80_0222A7EC(profile), 0, 1, HEAP_ID_101);
    tmpl = ov80_0223D554.ov80_0223D554;
    tmpl.narcId = (NarcId)ov80_0222ACA0(scene, 5);
    tmpl.fileId = ov80_0222ACA0(scene, 6);
    hasSub = ov80_0222ACA0(scene, 0xC);
    if (hasSub == 0) {
        map->unk24 = ov42_022293B8(SpriteSystem_GetRenderer(map->spriteSystem), map->bgConfig, &tmpl, HEAP_ID_101);
    }
    if (ov80_0222ACA0(scene, 9) != 0xFFFF) {
        tmpl.fileId = ov80_0222ACA0(scene, 9);
        tmpl.unk_1 = 2;
        tmpl.screenBase = 1;
        tmpl.charBase = 8;
        tmpl.priority = 1;
        if (hasSub == 0) {
            map->unk28 = ov42_022293B8(SpriteSystem_GetRenderer(map->spriteSystem), map->bgConfig, &tmpl, HEAP_ID_101);
        }
    }
    map->unk2C = ov42_02229A40(0x80, HEAP_ID_101);
    map->unk30 = (UnkStruct_ov42_022299C0 *)ov42_02229974(0x80, HEAP_ID_101);
    ov42_02227F48(map->unk18, ov80_0223D554.ov80_0223D654);
    variant = ov80_0222ACA0(scene, 0);
    narcId = (NarcId)ov80_0222ACA0(scene, 5);
    narc = NARC_New(narcId, HEAP_ID_101);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, ov80_0222ACA0(scene, 7), map->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_101);
    if (variant == 0) {
        PaletteData_LoadNarc(map->paletteData, narcId, ov80_0222ACA0(scene, 8), HEAP_ID_101, PLTTBUF_MAIN_BG, 0x160, 0);
    } else {
        NNSG2dPaletteData *plttData;
        void *buf = GfGfxLoader_GetPlttDataFromOpenNarc(narc, ov80_0222ACA0(scene, 8), &plttData, HEAP_ID_101);
        DC_FlushRange(plttData->pRawData, plttData->szByte);
        GX_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(plttData->pRawData, 0x6000, 0x2000);
        GX_EndLoadBGExtPltt();
        Heap_Free(buf);
    }
    PaletteData_FillPaletteInBuffer(map->paletteData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0, 0, 1);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, ov80_0222ACA0(scene, 6), map->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_101);
    if (ov80_0222ACA0(scene, 9) != 0xFFFF) {
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, ov80_0222ACA0(scene, 0xA), map->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_101);
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, ov80_0222ACA0(scene, 9), map->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_101);
        if (variant != 0) {
            NNSG2dPaletteData *plttData;
            void *buf = GfGfxLoader_GetPlttDataFromOpenNarc(narc, ov80_0222ACA0(scene, 0xB), &plttData, HEAP_ID_101);
            DC_FlushRange(plttData->pRawData, plttData->szByte);
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(plttData->pRawData, 0x4000, 0x2000);
            GX_EndLoadBGExtPltt();
            Heap_Free(buf);
        }
    }
    ScheduleBgTilemapBufferTransfer(map->bgConfig, 3);
    NARC_Delete(narc);
}
