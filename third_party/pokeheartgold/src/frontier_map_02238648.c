#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_map.h"

#include "assert.h"
#include "bg_window.h"
#include "field_bgm.h"
#include "filesystem.h"
#include "frontier_map.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
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
#include "frontier_map_internal.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BugContest;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dRenderObjManager;

struct FieldEffectManager;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FrontierFieldSystem;

struct FieldFrontierDTO;

struct FrontierMap;

struct FrontierMapInternal;

struct FrontierMapSpriteData;

struct GF3DVramMan;

struct GearPhoneRingManager;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct ManagedSprite;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMatrix;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct NARC;

struct PaletteData;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerProfile;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

struct UnkStruct_ov42_02227F68;

struct UnkStruct_ov42_022280A8;

struct UnkStruct_ov42_02228EDC;

struct UnkStruct_ov42_022293B8;

struct UnkStruct_ov42_022299C0;

struct UnkStruct_ov42_02229A40;

struct UnkStruct_ov44_02232914;

s32 ov80_0222ACA0(s32 index, s32 fieldSel);

void ov80_0222AD9C(void *a0, void *a1, s32 index);

typedef struct FrontierMapSpriteData {
    ManagedSprite *sprites[8];
    u16 ids[8];
    u32 flags;
    u16 resIds[8];
} FrontierMapSpriteData;

extern void Sound_SetFieldBGM(u16 seqNo);

extern void *ov80_02239960(enum HeapID heapId);

void FrontierMap_VBlank(void *arg);

void ov80_02238AAC(SysTask *task, void *arg);

void ov80_02238AB0(SysTask *task, void *arg);

void ov80_02238ABC(SysTask *task, void *arg);

void FrontierMap_Update(SysTask *task, void *arg);

void FrontierMap_SetVramBank(BgConfig *bgConfig, u8 scene);

void FrontierMap_LoadPaletteData(FrontierMapInternal *map);

void ov80_02238FA0(FrontierMapInternal *map);

void ov80_02239004(FrontierMapInternal *map, u8 scene, PlayerProfile *profile);

GF3DVramMan *ov80_022392DC(enum HeapID heapId);

void ov80_02239384(FrontierMapInternal *map);

FrontierMap *FrontierMap_Init(void *data);

FrontierMap *FrontierMap_Init(void *data) {
    FrontierMapInternal *map;
    FieldFrontierDTO *args;
    PlayerProfile *profile;
    u8 scene;
    int i;

    args = Frontier_GetLaunchArgs(data);
    profile = Save_PlayerData_GetProfile(args->saveData);
    scene = args->sceneID;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    Heap_Create(HEAP_ID_3, HEAP_ID_101, 0x90000);
    map = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMapInternal));
    MI_CpuFill8(map, 0, sizeof(FrontierMapInternal));
    map->work = data;
    map->scene = scene;
    for (i = 0; i < 8; i++) {
        map->spriteData.resIds[i] = 0xFFFF;
    }
    map->vramMan = ov80_022392DC(HEAP_ID_101);
    map->paletteData = PaletteData_Init(HEAP_ID_101);
    PaletteData_SetAutoTransparent(map->paletteData, TRUE);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_MAIN_OBJ, 0x1C0, HEAP_ID_101);
    PaletteData_AllocBuffers(map->paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_101);
    map->bgConfig = BgConfig_Alloc(HEAP_ID_101);
    GF_CreateVramTransferManager(0x40, HEAP_ID_101);
    SetKeyRepeatTimers(4, 8);
    FrontierMap_SetVramBank(map->bgConfig, scene);
    FrontierMap_LoadPaletteData(map);
    ov80_02238FA0(map);
    EnableTouchPad();
    InitializeTouchPad(4);
    ov80_02239384(map);
    map->particles = ov80_02239960(HEAP_ID_101);
    ov80_02239004(map, scene, profile);
    map->unk94 = SysTask_CreateOnMainQueue(ov80_02238AB0, map, 0xEA60);
    map->unk98 = SysTask_CreateOnMainQueue(ov80_02238ABC, map, 0xEE48);
    map->unk9C = SysTask_CreateOnMainQueue(FrontierMap_Update, map, 0x13880);
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    Sound_SetFieldBGM(ov80_0222ACA0(scene, 3));
    sub_02055198(NULL, ov80_0222ACA0(scene, 3));
    TextFlags_SetAutoScrollParam(1);
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    Main_SetVBlankIntrCB(FrontierMap_VBlank, map);
    map->unkA0 = SysTask_CreateOnVBlankQueue(ov80_02238AAC, map, 0xA);
    ov80_0222AD9C(map, map->unk90, map->scene);
    sub_0203A880();
    return (FrontierMap *)map;
}
