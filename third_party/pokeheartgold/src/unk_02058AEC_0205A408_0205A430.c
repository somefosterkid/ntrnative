#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "mail_message.h"
#include "player_data.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02034354.h"
#include "unk_02034B0C.h"
#include "unk_02035900.h"
#include "unk_02091564.h"
#include "union_room_manager.h"

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

struct UnkStruct_02059E1C_Sub80;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

struct UnkStruct_021D41CC;

struct WarpEvent;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef void (*UnkFuncPtr_02059E1C)(struct UnionRoomManager *param0);

typedef struct UnkStruct_02059E1C_Sub80 {
    u8 unk0[0x1C];
    u8 unk1C;
} UnkStruct_02059E1C_Sub80;

typedef struct UnkStruct_021D41CC {
    int unk0;
    UnkStruct_02059E1C_Sub80 *unk4;
    int unk8;
} UnkStruct_021D41CC;

extern UnkStruct_021D41CC _021D41CC;

void sub_0205A430(int param0, int param1, void *param2, void *param3);

void sub_0205A430(int param0, int param1, void *param2, void *param3) {
    FieldSystem *fieldSystem = (FieldSystem *)param3;
    u8 *v1 = (u8 *)param2;

    if (fieldSystem->unk80->chatEnded == 0) {
        fieldSystem->unk80->peerActivity = *v1;
        _021D41CC.unk8 = *v1;
    }
}
