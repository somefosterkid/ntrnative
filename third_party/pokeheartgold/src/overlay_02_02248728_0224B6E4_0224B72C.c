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
#include "overworld_anim_manager.h"
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
#include "overlay_02_hmcut_in.h"

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

struct Field3dObjectTask;

struct Field3dObjectTaskManager;

struct Field3dObjectTaskTemplate;

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

struct MovementScriptCommand;

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

struct ov02_A9D8Entry;

struct ov02_BF58Cfg;

struct ov02_FieldList5;


typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct ov02_A9D8Entry {
    fx32 unk0;
    fx32 unk4;
    fx32 unk8;
    void *unkC;
    u32 unk10;
} ov02_A9D8Entry;

typedef BOOL (*ov02_StateMachineFunc)(void *sm);

typedef int (*ov02_FieldTaskFunc)(TaskManager *taskManager, FieldSystem *fieldSystem, void *env);

void ov02_0224D488(void *a0, void *a1, void *a2);

void ov02_0224AAD4(void *a0, void *a1);

int ov02_0224DB9C(void *a0, void *a1, void *a2);

typedef struct ov02_BF58Cfg {
    int unk0;
    int unk4;
} ov02_BF58Cfg;

typedef struct ov02_FieldList5 {
    u32 v[5];
} ov02_FieldList5;

typedef int (*ov02_AnimDispatchFunc)(void *data);

typedef Field3dObjectTask *(*ov02_CreateDispatchFunc)(FieldSystem *fieldSystem);


void ov02_02248DE4(void *a0, void *work);

int ov02_0224997C(void *work);

int ov02_0224939C(void *work);

int ov02_02249774(void *work);


u32 ov02_0224C87C(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3, u32 param_4);

void ov02_0224AAC8(void *a0, void *work);

void ov02_0224ABF8(void *a0, void *work);

BOOL ov02_0224B0E0(void *work);

int ov02_0224C338(void *a0, void *a1, void *work);

int ov02_0224B938(void *work);

void ov02_0224B808(void *a0, void *work);

int ov02_0224B964(void *work);

int ov02_0224C71C(void *a0, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

void ov02_0224DD4C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

int ov02_0224C14C(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

int ov02_0224C7D4(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

int ov02_02249858(void *work);



int ov02_02249C74(HMCutIn *work);

int ov02_02249A5C(HMCutIn *work);

int ov02_022499EC(HMCutIn *work);

int ov02_022495E8(HMCutIn *work);

int ov02_022497C0(HMCutIn *work);

int ov02_0224B664(void *work);

int ov02_02249658(void *work);

int ov02_0224C75C(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);




int ov02_022498BC(HMCutIn *work);

BOOL ov02_02249088(void *mgr);

BOOL ov02_02248D98(void *a0, void *obj);

void ov02_0224D288(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);

void ov02_0224D3B4(Field3dObjectTask *task, FieldSystem *fieldSystem, Field3dObject *obj);

BOOL ov02_0224B7CC(void *a0, void **out);

void ov02_0224D950(void *a0, void *a1, void *data);

void ov02_0224DD8C(void *a0, void *a1, void *data);

int ov02_02249690(HMCutIn *work);

int ov02_022496D0(HMCutIn *work);

int ov02_022491CC(void *work);

int ov02_02249290(void *work);

int ov02_02248F88(void *work);

int ov02_022490BC(void *work);

int ov02_0224B158(void *work);

int ov02_0224AF70(void *work);

int ov02_0224ACE0(void *work);

int ov02_02249BD8(HMCutIn *work);

void ov02_0224D310(void *a0, void *a1, void *data);


void ov02_0224D880(void *a0, FieldSystem *fieldSystem, void *work);

int ov02_0224C6DC(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

int ov02_0224C2A8(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

u32 ov02_0224C698(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3, u32 param_4);

BOOL ov02_0224AA80(void *a0, void *work);

int ov02_0224B5F0(void *work);

void ov02_0224D1E4(void *a0, void *a1, void *data);

int ov02_0224C2EC(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

void ov02_0224D22C(void *a0, void *a1, void *data);

void ov02_0224DAA4(void *a0, void *a1, void *data);

void ov02_0224D9C0(void *a0, void *a1, void *data);

void ov02_0224D358(void *a0, void *a1, void *data);

void ov02_0224B6E4(void *a0, void *work);

void ov02_0224D914(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

BOOL ov02_0224ABCC(void *a0, void *a1);

int ov02_0224B4AC(void *work);

void ov02_02248DF0(void *a0, u8 *sm);

int ov02_02248E10(void *work);

int ov02_022491A8(void *work);

int ov02_022495B8(HMCutIn *work);

int ov02_022495D0(HMCutIn *work);

int ov02_02249754(void *work);

int ov02_02249838(void *work);

int ov02_02249940(void *work);

int ov02_02249954(void *work);

int ov02_02249968(void *work);

int ov02_02249AC4(HMCutIn *work);

int ov02_02249AD8(HMCutIn *work);

BOOL ov02_02249AF0(HMCutIn *work);

int ov02_02249B10(HMCutIn *work);

int ov02_02249B38(HMCutIn *work);

BOOL ov02_02249B60(HMCutIn *work);

BOOL ov02_02249B80(HMCutIn *work);

BOOL ov02_02249BA8(HMCutIn *work);

int ov02_02249CD8(HMCutIn *work);

void ov02_0224AB54(void);

void ov02_0224AC04(void *a0, u8 *sm);

void ov02_0224AC24(void);

int ov02_0224AC28(void *work);

BOOL ov02_0224B350(void *a0, void *out);

void ov02_0224B3FC(void *a0, int *a1);

int ov02_0224B494(void *work);

int ov02_0224B638(void *work);

BOOL ov02_0224B6D0(void *a0, void *out);

void ov02_0224B804(void);

void ov02_0224B87C(void *a0, void *a1);

int ov02_0224C1B8(TaskManager *taskManager, void *a1, void *a2);

int ov02_0224C4B4(void *a0, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *work);

int ov02_0224C4D8(void *a0, void *a1, FieldMoveTaskEnvironment *work);

int ov02_0224C680(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *data);

int ov02_0224CA38(TaskManager *taskManager, void *a1, FieldMoveTaskEnvironment *a2);

void ov02_0224D278(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D2BC(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

Field3dObjectTask *ov02_0224D2C8(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224D2DC(FieldSystem *fieldSystem);

void ov02_0224D2F0(Field3dObjectTask *task);

BOOL ov02_0224D2F8(Field3dObjectTask *task);

void ov02_0224D3A4(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D3E8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

Field3dObjectTask *ov02_0224D3F4(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224D408(FieldSystem *fieldSystem);

void ov02_0224D41C(Field3dObjectTask *task);

BOOL ov02_0224D424(Field3dObjectTask *task);


void ov02_0224D468(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D580(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D648(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D658(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D670(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224D98C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DB8C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DC58(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

Field3dObjectTask *ov02_0224DC64(FieldSystem *fieldSystem);

Field3dObjectTask *ov02_0224DC78(FieldSystem *fieldSystem);

void ov02_0224DC8C(Field3dObjectTask *task);

BOOL ov02_0224DC94(Field3dObjectTask *task);

void ov02_0224DCB0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DD38(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

void ov02_0224DDC8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data);

extern const struct {
    ov02_StateMachineFunc f022532F8[1];
    u16 f022532FC[2];
    ov02_StateMachineFunc f02253300[1];
    u16 f02253304[3];
    u16 f0225330A[3];
    u16 f02253310[8];
    ov02_StateMachineFunc *f02253320[4];
    ov02_StateMachineFunc f02253330[6];
    VecFx32 f02253348;
    VecFx32 f02253354;
    VecFx32 f02253360;
    VecFx32 f0225336C;
    VecFx32 f02253378;
    VecFx32 f02253384;
    VecFx32 f02253390;
    VecFx32 f0225339C;
    VecFx32 f022533A8;
    VecFx32 f022533B4;
    ov02_StateMachineFunc *f022533C0[3];
    VecFx32 f022533CC;
    VecFx32 f022533D8;
    VecFx32 f022533E4;
    VecFx32 f022533F0;
    VecFx32 f022533FC;
    VecFx32 f02253408;
    VecFx32 f02253414;
    ov02_StateMachineFunc f02253420[4];
    fx32 f02253430[4];
    OverworldAnimManagerFuncs f02253440;
    OverworldAnimManagerFuncs f02253454;
    OverworldAnimManagerFuncs f02253468;
    OverworldAnimManagerFuncs f0225347C;
    OverworldAnimManagerFuncs f02253490;
    OverworldAnimManagerFuncs f022534A4;
    ov02_StateMachineFunc f022534B8[6];
    ov02_StateMachineFunc f022534D0[8];
    ov02_StateMachineFunc f022534F0[12];
    fx32 f02253520[12];
    ov02_StateMachineFunc f02253550[14];
    ov02_StateMachineFunc f02253588[23];
    ov02_A9D8Entry f022535E4[13];
    ov02_BF58Cfg f022536E8;
    ov02_FieldTaskFunc f022536F0[4];
    ov02_FieldTaskFunc f02253700[4];
    ov02_FieldTaskFunc f02253710[5];
    ov02_FieldTaskFunc f02253724[6];
    ov02_FieldTaskFunc f0225373C[6];
    ov02_FieldTaskFunc f02253754[7];
    MovementScriptCommand f02253770[9];
    MovementScriptCommand f02253794[9];
    MovementScriptCommand f022537B8[9];
    MovementScriptCommand f022537DC[17];
    MovementScriptCommand f02253820[25];
    MovementScriptCommand f02253884[26];
    u8 f022538EC[16];
    Field3dObjectTaskTemplate f022538FC;
    Field3dObjectTaskTemplate f02253914;
    Field3dObjectTaskTemplate f0225392C;
    Field3dObjectTaskTemplate f02253944;
    Field3dObjectTaskTemplate f0225395C;
    Field3dObjectTaskTemplate f02253974;
    Field3dObjectTaskTemplate f0225398C;
    Field3dObjectTaskTemplate f022539A4;
    Field3dObjectTaskTemplate f022539BC;
    Field3dObjectTaskTemplate f022539D4;
    Field3dObjectTaskTemplate f022539EC;
    ov02_AnimDispatchFunc f02253A04[6];
    ov02_CreateDispatchFunc f02253A1C[6];
    ov02_AnimDispatchFunc f02253A34[6];
    u8 f02253A4C[8];
    u8 f02253A54[8];
    ov02_FieldList5 f02253A5C;
    MovementScriptCommand f02253A70[4][5];
    u32 f02253AC0[25];
    VecFx32 f02253B24;
    VecFx32 f02253B30;
} sRodata;

void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);

void *ov02_0224A800(u16 *a0, enum HeapID heapID);

void FollowMon_ExpandInteractionMessage(void *work, void *dest, enum HeapID heapID, int strno);

void ov02_0224FCE0(void *work, String *dest, enum HeapID heapID, int flags, u8 a5);

void ov02_0224B72C(HMCutIn *work);

extern const OverworldAnimManagerFuncs ov02_02253440;

void ov02_0224B72C(HMCutIn *work) {
    VecFx32 pos = { 0, 0, 0 };
    struct {
        void *unk0;
        void *unk4;
    } a4;
    a4.unk0 = ((void *)work->playerSprite);
    a4.unk4 = work;
    work->unk_1F4 = sub_02068B0C(((void *)work->animManagerList), &(ov02_02253440), &pos, 0, &a4, 0x87);
}
