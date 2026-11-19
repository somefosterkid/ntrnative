#include "global.h"

#include "message_format.h"

void ov96_021EDF3C(MessageFormat *arg0, s32 arg1, u32 arg2, u32 arg3);

void ov96_021EDF3C(MessageFormat *messageFormat, s32 num, u32 numDigits, u32 idx) {
    BufferIntegerAsString(messageFormat, idx, num, numDigits, PRINTING_MODE_LEFT_ALIGN, 1);
}
