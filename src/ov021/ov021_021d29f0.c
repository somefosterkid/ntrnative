// Black's code at ov021 0x021d29f0-0x021d2edc. No name is known: the module and address.

#include "types.h"
#include "field/field_script.h"
#include "gfl/random.h"
#include "save/records.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "field/field.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/bmp_winframe.h"
#include "system/printsys.h"
#include "system/vm.h"
#include "save/trainer_card.h"
#include "field/field_camera.h"
#include "save/player_info.h"

u32 func_ov021_021d2ce8(GameData *gameData, u32 monsNo);
#include "nitro/rtc.h"
#include "system/wordset.h"
#include "pml/poke_party.h"
#include "gfl/rtc_cache.h"
#include "nitro/fx/fx_trig.h"


const u32 data_ov021_021dd718[8] = {
    0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
};

typedef struct {
    s32 recordId1;
    s32 recordId2;
    u32 digits1;
    u32 digits2;
} Ov021WordSetRecord;

const Ov021WordSetRecord data_ov021_021dd774[29] = {
#include "asset/ov021_021d29f0_data_ov021_021dd774.inc"
};

static const u32 data_ov021_021dd738[15] = {
#include "asset/ov021_021d29f0_data_ov021_021dd738.inc"
};
s32 func_ov021_021d2d24(GameData *param_1, s32 param_2, s32 param_3);

u32 func_ov021_021d29f0(VM *param_1, FieldScriptEnv *param_2) {
    u16 heapId;
    ScriptWork *work;
    ScriptFieldWork *fieldWork;
    FieldMsgBG *msgBGSys;
    MsgData *msgData;
    BmpWin *window;
    GFLBitmap *pGVar3;
    StrBuf *strbuf;
    Font *font;
    work = FieldScriptEnv_GetScriptWork(param_2);
    fieldWork = ScriptWork_GetFieldWork(work);
    heapId = Field_GetHeapID(fieldWork->field);
    msgBGSys = Field_GetMsgBGSys(fieldWork->field);
    if (VM_Read16(param_1) != 0) {
        msgData = GFL_MsgSysLoadData(0, 2, 0x16, heapId);
        window = BmpWin_CreateStatic(2, 1, 1, 0x1e, 0xa, 0xe, 0x40);
        pGVar3 = BmpWin_GetBitmap(window);
        GFL_BitmapFill(pGVar3, 0xf);
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        BmpWin_DrawFrame(window, 0, 1, 0xd);
        strbuf = GFL_StrBufCreate(0xb4, heapId);
        GFL_MsgDataLoadStrbuf(msgData, 3, strbuf);
        pGVar3 = BmpWin_GetBitmap(window);
        font = func_ov021_021904ac(msgBGSys);
        GFL_TextRendererDrawToBitmap(pGVar3, 0, 0, strbuf, font);
        BmpWin_FlushChar(window);
        GFL_BGSysQueueScrLoad(2);
        GFL_StrBufFree(strbuf);
        GFL_MsgDataFree(msgData);
        BmpWin_Free(window);
    } else {
        GFL_BGSysFillScrArea(2, 0, 0, 0, 0x20, 0xc, 0x11);
        GFL_BGSysQueueScrLoad(2);
    }
    return 0;
}

u32 func_ov021_021d2adc(VM *param_1, FieldScriptEnv *param_2) {
    u16 id;
    u16 value;
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    GameRecords *records;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    id = ScriptReadAny(param_1, param_2);
    value = ScriptReadAny(param_1, param_2);
    gameData = GSYS_GetGameData(gsys);
    save = GameData_GetSaveControl(gameData);
    records = getTrainerCardInfoBlkAddress(save);
    RecordAdd(records, (u32)id, (u32)value);
    return 0;
}

u32 func_ov021_021d2b18(VM *param_1, FieldScriptEnv *param_2) {
    u16 id;
    GameSystem *gsys;
    u16 *ptr;
    GameData *gameData;
    SaveControl *save;
    GameRecords *records;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    id = ScriptReadAny(param_1, param_2);
    ptr = ScriptReadVar(param_1, param_2);
    gameData = GSYS_GetGameData(gsys);
    save = GameData_GetSaveControl(gameData);
    records = getTrainerCardInfoBlkAddress(save);
    *ptr = (u16)RecordGet(records, (u32)id);
    return 0;
}

u32 func_ov021_021d2b54(VM *vm, FieldScriptEnv *env) {
    ScriptWork *scriptWork = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = GSYS_GetGameData(FieldScriptEnv_GetGameSystem(env));
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(gameData);
    WordSet *wordSet = ScriptWork_GetWordSet(scriptWork);
    u32 value = ScriptReadAny(vm, env);
    u16 *dest = ScriptReadVar(vm, env);
    PokeParty *party;
    s32 i;
    s32 pkmCount;
    RTCTime time;
    u32 offset;
    s32 index;

    copyVarForText(wordSet, 0, playerInfo);
    party = GameData_GetParty(gameData);
    pkmCount = PokeParty_GetPkmCount(party);
    for (i = 0; i < pkmCount; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);

        if (PokeParty_GetParam(pkm, 0x4C, NULL) == 0) {
            WordSet_LoadSpeciesName(wordSet, 3, (u16)PokeParty_GetParam(pkm, 5, NULL));
            break;
        }
    }

    RTC_GetCachedTime(&time);
    if (time.minute < 10) {
        s32 value1;
        s32 value2;
        GameRecords *records;

        offset = 0;
        index = GFL_RandomLC(29);
        value1 = 0;
        value2 = 0;
        records = GameData_GetRecords(gameData);
        if (data_ov021_021dd774[index].recordId1 != -1) {
            value1 = RecordGet(records, data_ov021_021dd774[index].recordId1);
            WordSetNumber(wordSet, 1, value1, data_ov021_021dd774[index].digits1, 0, 1);
        }
        if (data_ov021_021dd774[index].recordId2 != -1) {
            value2 = RecordGet(records, data_ov021_021dd774[index].recordId2);
            WordSetNumber(wordSet, 2, value2, data_ov021_021dd774[index].digits2, 0, 1);
        }
        if (index < 11) {
            if (data_ov021_021dd774[index].recordId1 != -1 && data_ov021_021dd774[index].recordId2 != -1) {
                if (value1 == 0 || value2 == 0) {
                    index = 0;
                }
            } else if (data_ov021_021dd774[index].recordId2 == -1 && value1 == 0) {
                index = 0;
            }
        }
        if (index < 11) {
            u8 gender = getTrainerGender(playerInfo);

            if (gender == 1) {
                index += 11;
            }
        } else {
            index += 11;
        }
    } else {
        if (time.minute < 35) {
            offset = 40;
            index = GFL_RandomLC(50);
        } else {
            offset = 90;
            index = GFL_RandomLC(80);
        }
    }
    *dest = func_ov021_021d2d24(gameData, func_ov021_021d2ce8(gameData, offset + index), value);
    return 0;
}


u32 func_ov021_021d2ce8(GameData *gameData, u32 monsNo) {
    TrainerGameInfoSave *trainerInfo = getTrainerGameInfoAddress(GameData_GetSaveControl(gameData));
    int i;
    u32 random;

    for (i = 0; i < 8; i++) {
        if (monsNo == data_ov021_021dd718[i]) {
            if (isBadgeObtained(trainerInfo, i)) {
                return monsNo;
            }
            random = GFL_RandomLC(12);
            return random + 90;
        }
    }
    return monsNo;
}


s32 func_ov021_021d2d24(GameData *param_1, s32 param_2, s32 param_3) {
    s32 i;

    if (param_3 == 0) {
        i = 0;
        do {
            if (param_2 == data_ov021_021dd738[i]) {
                return GFL_RandomLC(0x14) + 0x6e;
            }
            i = i + 1;
        } while (i < 0xf);
    }
    return param_2;
}

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u8 unk0E;
    u8 unk0F;
    u32 unk10;
    u32 unk14;
    VecFx32 *bind;
} EvCameraShakeWork;

static GameEventReturnCode func_ov021_021d2d7c(GameEvent *event, u32 *state, void *data);

GameEvent *EventEvCameraShake_Create(GameSystem *gsys, const FieldEvCameraShake *shake) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov021_021d2d7c, sizeof(EvCameraShakeWork));
    FieldEvCameraShake *data = GameEvent_GetData(event);

    *data = *shake;
    return event;
}

static GameEventReturnCode func_ov021_021d2d7c(GameEvent *event, u32 *state, void *data) {
    BOOL finished = FALSE;
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);
    EvCameraShakeWork *work = GameEvent_GetData(event);
    FieldCamera *camera = Field_GetCameraSystem(field);

    switch (*state) {
    case 0:
        FieldCamera_FinishDelay(camera);
        (*state)++;
        break;
    case 1:
        if (FieldCamera_IsDelayActive(camera) == FALSE) {
            work->bind = FieldCamera_GetBind(camera);
            FieldCamera_ClearBind(camera);
            (*state)++;
        }
        break;
    case 2: {
        u16 phase;

        work->unk14++;
        phase = (u16)((work->unk14 << 16) / work->unk10);
        if (work->unk08 != 0 && work->unk0E - work->unk0F >= work->unk08) {
            work->unk0A++;
            if (work->unk0A >= work->unk0C) {
                work->unk0A = 0;
                work->unk02 -= work->unk06;
                work->unk00 -= work->unk04;
                if (work->unk02 <= 0) {
                    work->unk02 = 1;
                }
                if (work->unk00 <= 0) {
                    work->unk00 = 1;
                }
            }
        }
        if (work->unk14 >= work->unk10) {
            work->unk0F--;
            work->unk14 = 0;
        }
        if (work->unk0F == 0) {
            work->unk14 = 0;
            finished = TRUE;
        }
        if (finished) {
            VecFx32 zero = { 0, 0, 0 };

            func_ov021_0218ed04(camera, &zero);
            FieldCamera_SetBind(camera, work->bind);
            FieldCamera_EnableDelay(camera);
            (*state)++;
            break;
        }
        {
            fx32 dx = work->unk00 * FX_SinCosTable_[(phase >> 4) << 1];
            fx32 dy = work->unk02 * FX_SinCosTable_[(phase >> 4) << 1];
            VecFx32 pos = *work->bind;
            VecFx32 delta;

            delta.x = dx;
            pos.x += dx;
            delta.y = dy;
            pos.y += dy;
            delta.z = 0;
            FieldCamera_CoordsSetTarget(camera, &pos);
            func_ov021_0218ed04(camera, &delta);
        }
        break;
    }
    case 3:
        return 1;
    }
    return 0;
}
