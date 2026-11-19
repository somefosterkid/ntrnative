#include "global.h"
#include "message_format.h"

void ov46_02259534(MessageFormat **arg0, s32 arg1);

void ov46_02259534(MessageFormat **arg0, s32 arg1) {
    BufferIntegerAsString(*arg0, 0U, arg1, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
}
