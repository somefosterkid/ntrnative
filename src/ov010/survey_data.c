// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/survey_data.c, ported by tools/port_file.py.
#include "types.h"
#include "field/survey.h"
#include "save/survey_savedata.h"

// The tables of the passerby survey's questions: the messages of their answers, and where the answers are kept in
// the save, which survey_savedata.c and the trainer's game info read. The ROM has no string for the file, so the name
// is a guess. GetSurveyAnswerMsgID and GetSurveyAnswerMsgIDCount are swan's names.

#define SURVEY_ANSWER_MAX 17

u16 GetSurveyAnswerMsgID(int question, int answer);

int GetSurveyAnswerMsgIDCount(int question);

u8 func_ov010_02167358(u8 question) {
    if (question > SURVEY_QUESTION_COUNT - 1) {
        return 0;
    }
    return data_ov010_02171f5c[question].bits;
}

