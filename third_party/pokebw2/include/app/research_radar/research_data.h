#ifndef POKEBW2_APP_RESEARCH_RADAR_RESEARCH_DATA_H
#define POKEBW2_APP_RESEARCH_RADAR_RESEARCH_DATA_H

#include "types.h"
#include "struct_decls.h"

// The questions of a survey and their answers, as the Research Radar's graph shows them (research_data.c). The game
// doesn't name this file; it is named after what it reads
// The names of these functions and types are ours

#define RESEARCH_QUESTION_COUNT 3
#define RESEARCH_ANSWER_MAX 17

typedef struct {
    u16 id;
    // The answer's color in the graph
    u8 red;
    u8 green;
    u8 blue;
    // How many people gave the answer today, and in all
    u16 todayCount;
    u32 totalCount;
} ResearchAnswer;

typedef struct {
    u8 id;
    u8 answerCount;
    // How many people answered the question today, and in all
    u16 todayCount;
    u32 totalCount;
    ResearchAnswer answers[RESEARCH_ANSWER_MAX];
} ResearchQuestion;

struct ResearchData {
    // The survey's index in the Research Radar's list
    u8 surveyId;
    ResearchQuestion questions[RESEARCH_QUESTION_COUNT];
};

u16 ResearchData_GetAnswerID(const ResearchData *data, u8 question, u8 answer);
u8 ResearchData_GetAnswerRed(const ResearchData *data, u8 question, u8 answer);
u8 ResearchData_GetAnswerGreen(const ResearchData *data, u8 question, u8 answer);
u8 ResearchData_GetAnswerBlue(const ResearchData *data, u8 question, u8 answer);
// The index of the answer to a question, both by their IDs, or 0 if it isn't there
u8 ResearchData_GetAnswerIndex(const ResearchData *data, u8 questionId, u8 answerId);

#endif // POKEBW2_APP_RESEARCH_RADAR_RESEARCH_DATA_H
