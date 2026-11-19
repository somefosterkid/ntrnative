#include "global.h"

#include "constants/gx.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/draw_map_name.h"
#include "field/dynamic_terrain_height.h"
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
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "frontier_data.h"
#include "gear_phone.h"
#include "heap.h"
#include "map_events_internal.h"
#include "map_matrix.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021F6830.h"
#include "overlay_109.h"
#include "overlay_27.h"
#include "overlay_29.h"
#include "overlay_32.h"
#include "overlay_33.h"
#include "overlay_manager.h"
#include "photo_album.h"
#include "photo_types_def.h"
#include "player_avatar.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_link_ruleset.h"
#include "save_pokegear.h"
#include "scrcmd_9.h"
#include "screen_fade.h"
#include "script.h"
#include "seal_case.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "task.h"
#include "terrain_attributes.h"
#include "touch_save_app.h"
#include "unk_0202F370.h"
#include "unk_02055418.h"
#include "unk_02092BB8.h"
#include "wild_encounter.h"

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

struct FieldMenuManager;

struct WarpEvent;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

void ov01_021F6B34(FieldSystem *fieldSystem, UnkCallback_021F6B34 cb, struct FieldMenuManager *menu) {
    SubAppTaskData *data = SysTask_GetData(fieldSystem->unk_D8);
    ov27_0225C41C(data->appTask, cb, menu);
}
