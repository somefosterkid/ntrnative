#include "global.h"

#include "overlay_88_wifi_plaza_news.h"

void ov88_02258F74(WiFiPlazaNewsCountdown *countdown);

void ov88_02258F74(WiFiPlazaNewsCountdown *countdown) {
    countdown->blinkCounter = (u16)(countdown->blinkCounter + 1);
    if ((u32)countdown->blinkCounter > 0x10U) {
        countdown->blinkCounter = 0U;
    }
}
