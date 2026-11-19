#include "global.h"

#include "constants/sndseq.h"

#include "error_handling.h"
#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01_02203A18.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "sprite.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_02023694.h"
#include "vram_transfer_manager.h"

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

struct Sprite;

struct SpriteList;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct TextureSlot;

struct UnkOv01_02203A18;

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

BOOL ov01_02203DF8(UnkOv01_02203A18_Anim *anim);

void ov01_02203CB8(OverworldAnimManager *param0, UnkOv01_02203A18_Work *work);

void ov01_02203CB8(OverworldAnimManager *param0, UnkOv01_02203A18_Work *work) {
    VecFx32 result;
    VecFx32 pos;
    VecFx32 facing;
    VecFx32 c;
    VecFx32 d;
    LocalMapObject *mapObject = work->unk30.unk8;
    GF_ASSERT(sub_0205F0A8(mapObject, work->unk8, work->unkC));
    MapObject_CopyPositionVector(mapObject, &pos);
    MapObject_CopyFacingVector(mapObject, &facing);
    sub_0205F990(mapObject, &c);
    sub_0205F9B0(mapObject, &d);
    result.x = pos.x + facing.x + c.x + d.x;
    result.y = pos.y + c.y + d.y;
    result.z = pos.z + facing.z + c.z + d.z;
    result.y += 2 << 0x10;
    result.z += 2 << 0xb;
    switch (work->unk0) {
    case 0:
        sub_02023EA4(work->unk3C, 1);
        work->unk1C += work->unk28;
        if (work->unk1C > 0) {
            work->unk28 -= 2 << 0xc;
        } else {
            work->unk1C = 0;
            work->unk28 = 0;
            work->unk0++;
        }
        break;
    case 1:
        if (ov01_02203DF8(&work->anim) != 0) {
            work->unk0++;
        }
        break;
    case 2:
        work->unk4++;
        if (work->unk4 >= 2) {
            work->unk0++;
            work->unk4 = 0;
            work->unk14 = 1;
        }
        break;
    case 3:
        work->unk30.unk4->busy = 0;
        ov01_021F1640(param0);
        return;
    }
    sub_02068DA8(param0, &result);
}
