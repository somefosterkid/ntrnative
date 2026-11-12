// Black's code at ov038 0x021f36c0-0x021f45e8, the whole overlay: Castelia City's gym (gimmick 1, its state in
// gimmick slot 6, as pokebw2's src/ov091/gym_insect.c, whose B2 gym is another). One file: its scene setup
// 0x21f464c, which only the first function loads, sits inside the run of the other functions' .rodata.

#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heapsys.h"
#include "nitro/fx/fx.h"
#include "field/calender.h"
#include "field/city_state.h"
#include "field/encounter.h"
#include "field/event_data.h"
#include "field/field.h"
#include "field/field_actor_internal.h"
#include "field/field_exp_obj.h"
#include "field/field_map.h"
#include "field/field_status.h"
#include "field/player_state.h"
#include "field/pleasure_boat.h"
#include "field/trial_house.h"
#include "field/unity_tower.h"
#include "field/zone.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/config.h"
#include "save/event_work.h"
#include "save/gimmick_savedata.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "save/player_savedata.h"
#include "save/playtime.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/shortcut_util.h"
#include "gfl/sound.h"
#include "field/field_acmd.h"
#include "field/field_script.h"
#include "gfl/key.h"
#include "field/event_action_call.h"

typedef struct Ov038State {
    u32 unk_0[8];
    u32 unk_20[10];
    u32 unk_48[7];
} Ov038State;

struct BoxSaveAccessor {
    SaveControl *save;
};
struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};
struct GameData {
    SaveControl *saveControl;
    PlayerState playerStates[5];
    EventData *eventData;
    PlayerSave *playerSave;
    ZoneSpawnInfo *nowSpawn;
    ZoneSpawnInfo *outboundWarpSpawn;
    ZoneSpawnInfo *nextSpawn;
    ZoneSpawnInfo *escapeRopeSpawn;
    ZoneSpawnInfo entralinkParentSpawn;
    void *bagCursor;
    BagSave *bag;
    PokeParty *party;
    Config *config;
    BoxSaveAccessor *boxes;
    WifiList *wifiList;
    BGMInfo *bgmInfo;
    MMSys *mmSys;
    EventWork *eventWork;
    MapMatrix *mapMatrix;
    FieldSound *fieldSound;
    FieldStatus *fieldStatus;
    u32 lastBtlResult;
    u32 unk1C0;
    u8 season;
    u8 weather;
    u8 unk1C6;
    u8 unk1C7;
    u8 counter;
    u8 counterEnabled;
    u8 lastSubscreen;
    u8 netId;
    u8 forceSeasonSync;
    u8 savedCGearPowerRequest;
    u8 saving;
    u8 is30FPSMode;
    u8 seasons[4];
    PassPowerLevel levels[4];
    CityState cityStates[2];
    EncountState *encountState;
    ShortcutMenuPos shortcutMenuPos;
    GimmickState gimmickState;
    PleasureBoat *pleasureBoat;
    CommPlayerSupport *commPlayerSupport;
    u32 unk688;
    void *unk68C;
    Calendar *calendar;
    UnityTowerFloor unityTowerFloor;
    TrialHouseWork *trialHouseWork;
    u32 unk6A8;
    PlayTime *playTime;
};
struct BGMInfo {
    u8 count;
    u16 *bgms;
    u8 *subsystems;
};
struct GameEvent {
    GameEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
    GameSystem *gsys;
};

u32 func_ov038_021f4200(GameEvent *param_1, s32 *param_2);
u32 func_ov038_021f42bc(GameEvent *param_1, s32 *param_2);
u32 func_ov038_021f44c0(GameEvent *param_1, s32 *param_2);

struct Ov038WorkBlock {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    u8 unk_7;
    VecFx32 unk_8;
    FieldAcmdTCB *unk_14;
    u32 unk_18;
    u32 unk_1c;
    u32 unk_20;
};
struct Ov038Cmd {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
};
struct Ov038Rec {
    u32 unk_0;
    u32 unk_4;
    const u32 *unk_8;
    u32 unk_c;
};

const u32 data_ov038_021f4618[2] = { 0xa, 0xb };
const u32 data_ov038_021f4608[2] = { 0x0001000c, 0xfe };
const u32 data_ov038_021f45e8[2] = { 8, 9 };
const u32 data_ov038_021f4620[2] = { 0x00010018, 0xfe };
const u32 data_ov038_021f4610[2] = { 0xc, 0xd };
const u32 data_ov038_021f45f0[2] = { 14, 15 };
const u32 data_ov038_021f45f8[2] = { 0x00010019, 0xfe };
const u32 data_ov038_021f4600[2] = { 0x0001000d, 0xfe };

const s8 data_ov038_021f4628[10] = { 1, 0, 3, 2, 5, 4, -1, -1, 9, 8 };

const u8 data_ov038_021f4632[10] = { 0, 0, 1, 1, 2, 2, 3, 4, 5, 5 };

const u32 data_ov038_021f463c[4] = { 0x18, 0x19, 0x1a, 0x1b };
extern const struct Ov038Cmd data_ov038_021f47c0[28];
extern const struct Ov038Rec data_ov038_021f4910[35];
const G3DSceneSetup data_ov038_021f464c = { (const G3DSceneResourceSetup *)data_ov038_021f47c0, 0x1c, (const G3DSceneActorSetup *)data_ov038_021f4910, 0x23 };
const VecFx32 data_ov038_021f465c[2] = {
    { 0x268000, 0, 0x198000 },
    { 0x148000, 0, 0x1d8000 },
};
const u32 data_ov038_021f4674[8] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
const VecFx32 data_ov038_021f4694[7] = {
#include "asset/gym_insect_data_ov038_021f4694.inc"
};

const VecFx32 data_ov038_021f46e8[8] = {
#include "asset/gym_insect_data_ov038_021f46e8.inc"
};
const VecFx32 data_ov038_021f4748[10] = {
#include "asset/gym_insect_data_ov038_021f4748.inc"
};
const struct Ov038Cmd data_ov038_021f47c0[28] = {
#include "asset/gym_insect_data_ov038_021f47c0.inc"
};
const struct Ov038Rec data_ov038_021f4910[35] = {
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 3, 3, data_ov038_021f45f0, 2 },
    { 0, 0, data_ov038_021f45e8, 2 },
    { 1, 1, data_ov038_021f4618, 2 },
    { 0, 0, data_ov038_021f45e8, 2 },
    { 1, 1, data_ov038_021f4618, 2 },
    { 0, 0, data_ov038_021f45e8, 2 },
    { 1, 1, data_ov038_021f4618, 2 },
    { 1, 1, data_ov038_021f4618, 2 },
    { 1, 1, data_ov038_021f4618, 2 },
    { 0, 0, data_ov038_021f45e8, 2 },
    { 1, 1, data_ov038_021f4618, 2 },
    { 6, 6, NULL, 0 },
    { 6, 6, NULL, 0 },
    { 6, 6, NULL, 0 },
    { 6, 6, NULL, 0 },
    { 6, 6, NULL, 0 },
    { 6, 6, NULL, 0 },
    { 6, 6, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 7, 7, NULL, 0 },
    { 2, 2, data_ov038_021f4610, 2 },
    { 4, 4, data_ov038_021f4674, 8 },
    { 5, 5, data_ov038_021f463c, 4 },
};

GameEventReturnCode func_ov038_021f3f40(GameEvent *param_1, u32 *param_2, void *param_3);
GameEventReturnCode func_ov038_021f3b3c(GameEvent *param_1, u32 *param_2, void *param_3);
GameEventReturnCode func_ov038_021f407c(GameEvent *param_1, u32 *param_2, void *param_3);
GameEventReturnCode func_ov038_021f3dfc(GameEvent *param_1, u32 *param_2, void *param_3);

void func_ov038_021f36c0(Field *field) {
    FieldExpObjSystem *system;
    s32 i;
    Ov038State *state;
    FieldExpObjAnm *anm;
    SRTMatrix *matrix;
    VecFx32 pos;
    s32 j;
    s32 id;
    s32 closedId;
    s32 openId;

    system = Field_GetExpObjSystem(field);
    state = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 6);
    GFL_HeapGetFreeSizeCore(0x15);
    Field_AllocGimmickWorkBlock(field, 1, Field_GetHeapID(field), sizeof(struct Ov038WorkBlock));
    LoadFieldExpandObjData(system, &data_ov038_021f464c, 0);
    GFL_HeapGetFreeSizeCore(0x15);
    for (i = 0; i < 8; i++) {
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, i);
        matrix->translation = data_ov038_021f46e8[i];
        func_ov021_021be4a4(system, 0, i, TRUE);
        for (j = 0; j < 2; j++) {
            anm = FieldExpObj_GetAnmInfo(system, 0, i, j);
            FieldExpObjAnm_SetLooped(anm, FALSE);
            FieldExpObj_SetAnm(system, 0, i, j, TRUE);
            FieldExpObjAnm_SetPaused(anm, TRUE);
            if (state->unk_0[i] != 0) {
                FieldExpObj_SetAnmFrame(system, 0, i, j, func_ov021_021be748(anm));
            }
        }
    }
    for (i = 0; i < 10; i++) {
        id = i + 8;
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, id);
        matrix->translation = data_ov038_021f4748[i];
        func_ov021_021be4a4(system, 0, id, TRUE);
        for (j = 0; j < 2; j++) {
            anm = FieldExpObj_GetAnmInfo(system, 0, id, j);
            FieldExpObjAnm_SetLooped(anm, FALSE);
            FieldExpObj_SetAnm(system, 0, id, j, TRUE);
            FieldExpObjAnm_SetPaused(anm, TRUE);
            if (state->unk_20[i] != 0) {
                FieldExpObj_SetAnmFrame(system, 0, id, j, func_ov021_021be748(anm));
            }
        }
    }
    for (i = 0; i < 7; i++) {
        closedId = i + 0x12;
        openId = i + 0x19;
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, closedId);
        pos = data_ov038_021f4694[i];
        matrix->translation = pos;
        matrix = FieldExpObj_GetActorMatrixPtr(system, 0, openId);
        matrix->translation = pos;
        func_ov021_021be4a4(system, 0, closedId, TRUE);
        func_ov021_021be4a4(system, 0, openId, TRUE);
        switch (state->unk_48[i]) {
        case 0:
            FieldExpObj_SetActorHidden(system, 0, openId, TRUE);
            break;
        case 1:
            FieldExpObj_SetActorHidden(system, 0, closedId, TRUE);
            break;
        }
    }
    func_ov021_021be4a4(system, 0, 0x20, TRUE);
    for (i = 0; i < 2; i++) {
        anm = FieldExpObj_GetAnmInfo(system, 0, 0x20, i);
        FieldExpObjAnm_SetLooped(anm, FALSE);
        FieldExpObjAnm_SetPaused(anm, TRUE);
    }
    for (j = 0; j < 8; j++) {
        FieldExpObjAnm_SetLooped(FieldExpObj_GetAnmInfo(system, 0, 0x21, j), FALSE);
        FieldExpObj_SetAnm(system, 0, 0x21, j, FALSE);
    }
    for (i = 0; i < 4; i++) {
        FieldExpObjAnm_SetLooped(FieldExpObj_GetAnmInfo(system, 0, 0x22, i), FALSE);
    }
    FieldExpObj_SetActorHidden(system, 0, 0x21, TRUE);
    FieldExpObj_SetActorHidden(system, 0, 0x22, TRUE);
    GFL_HeapGetFreeSizeCore(0x15);
}

void func_ov038_021f39b4(Field *param_1) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(param_1);
    Field_GetGimmickWorkBlock(param_1, 1);
    Field_DeleteGimmickWorkBlock(param_1, 1);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov038_021f39d8(Field *param_1) {
    FieldExpObjSystem *system;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;

    system = Field_GetExpObjSystem(param_1);
    Field_GetGimmickWorkBlock(param_1, 1);
    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 6);
    FieldExpObj_StepAllAnimations(system);
}

s32 func_ov038_021f3a08(struct Ov038State *param_1, s32 param_2, s32 param_3, u32 *param_4, u32 *param_5) {
    s32 i;
    BOOL found;

    for (i = 0; i < 7; i++) {
        s32 x = data_ov038_021f4694[i].x / 0x10000;
        s32 y = data_ov038_021f4694[i].z / 0x10000;
        found = FALSE;
        if (x == param_2 && y == param_3) {
            *param_5 = 1;
            found = TRUE;
        } else if (x - 1 == param_2 && y == param_3) {
            *param_5 = 0;
            found = TRUE;
        } else if (x + 1 == param_2 && y == param_3) {
            *param_5 = 0;
            found = TRUE;
        }
        if (found) {
            *param_4 = param_1->unk_48[i];
            return i;
        }
    }
    *param_5 = 0;
    *param_4 = 0;
    return 7;
}

GameEvent *func_ov038_021f3a88(GameSystem *param_1, s32 param_2, s32 param_3) {
    struct Ov038WorkBlock *workBlock;
    FieldPlayer *player;
    GameEvent *event;
    Field *field;
    u32 unk_10;
    u32 unk_c;
    s32 result;
    u16 faceDir;
    struct Ov038State *state;

    field = GSYS_GetField(param_1);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    player = Field_GetPlayer(field);
    state = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(GSYS_GetField(param_1)))), 6);
    faceDir = FieldPlayer_GetFaceDir(player);
    if (faceDir != 0 && faceDir != 1) {
        return NULL;
    }
    result = func_ov038_021f3a08(state, param_2, param_3, &unk_10, &unk_c);
    if (result < 7) {
        if (unk_c != 0) {
            if (unk_10 == 1) {
                event = GameEvent_Create(param_1, NULL, func_ov038_021f3b3c, 0);
            } else {
                event = GameEvent_Create(param_1, NULL, func_ov038_021f407c, 0);
            }
        } else {
            event = GameEvent_Create(param_1, NULL, func_ov038_021f3f40, 0);
        }
    } else {
        return NULL;
    }
    workBlock->unk_5 = 0;
    workBlock->unk_7 = 0;
    workBlock->unk_6 = faceDir;
    workBlock->unk_3 = result;
    workBlock->unk_14 = 0;
    workBlock->unk_18 = 0;
    workBlock->unk_1c = 0;
    return event;
}

GameEventReturnCode func_ov038_021f3b3c(GameEvent *param_1, u32 *param_2, void *param_3) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    FieldPlayer *player;
    u8 actor;
    FieldExpObjAnm *anm;
    u32 mask;
    s32 delta;
    s32 frame;
    GameData *gameData;
    GimmickState *gimmick;
    SRTMatrix *src;
    SRTMatrix *dst;
    FieldActor *fieldActor;
    FieldAcmdTCB *tcb;
    GameEvent *event;
    VecFx32 pos;
    u8 anmId;
    u32 keys;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    player = Field_GetPlayer(field);
    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 6);
    actor = workBlock->unk_3 + 0x19;
    anmId = 0;
    if (workBlock->unk_6 == 1) {
        anmId++;
    }
    anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, 0x22, anmId);
    switch (*param_2) {
    case 0:
        fieldActor = FieldPlayer_GetActor(player);
        if (func_ov010_0216d454(fieldActor) == 0 || IsAllActorAcmdFinished(fieldActor) == 0) {
            break;
        }
        workBlock->unk_1c = 1;
        FieldPlayer_GetWPos(player, &workBlock->unk_8);
        FieldExpObj_SetActorHidden(expObjSystem, 0, actor, 1);
        FieldExpObj_SetActorHidden(expObjSystem, 0, 0x22, 0);
        src = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, actor);
        dst = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, 0x22);
        dst->translation = src->translation;
        FieldExpObj_SetAnm(expObjSystem, 0, 0x22, anmId, TRUE);
        FieldExpObj_SetAnmFrame(expObjSystem, 0, 0x22, anmId, 0);
        FieldExpObjAnm_SetPaused(anm, 1);
        workBlock->unk_20 = 0;
        (*param_2)++;
        break;
    case 1:
        if (workBlock->unk_6 == 0) {
            mask = 0x40;
            delta = -1;
        } else {
            mask = 0x80;
            delta = 1;
        }
        keys = GCTX_HIDGetHeldKeys();
        if ((keys & mask) != 0) {
            workBlock->unk_7++;
            if (workBlock->unk_7 < 2) {
                break;
            }
            workBlock->unk_7 = 0;
            workBlock->unk_5++;
            FieldPlayer_GetWPos(player, &pos);
            pos.z = pos.z + delta * 0x1000;
            FieldPlayer_SetWPos(player, &pos);
            frame = workBlock->unk_5 / 2;
            if (workBlock->unk_20 == 0 && frame != 0) {
                GFL_SndSEPlay(0x6c2);
                workBlock->unk_20 = 1;
            }
            FieldExpObj_SetAnmFrame(expObjSystem, 0, 0x22, anmId, frame << 12);
            if (workBlock->unk_5 < 0x20) {
                break;
            }
            (*param_2)++;
        } else {
            event = GameEvent_Create(gsys, NULL, func_ov038_021f3dfc, 0);
            GameEvent_Replace(param_1, event);
            return 0;
        }
        break;
    case 2:
        if (workBlock->unk_14 != NULL) {
            if (FieldAcmdTCB_CheckEnded(workBlock->unk_14) == 1) {
                FieldAcmdTCB_Remove(workBlock->unk_14);
                workBlock->unk_14 = NULL;
                workBlock->unk_18 = 1;
                (*param_2)++;
            }
        } else {
            workBlock->unk_18 = 1;
            (*param_2)++;
        }
        if (workBlock->unk_18 != 0) {
            FieldExpObjAnm_SetPaused(anm, 0);
            event = EventActionCall_Create(gsys, field, 0xff, workBlock->unk_6 == 0 ? data_ov038_021f4608 : data_ov038_021f4600);
            GameEvent_ChainNext(param_1, event);
            GFL_SndStop();
            GFL_SndSEPlay(0x6c3);
        }
        break;
    case 3:
        func_ov021_021be6e8(expObjSystem, 0, 0x22, anmId);
        if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
            FieldExpObj_SetAnm(expObjSystem, 0, 0x22, anmId, FALSE);
            FieldExpObj_SetActorHidden(expObjSystem, 0, 0x22, 1);
            FieldExpObj_SetActorHidden(expObjSystem, 0, actor, 0);
            return 1;
        }
        break;
    default:
        break;
    }
    if (workBlock->unk_1c != 0 && workBlock->unk_18 == 0) {
        fieldActor = FieldPlayer_GetActor(player);
        if (workBlock->unk_14 == NULL) {
            tcb = FieldAcmdTCB_Create(fieldActor, workBlock->unk_6 == 0 ? data_ov038_021f4620 : data_ov038_021f45f8);
            workBlock->unk_14 = tcb;
        } else if (FieldAcmdTCB_CheckEnded(workBlock->unk_14) == 1) {
            FieldAcmdTCB_Remove(workBlock->unk_14);
            workBlock->unk_14 = NULL;
        }
    }
    return 0;
}

GameEventReturnCode func_ov038_021f3dfc(GameEvent *param_1, u32 *param_2, void *param_3) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    FieldPlayer *player;
    VecFx32 pos;
    u8 slot;
    u8 anmId;
    BOOL reached;
    s32 frame;
    s32 diff;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    player = Field_GetPlayer(field);
    slot = workBlock->unk_3 + 0x19;
    anmId = 0;
    if (workBlock->unk_6 == 1) {
        anmId++;
    }
    switch (*param_2) {
    case 0:
        GFL_SndStop();
        (*param_2)++;
        /* fall through */
    case 1:
        if (workBlock->unk_14 != NULL) {
            if (FieldAcmdTCB_CheckEnded(workBlock->unk_14) == 1) {
                FieldAcmdTCB_Remove(workBlock->unk_14);
                workBlock->unk_14 = NULL;
                workBlock->unk_5 = 0;
                (*param_2)++;
            }
        } else {
            workBlock->unk_5 = 0;
            (*param_2)++;
        }
        if (*param_2 == 2) {
            workBlock->unk_20 = 0;
        }
        break;
    case 2:
        FieldPlayer_GetWPos(player, &pos);
        reached = FALSE;
        workBlock->unk_5++;
        if (workBlock->unk_6 == 0) {
            pos.z = pos.z + (workBlock->unk_5 << 12);
            if (pos.z >= workBlock->unk_8.z) {
                pos.z = workBlock->unk_8.z;
                reached = TRUE;
            }
            diff = workBlock->unk_8.z - pos.z;
        } else {
            pos.z = pos.z + ((-workBlock->unk_5) << 12);
            if (pos.z <= workBlock->unk_8.z) {
                pos.z = workBlock->unk_8.z;
                reached = TRUE;
            }
            diff = pos.z - workBlock->unk_8.z;
        }
        frame = diff / 2;
        if (workBlock->unk_20 == 0 && frame != 0) {
            GFL_SndSEPlay(0x6c4);
            workBlock->unk_20 = 1;
        }
        FieldExpObj_SetAnmFrame(expObjSystem, 0, 0x22, anmId, frame);
        FieldPlayer_SetWPos(player, &pos);
        if (reached) {
            FieldExpObj_SetAnm(expObjSystem, 0, 0x22, anmId, FALSE);
            FieldExpObj_SetActorHidden(expObjSystem, 0, 0x22, 1);
            FieldExpObj_SetActorHidden(expObjSystem, 0, slot, 0);
            return 1;
        }
        break;
    }
    return 0;
}

GameEventReturnCode func_ov038_021f3f40(GameEvent *param_1, u32 *param_2, void *param_3) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    GameData *gameData;
    GimmickState *gimmick;
    struct Ov038State *state;
    FieldExpObjAnm *anm;
    SRTMatrix *src;
    SRTMatrix *dst;
    u8 anmId;
    u8 actor;
    u8 slot;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    Field_GetPlayer(field);
    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    gimmick = GameData_GetGimmickState(gameData);
    state = GimmickState_GetUserData(gimmick, 6);
    if (state->unk_48[workBlock->unk_3] == 0) {
        slot = workBlock->unk_3 + 0x12;
        actor = 0x21;
    } else {
        slot = workBlock->unk_3 + 0x19;
        actor = 0x22;
    }
    anmId = 2;
    if (workBlock->unk_6 == 1) {
        anmId++;
    }
    anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, actor, anmId);
    switch (*param_2) {
    case 0:
        FieldExpObj_SetActorHidden(expObjSystem, 0, slot, 1);
        FieldExpObj_SetActorHidden(expObjSystem, 0, actor, 0);
        src = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, slot);
        dst = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, actor);
        dst->translation = src->translation;
        FieldExpObj_SetAnm(expObjSystem, 0, actor, anmId, TRUE);
        FieldExpObjAnm_SetPaused(anm, 0);
        FieldExpObj_SetAnmFrame(expObjSystem, 0, actor, anmId, 0);
        GFL_SndSEPlay(0x6c5);
        (*param_2)++;
        break;
    case 1:
        if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
            FieldExpObj_SetAnm(expObjSystem, 0, actor, anmId, FALSE);
            FieldExpObj_SetActorHidden(expObjSystem, 0, actor, 1);
            FieldExpObj_SetActorHidden(expObjSystem, 0, slot, 0);
            return 1;
        }
        break;
    }
    return 0;
}

GameEventReturnCode func_ov038_021f407c(GameEvent *param_1, u32 *param_2, void *param_3) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    FieldExpObjAnm *anm;
    SRTMatrix *src;
    SRTMatrix *dst;
    u8 anmId;
    u8 actor;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    Field_GetPlayer(field);
    actor = workBlock->unk_3 + 0x12;
    anmId = 0;
    if (workBlock->unk_6 == 1) {
        anmId++;
    }
    anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, 0x21, anmId);
    switch (*param_2) {
    case 0:
        FieldExpObj_SetActorHidden(expObjSystem, 0, actor, 1);
        FieldExpObj_SetActorHidden(expObjSystem, 0, 0x21, 0);
        src = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, actor);
        dst = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, 0x21);
        dst->translation = src->translation;
        FieldExpObj_SetAnm(expObjSystem, 0, 0x21, anmId, TRUE);
        FieldExpObjAnm_SetPaused(anm, 0);
        FieldExpObj_SetAnmFrame(expObjSystem, 0, 0x21, anmId, 0);
        GFL_SndSEPlay(0x6c5);
        (*param_2)++;
        break;
    case 1:
        if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
            FieldExpObj_SetAnm(expObjSystem, 0, 0x21, anmId, FALSE);
            FieldExpObj_SetActorHidden(expObjSystem, 0, 0x21, 1);
            FieldExpObj_SetActorHidden(expObjSystem, 0, actor, 0);
            return 1;
        }
        break;
    }
    return 0;
}

GameEvent *func_ov038_021f4184(GameSystem *param_1, s32 param_2) {
    Field *field;
    struct Ov038WorkBlock *workBlock;
    GameEvent *pGVar2;

    field = GSYS_GetField(param_1);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    if (param_2 >= 8) {
        return NULL;
    }
    workBlock->unk_0 = (s8)param_2;
    pGVar2 = GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov038_021f4200, 0);
    return pGVar2;
}

GameEvent *func_ov038_021f41b0(GameSystem *param_1, s32 param_2) {
    Field *field;
    struct Ov038WorkBlock *workBlock;
    GameEvent *pGVar2;

    field = GSYS_GetField(param_1);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    if (param_2 >= 0xa) {
        return NULL;
    }
    workBlock->unk_1 = (s8)param_2;
    pGVar2 = GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov038_021f42bc, 0);
    return pGVar2;
}

GameEvent *func_ov038_021f41dc(GameSystem *param_1, u16 param_2) {
    Field *field;

    field = GSYS_GetField(param_1);
    ((struct Ov038WorkBlock *)Field_GetGimmickWorkBlock(field, 1))->unk_2 = param_2;
    return GameEvent_Create(param_1, (GameEvent *)0x0, (void *)func_ov038_021f44c0, 0);
}

u32 func_ov038_021f4200(GameEvent *param_1, s32 *param_2) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    GameData *gameData;
    GimmickState *gimmick;
    struct Ov038State *state;
    u16 *unk;
    FieldExpObjAnm *anm;
    s32 i;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    gimmick = GameData_GetGimmickState(gameData);
    state = GimmickState_GetUserData(gimmick, 6);
    unk = func_0200b3a8(gimmick, 6);
    switch (*param_2) {
    case 0: {
        u8 actor = workBlock->unk_0;
        for (i = 0; i < 2; i++) {
            FieldExpObj_SetAnm(expObjSystem, 0, actor, i, TRUE);
            anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, actor, i);
            FieldExpObjAnm_SetPaused(anm, 0);
        }
        (*param_2)++;
        break;
    }
    case 1:
        anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, workBlock->unk_0, 0);
        if (FieldExpObjAnm_IsPlaybackFinished(anm)) {
            state->unk_0[workBlock->unk_0] = 1;
            unk[workBlock->unk_0] = 1;
            return 1;
        }
        break;
    }
    return 0;
}

u32 func_ov038_021f42bc(GameEvent *param_1, s32 *param_2) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    GameData *gameData;
    GimmickState *gimmick;
    struct Ov038State *state;
    SRTMatrix *src;
    SRTMatrix *dst;
    FieldExpObjAnm *anm;
    u8 actor;
    u8 slot;
    s8 next;
    u8 next2;
    BOOL flag;
    s32 i;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    gimmick = GameData_GetGimmickState(gameData);
    state = GimmickState_GetUserData(gimmick, 6);
    switch (*param_2) {
    case 0:
        actor = workBlock->unk_1 + 8;
        for (i = 0; i < 2; i++) {
            FieldExpObj_SetAnm(expObjSystem, 0, actor, i, TRUE);
            anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, actor, i);
            FieldExpObjAnm_SetPaused(anm, 0);
        }
        (*param_2)++;
        break;
    case 1:
        actor = workBlock->unk_1 + 8;
        anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, actor, 0);
        if (!FieldExpObjAnm_IsPlaybackFinished(anm)) {
            break;
        }
        state->unk_20[workBlock->unk_1] = 1;
        next = data_ov038_021f4628[workBlock->unk_1];
        flag = FALSE;
        if (next != -1) {
            if (state->unk_20[next] != 0) {
                flag = TRUE;
            }
        } else {
            flag = TRUE;
        }
        if (flag) {
            next2 = data_ov038_021f4632[workBlock->unk_1];
            if (state->unk_48[next2] == 0) {
                workBlock->unk_4 = next2;
                slot = next2 + 0x12;
                FieldExpObj_SetActorHidden(expObjSystem, 0, slot, 1);
                FieldExpObj_SetActorHidden(expObjSystem, 0, 0x21, 0);
                src = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, slot);
                dst = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, 0x21);
                dst->translation = src->translation;
                for (i = 0; i < 4; i++) {
                    actor = i + 4;
                    FieldExpObj_SetAnm(expObjSystem, 0, 0x21, actor, TRUE);
                    anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, 0x21, actor);
                    FieldExpObjAnm_SetPaused(anm, 0);
                    FieldExpObj_SetAnmFrame(expObjSystem, 0, 0x21, actor, 0);
                }
                GFL_SndSEPlay(0x6c8);
                (*param_2)++;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
        break;
    case 2:
        anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, 0x21, 4);
        if (!FieldExpObjAnm_IsPlaybackFinished(anm)) {
            break;
        }
        for (i = 0; i < 4; i++) {
            actor = i + 4;
            FieldExpObj_SetAnm(expObjSystem, 0, 0x21, actor, FALSE);
        }
        state->unk_48[workBlock->unk_4] = 1;
        slot = workBlock->unk_4;
        FieldExpObj_SetActorHidden(expObjSystem, 0, slot + 0x12, 1);
        FieldExpObj_SetActorHidden(expObjSystem, 0, slot + 0x19, 0);
        FieldExpObj_SetActorHidden(expObjSystem, 0, 0x21, 1);
        GFL_SndSEPlay(0x6dc);
        return 1;
    default:
        break;
    }
    return 0;
}

u32 func_ov038_021f44c0(GameEvent *param_1, s32 *param_2) {
    GameSystem *gsys;
    Field *field;
    FieldExpObjSystem *expObjSystem;
    struct Ov038WorkBlock *workBlock;
    GameData *gameData;
    GimmickState *gimmick;
    SRTMatrix *dst;
    FieldExpObjAnm *anm;
    s8 anmId;
    s32 i;

    gsys = GameEvent_GetGameSystem(param_1);
    field = GSYS_GetField(gsys);
    expObjSystem = Field_GetExpObjSystem(field);
    workBlock = Field_GetGimmickWorkBlock(field, 1);
    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 6);
    switch (*param_2) {
    case 0:
        for (i = 0; i < 2; i++) {
            FieldExpObj_SetAnm(expObjSystem, 0, 0x20, i, TRUE);
            anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, 0x20, i);
            FieldExpObjAnm_SetPaused(anm, 0);
            FieldExpObj_SetAnmFrame(expObjSystem, 0, 0x20, i, 0);
        }
        dst = FieldExpObj_GetActorMatrixPtr(expObjSystem, 0, 0x20);
        dst->translation = data_ov038_021f465c[workBlock->unk_2];
        (*param_2)++;
        break;
    case 1:
        anmId = 4;
        if (workBlock->unk_2 == 0) {
            anmId = 3;
        }
        EventScriptCall_Start(param_1, anmId, NULL, 0, 0x8015);
        (*param_2)++;
        break;
    case 2:
        anm = FieldExpObj_GetAnmInfo(expObjSystem, 0, 0x20, 0);
        if (!FieldExpObjAnm_IsPlaybackFinished(anm)) {
            break;
        }
        anmId = 6;
        if (workBlock->unk_2 == 0) {
            anmId = 5;
        }
        EventScriptCall_Replace(param_1, anmId, NULL, 0x8015);
        (*param_2)++;
        break;
    case 3:
        return 1;
    }
    return 0;
}

