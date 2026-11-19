#include "global.h"

#include "unk_020318C8.h"

typedef struct WiFiPlazaQuestion {
    u32 questionNumber;
    u32 selectedAnswer;
} WiFiPlazaQuestion;

u32 sub_020318C8(void) {
    return sizeof(WiFiPlazaQuestion);
}
