// Black's code at ov021 0x021ca544-0x021cbd08. No name is known: the module and address.

#include "types.h"
#include "app/ircbattlematch.h"
#include "app/ov306.h"
#include "battle/battle_proc.h"
#include "battle/battle_rec_tool.h"
#include "battle/btl_setup.h"
#include "battle/regulation.h"
#include "constants/directions.h"
#include "constants/pokemon.h"
#include "field/battle_facility.h"
#include "field/bsubway_scr.h"
#include "field/event_wifi_bsubway.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_gimmick_bsubway.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/net.h"
#include "gfl/net_system.h"
#include "gfl/overlay.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "save/box.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "system/game_event.h"
#include "system/iss_switch_sys.h"
#include "system/iss_sys.h"
#include "field/battle_facility.h"
#include "field/bsubway_scr.h"
#include "field/field_actor.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "field/field_script.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "gfl/random.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "save/bsubway_save.h"
#include "field/union_oneself.h"
#include "field/field_effect.h"

GameEventReturnCode func_ov021_021cb8ac(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021cb934(GameEvent *event, u32 *state, void *data);
u16 randFFFFFFFFdivFFFF(BSubwayScrWork *bsw);
BOOL func_ov021_021cb6dc(VM *vm, void *env);
BOOL func_ov021_021cb700(VM *vm, void *env);
BOOL func_ov021_021cb754(VM *vm, void *env);
BOOL func_ov021_021cb7e8(VM *vm, void *env);
BOOL func_ov021_021cb86c(VM *vm, void *env);
GameEvent *func_ov021_021cb914(GameSystem *gsys, BSubwayScrWork *bsw);
GameEvent *func_ov021_021cb9bc(GameSystem *gsys, FieldScriptEnv *env, u16 *var, u32 message);
void func_ov021_021cb9e4(BSubwayScrWork *bsw, GameData *gameData, MMSys *mmSys, Field *field);
u16 func_ov021_021cbba0(FieldActor *actor);
BOOL func_ov021_021cbbac(BSubwayScrWork *bsw, u16 index);
void func_ov021_021cbbd0(BSubwayScrWork *bsw, u16 index, u16 *a, u16 *b, u16 *c);
BOOL func_ov021_021cbc14(VM *vm, void *env);
u32 func_ov021_021cbc94(u32 mode);
u32 func_ov021_021cbcc4(Field *field);
void func_ov021_021d024c(FieldEffectTask *task, s32 param_2);
void func_ov021_021d028c(FieldEffectTask *param_1, u32 param_2);

BOOL func_ov021_021ca544(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptWork_GetFieldWork(work);
    GameSystem *gsys = ScriptWork_GetGameSystem(work);
    u16 a1 = VM_Read16(vm);
    u16 a2 = ScriptReadAny(vm, env);
    func_ov021_021c9934(gsys, a1, a2);
    return FALSE;
}

BOOL func_ov021_021ca57c(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptWork_GetFieldWork(work);
    func_ov021_021c9924(ScriptWork_GetGameSystem(work));
    return FALSE;
}

BOOL func_ov021_021ca598(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptWork_GetFieldWork(work);
    GameSystem *gsys = ScriptWork_GetGameSystem(work);
    BSubwayScrWork *bsw = func_02012ed8(GSYS_GetGameData(gsys));
    func_ov021_021c9b24(gsys, bsw);
    return FALSE;
}


#define FLAG_BSUBWAY_ON_TRAIN 0x966
#define BSUBWAY_COUNT_MAX 9999

typedef struct BSubwayActorPos {
    s16 x;
    s16 unk_2;
    s16 z;
    s16 unk_6;
} BSubwayActorPos;

typedef struct BSubwayActor {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_a;
    u16 unk_c;
    u16 unk_e;
} BSubwayActor;

static const u16 data_ov021_021dad24[9] = { 20, 21, 22, 22, 20, 20, 21, 22, 22 };

static const u16 data_ov021_021dad36[9] = { 4, 5, 6, 6, 11, 7, 8, 9, 9 };

const u16 data_ov021_021dad5c[10] = { 0x0021, 0x000f, 0x0010, 0x0031, 0x0035, 0x0011, 0x001f, 0x0021, 0x002d, 0x001f };
const u16 data_ov021_021dad48[10] = { 0x002c, 0x000b, 0x000c, 0x0030, 0x0048, 0x001c, 0x000d, 0x0047, 0x001e, 0x0020 };
static const VecFx32 data_ov021_021dad70[2] = {
    { 0x2d0000, -0x20000, 0xb6000 },
    { 0x2f0000, -0x30000, 0x97000 },
};

static const u32 data_ov021_021dad88[9] = { 0, 2, 4, 4, 6, 1, 3, 5, 5 };

static const u32 data_ov021_021dadac[9] = { 67, 69, 71, 71, 73, 68, 70, 72, 72 };

const BSubwayActorPos data_ov021_021dadd0[5] = {
#include "asset/ov021_021ca544_data_ov021_021dadd0.inc"
};
const u32 data_ov021_021dadf8[22] = {
#include "asset/ov021_021ca544_data_ov021_021dadf8.inc"
};

static const VecFx32 data_ov021_021dae50[9] = {
#include "asset/ov021_021ca544_data_ov021_021dae50.inc"
};

const BSubwayActor data_ov021_021daebc[81] = {
#include "asset/ov021_021ca544_data_ov021_021daebc.inc"
};


BOOL func_ov021_021ca5c4(VM *vm, FieldScriptEnv *env) {
    PartyPkm *pkm;
    BOOL result = FALSE;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptWork_GetFieldWork(work);
    GameSystem *gsys = ScriptWork_GetGameSystem(work);
    GameData *gameData = GSYS_GetGameData(gsys);
    SaveControl *save = GameData_GetSaveControl(gameData);
    EventWork *eventWork = GameData_GetEventWork(gameData);
    BSubwayScrWork *bsw = func_02012ed8(gameData);
    BSubwayPlayData *playData = SaveControl_GetBlockPtr(save, 0x39);
    u32 mode = func_0200d884(playData, 0, NULL);
    BSubwayScoreData *score = SaveControl_GetBlockPtr(save, 0x3a);
    void *data3A = SaveControl_GetBlockPtr(save, 0x3b);
    Field *field = GSYS_GetField(gsys);
    u16 cmd = VM_Read16(vm);
    u16 param0 = ScriptReadAny(vm, env);
    u16 param1 = ScriptReadAny(vm, env);
    u16 retWkId = VM_Read16(vm);
    u16 *retWk = ScriptWork_GetWkAddr(work, gameData, retWkId);
    FieldActor *actor;
    EventWifiBSubwayArgs args;

    if (bsw == NULL && cmd >= 300) {
        return result;
    }
    switch (cmd) {
    case 0:
        *retWk = func_ov021_021cbcc4(field);
        break;
    case 1:
        OS_ResetSystem(0);
        break;
    case 2:
        func_0200d85c(playData);
        break;
    case 3:
        *retWk = func_0200d87c(playData);
        break;
    case 4: {
        VecFx32 pos;
        ZoneSpawnInfo spawn;
        FieldPlayer *player = Field_GetPlayer(field);
        u32 dir = FieldPlayer_GetFaceDir(player);
        int warpDir;

        FieldPlayer_GetWPos(player, &pos);
        switch (dir) {
        case DIR_UP:
            warpDir = 1;
            break;
        case DIR_DOWN:
            warpDir = 2;
            break;
        case DIR_LEFT:
            warpDir = 3;
            break;
        case DIR_RIGHT:
            warpDir = 4;
            break;
        default:
            warpDir = 0;
            break;
        }
        CreateZoneChangeData(&spawn, Field_GetPlayerStateZoneID(field), warpDir, pos.x, pos.y, pos.z);
        GameData_SetNextZone(gameData, &spawn);
        EventWork_FlagSet(eventWork, FLAG_BSUBWAY_ON_TRAIN);
        break;
    }
    case 5:
        EventWork_FlagReset(eventWork, FLAG_BSUBWAY_ON_TRAIN);
        break;
    case 6:
        *retWk = func_0200dac4(score, param0);
        if (*retWk > BSUBWAY_COUNT_MAX) {
            *retWk = BSUBWAY_COUNT_MAX;
        }
        break;
    case 7:
        *retWk = func_ov021_021c9d30(gsys);
        break;
    case 12:
        *retWk = FieldPlayer_GetObjCodeByForme(func_ov021_021a30b0(Field_GetPlayer(field)), 0);
        break;
    case 13:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        if (actor != NULL) {
            SetActorHidden(actor, param1);
        }
        break;
    case 8:
        *retWk = func_0200da54(playData) + 1;
        break;
    case 11:
        *retWk = func_0200da54(bsw->unk70);
        *retWk = *retWk + 1;
        break;
    case 14:
        if (data_ov021_021dad36[param0] == 11) {
            *retWk = TRUE;
        } else {
            *retWk = func_0200dba0(score, data_ov021_021dad36[param0], 0);
        }
        break;
    case 15:
        if (data_ov021_021dad36[param0] != 11) {
            func_0200dba0(score, data_ov021_021dad36[param0], 1);
        }
        break;
    case 16:
        *retWk = func_0200dba0(score, 10, 0);
        break;
    case 17:
        func_0200dba0(score, 10, 1);
        break;
    case 18:
        func_ov052_021f37c0(field, data_ov021_021dad88[param0], &data_ov021_021dad70[param1]);
        break;
    case 19:
        func_ov021_021d024c(func_ov052_021f380c(field), param0);
        break;
    case 20:
        func_ov021_021d028c(func_ov052_021f380c(field), param0);
        break;
    case 21:
        *retWk = func_0200d884(playData, 0, NULL);
        break;
    case 22:
        *retWk = FALSE;
        switch (param0) {
        case 2:
        case 3:
        case 7:
        case 8:
            *retWk = TRUE;
            break;
        }
        break;
    case 23: {
        fx32 y = param1 << 16;
        VecFx32 pos;

        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        if (actor != NULL) {
            CopyActorWPos(actor, &pos);
            pos.y = y;
            SetActorWPosAll(actor, &pos, GetActorFaceDir(actor));
            func_ov010_0216db4c(actor, TRUE);
        }
        break;
    }
    case 24:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        if (actor != NULL) {
            func_ov010_0216db4c(actor, FALSE);
        }
        break;
    case 25:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        *retWk = FALSE;
        if (actor != NULL) {
            *retWk = TRUE;
        }
        break;
    case 26:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        *retWk = 0;
        if (actor != NULL) {
            *retWk = func_ov021_021cbba0(actor);
        }
        break;
    case 27:
        *retWk = func_ov021_021ca164(score, gsys, 0);
        if ((s16)*retWk <= 0) {
            *retWk = 1;
        }
        break;
    case 28:
        *retWk = func_ov021_021ca164(score, gsys, 3);
        break;
    case 29:
        *retWk = func_ov021_021ca164(score, gsys, 4);
        break;
    case 30:
        *retWk = func_0200db80(score, param0);
        break;
    case 31:
        *retWk = FieldPlayer_GetObjCodeByExState(
            getTrainerGender(&GameData_GetPlayerState(gameData)->playerInfo) == GENDER_MALE ? 1 : 0, 0);
        break;
    case 32: {
        BattleBoxSave *battleBox = getBattleBox(save);

        *retWk = FALSE;
        if (func_0200bba8(battleBox) == TRUE) {
            *retWk = TRUE;
        }
        break;
    }
    case 33:
        *retWk = 0;
        break;
    case 34:
        func_ov052_021f37c0(field, 7, data_ov021_021dad70);
        break;
    case 35:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        if (actor != NULL) {
            CheckSetActorFaceDir(actor, param1);
        }
        break;
    case 36:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        if (actor != NULL) {
            DisableActorMovement(actor);
        }
        break;
    case 37:
        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        ScriptWork_CallEvent(work, func_ov010_0216cb48(gsys, GetActorUserParam(actor, 0), param0));
        result = TRUE;
        break;
    case 38:
        *retWk = func_0200db44(score, param0);
        break;
    case 39: {
        u16 index;

        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        index = GetActorUserParam(actor, 0);
        *retWk = FALSE;
        if (func_ov021_021cbbac(bsw, index) == TRUE) {
            *retWk = TRUE;
        }
        break;
    }
    case 40: {
        u16 a, b, c;

        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        func_ov021_021cbbd0(bsw, GetActorUserParam(actor, 0), &a, &b, &c);
        *retWk = a;
        break;
    }
    case 41: {
        u16 a, b, c;

        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        func_ov021_021cbbd0(bsw, GetActorUserParam(actor, 0), &a, &b, &c);
        *retWk = b;
        break;
    }
    case 42: {
        u16 a, b, c;

        actor = FindFieldActor(Field_GetActorSystem(field), param0);
        func_ov021_021cbbd0(bsw, GetActorUserParam(actor, 0), &a, &b, &c);
        *retWk = c;
        break;
    }
    case 43: {
        void *list = func_0200df58(data3A, 4);

        *retWk = func_0200df94(list);
        GFL_HeapFree(list);
        break;
    }
    case 44: {
        BOOL message = FALSE;

        if (param0 == TRUE) {
            message = TRUE;
        }
        ScriptWork_CallEvent(work, func_ov021_021cb9bc(gsys, env, retWk, message));
        result = TRUE;
        break;
    }
    case 45: {
        ZoneSpawnInfo spawn;
        u16 index = func_ov021_021cbcc4(field);
        const VecFx32 *pos = &data_ov021_021dae50[index];

        CreateZoneChangeData(&spawn, data_ov021_021dadac[index], 4, pos->x, pos->y, pos->z);
        GameData_SetNextZone(gameData, &spawn);
        EventWork_FlagSet(eventWork, FLAG_BSUBWAY_ON_TRAIN);
        break;
    }
    case 100:
        if (param0 == 0) {
            func_0200dba0(score, 1, 2);
        } else {
            func_0200dba0(score, 1, 1);
        }
        break;
    case 101:
        *retWk = func_0200dba0(score, 1, 0);
        break;
    case 103:
        args.mode = 0;
        args.result = retWk;
        ScriptWork_CallEvent(work, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(7), func_ov007_021542c0, &args));
        result = TRUE;
        break;
    case 104:
        result = TRUE;
        args.mode = 1;
        args.result = retWk;
        ScriptWork_CallEvent(work, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(7), func_ov007_021542c0, &args));
        break;
    case 105:
        args.mode = 2;
        args.result = retWk;
        ScriptWork_CallEvent(work, GameEvent_CreateOverlayDelegate(gsys, OVERLAY_ID(7), func_ov007_021542c0, &args));
        result = TRUE;
        break;
    case 106:
        *retWk = func_0200de94(data3A);
        break;
    case 107:
        *retWk = func_0200de5c(data3A);
        break;
    case 108: {
        u8 value = param0;

        func_0200d914(playData, 11, &value);
        break;
    }
    case 109:
        *retWk = func_0200d884(playData, 11, NULL);
        break;
    case 110:
        *retWk = func_0200de64(data3A);
        break;
    case 111:
        ScriptWork_CallEvent(work, func_ov010_0216c860(gsys));
        result = TRUE;
        break;
    case 112:
        *retWk = func_0200df40(data3A);
        break;
    case 113:
        *retWk = func_0200df4c(data3A);
        break;
    case 200:
        *retWk = 0;
        break;
    case 203:
        *retWk = 0;
        break;
    case 322:
        if (mode < 9) {
            ZoneSpawnInfo spawn;
            const VecFx32 *pos = &data_ov021_021dae50[mode];

            CreateZoneChangeData(&spawn, data_ov021_021dadac[mode], 4, pos->x, pos->y, pos->z);
            GameData_SetNextZone(gameData, &spawn);
            EventWork_FlagSet(eventWork, FLAG_BSUBWAY_ON_TRAIN);
        }
        break;
    case 300: {
        u8 value = func_0200d884(playData, 10, NULL);

        ScriptWork_CallEvent(work, func_ov010_0216c538(bsw, gsys, value));
        result = TRUE;
        break;
    }
    case 301:
        *retWk = func_ov021_021c9f44(bsw, gsys);
        break;
    case 302:
        *retWk = 0;
        break;
    case 303:
        if (func_ov021_021c9fd0(bsw) == TRUE) {
            *retWk = 1;
        } else {
            *retWk = 0;
        }
        break;
    case 304:
        func_ov021_021c9d70(gsys, bsw);
        break;
    case 305:
        func_ov021_021c9ffc(bsw, gsys);
        break;
    case 306:
        func_ov021_021c9bcc(bsw);
        break;
    case 307:
        func_ov021_021ca05c(bsw);
        break;
    case 308:
        *retWk = func_ov021_021ca150(bsw, param0);
        break;
    case 309:
        ScriptWork_CallEvent(work, func_ov010_0216c640(bsw, gsys, field));
        result = TRUE;
        break;
    case 310:
        *retWk = bsw->playMode;
        break;
    case 311:
        bsw->unkC_1 = param0;
        break;
    case 312:
        *retWk = bsw->unkC_1;
        break;
    case 313:
        *retWk = func_ov021_021c9db0(bsw);
        break;
    case 314:
        bsw->unkC_5 = param0;
        break;
    case 315:
        *retWk = bsw->unkC_5;
        break;
    case 316:
        func_ov021_021c9e8c(bsw);
        break;
    case 317:
        *retWk = bsw->memberSlots[param0];
        break;
    case 319:
        bsw->recvCount = 0;
        MI_CpuFill8(bsw->recvBuf, 0, sizeof(bsw->recvBuf));
        break;
    case 320:
        ScriptWork_CallEvent(work, func_ov010_0216c6e8(bsw, gsys, param0, param1, 5));
        result = TRUE;
        break;
    case 321:
        func_ov021_021c9cac(bsw);
        break;
    case 323:
        func_ov021_021c9c54(bsw, gsys);
        break;
    case 324: {
        u8 value = param0;

        func_0200d914(playData, 10, &value);
        break;
    }
    case 325:
        func_ov021_021c9b78(bsw);
        break;
    case 326:
        if (mode == 5 || mode == 6 || mode == 7 || mode == 8) {
            *retWk = 1;
        } else {
            *retWk = 0;
        }
        break;
    case 329:
        func_ov021_021cb9e4(bsw, gameData, Field_GetActorSystem(field), field);
        break;
    case 330:
        bsw->unkC_8 = param0;
        break;
    case 331:
        *retWk = bsw->unkC_8;
        break;
    case 332:
        if (bsw->btlSetup == NULL) {
            *retWk = 0;
        } else if (bsw->btlSetup->unk8C == 1) {
            *retWk = 1;
        } else {
            *retWk = 0;
        }
        break;
    case 333:
        func_ov021_021ca048(bsw, gsys);
        break;
    case 334: {
        ISSSwitchIndex i = 0;
        u16 count = func_0200da54(playData);
        ISSSwitchSys *switchSys;

        if (count != 0) {
            switchSys = ISS_GetSwitchSys(GameSystem_GetISS(gsys));
            count = i + count + 1;
            for (; i < count; i++) {
                if (i < ISS_SWITCH_COUNT && i != 0) {
                    ISSSwitchSys_ReqSwitchFadeIn(switchSys, i);
                }
            }
        }
        break;
    }
    case 335:
        func_ov021_021ca338(bsw, getTrainerGender(&GameData_GetPlayerState(gameData)->playerInfo) == GENDER_MALE ? 1 : 0);
        break;
    case 336:
        *retWk = bsw->memberSpecies[param0];
        break;
    case 337:
        bsw->unkC_9 = param0;
        break;
    case 338:
        *retWk = bsw->unkC_9;
        break;
    case 339: {
        PokeParty *party = NULL;
        u32 useBattleBox;
        Regulation *regulation;
        PokeParty *checked;
        u32 check;

        *retWk = 0;
        useBattleBox = func_0200d884(playData, 10, NULL);
        regulation = func_0201a354(func_ov021_021cbc94(mode), 4);
        if (useBattleBox == TRUE) {
            party = convertBoxedPokeSetToParty(getBattleBox(save), 4);
            checked = party;
        } else {
            checked = GameData_GetParty(gameData);
        }
        check = func_02019ed4(regulation, checked);
        GFL_HeapFree(regulation);
        if (party != NULL) {
            GFL_HeapFree(party);
        }
        if (check <= 1) {
            *retWk = 1;
        }
        break;
    }
    case 340:
        func_ov021_021ca1e4(bsw);
        break;
    case 341:
        *retWk = func_ov021_021cbc94(mode);
        break;
    case 342:
        *retWk = func_ov021_021ca270(mode);
        break;
    case 343: {
        u32 buffer;

        if (bsw->unkC_10 == 0) {
            bsw->unkC_10 = 1;
            if (func_0200b5cc(save, 4, &buffer, 0) == 1) {
                bsw->unkC_10 = 2;
            }
        }
        break;
    }
    case 344:
        if (bsw->unkC_10 == 1) {
            *retWk = 0;
        } else {
            *retWk = 1;
        }
        break;
    case 345:
        func_0200ba58();
        func_ov188_021eee78(bsw->btlSetup);
        func_0200ba68();
        break;
    case 346:
        bsw->unk7EC = 0;
        bsw->unk7EE = 0;
        bsw->unk10 = param0;
        VM_SetNativeCallback(vm, func_ov021_021cbc14);
        bsw->unkC_10 = 2;
        result = TRUE;
        break;
    case 347:
        if (bsw->btlSetup != NULL) {
            freeVSPlayerBlkClearPtr();
            BtlSetup_Free(bsw->btlSetup);
            bsw->btlSetup = NULL;
        }
        break;
    case 348:
        MI_CpuFill8(&bsw->ov306Param, 0, sizeof(bsw->ov306Param));
        bsw->ov306Param.gameData = gameData;
        bsw->ov306Param.unk4 = 1;
        bsw->ov306Param.unk8 = 0;
        bsw->ov306Param.unkC = 1;
        bsw->ov306Param.unk10 = data_ov021_021dad24[mode];
        bsw->ov306Param.unk14 = func_ov021_021ca234(bsw);
        if (bsw->ov306Param.unk14 > BSUBWAY_COUNT_MAX) {
            bsw->ov306Param.unk14 = BSUBWAY_COUNT_MAX;
        }
        ScriptWork_CallEvent(work, func_02014808(gsys, field, OVERLAY_ID(214), &data_ov214_021bb474, &bsw->ov306Param, NULL, NULL));
        result = TRUE;
        break;
    case 349:
        func_0200da14(playData);
        func_0200db1c(score, mode);
        break;
    case 350:
        func_ov052_021f388c(field);
        break;
    case 351: {
        u16 index = func_0200da54(playData);

        switch (mode) {
        case 2:
        case 3:
        case 7:
        case 8:
            *retWk = bsw->unk32[param0 + index * 2];
            break;
        default:
            *retWk = bsw->unk32[index];
            break;
        }
        break;
    }
    case 352:
        ScriptWork_CallEvent(work, func_ov010_0216c6e8(bsw, gsys, param0, param1, 6));
        result = TRUE;
        break;
    case 353:
        if (bsw->btlSetup != NULL) {
            u8 a = bsw->btlSetup->unkB6;
            u8 b = bsw->btlSetup->unkBF;
            int count;
            PokeParty *party = bsw->btlSetup->party[0];
            int i;
            u32 hp;
            u32 hpSum;
            u32 maxHpSum;

            count = PokeParty_GetPkmCount(party);
            hpSum = 0;
            maxHpSum = 0;
            for (i = 0; i < count; i++) {
                pkm = PokeParty_GetPkm(party, i);
                hp = PokeParty_GetParam(pkm, 0xa0, NULL);
                maxHpSum += PokeParty_GetParam(pkm, 0xa1, NULL);
                hpSum += hp;
            }
            func_0200d9e8(playData, b, a, maxHpSum - hpSum);
        }
        break;
    case 354:
        bsw->unkC_12 = param0;
        break;
    case 355:
        *retWk = bsw->unkC_12;
        break;
    case 356:
        *retWk = func_0200d884(playData, 10, NULL);
        break;
    case 357:
        func_0200da28(playData);
        func_ov021_021ca23c(bsw);
        *retWk = func_ov021_021ca234(bsw);
        if (*retWk > BSUBWAY_COUNT_MAX) {
            *retWk = BSUBWAY_COUNT_MAX;
        }
        break;
    case 358:
        *retWk = func_ov021_021ca234(bsw);
        if (*retWk > BSUBWAY_COUNT_MAX) {
            *retWk = BSUBWAY_COUNT_MAX;
        }
        break;
    case 359:
        if (func_0200db44(score, mode) == 0) {
            func_0200db30(score, mode);
            func_0200da14(playData);
            func_ov021_021ca250(bsw);
        }
        break;
    case 400:
        if (bsw->unkC_9 == TRUE) {
            func_ov010_02167b00(bsw);
        } else {
            func_ov010_02167ab0(bsw);
        }
        break;
    case 401:
        ScriptWork_CallEvent(work, func_ov021_021cb914(gsys, bsw));
        result = TRUE;
        break;
    case 402:
        bsw->unk727 = param0;
        bsw->unk72A = retWkId;
        func_ov010_02167b24(bsw->unk727);
        VM_SetNativeCallback(vm, func_ov021_021cb700);
        result = TRUE;
        break;
    case 403:
        bsw->resultVar = retWk;
        bsw->unk71C = func_ov021_021cd48c(GetGameDataPlayerInfo(gameData), field, 2, 2, 4, 0, 2, 0);
        VM_SetNativeCallback(vm, func_ov021_021cb754);
        result = TRUE;
        break;
    case 404:
        bsw->resultVar = retWk;
        bsw->unk71C = func_ov021_021cd48c(GetGameDataPlayerInfo(gameData), field, 2, 2, 4, 1, 2, 0);
        result = TRUE;
        VM_SetNativeCallback(vm, func_ov021_021cb7e8);
        break;
    case 405:
        func_ov010_02167bfc(bsw, param0, param1);
        VM_SetNativeCallback(vm, func_ov021_021cb6dc);
        result = TRUE;
        break;
    case 406:
        func_ov010_02167cf4(bsw, param0);
        bsw->unk72A = retWkId;
        VM_SetNativeCallback(vm, func_ov021_021cb86c);
        result = TRUE;
        break;
    case 407:
        *retWk = func_02039338();
        break;
    case 408:
        MI_CpuFill8(&bsw->ircMatchParam, 0, sizeof(bsw->ircMatchParam));
        bsw->ircMatchParam.gameData = gameData;
        bsw->ircMatchParam.result = IRCBATTLEMATCH_MODE_BSUBWAY;
        ScriptWork_CallEvent(work, func_02014808(gsys, field, OVERLAY_ID(100), &data_ov100_021bb6a4, &bsw->ircMatchParam, NULL, NULL));
        result = TRUE;
        break;
    case 409:
        switch (bsw->ircMatchParam.result) {
        case IRCBATTLEMATCH_MODE_BSUBWAY:
            *retWk = 0;
            break;
        case IRCBATTLEMATCH_RESULT_QUIT:
            *retWk = 1;
            break;
        case IRCBATTLEMATCH_RESULT_BACK:
            *retWk = 2;
            break;
        default:
            *retWk = 1;
            break;
        }
        break;
    case 410:
        func_0200ed54(getHollow_RivalData(save), bsw->partner.id);
        break;
    case 411:
        *retWk = FieldPlayer_GetObjCodeByForme(func_02039338() == FALSE ? func_ov021_021a30b0(Field_GetPlayer(field)) : bsw->partner.gender, 0);
        break;
    case 412:
        *retWk = FieldPlayer_GetObjCodeByForme(func_02039338() == FALSE ? bsw->partner.gender : func_ov021_021a30b0(Field_GetPlayer(field)), 0);
        break;
    case 413:
        func_0200fb38();
        break;
    case 414:
        func_02012124(gsys, 1);
        break;
    case 415:
        *retWk = FALSE;
        if (func_0200fb24()) {
            *retWk = TRUE;
        }
        break;
    case 416:
        *retWk = FALSE;
        if (bsw->btlSetup != NULL && func_0200ba38(bsw->btlSetup->unk91) == TRUE) {
            *retWk = TRUE;
        }
        break;
    case 102:
    case 201:
    case 202:
    case 501:
        break;
    }
    return result;
}

BOOL func_ov021_021cb6dc(VM *vm, void *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = ScriptWork_GetGameSystem(work);
    BSubwayScrWork *bsw = func_02012ed8(GSYS_GetGameData(gsys));

    if (func_ov010_02167cb4(bsw) == 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021cb700(VM *vm, void *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameSystem *gsys = ScriptWork_GetGameSystem(work);
    GameData *gameData = GSYS_GetGameData(gsys);
    BSubwayScrWork *bsw = func_02012ed8(gameData);
    u16 *var = ScriptWork_GetWkAddr(work, gameData, bsw->unk72A);

    if (func_0200fb24() != 0) {
        *var = 0;
        return TRUE;
    }
    if (func_ov010_02167b34(bsw->unk727) == 1) {
        *var = 1;
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov021_021cb754(VM *vm, void *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData;
    SaveControl *save;
    BSubwayScrWork *bsw;
    u32 answer;

    ScriptWork_GetFieldWork(work);
    gameData = GSYS_GetGameData(ScriptWork_GetGameSystem(work));
    save = GameData_GetSaveControl(gameData);
    GameData_GetEventWork(gameData);
    bsw = func_02012ed8(gameData);
    func_0200d884(SaveControl_GetBlockPtr(save, 0x39), 0, NULL);
    SaveControl_GetBlockPtr(save, 0x3a);
    answer = func_ov021_021cd688(bsw->unk71C);
    if (answer != 0) {
        func_ov021_021cd5a4(bsw->unk71C);
        bsw->unk71C = NULL;
        switch (answer) {
        case 1:
            *bsw->resultVar = 0;
            return TRUE;
        case 2:
            *bsw->resultVar = 1;
            return TRUE;
        case 4:
        default:
            *bsw->resultVar = 2;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov021_021cb7e8(VM *vm, void *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData;
    SaveControl *save;
    BSubwayScrWork *bsw;
    u32 answer;

    ScriptWork_GetFieldWork(work);
    gameData = GSYS_GetGameData(ScriptWork_GetGameSystem(work));
    save = GameData_GetSaveControl(gameData);
    GameData_GetEventWork(gameData);
    bsw = func_02012ed8(gameData);
    func_0200d884(SaveControl_GetBlockPtr(save, 0x39), 0, NULL);
    SaveControl_GetBlockPtr(save, 0x3a);
    answer = func_ov021_021cd688(bsw->unk71C);
    if (answer != 0) {
        func_ov021_021cd5a4(bsw->unk71C);
        bsw->unk71C = NULL;
        switch (answer) {
        case 1:
            *bsw->resultVar = 0;
            return TRUE;
        case 2:
        default:
            *bsw->resultVar = 1;
            return TRUE;
        }
    }
    return FALSE;
}



BOOL func_ov021_021cb86c(VM *vm, void *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = GSYS_GetGameData(ScriptWork_GetGameSystem(work));
    BSubwayScrWork *bsw = func_02012ed8(gameData);
    u16 *var = ScriptWork_GetWkAddr(work, gameData, bsw->unk72A);

    if (func_ov010_02167d00(bsw, var) == 1) {
        return TRUE;
    }
    return FALSE;
}

GameEventReturnCode func_ov021_021cb8ac(GameEvent *event, u32 *state, void *data) {
    BSubwayScrWork **bsw = data;

    switch (*state) {
    case 0:
        if (func_0203d378() == TRUE) {
            func_0203d7bc(FALSE);
        }
        if ((*bsw)->unkC_9 == TRUE) {
            func_ov010_02167b18(*bsw);
        }
        (*state)++;
        break;
    case 1:
        if (func_0203d378() == TRUE) {
            if (func_0203d1b4() <= 1) {
                func_0203ceac(NULL);
                (*state)++;
            }
        } else {
            func_0203ceac(NULL);
            (*state)++;
        }
        break;
    case 2:
        if (func_0203cdbc() == TRUE) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov021_021cb914(GameSystem *gsys, BSubwayScrWork *bsw) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov021_021cb8ac, sizeof(BSubwayScrWork *));
    BSubwayScrWork **data = GameEvent_GetData(event);

    *data = bsw;
    return event;
}

typedef struct BSubwayMsgEvent {
    FieldScriptEnv *env;
    u16 *unk_4;
    ListMenuUI *unk_8;
    u32 unk_c;
} BSubwayMsgEvent;
GameEventReturnCode func_ov021_021cb934(GameEvent *event, u32 *state, void *data) {
    BSubwayMsgEvent *wk = data;

    FieldScriptEnv_GetScriptWork(wk->env);
    switch (*state) {
    case 0:
        FieldScriptEnv_SetWaitCounter(wk->env, 8);
        (*state)++;
    case 1:
        if (FieldScriptEnv_UpdateWaitCounter(wk->env) == TRUE) {
            wk->unk_8 = func_ov021_02190be4(Field_GetMsgBGSys(GSYS_GetField(FieldScriptEnv_GetGameSystem(wk->env))), wk->unk_c);
            (*state)++;
        }
        break;
    case 2:
        switch (func_ov021_02190c78(wk->unk_8)) {
        case 0:
            *wk->unk_4 = 0;
            (*state)++;
            break;
        case 2:
            break;
        default:
            *wk->unk_4 = 1;
            (*state)++;
            break;
        }
        break;
    case 3:
        func_ov021_021909bc(wk->unk_8);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEvent *func_ov021_021cb9bc(GameSystem *gsys, FieldScriptEnv *env, u16 *var, u32 message) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov021_021cb934, sizeof(BSubwayMsgEvent));
    BSubwayMsgEvent *data = GameEvent_GetData(event);

    data->env = env;
    data->unk_4 = var;
    data->unk_c = message;
    return event;
}


typedef struct BSubwayRecord {
    u8 unk_0[0x20];
    u8 unk_20_0 : 1;
    u8 female : 1;
    u8 unk_20_2 : 6;
    u8 unk_21;
} BSubwayRecord;










void func_ov021_021cb9e4(BSubwayScrWork *bsw, GameData *gameData, MMSys *mmSys, Field *field) {
    s32 i = 0;
    u32 zoneId = Field_GetPlayerStateZoneID(field);
    u16 id = 0x80;
    u8 mode = bsw->playMode;

    if (mode == 4) {
        BSubwayActorPos *pos = data_ov021_021dadd0;
        BSubwayRecord *list = func_0200df58(SaveControl_GetBlockPtr(GameData_GetSaveControl(gameData), 0x3b), 4);
        s32 count = func_0200df94(list);
        BSubwayRecord *record;
        FieldActor *actor;
        u32 index;
        u16 objCode;

        if (count != 0) {
            record = list + (count - 1);
            while (count != 0 && i < 5) {
                index = func_0200dfb4(record) % 10;
                if (record->female == FALSE) {
                    objCode = data_ov021_021dad48[index];
                } else {
                    objCode = data_ov021_021dad5c[index];
                }
                actor = CreateNewActorByParam(mmSys, pos->x, pos->z, i % 4, id, objCode, 2, zoneId);
                SetActorUserParam(actor, count - 1, 0);
                SetActorSCRID(actor, 5);
                i++;
                record--;
                id++;
                count--;
                pos++;
            }
        }
        GFL_HeapFree(list);
        return;
    }
    {
        BSubwayActor *entry = data_ov021_021daebc;
        u16 stage = func_0200db80(bsw->unk74, mode);
        FieldActor *actor;

        switch (mode) {
        case 5:
        case 6:
        case 7:
        case 8:
            stage += 3;
            break;
        }
        while (entry->unk_0 != 0xffff) {
            if (data_ov021_021dadf8[entry->unk_2 - 1] == stage) {
                actor = CreateNewActorByParam(mmSys, entry->unk_4, entry->unk_8, i % 4, id, entry->unk_0, 2, zoneId);
                SetActorUserParam(actor, i, 0);
                SetActorUserParam(actor, entry->unk_a, 1);
                SetActorSCRID(actor, 5);
                if (id < 0xff) {
                    id++;
                }
            }
            entry++;
            i++;
        }
    }
}

u16 func_ov021_021cbba0(FieldActor *param_1) {
    return GetActorUserParam(param_1, 1);
}

BOOL func_ov021_021cbbac(BSubwayScrWork *bsw, u16 index) {
    const BSubwayActor *entry = &data_ov021_021daebc[index];

    if (bsw->playMode == 4) {
        return FALSE;
    }
    if (entry->unk_e != 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov021_021cbbd0(BSubwayScrWork *bsw, u16 index, u16 *a, u16 *b, u16 *c) {
    const BSubwayActor *entry = &data_ov021_021daebc[index];

    if (bsw->playMode == 4) {
        *a = 0;
        *b = 0;
        *c = 4;
        return;
    }
    if (entry->unk_e == 0) {
        *a = 0;
        *b = 0;
        *c = 4;
        return;
    }
    *a = entry->unk_a;
    *b = entry->unk_c;
    *c = entry->unk_e;
}

BOOL func_ov021_021cbc14(VM *vm, void *env) {
    GameData *gameData = GSYS_GetGameData(ScriptWork_GetGameSystem(FieldScriptEnv_GetScriptWork(env)));
    BSubwayScrWork *bsw = func_02012ed8(gameData);
    u32 regulation = 20;
    s32 count = bsw->unk10;
    u32 result;

    switch (bsw->playMode) {
    case 1:
    case 6:
        regulation = 21;
        break;
    case 2:
    case 3:
    case 7:
    case 8:
        regulation = 22;
        break;
    }
    if (count > 9999) {
        count = 9999;
    }
    result = func_0200b6e0(gameData, 4, regulation, count, 0, &bsw->unk7EC, &bsw->unk7EE);
    if (result == 2 || result == 3) {
        return TRUE;
    }
    return FALSE;
}




u32 func_ov021_021cbc94(u32 param_1) {
    u32 val;

    val = 0x14;
    switch(param_1) {
    case 1:
    case 6:
        val = 0x15;
        break;
    case 2:
    case 3:
    case 7:
    case 8:
        val = 0x16;
    }
    return val;
}

u32 func_ov021_021cbcc4(Field *field) {
    u16 zoneId = Field_GetPlayerStateZoneID(field);
    u32 index = 0;

    switch (zoneId) {
    case 67:
        index = 0;
        break;
    case 68:
        index = 5;
        break;
    case 69:
        index = 1;
        break;
    case 70:
        index = 6;
        break;
    case 71:
        index = 2;
        break;
    case 72:
        index = 7;
        break;
    case 73:
        index = 4;
        break;
    }
    return index;
}
