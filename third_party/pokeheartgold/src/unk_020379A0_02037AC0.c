#include "global.h"

#include "unk_020379A0.h"
#include "comm_tool.h"


void sub_02037AC0(u8 a0) {
    _021D414C->sendTiming = a0;
    _021D414C->sendTimingPending = 1;
}
