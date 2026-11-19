#include "global.h"

#include "unk_020379A0.h"
#include "comm_tool.h"


void sub_02037BEC(void) {
    for (int i = 0; i < 8; i++) {
        _021D414C->hasRecievedTempData[i] = 0;
    }
}
