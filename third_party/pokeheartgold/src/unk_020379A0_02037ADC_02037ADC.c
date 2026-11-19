#include "global.h"

#include "unk_02035900.h"
#include "comm_tool.h"


void sub_02037ADC(void);

void sub_02037ADC(void) {
    if (_021D414C != NULL) {
        if ((_021D414C->sendTimingPending != 0) && (sub_020376E0(0x10, &_021D414C->sendTiming) != 0)) {
            _021D414C->sendTimingPending = 0U;
        }
        if ((_021D414C->syncState != 0) && (sub_02037184(0x11, _021D414C->syncNo) != 0)) {
            _021D414C->syncState = 0U;
        }
    }
}
