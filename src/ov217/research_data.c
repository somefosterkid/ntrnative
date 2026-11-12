// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_data.c, which is the same code in
// Black 2/White 2's overlay 310. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "types.h"
#include "app/research_radar/research_data.h"

static u8 func_ov217_021c2684(const ResearchData *data, u8 questionId);

// pokebw2's ResearchData_GetAnswerID
u16 func_ov217_021c2644(const ResearchData *data, u8 question, u8 answer) {
    return data->questions[question].answers[answer].id;
}

// pokebw2's ResearchData_GetAnswerRed
u8 func_ov217_021c2654(const ResearchData *data, u8 question, u8 answer) {
    return data->questions[question].answers[answer].red;
}

// pokebw2's ResearchData_GetAnswerGreen
u8 func_ov217_021c2664(const ResearchData *data, u8 question, u8 answer) {
    return data->questions[question].answers[answer].green;
}

// pokebw2's ResearchData_GetAnswerBlue
u8 func_ov217_021c2674(const ResearchData *data, u8 question, u8 answer) {
    return data->questions[question].answers[answer].blue;
}

// The index of a question by its ID, or 0 if it isn't there (pokebw2's ResearchData_GetQuestionIndex)
static u8 func_ov217_021c2684(const ResearchData *data, u8 questionId) {
    int i;

    for (i = 0; i < RESEARCH_QUESTION_COUNT; i++) {
        if (questionId == data->questions[i].id) {
            return i;
        }
    }
    return 0;
}

// pokebw2's ResearchData_GetAnswerIndex
u8 func_ov217_021c26ac(const ResearchData *data, u8 questionId, u8 answerId) {
    int i;
    const ResearchQuestion *question = &data->questions[func_ov217_021c2684(data, questionId)];

    for (i = 0; i < question->answerCount; i++) {
        if (answerId == question->answers[i].id) {
            return i;
        }
    }
    return 0;
}
