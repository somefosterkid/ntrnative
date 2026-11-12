// Black's code at ov010 0x0216b504-0x0216b970: the script commands of pokebw2's ov027 scrcmd_survey.c, the first part
// of that file in its order (SurveyCmd_GetRank, SurveyCmd_RaiseRank, s01FF_SurveyGetCurrentQuestionID,
// s0200_SurveyGetCurrentAnswerIDs, s0204_SurveyGetTime, SurveyCmd_Start ... SurveyCmd_GetCurrentRemaining and its two
// helpers), between scrcmd_entree_forest.c and survey.c as Black 2's ov012 orders those. Black 2 has these commands and
// the survey rank message event in one file; Black has the event apart at 0x02167080 (ov010_02167080.c, which
// func_ov010_0216b7f8, SurveyCmd_ShowRankMessage, starts): the commands are Black's scrcmd_survey.c.

#include "types.h"
#include "field/field_script.h"
#include "field/survey.h"
#include "gfl/rtc_cache.h"
#include "save/save_control.h"
#include "save/survey_savedata.h"
#include "save/trainer_card.h"
#include "struct_decls.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/trainer_game_info_savedata.h"

const u8 data_ov010_0217219c[0x20] = {
#include "asset/scrcmd_survey_data_ov010_0217219c.inc"
};

const u8 data_ov010_021721bc[0x200] = {
#include "asset/scrcmd_survey_data_ov010_021721bc.inc"
};

u32 func_ov010_0216b504(VM *param_1, FieldScriptEnv *param_2) {
    GameData *gameData;
    SaveControl *save;
    TrainerGameInfoSave *info;
    u16 *ptr;

    FieldScriptEnv_GetScriptWork(param_2);
    gameData = FieldScriptEnv_GetGameData(param_2);
    save = GameData_GetSaveControl(gameData);
    info = getTrainerGameInfoAddress(save);
    ptr = ScriptReadVar(param_1, param_2);
    *ptr = (u16)func_0200c224(info);
    return 0;
}

BOOL func_ov010_0216b538(VM *vm, FieldScriptEnv *env) {
    int rank;
    TrainerGameInfoSave *info;

    FieldScriptEnv_GetScriptWork(env);
    info = getTrainerGameInfoAddress(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));
    rank = func_0200c224(info);
    if (rank >= 5) {
        return FALSE;
    }
    func_0200c22c(info, rank + 1);
    func_02027138((u8)(rank + 1));
    return FALSE;
}


u32 func_ov010_0216b578(VM *param_1, FieldScriptEnv *param_2) {
    GameData *gameData;
    SaveControl *save;
    TrainerGameInfoSave *info;
    u16 *ptr;

    FieldScriptEnv_GetScriptWork(param_2);
    gameData = FieldScriptEnv_GetGameData(param_2);
    save = GameData_GetSaveControl(gameData);
    info = getTrainerGameInfoAddress(save);
    ptr = ScriptReadVar(param_1, param_2);
    *ptr = func_0200c32c(info);
    return 0;
}

BOOL s0200_SurveyGetCurrentAnswerIDs(VM *vm, FieldScriptEnv *env) {
    u8 result;
    GameData *gameData;
    SaveControl *save;
    TrainerGameInfoSave *info;
    u16 *ptr;
    u16 *ptr2;
    u16 *ptr3;

    FieldScriptEnv_GetScriptWork(env);
    gameData = FieldScriptEnv_GetGameData(env);
    save = GameData_GetSaveControl(gameData);
    info = getTrainerGameInfoAddress(save);
    ptr = ScriptReadVar(vm, env);
    ptr2 = ScriptReadVar(vm, env);
    ptr3 = ScriptReadVar(vm, env);
    result = func_0200c33c(info, 0);
    *ptr = (u16)result;
    result = func_0200c33c(info, 1);
    *ptr2 = (u16)result;
    result = func_0200c33c(info, 2);
    *ptr3 = (u16)result;
    return 0;
}

BOOL func_ov010_0216b60c(VM *vm, FieldScriptEnv *env) {
    SaveControl *save;
    u16 *out;

    FieldScriptEnv_GetScriptWork(env);
    save = GameData_GetSaveControl(FieldScriptEnv_GetGameData(env));
    out = ScriptReadVar(vm, env);
    *out = detectLengthSinceLastSession(save);
    return FALSE;
}

BOOL func_ov010_0216b63c(VM *vm, FieldScriptEnv *env) {
    SaveControl *save;
    TrainerGameInfoSave *info;
    u16 values[3];
    u16 first;
    s32 i;

    FieldScriptEnv_GetScriptWork(env);
    save = GameData_GetSaveControl(FieldScriptEnv_GetGameData(env));
    info = getTrainerGameInfoAddress(save);
    func_0200e394(save);
    first = ScriptReadAny(vm, env);
    values[0] = ScriptReadAny(vm, env);
    values[1] = ScriptReadAny(vm, env);
    values[2] = ScriptReadAny(vm, env);
    func_0200c334(info, first);
    for (i = 0; i < 3; i++) {
        func_0200c344(info, i, values[i]);
        func_0200c358(info, i, 0);
    }
    setSecondsCurrentTimeInTrainerCard(info, RTC_ConvertSecondsCached());
    return FALSE;
}

u32 func_ov010_0216b6dc(u32 param_1, FieldScriptEnv *param_2) {
    GameData *gameData;
    SaveControl *save;
    TrainerGameInfoSave *info;
    s32 index;

    FieldScriptEnv_GetScriptWork(param_2);
    gameData = FieldScriptEnv_GetGameData(param_2);
    save = GameData_GetSaveControl(gameData);
    info = getTrainerGameInfoAddress(save);
    func_0200e394(save);
    index = 0;
    func_0200c334(info, 0);
    do {
        func_0200c344(info, (u8)index, 0xff);
        index = index + 1;
    } while (index < 3);
    return 0;
}

BOOL func_ov010_0216b720(VM *vm, FieldScriptEnv *env) {
    s32 i;
    u16 question;
    SaveControl *save;
    SurveySave *survey;
    s32 best;
    u16 *out;
    u16 answer;
    int count;
    s32 score;

    FieldScriptEnv_GetScriptWork(env);
    save = GameData_GetSaveControl(FieldScriptEnv_GetGameData(env));
    survey = func_0200e394(save);
    question = ScriptReadAny(vm, env);
    out = ScriptReadVar(vm, env);
    count = data_ov010_0217219c[question];
    best = -1;
    for (i = 0; i < count; i++) {
        score = func_0200e4f8(survey, question, i + 1);
        score += func_0200e4b0(survey, question, i + 1);
        if (best < score) {
            best = score;
            answer = data_ov010_021721bc[question * 0x11 + i];
        }
    }
    *out = answer;
    return FALSE;
}


BOOL func_ov010_0216b7b0(VM *vm, FieldScriptEnv *env) {
    GameData *gameData;
    SaveControl *save;
    SurveySave *survey;
    u8 *answers;

    FieldScriptEnv_GetScriptWork(env);
    gameData = FieldScriptEnv_GetGameData(env);
    save = GameData_GetSaveControl(gameData);
    survey = func_0200e394(save);
    answers = func_0200e3a0(survey);
    func_0200e3e8(answers, (u8)ScriptReadAny(vm, env), (u32)ScriptReadAny(vm, env));
    func_02027100(answers);
    return 0;
}

u32 func_ov010_0216b7f8(u32 param_1, FieldScriptEnv *param_2) {
    ScriptWork *work;
    GameSystem *gsys;
    GameEvent *event;

    work = FieldScriptEnv_GetScriptWork(param_2);
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    event = func_ov010_021670f8(gsys);
    ScriptWork_CallEvent(work, event);
    return 1;
}

BOOL func_ov010_0216b81c(VM *vm, FieldScriptEnv *env) {
    SaveControl *save;
    TrainerCardSave *card;
    u8 index;
    u16 *out;

    save = GameData_GetSaveControl(FieldScriptEnv_GetGameData(env));
    card = getTrainerCardData_wrapper(save);
    index = ScriptReadAny(vm, env);
    out = ScriptReadVar(vm, env);
    if (index == func_0200c32c(card)) {
        func_ov010_0216b8b8(save, out);
    } else {
        func_ov010_0216b91c(save, index, out);
    }
    return FALSE;
}


BOOL func_ov010_0216b86c(VM *vm, FieldScriptEnv *env) {
    SaveControl *save;
    TrainerCardSave *card;
    u16 *out;
    u8 kind;
    u16 result;

    save = GameData_GetSaveControl(FieldScriptEnv_GetGameData(env));
    card = getTrainerCardData_wrapper(save);
    out = ScriptReadVar(vm, env);
    kind = func_ov010_0216bb70(func_0200c32c(card));
    if (kind != 0) {
        if (kind == 1) {
            result = func_ov010_0216bae8(save);
        }
    } else {
        result = func_ov010_0216ba88(save);
    }
    *out = result;
    return FALSE;
}

void func_ov010_0216b8b8(SaveControl *save, u16 *out) {
    SurveySave *survey;
    TrainerCardSave *card;
    u16 question;
    u8 type;

    survey = func_0200e394(save);
    card = getTrainerCardData_wrapper(save);
    question = func_0200c32c(card);
    type = func_ov010_0216bb70(question);
    if (type == 0) {
        if (func_ov010_0216b994(survey, question)) {
            *out = 1;
        } else {
            *out = 2;
        }
    } else if (type == 1) {
        if (func_ov010_0216b9d8(save)) {
            if (func_ov010_0216ba08(save)) {
                *out = 1;
            } else {
                *out = 4;
            }
        } else {
            *out = 3;
        }
    }
}


void func_ov010_0216b91c(SaveControl *save, u8 question, u16 *out) {
    SurveySave *survey;
    u8 type;

    survey = func_0200e394(save);
    getTrainerCardData_wrapper(save);
    type = func_ov010_0216bb70(question);
    if (type == 0) {
        if (func_ov010_0216b994(survey, question)) {
            *out = 1;
        } else {
            *out = 2;
        }
    } else if (type == 1) {
        if (func_ov010_0216ba40(survey, question)) {
            *out = 1;
        } else {
            *out = 3;
        }
    }
}
