// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/rtc.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_RTC_H
#define BWPORT_TWL_RTC_H

#include "types.h"

// pokebw2's RTCi_ProcessExCommand
// Runs the command that main's RTC work holds (RTCi_GetSysWork in nitro/rtc.h), which is 0x10 to 0x12 here, and returns
// an RTCResult: any other command is RTC_RESULT_INVALID_COMMAND
int func_02700000(void);

#endif // BWPORT_TWL_RTC_H
