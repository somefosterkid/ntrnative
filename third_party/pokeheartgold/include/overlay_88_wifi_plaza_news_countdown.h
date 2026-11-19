#ifndef POKEHEARTGOLD_OVERLAY_88_WIFI_PLAZA_NEWS_COUNTDOWN_H
#define POKEHEARTGOLD_OVERLAY_88_WIFI_PLAZA_NEWS_COUNTDOWN_H

// The Wi-Fi Plaza news countdown (WiFiPlazaNewsData.countdown, 0xc bytes), passed to ov88_02258F60 / ov88_02258F74.

#include "global.h"

#include "overlay_88_unk_struct_ov66_02231428.h"

typedef struct WiFiPlazaNewsCountdown {
    UnkStruct_ov66_02231428 remainingTime;
    u8 waitingForTimeSync;
    u8 needsRedraw;
    u16 blinkCounter;
    u32 secondsPerSegment;
} WiFiPlazaNewsCountdown;

// sizeof(WiFiPlazaNewsCountdown) == 0xc

#endif // POKEHEARTGOLD_OVERLAY_88_WIFI_PLAZA_NEWS_COUNTDOWN_H
