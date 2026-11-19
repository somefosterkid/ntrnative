#include "global.h"

struct GlobalTrade;

BOOL DWC_GetDateTime(RTCDate *date, RTCTime *time);



typedef struct GlobalTrade GlobalTrade;

void sub_0202DB88(GlobalTrade *globalTrade, u32 date);

void sub_0202DB98(GlobalTrade *globalTrade, u32 date);

static inline u32 Date_EncodeParams(u32 year, u32 month, u32 day, int week) {
    return (year << 24) | ((month & 0xff) << 16) | ((day & 0xff) << 8) | week;
}

static inline u32 Date_Encode(RTCDate *rtcDate) {
    return Date_EncodeParams(rtcDate->year, rtcDate->month, rtcDate->day, rtcDate->week);
}

void ov70_02240CA0(GlobalTrade *globalTrade, int type);

void ov70_02240CA0(GlobalTrade *globalTrade, int type) {
    RTCDate currentDate;
    RTCTime unused;
    u32 timestamp;

    DWC_GetDateTime(&currentDate, &unused);

    timestamp = Date_Encode(&currentDate);

    if (type == 1) {
        sub_0202DB98(globalTrade, timestamp);
    } else {
        sub_0202DB88(globalTrade, timestamp);
    }
}
