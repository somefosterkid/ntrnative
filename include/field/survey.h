// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/survey.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_FIELD_SURVEY_H
#define BWPORT_FIELD_SURVEY_H

// Survey function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "save/survey_savedata.h"

// The hours since the survey started, up to 24
u16 detectLengthSinceLastSession(SaveControl *save);

// Black 2's, not checked against Black
typedef struct {
    // Whether the question has no rank of its own in the trainer's game info
    u8 unk00_0 : 1;
    u8 unk00_1 : 7;
    // How many bits the answer takes
    u8 bits;
    // The index of the first answer in the save's answer counts
    u8 firstAnswer;
    // The first bit of the player's answer
    u8 firstBit;
} SurveyAnswerInfo;

extern const SurveyAnswerInfo data_ov010_02171f5c[SURVEY_QUESTION_COUNT];

void func_ov010_0216bb90(u16 question, u8 *answers);
u32 func_ov010_0216b970(void *survey, u8 answer);

void probabilityLoop(SurveyProbabilityState *state);
// Black 2's; twinned code confirms the offsets of probability in Black
struct SurveyProbabilityState {
    u8 selection[6];
    u8 probability[6];
    u16 score;
    u16 bonus;
};

// Black 2's, not checked against Black
struct SurveyProbabilityEntry {
    u8 selection;
    u8 requiredScore;
    u8 probability;
    u8 flags;
};

// Black 2's; twinned code confirms the offsets of heapId, gameSystem, gameData, field, msgBGSys, window, message and
// wordSet in Black
struct SurveyTextWork {
    HeapID heapId;
    u8 unused02[2];
    GameSystem *gameSystem;
    GameData *gameData;
    Field *field;
    void *msgBGSys;
    void *window;
    MsgData *message;
    WordSet *wordSet;
};

BOOL s0200_SurveyGetCurrentAnswerIDs(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b538(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b63c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b7b0(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0216b86c(VM *vm, FieldScriptEnv *env);
u16 func_ov010_0216ba88(SaveControl *save);
u16 func_ov010_0216bae8(SaveControl *save);
void func_ov010_0216b8b8(SaveControl *save, u16 *out);
void func_ov010_0216b91c(SaveControl *save, u8 index, u16 *out);
u32 func_ov010_0216bb70(u16 question);
BOOL func_ov010_0216b994(void *survey, u16 question);
BOOL func_ov010_0216b9d8(SaveControl *save);
BOOL func_ov010_0216ba08(SaveControl *save);
BOOL func_ov010_0216ba40(void *survey, u16 question);
GameEvent *func_ov010_021670f8(GameSystem *gsys);
void func_ov010_0216711c(SurveyTextWork *work, GameSystem *gsys);
void func_ov010_02167168(SurveyTextWork *work);
void func_ov010_02167178(SurveyTextWork *work);
void func_ov010_021671dc(SurveyTextWork *work);
void func_ov010_021671ec(SurveyTextWork *work);
void func_ov010_021671fc(SurveyTextWork *work);
void func_ov010_021672b4(SurveyTextWork *work);
u32 func_ov010_021672d0(SurveyTextWork *work);
// How many answers a survey question has, 0 past the last question
u8 func_ov010_02167394(u8 question);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void insideProbabilityLoop(SurveyProbabilityState *state, u32 selection);
void getSurveyText(SurveyTextWork *work);
// Whether the survey's answer of the index has been given
u16 func_0200c34c(TrainerGameInfoSave *info, u8 index);
void func_0200c344(TrainerGameInfoSave *info, u8 index, u8 answer);
void func_0200c358(TrainerGameInfoSave *info, u8 index, u16 count);
// Adds to the count of the answer, up to 0xffff
void func_0200c364(TrainerGameInfoSave *info, u8 index, int count);
void func_ov010_021671cc(SurveyTextWork *work);
u32 func_ov010_021672e4(SurveyTextWork *work);
u32 func_ov010_021672f8(SurveyTextWork *work);

#endif // BWPORT_FIELD_SURVEY_H
