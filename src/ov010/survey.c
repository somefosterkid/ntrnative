// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/survey.c, ported by tools/port_file.py.
// The questions of the surveys, and whether the one asked is done: answered enough times, or run for its days. The
// name is descriptive
#include "types.h"
#include "field/survey.h"
#include "gfl/rtc_cache.h"
#include "nitro/rtc.h"
#include "save/event_work.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"

// The most an answer counts
#define SURVEY_COUNT_MAX 999999
// The flags that a survey's done event sets, from question 1
#define FLAG_SURVEY_DONE_BASE 0x91

// A question of the surveys
typedef struct {
    // 0 when the answers must reach a count, 1 when the survey runs for days
    u8 type;
    u8 answerCount;
    u8 answers[3];
    u8 threshold;
    u8 days;
} SurveyQuestion;

static const SurveyQuestion data_ov010_021723bc[] = {
#include "asset/survey_data_ov010_021723bc.inc"
};

BOOL func_ov010_0216b994(void *survey, u16 question);
BOOL func_ov010_0216b9d8(SaveControl *save);
BOOL func_ov010_0216ba08(SaveControl *save);
BOOL func_ov010_0216ba40(void *survey, u16 question);

u32 func_ov010_0216bbb4(u16 question);
u16 func_ov010_0216bb80(u16 question);
int func_ov010_0216bbc4(u8 question);

u16 detectLengthSinceLastSession(SaveControl *save);

u32 func_ov010_0216b970(void *survey, u8 answer) {
    u32 count = func_0200e458(survey, answer) + func_0200e47c(survey, answer);

    if (count > SURVEY_COUNT_MAX) {
        count = SURVEY_COUNT_MAX;
    }
    return count;
}

BOOL func_ov010_0216b994(void *survey, u16 question) {
    u8 answers[3];
    u8 count;
    u32 threshold;
    int i;

    func_ov010_0216bb90(question, answers);
    count = func_ov010_0216bb80(question);
    threshold = func_ov010_0216bbb4(question);
    for (i = 0; i < count; i++) {
        if (func_ov010_0216b970(survey, answers[i]) < threshold) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_ov010_0216b9d8(SaveControl *save) {
    u16 question;
    int hours;

    func_0200e394(save);
    question = func_0200c32c(getTrainerCardData_wrapper(save));
    hours = detectLengthSinceLastSession(save);
    if (func_ov010_0216bbc4(question) > hours) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov010_0216ba08(SaveControl *save) {
    TrainerCardSave *info = getTrainerCardData_wrapper(save);
    u8 count = func_ov010_0216bb80(func_0200c32c(info));
    int i;

    for (i = 0; i < count; i++) {
        if (!func_0200c34c(info, i)) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_ov010_0216ba40(void *survey, u16 question) {
    u8 answers[3];
    u8 count;
    int i;

    func_ov010_0216bb90(question, answers);
    count = func_ov010_0216bb80(question);
    func_ov010_0216bbb4(question);
    for (i = 0; i < count; i++) {
        if (func_ov010_0216b970(survey, answers[i]) < SURVEY_COUNT_MAX) {
            return FALSE;
        }
    }
    return TRUE;
}

u16 func_ov010_0216ba88(SaveControl *save) {
    u16 question;
    int i;
    u32 min;
    u8 count;
    void *survey;
    u8 answers[3];
    u32 total;

    survey = func_0200e394(save);
    question = func_0200c32c(getTrainerCardData_wrapper(save));
    total = func_ov010_0216bbb4(question);
    count = func_ov010_0216bb80(question);
    func_ov010_0216bb90(question, answers);
    min = 0xF694E;
    for (i = 0; i < count; i++) {
        u32 c = func_ov010_0216b970(survey, answers[i]);
        if (c < min) {
            min = c;
        }
    }
    return (u16)(total - min);
}

u16 func_ov010_0216bae8(SaveControl *save) {
    u16 question;
    int hours;
    int days;
    int diff;

    func_0200e394(save);
    question = func_0200c32c(getTrainerCardData_wrapper(save));
    hours = detectLengthSinceLastSession(save);
    days = func_ov010_0216bbc4(question);
    diff = days - hours;
    if (diff < 0) {
        diff = 0;
    }
    return (u16)diff;
}


u16 detectLengthSinceLastSession(SaveControl *save) {
    TrainerGameInfoSave *info = getTrainerGameInfoAddress(save);
    s64 start;
    RTCDate date;
    RTCTime time;

    if (func_0200c32c(info) == 0) {
        return 0;
    }
    start = getSecondsFromTrainerCardData(info);
    RTC_ConvertSecondToDateTime(&date, &time, RTC_ConvertSecondsCached() - start);
    if (date.year != 0 || date.month > 1 || date.day > 1) {
        return 24;
    }
    return time.hour;
}

u32 func_ov010_0216bb70(u16 question) {
    return data_ov010_021723bc[question].type;
}

u16 func_ov010_0216bb80(u16 question) {
    return data_ov010_021723bc[question].answerCount;
}


void func_ov010_0216bb90(u16 question, u8 *answers) {
    answers[0] = data_ov010_021723bc[question].answers[0];
    answers[1] = data_ov010_021723bc[question].answers[1];
    answers[2] = data_ov010_021723bc[question].answers[2];
}

u32 func_ov010_0216bbb4(u16 question) {
    return data_ov010_021723bc[question].threshold;
}

int func_ov010_0216bbc4(u8 question) {
    return data_ov010_021723bc[question].days;
}
