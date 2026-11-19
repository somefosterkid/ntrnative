#include "global.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "field_warp_tasks.h"
#include "follow_mon.h"
#include "heap.h"
#include "overlay_01.h"
#include "overlay_01_022053EC.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save_local_field_data.h"
#include "script_pokemon_util.h"
#include "seal_case.h"
#include "sound_chatot.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_0203BA5C.h"
#include "unk_02062108.h"
#include "unk_02067A60.h"

struct BoxPokemon;

struct CAPSULE;

struct FieldSystem;

struct Mail;

struct MailMessage;

struct PartyPokemon;

struct SEAL;

struct TaskManager;

struct UnkStruct_02067BF8;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

struct UnkStruct_02067BF8 {
    FieldSystem *fieldSystem;
    u16 state;
    u16 unk06;
    u16 unk08;
    s16 unk0a;
    s16 unk0c;
    u16 unk0e;
    Pokemon *pokemon;
    void *unk14;
    void *unk18;
};

extern const u16 _020FE7AC[];

s32 sub_0203BB50(u32 param_1);

void GetFlyWarpData(s32 arg0, void *arg1);

BOOL sub_02067C30(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkStruct_02067BF8 *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 0:
        TaskManager_Call(taskManager, ov01_02205A60, 0L);
        env->state++;
        break;
    case 1:
        if ((u32)(PlayerAvatar_GetState(((void *)fieldSystem->playerAvatar)) - 1) <= 1) {
            env->state = 4;
            env->unk06 = 0;
        } else if (env->unk0e != GetIdxOfFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData))) {
            ov01_02205D68(env->fieldSystem);
            env->state = 4;
            env->unk06 = 0;
        } else if (FollowMon_IsVisible(fieldSystem)) {
            int mood;
            if (ov02_02250780(fieldSystem, 2)) {
                mood = 2;
                FieldSystem_UnkSub108_AddMonMood(((void *)fieldSystem->unk108), 1);
            } else {
                mood = 1;
            }
            ov02_022507B4(fieldSystem, mood);
            env->unk06 = 1;
            env->state = 2;
        } else {
            env->state = 4;
            env->unk06 = 0;
        }
        break;
    case 2:
        PlayCry((u16)GetMonData(env->pokemon, 5, 0L), (u8)GetMonData(env->pokemon, 0x70, 0L));
        env->state++;
        break;
    case 3:
        if (IsCryFinished() == 0) {
            env->state++;
        }
        break;
    case 4:
        env->unk18 = (const struct MovementScriptCommand *)(EventObjectMovementMan_Create(PlayerAvatar_GetMapObject(((void *)fieldSystem->playerAvatar)), (const struct MovementScriptCommand *)(_020FE7AC)));
        env->state++;
        break;
    case 5:
        if (EventObjectMovementMan_IsFinish(env->unk18)) {
            EventObjectMovementMan_Delete(env->unk18);
            if (env->unk06 == 0) {
                env->unk14 = ov02_02249458(env->fieldSystem, 1, env->pokemon, PlayerAvatar_GetGender(env->fieldSystem->playerAvatar));
            } else {
                env->unk14 = ov02_02249458(env->fieldSystem, 2, env->pokemon, PlayerAvatar_GetGender(env->fieldSystem->playerAvatar));
            }
            env->state++;
        }
        break;
    case 6:
        if (ov02_0224953C(env->unk14)) {
            u8 warp[0x14];
            void *localFieldData;
            u16 mapId;
            ov02_02249548(env->unk14);
            localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
            mapId = sub_0203BB50(env->unk08);
            ((mapId) ? (void)0 : GF_AssertFail());
            GetFlyWarpData(mapId, warp);
            GetSpecialSpawnWarpData(mapId, LocalFieldData_GetSpecialSpawnWarpPtr(localFieldData));
            sub_02053908(taskManager, *(u32 *)(warp + 0), -1, *(u32 *)(warp + 8), *(u32 *)(warp + 0xc), 1);
            Heap_Free(env);
        }
        break;
    }
    return 0;
}
