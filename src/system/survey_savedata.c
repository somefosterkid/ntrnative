// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/survey_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/survey_savedata.h"
#include "gfl/std.h"
#include "save/save_control.h"

// The save block of the Passerby Analytics HQ's surveys: how many people answered each question and how, today and
// before today, the player's own answers and the questions being surveyed. The ROM has no string for the file, so the
// name is descriptive. Block 0x3d.

#define SAVE_BLOCK_SURVEY 0x3f
#define SURVEY_TODAY_COUNT_MAX 9999
#define SURVEY_TODAY_ANSWER_MAX 99
#define SURVEY_TOTAL_COUNT_MAX 999999
#define SURVEY_TOTAL_ANSWER_MAX 9999

static u32 func_0200e488(u16 question, int answer);

u32 func_0200e368(void) {
    return sizeof(SurveySave);
}

void func_0200e370(SurveySave *survey) {
    MI_CpuFill8(survey, 0, sizeof(SurveySave));
    survey->questions[0] = 0;
    survey->questions[1] = 8;
    survey->questions[2] = 29;
}

SurveySave *func_0200e394(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_SURVEY);
}

u8 *func_0200e3a0(SurveySave *survey) {
    return survey->ownAnswers;
}

u32 func_0200e3a4(u8 *answers, u8 question) {
    u32 offset = func_ov010_02167380(question);
    u32 count = func_ov010_02167358(question);
    u32 byte = offset / 8;
    u32 bit = offset % 8;
    u32 answer = 0;
    u32 i;

    for (i = 0; count != 0; count--) {
        answer |= ((answers[byte] >> bit) & 1) << i;
        bit++;
        if (bit > 7) {
            byte++;
            bit = 0;
        }
        i++;
    }
    return answer;
}

void func_0200e3e8(u8 *answers, u8 question, u32 answer) {
    u32 offset = func_ov010_02167380(question);
    u32 count = func_ov010_02167358(question);
    u32 byte = offset / 8;
    u32 bit = offset % 8;

    for (; count != 0; count--) {
        answers[byte] &= 0xff ^ (1 << bit);
        answers[byte] |= (answer & 1) << bit;
        bit++;
        if (bit > 7) {
            byte++;
            bit = 0;
        }
        answer >>= 1;
    }
}

void func_0200e440(SurveySave *survey, u8 question, u32 index) {
    survey->questions[index] = question;
}

u8 func_0200e44c(SurveySave *survey, u32 index) {
    return survey->questions[index];
}

u16 func_0200e458(SurveySave *survey, u32 question) {
    return survey->todayCounts[question];
}

void func_0200e460(SurveySave *survey, u32 question, u32 count) {
    survey->todayCounts[question] += (u16)count;
    if (survey->todayCounts[question] > SURVEY_TODAY_COUNT_MAX) {
        survey->todayCounts[question] = SURVEY_TODAY_COUNT_MAX;
    }
}

u32 func_0200e47c(SurveySave *survey, u32 question) {
    return survey->totalCounts[question];
}

// Answers are numbered from 1
// pokebw2's getAnswerIndex
static u32 func_0200e488(u16 question, int answer) {
    u32 first = func_ov010_0216736c(question);

    return first + (answer - 1);
}

BOOL func_0200e49c(SurveySave *survey, u16 question, int answer) {
    if (func_0200e4b0(survey, question, answer) >= SURVEY_TODAY_ANSWER_MAX) {
        return TRUE;
    }
    return FALSE;
}

u32 func_0200e4b0(SurveySave *survey, u16 question, int answer) {
    if (answer == 0) {
        return 0;
    }
    return survey->todayAnswers[func_0200e488(question, answer)];
}

void func_0200e4cc(SurveySave *survey, u16 question, int answer, int count) {
    u32 index;

    if (answer == 0) {
        return;
    }
    index = func_0200e488(question, answer);
    if (count + survey->todayAnswers[index] > SURVEY_TODAY_ANSWER_MAX) {
        survey->todayAnswers[index] = SURVEY_TODAY_ANSWER_MAX;
    } else {
        survey->todayAnswers[index] += count;
    }
}

u32 func_0200e4f8(SurveySave *survey, u16 question, int answer) {
    if (answer == 0) {
        return 0;
    }
    return survey->totalAnswers[func_0200e488(question, answer)];
}

// Adds today's counts to the totals and clears them
void func_0200e518(SurveySave *survey) {
    int i;

    for (i = 0; i < SURVEY_QUESTION_COUNT; i++) {
        survey->totalCounts[i] += survey->todayCounts[i];
        if (survey->totalCounts[i] > SURVEY_TOTAL_COUNT_MAX) {
            survey->totalCounts[i] = SURVEY_TOTAL_COUNT_MAX;
        }
        survey->todayCounts[i] = 0;
    }
    for (i = 0; i < SURVEY_ANSWER_COUNT; i++) {
        survey->totalAnswers[i] += survey->todayAnswers[i];
        if (survey->totalAnswers[i] > SURVEY_TOTAL_ANSWER_MAX) {
            survey->totalAnswers[i] = SURVEY_TOTAL_ANSWER_MAX;
        }
        survey->todayAnswers[i] = 0;
    }
}
