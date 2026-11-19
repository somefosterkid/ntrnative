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
#include "follow_mon_interact_work.h"

struct AreaDataManager;

struct AreaLightManager;

struct AsyncWriteManager;

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

struct Party;

struct PartyCore;

struct PartyExtra;

struct PartyExtraSub;

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokeathlonPerformanceStars;

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

struct UnionRoomManager;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

struct ov02_PokeathlonStarBits;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

struct ov02_PokeathlonStarBits {
    u16 s0 : 3;
    u16 s1 : 3;
    u16 s2 : 3;
    u16 s3 : 3;
    u16 s4 : 3;
};

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void *ov02_0224B690(enum HeapID heapID, u32 size);

void *ov02_0224C660(enum HeapID heapID, u32 size);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

void ov02_0224F6AC(FieldSystem *fieldSystem, int a1, int a2, FollowMonInteractWork *work);

void ov02_0224F6AC(FieldSystem *fieldSystem, int a1, int a2, FollowMonInteractWork *work) {
    PartyExtraSub aprijuice;
    struct PokeathlonPerformanceStars stars;
    int code;
    u32 best;
    Party *party = SaveArray_Party_Get(fieldSystem->saveData);
    int idx = GetIdxOfFirstAliveMonInParty_CrashIfNone(party);
    Party_GetMonAprijuiceModifiers(party, &aprijuice, idx);
    CalcMonPokeathlonStars(&stars, Party_GetMonByIndex(party, idx), (s8 *)&aprijuice, HEAP_ID_FIELD2);
    best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s0;
    code = 1;
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s4) {
        best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s4;
        code = 2;
    }
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s3) {
        best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s3;
        code = 4;
    }
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s1) {
        best = ((struct ov02_PokeathlonStarBits *)&stars.stars)->s1;
        code = 3;
    }
    if (best < ((struct ov02_PokeathlonStarBits *)&stars.stars)->s2) {
        code = 5;
    }
    work->unk_16 = code;
}
