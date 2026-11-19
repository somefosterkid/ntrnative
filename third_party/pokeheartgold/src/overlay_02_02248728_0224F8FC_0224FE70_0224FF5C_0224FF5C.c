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
#include "save_local_field_data.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "unk_02062108.h"
#include "overlay_02_follow_app.h"

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

struct ov02_FollowMonStep;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct ov02_FollowMonStep {
    u8 idx : 4;
    u8 hi : 4;
} ov02_FollowMonStep;

void ov02_0224FDF8(void *arg0, u16 arg1, int arg2, int arg3);

void ov02_0224FD9C(void *arg0, LocalMapObject *mapObject);

void ov02_0224FE40(void *a0, u8 *a1, LocalMapObject *obj);

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

int ov02_0224FF5C(void *a0, LocalMapObject *a1);

int ov02_0224FF5C(void *a0, LocalMapObject *a1) {
    u8 *entry = (u8 *)a0 + 0x818 + *((u8 *)&((Ov02FollowApp *)a0)->unk_810[91]) * 8;
    if (*((u8 *)&((Ov02FollowApp *)a0)->unk_810[90]) == 0) {
        ov02_0224FD9C(entry, a1);
        ov02_0224FDF8(entry,
            *(u16 *)((u8 *)a0 + ((ov02_FollowMonStep *)((u8 *)a0 + 0x86c))->idx * 8 + 0x7e8),
            *((u16 *)&((Ov02FollowApp *)a0)->unk_870[14]),
            *((u8 *)&((Ov02FollowApp *)a0)->unk_870[13]));
        ov02_0224FE40(a0, entry, a1);
    }
    (*((u8 *)&((Ov02FollowApp *)a0)->unk_810[90]))++;
    if (*((u8 *)&((Ov02FollowApp *)a0)->unk_810[90]) >= entry[1]) {
        (*((u8 *)&((Ov02FollowApp *)a0)->unk_810[91]))++;
        return 1;
    }
    return 0;
}
