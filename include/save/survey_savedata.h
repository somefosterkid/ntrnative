// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/survey_savedata.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_SURVEY_SAVEDATA_H
#define BWPORT_SAVE_SURVEY_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"

#define SURVEY_QUESTION_COUNT 30
#define SURVEY_ANSWER_COUNT 150

// Black 2's, not checked against Black
#define SURVEY_ACTIVE_QUESTIONS 3

typedef struct SurveySave {
    // How many people answered each question today, and how many gave each answer
    u16 todayCounts[SURVEY_QUESTION_COUNT];
    u8 todayAnswers[SURVEY_ANSWER_COUNT];
    // The player's own answers, each question's in its bits
    u8 ownAnswers[10];
    // The same counts before today
    u32 totalCounts[SURVEY_QUESTION_COUNT];
    u16 totalAnswers[SURVEY_ANSWER_COUNT];
    // The questions being surveyed
    u8 questions[SURVEY_ACTIVE_QUESTIONS];
} SurveySave;

// The table of the questions, of a file that is not written yet: how many bits a question's answer takes, the index
// of its first answer in the answer counts, and the first bit of the player's answer
u8 func_ov010_02167358(u8 question);
// The block's size and its initial state
u32 func_0200e368(void);
void func_0200e370(SurveySave *survey);
SurveySave *func_0200e394(SaveControl *save);
// How many people answered a question, today and before today, which the Research Radar's graph adds up
u16 func_0200e458(SurveySave *survey, u32 question);
u32 func_0200e47c(SurveySave *survey, u32 question);
// The player's own answers, which the next two read and write a question's answer of
u8 *func_0200e3a0(SurveySave *survey);
u32 func_0200e3a4(u8 *answers, u8 question);
void func_0200e3e8(u8 *answers, u8 question, u32 answer);
// Sets the survey's question at index, and returns it
void func_0200e440(SurveySave *survey, u8 question, u32 index);
u8 func_0200e44c(SurveySave *survey, u32 index);
void func_0200e460(SurveySave *survey, u32 question, u32 count);
// How many people gave an answer, numbered from 1, today: whether it is full, the count, and adding to it, up to 99
BOOL func_0200e49c(SurveySave *survey, u16 question, int answer);
u32 func_0200e4b0(SurveySave *survey, u16 question, int answer);
void func_0200e4cc(SurveySave *survey, u16 question, int answer, int count);
// The same count before today
u32 func_0200e4f8(SurveySave *survey, u16 question, int answer);
// Adds today's counts to the totals and clears them
void func_0200e518(SurveySave *survey);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 func_ov010_02167380(u8 question);
u8 func_ov010_0216736c(u8 question);

#endif // BWPORT_SAVE_SURVEY_SAVEDATA_H
