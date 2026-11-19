#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

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
#include "field/overlay_01_021FB878.h"
#include "field/overlay_01_02204004.h"
#include "field/signpost.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "bug_contest_internal.h"
#include "camera.h"
#include "dex_mon_measures.h"
#include "field_roamer.h"
#include "field_system.h"
#include "field_types_def.h"
#include "field_warp_tasks.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "frontier_data.h"
#include "gear_phone.h"
#include "heap.h"
#include "map_events_internal.h"
#include "map_matrix.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "metatile_behavior.h"
#include "msgdata.h"
#include "overlay_01_021F1348.h"
#include "overlay_02.h"
#include "overlay_109.h"
#include "overlay_manager.h"
#include "party.h"
#include "photo_album.h"
#include "photo_types_def.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "roamer.h"
#include "safari_zone.h"
#include "save.h"
#include "save_link_ruleset.h"
#include "save_local_field_data.h"
#include "save_pokegear.h"
#include "scrcmd_9.h"
#include "screen_fade.h"
#include "script.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "task.h"
#include "terrain_attributes.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "unk_0202F370.h"
#include "unk_02055418.h"
#include "unk_0205FD20.h"
#include "unk_02062108.h"
#include "unk_02092BB8.h"
#include "unk_02097268.h"
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

struct PlayerProfile;

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

struct ov02_SafariObjCfg;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct ov02_SafariObjCfg {
    u8 buildModel;
    u8 isAnimated : 1;
    u8 width : 3;
    u8 height : 3;
    u8 hasGenderedLayout : 1;
    u8 objectType;
} ov02_SafariObjCfg;

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void *ov02_0224B690(enum HeapID heapID, u32 size);

void *ov02_0224C660(enum HeapID heapID, u32 size);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

s32 ov02_0224E31C(s32 arg0, s32 arg1);

u16 ov02_0224E754(FieldSystem *work, u16 *out) {
    ov02_SafariObjCfg cfg;
    int facing = PlayerAvatar_GetFacingDirection(work->playerAvatar);
    int x = PlayerAvatar_GetXCoord(work->playerAvatar) + GetDeltaXByFacingDirection(facing);
    int z = PlayerAvatar_GetZCoord(work->playerAvatar) + GetDeltaYByFacingDirection(facing);
    int cellIdx;
    int i;
    u8 *entry;
    u8 *obj;
    int gender = (u8)PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(work->saveData));
    cellIdx = ov02_0224E31C(x, z);
    x = x % 0x20;
    z = z % 0x20;
    entry = (u8 *)SafariZone_GetAreaSet(Save_SafariZone_Get(work->saveData), 0) + cellIdx * 0x7a;
    i = 0;
    if (i < entry[1]) {
        obj = entry + 2;
        do {
            GetSafariObjectConfig((struct SafariObjectConfig *)(&cfg), obj[0], gender);
            if (x >= obj[1] && z <= obj[3] && x < obj[1] + cfg.width && z > obj[3] - cfg.height) {
                if (out != 0L) {
                    *out = i;
                }
                return obj[0];
            }
            i++;
            obj += 4;
        } while (i < entry[1]);
    }
    *out = 0;
    return 0xff;
}
