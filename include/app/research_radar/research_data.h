// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/research_data.h. The
// names of the types are pokebw2's; its function names (ResearchData_GetAnswerID, ResearchData_GetAnswerRed,
// ResearchData_GetAnswerGreen, ResearchData_GetAnswerBlue and ResearchData_GetAnswerIndex) are in comments.
#ifndef BWPORT_APP_RESEARCH_RADAR_RESEARCH_DATA_H
#define BWPORT_APP_RESEARCH_RADAR_RESEARCH_DATA_H

#include "types.h"
#include "struct_decls.h"

// The questions of a survey and their answers, as the Research Radar's graph shows them (research_data.c). The game
// doesn't name this file; pokebw2 named it after what it reads

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

// pokebw2's ResearchData_GetAnswerID, ResearchData_GetAnswerRed, ResearchData_GetAnswerGreen and
// ResearchData_GetAnswerBlue
u16 func_ov217_021c2644(const ResearchData *data, u8 question, u8 answer);
u8 func_ov217_021c2654(const ResearchData *data, u8 question, u8 answer);
u8 func_ov217_021c2664(const ResearchData *data, u8 question, u8 answer);
u8 func_ov217_021c2674(const ResearchData *data, u8 question, u8 answer);
// The index of the answer to a question, both by their IDs, or 0 if it isn't there (pokebw2's
// ResearchData_GetAnswerIndex)
u8 func_ov217_021c26ac(const ResearchData *data, u8 questionId, u8 answerId);

#endif // BWPORT_APP_RESEARCH_RADAR_RESEARCH_DATA_H
