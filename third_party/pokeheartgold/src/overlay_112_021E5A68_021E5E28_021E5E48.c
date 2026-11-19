#include "global.h"

#include "overlay_112_ir_state.h"


void ov112_021E5E48(void);

void ov112_021E5E48(void) {
    RTCTime time;
    RTCDate date;
    s64 secs;
    u32 v;
    u32 mask, a, b, c, d;
    if (RTC_GetTime(&time) != 0) {
        return;
    }
    if (RTC_GetDate(&date) != 0) {
        return;
    }
    secs = RTC_ConvertDateTimeToSecond(&date, &time);
    if (secs > 0xFFFFFFFFLL) {
        secs = 0xFFFFFFFFLL;
    }
    if (secs < 0) {
        v = 0;
    } else {
        v = (u32)secs;
    }
    mask = 0xFF000000;
    a = v << 24;
    b = (v << 8) & (mask >> 8);
    c = (u8)(v >> 24);
    d = (v >> 8) & (mask >> 16);
    v = (a & mask) | ((c | d) | b);
    *(u32 *)(ov112_021FFAA4.unk_20 + 0x60) = v;
}
