#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "field_roamer.h"
#include "field_system.h"
#include "field_warp_tasks.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "roamer.h"
#include "safari_zone.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_flags.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "unk_0205FD20.h"
#include "unk_02062108.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BoxPokemon;

struct BugContest;

struct CAPSULE;

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

struct GearPhoneRingManager;

struct LinkBattleRuleset;

struct LocalFieldData;

struct LocalMapObject;

struct Location;

struct Mail;

struct MailMessage;

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

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SEAL;

struct SaveData;

struct SavePokegear;

struct Signpost;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

extern const fx32 sub_02054774(const FieldSystem *fieldSystem, fx32 refHeight, fx32 xFx32, fx32 zFx32, u8 *outSelector);

BOOL ov02_0224E308(int a0);

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

BOOL ov02_0224E35C(FieldSystem *fieldSystem);

BOOL ov02_0224E35C(FieldSystem *fieldSystem) {
    void *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    void *safariZone;
    int facing;
    int x;
    int z;
    int newX;
    int newZ;
    VecFx32 pos;
    u8 sel;
    fx32 h1;
    fx32 h2;
    f32 fx;
    f32 fz;
    int vx;
    int vz;

    LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(fieldSystem->saveData));
    safariZone = Save_SafariZone_Get(fieldSystem->saveData);
    if (ov02_0224E308(*((int *)fieldSystem->location)) == 0) {
        return 0;
    }
    if (Save_VarsFlags_CheckSafariSysFlag(varsFlags) == 0) {
        return 0;
    }
    if (sub_0202F620(safariZone) != 0) {
        return 0;
    }
    if (SafariZone_GetObjectUnlockLevel(safariZone) == 0) {
        return 0;
    }
    facing = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    newX = x + GetDeltaXByFacingDirection(facing);
    newZ = z + GetDeltaYByFacingDirection(facing);
    if (newX < 0x20 || newX >= 0x80 || newZ < 0x20 || newZ >= 0x60) {
        return 0;
    }
    if (newX < 0x20 || newX >= 0x80 || newZ < 0x20 || newZ >= 0x60) {
        return 0;
    }
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    h1 = sub_02054774(fieldSystem, pos.y, pos.x, pos.z, &sel);
    if (sel != 1) {
        return 0;
    }
    vz = (newZ << 4) + 8;
    if (vz > 0) {
        fz = (f32)(vz << 12) + 0.5f;
    } else {
        fz = (f32)(vz << 12) - 0.5f;
    }
    vx = (newX << 4) + 8;
    if (vx > 0) {
        fx = (f32)(vx << 12) + 0.5f;
    } else {
        fx = (f32)(vx << 12) - 0.5f;
    }
    h2 = sub_02054774(fieldSystem, pos.y, (s32)fx, (s32)fz, &sel);
    if (sel != 1) {
        return 0;
    }
    return h1 == h2;
}
