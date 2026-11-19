#include "global.h"

#include "overlay_01_021FC05C.h"

#include "constants/sndseq.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/draw_map_name.h"
#include "field/dynamic_terrain_height.h"
#include "field/encounter_check.h"
#include "field/field_3d_object_task.h"
#include "field/fog.h"
#include "field/hblank_system.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/overlay_01_021EAFD4.h"
#include "field/overlay_01_02204004.h"
#include "field/signpost.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "bug_contest_internal.h"
#include "camera.h"
#include "dex_mon_measures.h"
#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "frontier_data.h"
#include "gear_phone.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "map_events_internal.h"
#include "map_header.h"
#include "map_matrix.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FC05C.h"
#include "overlay_109.h"
#include "overlay_manager.h"
#include "photo_album.h"
#include "photo_types_def.h"
#include "player_avatar.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_link_ruleset.h"
#include "save_pokegear.h"
#include "scrcmd_9.h"
#include "script.h"
#include "script_manager.h"
#include "seal_case.h"
#include "sys_task.h"
#include "task.h"
#include "terrain_attributes.h"
#include "unk_02005D10.h"
#include "unk_0202F370.h"
#include "unk_02054648.h"
#include "unk_02055418.h"
#include "unk_02092BB8.h"
#include "wild_encounter.h"

typedef struct UnkStruct_ov5_021D1BEC UnkStruct_ov5_021D1BEC;

typedef struct TVEpisode TVEpisode;

struct AreaDataManager;

struct AreaLightManager;

struct AsyncWriteManager;

struct BUGMON;

struct Background;

struct BagCursor;

struct BagCursorBattle;

struct BagCursorField;

struct BgConfig;

struct BgEvent;

struct BicycleSlopeAnimation;

struct BoxPokemon;

struct BugContest;

struct BugContestant;

struct BugContestantData;

struct CAPSULE;

struct Camera;

struct CameraAngle;

struct CameraHistory;

struct CameraLookAt;

struct CameraPerspective;

struct CoordEvent;

struct DynamicTerrainHeightManager;

struct EncounterData;

struct EncounterDataSlot;

struct Field3dObjectTask;

struct Field3dObjectTaskManager;

struct Field3dObjectTaskTemplate;

struct Field3dRenderObj;

struct Field3dRenderObjManager;

struct FieldDrawMapNameInfo;

struct FieldEffectManager;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub104_Sub8;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub120_Sub7E4;

struct FieldSystemUnkSub120_Sub818;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

struct FieldSystemUnkSubCC_Sub0;

struct FieldTextureManager;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FrontierData;

struct FrontierFieldSystem;

struct FrontierMon;

struct FrontierMultiBattleAllyData;

struct FrontierTrainer;

struct GearPhoneRingManager;

struct HBlankSystem;

struct HoneyScentTaskEnv;

struct LandEncounterData;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct MAPDATA;

struct Mail;

struct MailMessage;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMarkingsSave;

struct MapMatrix;

struct MapMatrixData;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimation;

struct MapPropAnimationData;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimation;

struct MapPropOneShotAnimationList;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct MsgData;

struct NARC;

struct ObjectEvent;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Party;

struct PartyCore;

struct PartyExtra;

struct PartyExtraSub;

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PhoneRematch;

struct Photo;

struct PhotoAlbum;

struct PhotoAlbumArgs;

struct PhotoCameraParam;

struct PhotoMon;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SEAL;

struct SaveArrayHeader;

struct SaveData;

struct SavePokegear;

struct SaveSlotSpec;

struct Signpost;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct TextureSlot;

struct UnkPokegearSub8List;

struct UnkStruct_020556FC;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

struct WarpEvent;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

BOOL ov01_021FC2C4(FieldSystem *fieldSystem);

void ov01_021FC1EC(FieldSystem *fieldSystem);

UnkStruct_ov5_021D1BEC *ov01_021FC110(FieldSystem *fieldSystem);

UnkStruct_ov5_021D1BEC *ov01_021FC138(FieldSystem *fieldSystem);

BOOL Task_HoneyOrSweetScent(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    HoneyScentTaskEnv *env = TaskManager_GetEnvironment(taskManager);
    u32 x;
    u32 z;

    switch (env->state) {
    case 0:
        if (ov01_021FC2C8(fieldSystem) == 1) {
            env->state = 1;
            env->unk_4 = ov01_021FC2C4(fieldSystem);
            if (env->unk_4 != 0) {
                ov01_021FC1EC(fieldSystem);
            }
        } else {
            env->state = 7;
            env->timer = 0x14;
        }
        break;
    case 1:
        env->task = (struct Field3dObjectTask *)(ov01_021FC110(fieldSystem));
        env->state = 2;
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
        break;
    case 2:
        if (ov01_021FC12C(env->task) != 0) {
            env->timer = 0x16;
            env->state = 3;
        }
        break;
    case 3:
        env->timer--;
        if (MapHeader_HasWildEncounters(fieldSystem->location->mapId)) {
            if (env->timer < 0) {
                x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
                z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
                if (FieldSystem_CanGenerateStepEncounter(fieldSystem, GetMetatileBehavior(fieldSystem, x, z))) {
                    env->state = 6;
                } else {
                    env->state = 4;
                }
            }
        } else {
            env->state = 4;
        }
        break;
    case 4:
        ov01_021FC124(env->task);
        env->task = (struct Field3dObjectTask *)(ov01_021FC138(fieldSystem));
        env->state = 5;
        break;
    case 5:
        if (ov01_021FC12C(env->task) != 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
            reg_G2_BLDCNT = 0;
            SetBgPriority(GF_BG_LYR_MAIN_2, 3);
            env->state = 8;
        }
        break;
    case 6:
        ov01_021FC2F0(env);
        if (env->unk_4 != 0) {
            ov01_021FC260(fieldSystem);
        }
        ((FieldSystem_PerformSweetScentEncounterCheck(fieldSystem, taskManager)) ? (void)0 : GF_AssertFail());
        break;
    case 7:
        env->timer--;
        if (env->timer < 0) {
            QueueScript(taskManager, 0x7E3, 0L, 0L);
            env->state = 9;
        }
        break;
    case 8:
        QueueScript(taskManager, 0x7E2, 0L, 0L);
        env->state = 9;
        break;
    case 9:
        ov01_021FC2F0(env);
        if (env->unk_4 != 0) {
            ov01_021FC1FC(fieldSystem);
        }
        BgFillTilemapBufferAndCommit(fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
