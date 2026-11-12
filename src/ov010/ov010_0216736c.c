// Black's code at ov010 0x0216736c-0x021673a8. No name is known: the module and address.

#include "types.h"
#include "field/survey.h"
#include "save/survey_savedata.h"

static const u8 data_ov010_02171f3c[SURVEY_QUESTION_COUNT] = {
    2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 8, 8, 11, 17, 11,
};

// clang-format off
const SurveyAnswerInfo data_ov010_02171f5c[SURVEY_QUESTION_COUNT] = {
    { 1, 1, 2, 0, 0 },
    { 1, 1, 2, 2, 2 },
    { 1, 1, 2, 4, 4 },
    { 1, 1, 2, 6, 6 },
    { 1, 1, 2, 8, 8 },
    { 1, 1, 2, 10, 10 },
    { 1, 1, 2, 12, 12 },
    { 0, 2, 2, 14, 14 },
    { 0, 2, 2, 17, 16 },
    { 0, 2, 2, 20, 18 },
    { 0, 2, 2, 23, 20 },
    { 0, 2, 2, 26, 22 },
    { 1, 2, 3, 29, 24 },
    { 1, 2, 3, 33, 27 },
    { 1, 2, 3, 37, 30 },
    { 1, 2, 3, 41, 33 },
    { 1, 2, 3, 45, 36 },
    { 1, 2, 3, 49, 39 },
    { 1, 2, 3, 53, 42 },
    { 0, 3, 3, 57, 45 },
    { 0, 3, 3, 62, 48 },
    { 0, 3, 3, 67, 51 },
    { 1, 3, 3, 72, 54 },
    { 1, 3, 3, 78, 57 },
    { 1, 3, 3, 84, 60 },
    { 1, 4, 4, 90, 63 },
    { 1, 4, 4, 98, 67 },
    { 0, 6, 4, 106, 71 },
    { 0, 9, 5, 117, 75 },
    { 0, 6, 0, 134, 80 },
};

// clang-format on

u8 func_ov010_0216736c(u8 question) {
    if (0x1d < question) {
        return 0;
    }
    return data_ov010_02171f5c[question].firstAnswer;
}

u8 func_ov010_02167380(u8 question) {
    if (0x1d < question) {
        return 0;
    }
    return data_ov010_02171f5c[question].firstBit;
}

u8 func_ov010_02167394(u8 question) {
    if (0x1d < question) {
        return 0;
    }
    return data_ov010_02171f3c[question];
}
