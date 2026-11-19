#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/sndseq.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/overlay_01_021FB878.h"

#include "overworld_anim_manager.h"
#include "overlay_02_hmcut_in.h"

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
#include "unk_020689C8.h"

struct AreaDataManager;

struct AreaLightManager;

struct BagCursor;

struct BgConfig;

struct BoxPokemon;

struct BugContest;

struct CAPSULE;

struct Camera;

struct DynamicTerrainHeightManager;

struct Field3dObject;

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

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;


void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

BOOL ov02_0224B7CC(OverworldAnimManager *manager, FollowMonModelCtx *ctx);

BOOL ov02_0224B7CC(OverworldAnimManager *manager, FollowMonModelCtx *ctx) {
    VecFx32 pos;
    const void *p = sub_02068D98(manager);
    HMCutInFieldModel *model;
    ctx->cutIn = *(HMCutIn *const *)p;
    model = &(*(HMCutIn *const *)p)->fieldModel;
    sub_02068DB8(manager, &pos);
    Field3dObject_SetPos(&model->object, &pos);
    Field3dObject_SetActiveFlag(&model->object, 0);
    return 1;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_02_02248728_0224AC38_022533FC.s (same label), with the context size from the type, not the DS literal.
void ov02_0224B804(void);
void ov02_0224B808(OverworldAnimManager *manager, FollowMonModelCtx *ctx);
void ov02_0224B87C(OverworldAnimManager *manager, FollowMonModelCtx *ctx);
extern const OverworldAnimManagerFuncs ov02_02253490;
const OverworldAnimManagerFuncs ov02_02253490 = OVERWORLD_ANIM_MANAGER_FUNCS(FollowMonModelCtx, ov02_0224B7CC, ov02_0224B804, ov02_0224B808, ov02_0224B87C);
#endif
