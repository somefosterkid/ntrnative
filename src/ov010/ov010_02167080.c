// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov027/scrcmd_survey.c, ported by tools/port_file.py.
// Black's code at ov010 0x02167080-0x02167358. No name is known: the module and address. It is the second part of
// pokebw2's ov027 scrcmd_survey.c, in its order: the survey rank message event (SurveyTextEvent_Run/_Create, the
// SurveyTextWork functions) and GetTrainerCardTextMSGID. Black 2 has the commands and this event in one file; Black
// has them apart: its commands are its own scrcmd_survey.c (0x0216b504-0x0216b970, between scrcmd_entree_forest.c
// and survey.c, as Black 2's ov012 orders those), whose func_ov010_0216b7f8 (SurveyCmd_ShowRankMessage) starts this
// event, and this part sits between event_research_radar.c and survey_data.c (files Black 2 moved out of this overlay
// too). The head of pokebw2's file (sProbabilities, probabilityLoop, insideProbabilityLoop) is Black's ov011
// intrude_comm.c's (data_ov011_02176c4c), not this file's.
#include "types.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "field/festival.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/survey.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/random.h"
#include "gfl/rtc_cache.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/wordset.h"


static GameEventReturnCode func_ov010_02167080(GameEvent *event, u32 *state, void *data);

static GameEventReturnCode func_ov010_02167080(GameEvent *event, u32 *state, void *data) {
    SurveyTextWork *work = data;
    switch (*state) {
    case 0:
        getSurveyText(work);
        func_ov010_02167178(work);
        func_ov010_021671dc(work);
        func_ov010_021671fc(work);
        (*state)++;
        break;
    case 1:
        if (func_ov021_0219077c(work->window) == 1) (*state)++;
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & 0xf3) (*state)++;
        break;
    case 3:
        func_ov010_021672b4(work);
        func_ov010_021671ec(work);
        func_ov010_021671cc(work);
        func_ov010_02167168(work);
        return TRUE;
    }
    return FALSE;
}

GameEvent *func_ov010_021670f8(GameSystem *gsys) {
    GameEvent *event;
    SurveyTextWork *work;
    event = GameEvent_Create(gsys, NULL, func_ov010_02167080, sizeof(SurveyTextWork));
    work = GameEvent_GetData(event);
    func_ov010_0216711c(work, gsys);
    return event;
}

void func_ov010_0216711c(SurveyTextWork *work, GameSystem *gsys) {
    Field *field;
    field = GSYS_GetField(gsys);
    MI_CpuFill8(work, 0, sizeof(SurveyTextWork));
    work->heapId = Field_GetHeapID(field);
    work->gameSystem = gsys;
    work->gameData = GSYS_GetGameData(gsys);
    work->field = field;
    work->msgBGSys = Field_GetMsgBGSys(field);
}

void getSurveyText(SurveyTextWork *work) {
    work->message = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, 0x2F, work->heapId);
}

void func_ov010_02167168(SurveyTextWork *work) {
    GFL_MsgDataFree(work->message);
    work->message = NULL;
}

void func_ov010_02167178(SurveyTextWork *work) {
    u32 pos;
    u32 width;
    u32 height;
    u32 mode = func_ov010_021672d0(work);

    // Each case gives the whole rect of the window (x and y alike), as pokebw2's SurveyTextWork_CreateWindow does:
    // the code sets the registers in each case
    switch (mode) {
    case 0:
        pos = 1;
        width = 30;
        height = 4;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        pos = 1;
        width = 30;
        height = 8;
        break;
    }
    work->window = FieldMsgBG_CreateMoneyWin(work->msgBGSys, work->message, pos, pos, width, height);
}

void func_ov010_021671cc(SurveyTextWork *work) {
    func_ov021_02190728(work->window);
    work->window = NULL;
}

void func_ov010_021671dc(SurveyTextWork *work) {
    work->wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
}

void func_ov010_021671ec(SurveyTextWork *work) {
    GFL_WordSetSystemFree(work->wordSet);
    work->wordSet = NULL;
}

void func_ov010_021671fc(SurveyTextWork *work) {
    StrBuf *source;
    StrBuf *formatted;
    u32 kind;
    u32 num1;
    u32 num2;
    u32 msgId;
    PlayerInfo *player;
    BmpWin *window;

    source = GFL_StrBufCreate(256, work->heapId);
    formatted = GFL_StrBufCreate(256, work->heapId);
    kind = func_ov010_021672d0(work);
    num1 = func_ov010_021672e4(work);
    num2 = func_ov010_021672f8(work);
    msgId = GetTrainerCardTextMSGID(kind);
    player = GetGameDataPlayerInfo(work->gameData);
    copyVarForText(work->wordSet, 0, player);
    WordSetNumber(work->wordSet, 1, num1, 5, 1, 1);
    WordSetNumber(work->wordSet, 2, num2, 5, 1, 1);
    GFL_MsgDataLoadStrbuf(work->message, msgId, source);
    GFL_WordSetFormatStrbuf(work->wordSet, formatted, source);
    GFL_WordSetClearAll(work->wordSet);
    func_ov021_02190758(work->window, 8, 0, formatted);
    window = func_ov021_021907c0(work->window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    GFL_StrBufFree(source);
    GFL_StrBufFree(formatted);
}

void func_ov010_021672b4(SurveyTextWork *work) {
    func_ov021_02190788(work->window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(func_ov021_021907c0(work->window)));
}

u32 func_ov010_021672d0(SurveyTextWork *work) {
    return func_0200c224(getTrainerCardData_wrapper(GameData_GetSaveControl_(work->gameData)));
}

u32 func_ov010_021672e4(SurveyTextWork *work) {
    return func_0200c1dc(getTrainerCardData_wrapper(GameData_GetSaveControl_(work->gameData)));
}

u32 func_ov010_021672f8(SurveyTextWork *work) {
    return func_0200c1c4(getTrainerCardData_wrapper(GameData_GetSaveControl_(work->gameData)));
}

u32 GetTrainerCardTextMSGID(u32 type) {
    switch (type) {
    case 0:
        return 0x174;
    case 1:
        return 0x175;
    case 2:
        return 0x176;
    case 3:
        return 0x177;
    case 4:
        return 0x178;
    case 5:
        return 0x179;
    }
    return 0;
}
